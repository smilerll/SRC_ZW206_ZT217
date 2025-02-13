/* Copyright (C) 2019 RDA Technologies Limited and/or its affiliates("RDA").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * RDA assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. RDA reserves the right to make changes in the
 * software without notification.  RDA also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */

#include "usb_serial.h"
#include "usb_ch9.h"
//#include "usb_cdc.h"
#include "usb_composite_device.h"
//#include "osi_api.h"
//#include "osi_log.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "usb_utils.h"

typedef struct
{
    copsFunc_t func;
    usbSerial_t serial;
    uint32_t serial_name;
    struct
    {
        usb_interface_descriptor_t intf;
        usb_endpoint_descriptor_t in_ep;
        usb_endpoint_descriptor_t out_ep;
    } desc;
    int intf_num;
} fSerial_t;

#define SPRD_OPEN_V (1)
#define SPRD_CLOSE_V (0)
#define OPTION_CHANGE_V (3)
#define OPTION_OPEN_V (3)
#define OPTION_CLOSE_V (0)

//static_assert(SPRD_OPEN_V != OPTION_CLOSE_V, "serial port ctrl value illegal");
//static_assert(SPRD_CLOSE_V != OPTION_OPEN_V, "serial port ctrl value illegal");
//static_assert(SPRD_CLOSE_V != OPTION_CHANGE_V, "serial port ctrl value illegal");

static usbString_t generic_serial_str = {
    0,
    0,
    "Unisoc Generic Serial",
};

static const usb_interface_descriptor_t generic_serial_intf_desc = {
    USB_DT_INTERFACE_SIZE,        //bLength
    USB_DT_INTERFACE,                //bDescriptorType
    0,                                         /* .bInterfaceNumber = DYNAMIC, */
    0,                                          /* .bAlternateSetting = DYNAMIC, */
    2,                                          //bNumEndpoints
    USB_CLASS_VENDOR_SPEC,       //bInterfaceClass
    0,                                          //bInterfaceSubClass
    0,                                          //bInterfaceProtocol
    0,                                          // iInterface =DYNAMIC
};

static const usb_endpoint_descriptor_t hs_data_in = {
    USB_DT_ENDPOINT_SIZE,           //bLength
    USB_DT_ENDPOINT,                   //bDescriptorType
    USB_DIR_IN,                             //bEndpointAddress
    USB_ENDPOINT_XFER_BULK,        //bmAttributes
    512,                   //wMaxPacketSize
    /*note: these two are only used in audio endpoints*/
    /*use USB_DT_ENDPOINT_SIZE in blength,not sizeof*/
    0,
};

static const usb_endpoint_descriptor_t hs_data_out = {
    USB_DT_ENDPOINT_SIZE,           //bLength
    USB_DT_ENDPOINT,                   //bDescriptorType
    USB_DIR_OUT,                          //bEndpointAddress
    USB_ENDPOINT_XFER_BULK,        //bmAttributes
    512,                   //wMaxPacketSize
    /*note: these two are only used in audio endpoints*/
    /*use USB_DT_ENDPOINT_SIZE in blength,not sizeof*/
    0,
};

static fSerial_t *_f2s(copsFunc_t *f) { return (fSerial_t *)f; }

usb_interface_descriptor_t desc_inf;
usb_endpoint_descriptor_t desc_epin;
usb_endpoint_descriptor_t desc_epout;

static uint32_t _serialFillDesc(copsFunc_t *f, void *buf, uint32_t size)
{
    fSerial_t *s = _f2s(f);
    if (f->controller->speed >= USB_SPEED_HIGH)
    {
        s->desc.in_ep.wMaxPacketSize = 512;
        s->desc.out_ep.wMaxPacketSize = 512;
    }
    else if (f->controller->speed == USB_SPEED_FULL)
    {
        s->desc.in_ep.wMaxPacketSize = cpu_to_le16(64);
        s->desc.out_ep.wMaxPacketSize = cpu_to_le16(64);
    }
    else
    {
        s->desc.in_ep.wMaxPacketSize = cpu_to_le16(8);
        s->desc.out_ep.wMaxPacketSize = cpu_to_le16(8);
    }

    desc_inf=s->desc.intf;
    desc_epin=s->desc.in_ep;
    desc_epout=s->desc.out_ep;

    if (size > sizeof(s->desc))
        size = sizeof(s->desc);
    memcpy(buf, &s->desc, size);
    USB_LOG_TRACE("_serialFillDesc size=%d,sizeof_intf:%d,sizeof_ep:%d\n",size \
                        ,sizeof(s->desc.intf)    \
                        ,sizeof(s->desc.in_ep));
    return size;
}

static int _serialSetAlt(copsFunc_t *f, uint8_t intf, uint8_t alt_must_0)
{
    fSerial_t *s = _f2s(f);
    return intf == s->intf_num ? s->serial.ops->enable(&s->serial) : -1;
}

static int _serialSetup(copsFunc_t *f, const usb_device_request_t *ctrl)
{
    fSerial_t *s = _f2s(f);
    const uint16_t w_value = ctrl->wValue;
    switch ((ctrl->bRequest << 8) | ctrl->bRequestType)
    {
    case ((UCDC_SET_CONTROL_LINE_STATE << 8) | USB_TYPE_CLASS | USB_RECIP_INTERFACE):
        USB_LOG_TRACE("GS %4c port change value %u", s->serial_name, w_value);
        if (w_value == SPRD_OPEN_V || w_value == OPTION_OPEN_V)
            s->serial.ops->open(&s->serial);
        else if (w_value == SPRD_CLOSE_V || w_value == OPTION_CLOSE_V)
            s->serial.ops->close(&s->serial);
        return 0;

    default:
        USB_ERR_TRACE("GS %4c unknown setup %x %x %x %x %x", s->serial_name,
                 ctrl->bRequestType, ctrl->bRequest, ctrl->wValue,
                 ctrl->wIndex, ctrl->wLength);
        //SCI_ASSERT(0);
        break;
    }
    return -1;
}

static int _serialBind(copsFunc_t *f, cops_t *cops, udc_t *udc)
{
    fSerial_t *s = _f2s(f);
    int r = 0;
    int intf = 0;
    s->serial.func = f;
    s->serial.epin_desc = &s->desc.in_ep;
    s->serial.epout_desc = &s->desc.out_ep;
    r = s->serial.ops->bind(s->serial_name, &s->serial);
    if (r < 0)
        return r;

    intf = copsAssignInterface(cops, f);
    if (intf < 0)
        goto fail;
	
    s->intf_num = intf;
    s->desc.intf.bInterfaceNumber = intf;
    s->desc.intf.iInterface = copsAssignStringId(cops, &generic_serial_str);
    return 0;

fail:
    s->serial.ops->unbind(&s->serial);
    return -1;
}

static void _serialUnbind(copsFunc_t *f)
{
    fSerial_t *s = _f2s(f);
    s->serial.ops->unbind(&s->serial);

    if (s->intf_num != -1)
        copsRemoveInterface(f->cops, s->intf_num);
    s->intf_num = -1;
    copsRemoveString(f->cops, &generic_serial_str);
}

static void _serialDisable(copsFunc_t *f)
{
    fSerial_t *s = _f2s(f);
    s->serial.ops->disable(&s->serial);
}

static void _serialSuspend(copsFunc_t *f)
{
    fSerial_t *s = _f2s(f);
	if(s->serial.ops->suspend)
    	s->serial.ops->suspend(&s->serial);
}

static void _serialResume(copsFunc_t *f)
{
    fSerial_t *s = _f2s(f);
	if(s->serial.ops->resume)
    	s->serial.ops->resume(&s->serial);
}

static void _serialDestroy(copsFunc_t *f)
{
    if (f)
    {
        fSerial_t *s = _f2s(f);
        free(s);
    }
}
#if 0
copsFunc_t *createSerialFunc(uint32_t name)
{
    fSerial_t *s = (fSerial_t *)calloc(1, sizeof(fSerial_t));
    if (s == NULL)
        return NULL;

    memcpy(&s->desc.intf, &generic_serial_intf_desc, USB_DT_INTERFACE_SIZE);
    memcpy(&s->desc.in_ep, &hs_data_in, USB_DT_ENDPOINT_SIZE);
    memcpy(&s->desc.out_ep, &hs_data_out, USB_DT_ENDPOINT_SIZE);

    s->intf_num = -1;
    s->serial_name = name;
    s->serial.ops = usbSerialGetOps();
    s->func.name = OSI_MAKE_TAG('G', 'S', 'R', 'L');
    s->func.ops.fill_desc = _serialFillDesc;
    s->func.ops.destroy = _serialDestroy;
    s->func.ops.setup = _serialSetup;
    s->func.ops.bind = _serialBind;
    s->func.ops.unbind = _serialUnbind;
    s->func.ops.set_alt = _serialSetAlt;
    s->func.ops.disable = _serialDisable;

    return &s->func;
}
#endif
copsFunc_t *createDebugSerialFunc(uint32_t name)
{
    fSerial_t *s = (fSerial_t *)calloc(1, sizeof(fSerial_t));
    if (s == NULL)
        return NULL;

    memcpy(&s->desc.intf, &generic_serial_intf_desc, USB_DT_INTERFACE_SIZE);
    memcpy(&s->desc.in_ep, &hs_data_in, USB_DT_ENDPOINT_SIZE);
    memcpy(&s->desc.out_ep, &hs_data_out, USB_DT_ENDPOINT_SIZE);

    s->intf_num = -1;
    s->serial_name = name;
    s->serial.ops = usbDebugSerialGetOps();
    s->func.name = OSI_MAKE_TAG('G', 'S', 'R', 'L');
    s->func.ops.fill_desc = _serialFillDesc;
    s->func.ops.destroy = _serialDestroy;
    s->func.ops.setup = _serialSetup;
    s->func.ops.bind = _serialBind;
    s->func.ops.unbind = _serialUnbind;
    s->func.ops.set_alt = _serialSetAlt;
    s->func.ops.disable = _serialDisable;
    s->func.ops.suspend = _serialSuspend;
    s->func.ops.resume = _serialResume;
    return &s->func;
}

copsFunc_t *createVcomFunc(uint32_t name)
{
    fSerial_t *s = (fSerial_t *)calloc(1, sizeof(fSerial_t));
    if (s == NULL)
        return NULL;

    memcpy(&s->desc.intf, &generic_serial_intf_desc, USB_DT_INTERFACE_SIZE);
    memcpy(&s->desc.in_ep, &hs_data_in, USB_DT_ENDPOINT_SIZE);
    memcpy(&s->desc.out_ep, &hs_data_out, USB_DT_ENDPOINT_SIZE);

    s->intf_num = -1;
    s->serial_name = name;
    s->serial.ops = usbVcomGetOps();
    s->func.name = OSI_MAKE_TAG('G', 'S', 'R', 'L');
    s->func.ops.fill_desc = _serialFillDesc;
    s->func.ops.destroy = _serialDestroy;
    s->func.ops.setup = _serialSetup;
    s->func.ops.bind = _serialBind;
    s->func.ops.unbind = _serialUnbind;
    s->func.ops.set_alt = _serialSetAlt;
    s->func.ops.disable = _serialDisable;
    s->func.ops.suspend = _serialSuspend;
    s->func.ops.resume = _serialResume;

    return &s->func;
}

