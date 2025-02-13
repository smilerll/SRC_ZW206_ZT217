/******************************************************************************
 ** Copyright (c) 
 ** File Name:      sensor_OV2640.c                                           *
 ** Author:                                                       *
 ** DATE:                                                             *
 ** Description:   This file contains driver for sensor OV2640. 
 **                                                          
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 **       
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "ms_customize_trc.h"
#include "sensor_cfg.h"
#include "sensor_drv.h"
#include "os_api.h"
#include "chip.h"
#include "dal_dcamera.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/**---------------------------------------------------------------------------*
 **                     Extern Function Declaration                           *
 **---------------------------------------------------------------------------*/
//extern uint32 OS_TickDelay(uint32 ticks);

/**---------------------------------------------------------------------------*
 **                         Const variables                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                            Macro Define
 **---------------------------------------------------------------------------*/
#define OV2640_I2C_ADDR_W       0x60
#define OV2640_I2C_ADDR_R       0x61

 
/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
LOCAL uint32 OV2640_set_ae_enable(uint32 enable);
LOCAL uint32 OV2640_set_hmirror_enable(uint32 enable);
LOCAL uint32 OV2640_set_vmirror_enable(uint32 enable);
LOCAL uint32 OV2640_set_preview_mode(uint32 preview_mode);
LOCAL uint32 OV2640_Identify(uint32 param);
LOCAL uint32 OV2640_BeforeSnapshot(uint32 param);
    
LOCAL uint32 OV2640_set_brightness(uint32 level);
LOCAL uint32 OV2640_set_contrast(uint32 level);
LOCAL uint32 OV2640_set_sharpness(uint32 level);
LOCAL uint32 OV2640_set_saturation(uint32 level);
LOCAL uint32 OV2640_set_image_effect(uint32 effect_type);
LOCAL uint32 OV2640_set_work_mode(uint32 mode);
LOCAL uint32 OV2640_chang_image_format(uint32 param);
LOCAL uint32 OV2640_check_image_format_support(uint32 param);
LOCAL uint32 OV2640_after_snapshot(uint32 param);

LOCAL uint32 set_ov2640_ev(uint32 level);
LOCAL uint32 set_ov2640_awb(uint32 mode);
LOCAL uint32 set_ov2640_anti_flicker(uint32 mode);
LOCAL uint32 set_ov2640_video_mode(uint32 mode); 
LOCAL uint32 OV2640_pick_out_jpeg_stream(uint32 param);
LOCAL uint32 OV2640_zoom(uint32 level);



/**---------------------------------------------------------------------------*
 **                         Local Variables                                  *
 **---------------------------------------------------------------------------*/
LOCAL uint32 s_preview_mode = DCAMERA_ENVIRONMENT_NORMAL;
LOCAL uint32 s_image_effect = 0;

/*lint -save -e533 */
__align(4) LOCAL const SENSOR_REG_T ov2640_YUV_COMMON[]=
{
    //Wu Yiqing  QVGA normal
    {0xff, 0x01},                             
    {0x12, 0x80},   
    {SENSOR_WRITE_DELAY, 0x0a},//delay 10ms     
    {0xff, 0x00},                             
    {0x2c, 0xff},                             
    {0x2e, 0xdf},                             
    {0xff, 0x01},                             
    {0x3c, 0x32},                             
    {0x09, 0x02},                             
#ifndef PLATFORM_SC8800H
    {0x04, 0x28},
#else
    {0x04, 0xf8},
#endif
    {0x13, 0xe5},                             
    {0x14, 0x28},                             
    {0x2c, 0x0c},                             
    {0x33, 0x78},                             
    {0x3a, 0x33},                             
    {0x3b, 0xfB},                             
    {0x3e, 0x00},                             
    {0x43, 0x11},                             
    {0x16, 0x10},                             
    {0x4a, 0x81},                             
    {0x21, 0x99},                             
    {0x24, 0x48},//jerry,1020                         
    {0x25, 0x38},                             
   // {0x26, 0xd0},                           
    {0x26, 0x82},                             
    {0x5c, 0x00},                             
    {0x63, 0x00},                             
    {0x46, 0x00},//dummyline                  
    {0x0c, 0x3c}, //manual banding            
    {0x61, 0x70},                             
    {0x62, 0x80},                             
    {0x7c, 0x05},                             
    {0x20, 0x80},                             
    {0x28, 0x30},                             
    {0x6c, 0x00},                             
    {0x6d, 0x80},                             
    {0x6e, 0x00},                             
    {0x70, 0x02},                             
    {0x71, 0x94},                             
    {0x73, 0xc1},                             
    //jerry, uxga 14.28 fps                     
    {0xff, 0x01},                             
    {0x12, 0x00},                             
    {0x11, 0x00},                             
    {0x17, 0x11},                             
    {0x18, 0x75},                             
    {0x19, 0x01},                             
    {0x1a, 0x97},                             
    {0x32, 0x36},                             
    {0x37, 0x40},                             
    {0x4f, 0xbb},//banding  
    {0x50, 0x9c},                             
    {0x6d, 0x80},                             
    {0x3d, 0x34},                             
    {0x5a, 0x57},//step                   
    {0x39, 0x82},                             
    {0x35, 0x88},                             
    {0x22, 0x09},                             
    {0x37, 0x40},                             
    {0x23, 0x00},                             
    {0x34, 0xa0},                             
    {0x36, 0x1a},                             
    {0x06, 0x02},                             
    {0x07, 0xc0},                             
    {0x0d, 0xb7},                             
    {0x0e, 0x01},                             
    {0x4c, 0x00},                             
    //central exposure                          
    {0x5d, 0x55},
    {0x5e, 0x7d},
    {0x5f, 0x7d},
    {0x60, 0x55},                                         
    {0xff, 0x00},                     
    {0xe5, 0x7f},                     
    {0xf9, 0xc0},                             
    {0x41, 0x24},                             
    {0xe0, 0x14},                             
    {0x76, 0xff},                             
    {0x33, 0xa0},                             
    {0x42, 0x20},                             
    {0x43, 0x18},                             
    {0x4c, 0x00},                             
    {0x87, 0xd0},                             
    {0x88, 0x3f},                             
    {0xd7, 0x03},                             
    {0xd9, 0x10},                             
    {0xd3, 0x82},                                  
    {0xc8, 0x08},                             
    {0xc9, 0x80},                                   
    {0x7c, 0x00},                             
    {0x7d, 0x00},                             
    {0x7c, 0x03},                             
    {0x7d, 0x48},                             
    {0x7d, 0x48},   
    {0x7c, 0x07},                             
    {0x7d, 0x00},       
    {0x7c, 0x08},                             
    {0x7d, 0x20},                             
    {0x7d, 0x10},                             
    {0x7d, 0x02},                                                                     
    {0x90, 0x00},                             
    {0x91, 0x0e},                             
    {0x91, 0x1a},                             
    {0x91, 0x31},                             
    {0x91, 0x5a},                             
    {0x91, 0x69},                             
    {0x91, 0x75},                             
    {0x91, 0x7e},                             
    {0x91, 0x88},                             
    {0x91, 0x8f},                             
    {0x91, 0x96},                             
    {0x91, 0xa3},                             
    {0x91, 0xaf},                             
    {0x91, 0xc4},                             
    {0x91, 0xd7},                             
    {0x91, 0xe8},                             
    {0x91, 0x20},                             
    {0x92, 0x00},
    {0x93, 0x04},  
    {0x93, 0xe4},
    {0x93, 0x03},
    {0x93, 0x03},
    {0x93, 0x00},   
    {0x93, 0x02},   
    {0x96, 0x00},
    {0x97, 0x0c},
    {0x97, 0x13},
    {0x97, 0x04}, 
    {0x97, 0x0c},//cmx-3
    {0x97, 0x26},
    {0x97, 0x32},
    {0x97, 0x31},
    {0x97, 0x30},
    {0x97, 0x01},
    {0x97, 0x98},
    {0x97, 0x80},
    {0x97, 0x00},
    {0x97, 0x00},                          
    {0xbf, 0x00},
    {0xba, 0xdc},
    {0xbb, 0x08},
    {0xb6, 0x20},
    {0xb8, 0x30},
    {0xb7, 0x20},
    {0xb9, 0x30},
    {0xb3, 0xb4},
    {0xb4, 0xca},
    {0xb5, 0x35},
    {0xb0, 0x46},
    {0xb1, 0x46},
    {0xb2, 0x06},
    {0xc7, 0x00},
    {0xc6, 0x51},
    {0xc5, 0x11},
    {0xc4, 0x9c}, 
    {0xca, 0x01},
    {0xcb, 0x58},
    {0xcb, 0x40},
    {0xcb, 0x4c},
    {0xa6, 0x00},
    {0xa7, 0xe0},
    {0xa7, 0xe8},
    {0xa7, 0x2b},
    {0xa7, 0x21},
    {0xa7, 0x00},
    {0xa7, 0x18},
    {0xa7, 0xc0},
    {0xa7, 0xd8},
    {0xa7, 0x25},
    {0xa7, 0x21},
    {0xa7, 0x00},
    {0xa7, 0x18},
    {0xa7, 0xc0},
    {0xa7, 0xd8},
    {0xa7, 0x27},
    {0xa7, 0x21},
    {0xa7, 0x00},
    {0xa7, 0x18},
    //VGA                             
    {0xff, 0x00}, 
    {0xe0, 0x04},
    {0xc0, 0xc8},  
    {0xc1, 0x96},
    {0x8c, 0x00},
    {0x86, 0x3d},// ,enable dcw 
    {0x50, 0x89},
    {0x51, 0x90},
    {0x52, 0x2c},
    {0x53, 0x00},
    {0x54, 0x00},
    {0x55, 0x88},
    {0x57, 0x00},
    {0x5a, 0xa0},
    {0x5b, 0x78},
    {0x5c, 0x00},  
    {0xd3, 0x04},
    {0xe0, 0x00},                             
    {0x7f, 0x00},                             
    {0xda, 0x00},   
    {0xe5, 0x1f},                             
    {0xe1, 0x67},                             
    {0xe0, 0x00},                             
    {0xdd, 0xff},   //{0xdd ,0x7f},                           
    {0x05, 0x00},                               
    {0xc3, 0xef},
    ///////////////////add by Jerry, 1124//////////////For trigger BLC
    {SENSOR_WRITE_DELAY, 0x0a},//delay 10ms 
    {0xff, 0x01},
    {0x13, 0xe0},
    {0x00, 0x0f},
    {SENSOR_WRITE_DELAY, 0x14},//delay 20ms
    {0x00, 0x1f},
    {SENSOR_WRITE_DELAY, 0x14},//delay 20ms
    {0x00, 0x00},
    {SENSOR_WRITE_DELAY, 0x14},//delay 20ms
    {0x13, 0xe5},
    /////////////////////////////////

};

__align(4) LOCAL const SENSOR_REG_T ov2640_YUV_1600X1200[]=
{

    {0xff, 0x01 },
    {0x12, 0x00 },
    {0x17, 0x11 },
    {0x18, 0x75 },
    {0x19, 0x01 },
    {0x1a, 0x97 },
    {0x32, 0x36 },
    {0x37, 0x40 },
    {0x4f, 0xbb },
    {0x50, 0x9c },
    {0x6d, 0x80 },
    {0x3d, 0x34 }, 
                  
                  
    {0xff, 0x00 },
    {0x87, 0xd0},//kenxu for full  mode
    {0xe0, 0x04 },
    {0xc0, 0xc8 },
    {0xc1, 0x96 },
    {0x86, 0x1d },
    {0x50, 0x00 },
    {0x51, 0x90 },
    {0x52, 0x2c },
    {0x53, 0x00 },
    {0x54, 0x00 },
    {0x55, 0x88 },
    {0x57, 0x00 },
    {0x5a, 0x90 },
    {0x5b, 0x2c },
    {0x5c, 0x05 },
    {0xd3, 0x00 },
    {0xe0, 0x00 },
};

__align(4) LOCAL const SENSOR_REG_T ov2640_YUV_1280X960[]=
{
    
    {0xff, 0x01 },         
    {0x12, 0x00 },         
//  {0x13, 0xe0 },         
//  {0x11, 0x00},//{0x11, 0x01 },          
    {0x17, 0x11 },         
    {0x18, 0x75 },         
    {0x19, 0x01 },         
    {0x1a, 0x97 },         
    {0x32, 0x36 },         
    {0x37, 0x40 },         
//  {0x4f, 0xbb },//banding
    {0x4f, 0xb0 },  
    {0x50, 0x9c },         
    {0x6d, 0x80 },         
    {0x3d, 0x34 },  
    
    //SXGA
    {0xff ,0x00},
    {0x87, 0xd0},//kenxu for full  mode 
    {0xe0 ,0x04},
    {0xc0 ,0xc8},
    {0xc1 ,0x96},
    {0x86 ,0x3d},
    {0x50 ,0x00},
    {0x51 ,0x90},
    {0x52 ,0x2c},
    {0x53 ,0x00},
    {0x54 ,0x00},
    {0x55 ,0x88},
    {0x57 ,0x00},
    {0x5a ,0x40},
    {0x5b ,0xf0},
    {0x5c ,0x01},
    {0xd3 ,0x82},
    {0xe0 ,0x00},

};

__align(4) LOCAL const SENSOR_REG_T ov2640_YUV_640X480[]=
{

#if 0   
    {0xff, 0x01 },          
    {0x12, 0x00 },          
//  {0x13, 0xe5 },          
    //{0x11, 0x00 },            
    {0x17, 0x11 },          
    {0x18, 0x75 },          
    {0x19, 0x01 },          
    {0x1a, 0x97 },          
    {0x32, 0x36 },          
    {0x37, 0x40 },
    //banding 

    {0x4f, 0xbb},
    {0x50, 0x9c },          
    {0x6d, 0x80 },          
    {0x3d, 0x34 },          
        
        
    //VGA                       
    {0xff, 0x00},           
    {0xe0, 0x04},           
    {0xdd, 0xF3},           
    {0xc0, 0xc8},           
    {0xc1, 0x96},           
    {0x86, 0x3d},           
    {0x50, 0x89},           
    {0x51, 0x90},           
    {0x52, 0x2c},           
    {0x53, 0x00},           
    {0x54, 0x00},           
    {0x55, 0x88},           
    {0x57, 0x00},           
    {0x5a, 0xa0},           
    {0x5b, 0x78},           
    {0x5c, 0x00},           
    {0xd3, 0x04},           
    {0xdd, 0xFF},           
    {0xeb, 0x33},           
    {0xe0, 0x00}

#else

    {0xff, 0x01},
    {0x12, 0x40},
    {0x11, 0x00},
    {0x17, 0x11},
    {0x18, 0x43},
    {0x19, 0x00},
    {0x1a, 0x4b},
    {0x32, 0x09},
    {0x37, 0xc0},
    {0x4f, 0xca},
    {0x50, 0xa8},
    {0x5a, 0x33},//kenxu for 25fps
    {0x6d, 0x00},
    {0x3d, 0x38},//kenxu for 25fps
    {0x46, 0x88},//kenxu for 25fps
    {0x39, 0x12}, 
    {0x35, 0xda}, 
    {0x22, 0x1a}, 
    {0x37, 0xc3}, 
    {0x23, 0x00},
    {0x34, 0xc0}, 
    {0x36, 0x1a},
    {0x06, 0x88}, 
    {0x07, 0xc0},
    {0x0d, 0x87}, 
    {0x0e, 0x41}, 
    {0x4c, 0x00},
    {0xff, 0x00},
    {0x87, 0xd5},//kenxu for sub-sample mode
    {0xe0, 0x04},
    {0xc0, 0x64},
    {0xc1, 0x4B},
    {0x8c, 0x00},
    {0x86, 0x3D},
    {0x50, 0x00},
    {0x51, 0xC8},
    {0x52, 0x96},
    {0x53, 0x00},
    {0x54, 0x00},
    {0x55, 0x00},
    {0x5a, 0xa0},
    {0x5b, 0x78},
    {0x5c, 0x00},
    {0xd3, 0x82},
    {0xe0, 0x00},
       {SENSOR_WRITE_DELAY, 0x64}//delay 50ms 
#endif

};



__align(4) LOCAL const SENSOR_REG_T ov2640_jpeg_mode[]=
{
    /* jpeg setting */
    {0xff,0x00},
    {0x05,0x00},
    {0xDA,0x10},
    {0xD7,0x03},
    {0xDF,0x00},
    {0x33,0x80},
    {0x3c,0x20}, //href = 0x20 * 16 = 512
    {0x44,0x06},
    {0xe1,0x77},
    {0xD3,0x04}
};

__align(4) LOCAL const SENSOR_REG_T ov2640_yuv422_mode[]=
{
    /**/
    {0xff, 0xff},
};

LOCAL SENSOR_REG_TAB_INFO_T s_OV2640_resolution_Tab_YUV[]=
{   
    // COMMON INIT
    {ADDR_AND_LEN_OF_ARRAY(ov2640_YUV_COMMON), 0, 0, 24, SENSOR_IMAGE_FORMAT_YUV422}, 
    
    // YUV422 PREVIEW 1 
    {ADDR_AND_LEN_OF_ARRAY(ov2640_YUV_640X480), 640, 480, 24, SENSOR_IMAGE_FORMAT_YUV422},
    {ADDR_AND_LEN_OF_ARRAY(ov2640_YUV_1280X960), 1280, 960, 12, SENSOR_IMAGE_FORMAT_YUV422},
    {ADDR_AND_LEN_OF_ARRAY(ov2640_YUV_1600X1200), 1600, 1200, 12,SENSOR_IMAGE_FORMAT_JPEG},
    {PNULL, 0, 0, 0, 0, 0},
    
    // YUV422 PREVIEW 2 
    {PNULL, 0, 0, 0, 0, 0},
    {PNULL, 0, 0, 0, 0, 0},
    {PNULL, 0, 0, 0, 0, 0},
    {PNULL, 0, 0, 0, 0, 0}
};

LOCAL SENSOR_IOCTL_FUNC_TAB_T s_OV2640_ioctl_func_tab = 
{
    // Internal 
    PNULL,
    PNULL,
    PNULL,
    OV2640_Identify,

    PNULL,          // write register
    PNULL,          // read  register   
    PNULL,
    PNULL,

    // External
    OV2640_set_ae_enable,
    OV2640_set_hmirror_enable,
    OV2640_set_vmirror_enable,

    OV2640_set_brightness,
    OV2640_set_contrast,
    PNULL,//OV2640_set_sharpness,
    PNULL,//OV2640_set_saturation,

    OV2640_set_preview_mode,    
    OV2640_set_image_effect,

    OV2640_BeforeSnapshot,
    OV2640_after_snapshot,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    set_ov2640_awb,
    PNULL,
    PNULL,
    set_ov2640_ev,
    OV2640_check_image_format_support,
    OV2640_chang_image_format,
    OV2640_zoom,
    PNULL,
    PNULL,
    set_ov2640_anti_flicker,
    set_ov2640_video_mode,

    OV2640_pick_out_jpeg_stream,
    PNULL
    };

LOCAL SENSOR_EXTEND_INFO_T ov2640_ext_info = {
    256,    //jpeg_seq_width
    0//1938       //jpeg_seq_height
};

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
 PUBLIC SENSOR_INFO_T g_OV2640_yuv_info =
{
    OV2640_I2C_ADDR_W,              // salve i2c write address
    OV2640_I2C_ADDR_R,              // salve i2c read address
    
    0,                              // bit0: 0: i2c register value is 8 bit, 1: i2c register value is 16 bit
                                    // bit2: 0: i2c register addr  is 8 bit, 1: i2c register addr  is 16 bit
                                    // other bit: reseved
    SENSOR_HW_SIGNAL_PCLK_N|\
    SENSOR_HW_SIGNAL_VSYNC_N|\
    SENSOR_HW_SIGNAL_HSYNC_P,       // bit0: 0:negative; 1:positive -> polarily of pixel clock
                                    // bit2: 0:negative; 1:positive -> polarily of horizontal synchronization signal
                                    // bit4: 0:negative; 1:positive -> polarily of vertical synchronization signal
                                    // other bit: reseved                                           
                                            
    // preview mode
    SENSOR_ENVIROMENT_NORMAL|\
    SENSOR_ENVIROMENT_NIGHT|\
    SENSOR_ENVIROMENT_SUNNY,        
    
    // image effect
    SENSOR_IMAGE_EFFECT_NORMAL|\
    SENSOR_IMAGE_EFFECT_BLACKWHITE|\
    SENSOR_IMAGE_EFFECT_RED|\
    SENSOR_IMAGE_EFFECT_GREEN|\
    SENSOR_IMAGE_EFFECT_BLUE|\
    SENSOR_IMAGE_EFFECT_YELLOW|\
    SENSOR_IMAGE_EFFECT_NEGATIVE|\
    SENSOR_IMAGE_EFFECT_CANVAS,
    
    // while balance mode
    0,
        
    7,                              // bit[0:7]: count of step in brightness, contrast, sharpness, saturation
                                    // bit[8:31] reseved
    
    SENSOR_LOW_PULSE_RESET,         // reset pulse level
    20,                             // reset pulse width(ms)
    
    SENSOR_HIGH_LEVEL_PWDN,         // 1: high level valid; 0: low level valid
    
    1,                              // count of identify code
    0x0A, 0x26,                     // supply two code to identify sensor.
    0x0B, 0x40,                     // for Example: index = 0-> Device id, index = 1 -> version id                                          
                                            
    SENSOR_AVDD_2800MV,             // voltage of avdd  
    
    1600,                           // max width of source image
    1200,                           // max height of source image
    "OV2640",                       // name of sensor                                               

    SENSOR_IMAGE_FORMAT_YUV422,     // define in SENSOR_IMAGE_FORMAT_E enum,SENSOR_IMAGE_FORMAT_MAX
                                    // if set to SENSOR_IMAGE_FORMAT_MAX here, image format depent on SENSOR_REG_TAB_INFO_T

    SENSOR_IMAGE_PATTERN_YUV422_YUYV,   // pattern of input image form sensor;          

    s_OV2640_resolution_Tab_YUV,    // point to resolution table information structure
    &s_OV2640_ioctl_func_tab,       // point to ioctl function table
            
    PNULL,                          // information and table about Rawrgb sensor
    &ov2640_ext_info,               // extend information about sensor
    SENSOR_AVDD_2800MV,                     // iovdd
    SENSOR_AVDD_1300MV,                      // dvdd
    0,                     // skip frame num before preview 
    0,                      // skip frame num before capture        
    0,                      // deci frame num during preview    
    0,                      // deci frame num during video preview      

    0,  // threshold enable     
    0,  // threshold mode   
    0,  // threshold start postion  
    0,  // threshold end postion 
    0,
    0
};

/**---------------------------------------------------------------------------*
 **                             Function  Definitions
 **---------------------------------------------------------------------------*/
/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//      
/******************************************************************************/
LOCAL uint32 OV2640_set_ae_enable(uint32 enable)
{
    unsigned char value;
    
    //SCI_TRACE_LOW:"OV2640_set_ae_enable: enable = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_OV2640_664_112_2_18_0_31_28_1015,(uint8*)"d", enable);
    
    value = Sensor_ReadReg(0x13);
    value &= 0xFE;
    value |= enable;
    Sensor_WriteReg(0x13, value);
    
    return 0;
}
/******************************************************************************/
// Description: anti 50/60 hz banding flicker
// Global resource dependence: 
// Author:
// Note:
//      level  must smaller than 8
/******************************************************************************/
__align(4) LOCAL const SENSOR_REG_T ov2640_banding_flicker_tab[][3]=
{
    // 50HZ
    {
        {0xff, 0x01},{0x0c, 0x3c},{0xff, 0xff} 
    },    
    //60HZ
    {
        {0xff, 0x01},{0x0c, 0x38},{0xff, 0xff}       
    },
};   

LOCAL uint32 set_ov2640_anti_flicker(uint32 mode)
{//107 us
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)ov2640_banding_flicker_tab[mode];

    SCI_ASSERT(PNULL != sensor_reg_ptr);   /*assert verified*/

    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr)|| (0xFF != sensor_reg_ptr[i].reg_value); i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }

    SCI_Sleep(100); 
    //SCI_TRACE_LOW:"SENSOR: set_ov2640_flicker: 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_OV2640_703_112_2_18_0_31_28_1016,(uint8*)"d", mode);

    return 0;
}
/******************************************************************************/
// Description: set video mode
// Global resource dependence: 
// Author:
// Note:
//      
/******************************************************************************/
__align(4) LOCAL const SENSOR_REG_T ov2640_video_mode_nand_tab[][50]=
{
    // normal mode
    {   //15fps
        {0xff, 0x01},             
        {0x11, 0x01},             
        {0x3d, 0x38},             
        {0x46, 0x00},  
        {0x5a, 0x23},//kenxu for 15fps
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},           
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},           
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},           
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},           
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},           
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},   
        {0xff, 0xff},   
        {0xff, 0xff}
    },    
    //vodeo mode
    {
        {0xff, 0x01},
        {0x12, 0x40},
        {0x11, 0x00},
        {0x17, 0x11},
        {0x18, 0x43},
        {0x19, 0x00},
        {0x1a, 0x4b},
        {0x32, 0x09},
        {0x37, 0xc0},
        {0x4f, 0xca},
        {0x50, 0xa8},
        {0x5a, 0x23},
        {0x6d, 0x00},
        {0x3d, 0x38},
        {0x39, 0x92},
        {0x35, 0xda},
        {0x22, 0x1a},
        {0x37, 0xc3},
        {0x23, 0x00},
        {0x34, 0xc0},
        {0x36, 0x1a},
        {0x06, 0x88},
        {0x07, 0xc0},
        {0x0d, 0x87},
        {0x0e, 0x41},
        {0x4c, 0x00},
        {0x48, 0x00},
        {0x5B, 0x00},
        {0x42, 0x03},
        {0x5a, 0x23},
        {0x4f, 0xca},
        {0x50, 0xa8},
        {0xff, 0x00},
        {0xe0, 0x04},
        {0xc0, 0x64},
        {0xc1, 0x4B},
        {0x8c, 0x00},
        {0x86, 0x3D},
        {0x50, 0x00},
        {0x51, 0xC8},
        {0x52, 0x96},
        {0x53, 0x00},
        {0x54, 0x00},
        {0x55, 0x00},
        {0x5a, 0xA0},
        {0x5b, 0x78},
        {0x5c, 0x00},
        {0xd3, 0x02},
        {0xe0, 0x00},
        {0xff, 0xff}
    },
};   

/******************************************************************************/
// Description: set video mode
// Global resource dependence: 
// Author:
// Note:
//      
/******************************************************************************/
__align(4) LOCAL const SENSOR_REG_T ov2640_video_mode_nor_tab[][52]=
{
    // normal mode
    {   //15fps
        {0xff, 0x01},             
        {0x11, 0x01},             
        {0x3d, 0x38},             
        {0x46, 0x00},  
        {0x5a, 0x23},//kenxu for 15fps
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},           
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},           
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},           
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},           
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},           
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},             
        {0xff, 0xff},     
        {0xff, 0xff},     
        {0xff, 0xff},
        {0xff, 0xff}
    },    
    //vodeo mode
    {
        {0xff, 0x01},
        {0x12, 0x40},
        {0x11, 0x00},
        {0x17, 0x11},
        {0x18, 0x43},
        {0x19, 0x00},
        {0x1a, 0x4b},
        {0x32, 0x09},
        {0x37, 0xc0},
        {0x4f, 0xca},
        {0x50, 0xa8},
        {0x5a, 0x23},
        {0x6d, 0x00},
        {0x3d, 0x38},
        {0x39, 0x92},
        {0x35, 0xda},
        {0x22, 0x1a},
        {0x37, 0xc3},
        {0x23, 0x00},
        {0x34, 0xc0},
        {0x36, 0x1a},
        {0x06, 0x88},
        {0x07, 0xc0},
        {0x0d, 0x87},
        {0x0e, 0x41},
        {0x4c, 0x00},
        {0x48, 0x00},
        {0x5B, 0x00},
        {0x42, 0x03},
        {0x5a, 0x23},
        {0x4f, 0xca},
        {0x50, 0xa8},
        {0x46, 0x68},
        {0x47, 0x01},
        {0xff, 0x00},
        {0xe0, 0x04},
        {0xc0, 0x64},
        {0xc1, 0x4B},
        {0x8c, 0x00},
        {0x86, 0x3D},
        {0x50, 0x00},
        {0x51, 0xC8},
        {0x52, 0x96},
        {0x53, 0x00},
        {0x54, 0x00},
        {0x55, 0x00},
        {0x5a, 0xA0},
        {0x5b, 0x78},
        {0x5c, 0x00},
        {0xd3, 0x02},
        {0xe0, 0x00},
        {0xff, 0xff}
    },
};   

LOCAL uint32 set_ov2640_video_mode(uint32 mode)
{
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr = PNULL;


    if(CHIP_DetectMemType())
        sensor_reg_ptr = (SENSOR_REG_T*)ov2640_video_mode_nand_tab[mode];
    else
        sensor_reg_ptr = (SENSOR_REG_T*)ov2640_video_mode_nor_tab[mode];

    SCI_ASSERT(PNULL != sensor_reg_ptr);   /*assert verified*/

    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr)|| (0xFF != sensor_reg_ptr[i].reg_value); i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }

    SCI_Sleep(300); 
    //SCI_TRACE_LOW:"SENSOR: set_vodeo_mode: mode : %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_OV2640_960_112_2_18_0_31_28_1017,(uint8*)"d",mode);

    return 0;
}
/******************************************************************************/
// Description: set wb mode 
// Global resource dependence: 
// Author:
// Note:
//      
/******************************************************************************/
__align(4) LOCAL const SENSOR_REG_T ov2640_awb_tab[][7]=

{
    //AUTO
    {
        {0xff, 0x00},
        {0xc7, 0x00},
        {0xff, 0xff},       
        {0xff, 0xff},    
        {0xff, 0xff},    
        {0xff, 0xff},
        {0xff, 0xff}
    },    
    //INCANDESCENCE:
    {
        {0xff, 0x00},
        {0xc3, 0xed},           
        {0xc7, 0x40},
        {0xcc, 0x68}, //{0xcc, 0x46},
        {0xcd, 0x60}, //{0xcd, 0x41},
        {0xce, 0xa0}, //{0xce, 0x60},
        {0xff, 0xff} 
    },
    //U30
    {
        {0xff, 0x00},
     {0xce, 0x54},          
        {0xc7, 0x40},
        {0xcc, 0x42},       
        {0xcd, 0x3f},    
        {0xce, 0x71},             
        {0xff, 0xff}        
    },  
    //CWF
    {
        {0xff, 0x00},
     {0xce, 0x54},          
        {0xc7, 0x40},
        {0xcc, 0x52},
        {0xcd, 0x41},
        {0xce, 0x66},      
        {0xff, 0xff}                  
    },    
    //FLUORESCENT:
    {
     {0xff, 0x00},
     {0xc3, 0xed},      
        {0xc7, 0x40},
        {0xcc, 0x7c}, //{0xcc, 0x5e},
        {0xcd, 0x60}, //{0xcd, 0x41},
        {0xce, 0x94}, //{0xce, 0x54},  
        {0xff, 0xff}        
    },
    //SUN:
    {
        {0xff, 0x00},
     {0xc3, 0xed},          
        {0xc7, 0x40},
        {0xcc, 0x90}, //{0xcc, 0x68},
        {0xcd, 0x60}, //{0xcd, 0x41},
        {0xce, 0x6e}, //{0xce, 0x4c},  
        {0xff, 0xff}        
    },
    //CLOUD:
    {
        {0xff, 0x00},
     {0xc3, 0xed},          
        {0xc7, 0x40},
        {0xcc, 0x9c}, //{0xcc, 0x65},
        {0xcd, 0x60}, //{0xcd, 0x41},
        {0xce, 0x6e}, //{0xce, 0x4f},
        {0xff, 0xff}
    },
};

LOCAL uint32 set_ov2640_awb(uint32 mode)
{
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)ov2640_awb_tab[mode];

    SCI_ASSERT(mode < 7);                   /*assert to do*/
    SCI_ASSERT(PNULL != sensor_reg_ptr);   /*assert verified*/
    
    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    SCI_Sleep(20); 
    //SCI_TRACE_LOW:"SENSOR: set_awb_mode: mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_OV2640_1057_112_2_18_0_31_29_1018,(uint8*)"d", mode);
    
    return 0;
}

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//      
/******************************************************************************/
__align(4) LOCAL const SENSOR_REG_T ov2640_ev_tab[][4]=
{
    {{0xff, 0x01}, {0x24, 0x18}, {0x25, 0x08}, {0x26, 0x60}},
    {{0xff, 0x01}, {0x24, 0x28}, {0x25, 0x18}, {0x26, 0x60}},
    {{0xff, 0x01}, {0x24, 0x38}, {0x25, 0x28}, {0x26, 0x81}},
    {{0xff, 0x01}, {0x24, 0x48}, {0x25, 0x38}, {0x26, 0xa2}},
    {{0xff, 0x01}, {0x24, 0x70}, {0x25, 0x60}, {0x26, 0xb4}},
    {{0xff, 0x01}, {0x24, 0x90}, {0x25, 0x80}, {0x26, 0xc6}},
    {{0xff, 0x01}, {0x24, 0xc0}, {0x25, 0xa8}, {0x26, 0xc6}}    //Pan Lixiang   2006-10-11   
};

__align(4) LOCAL const SENSOR_REG_T ov2640_ev_tab_night[][4]=
{
    {{0xff, 0x01}, {0x24, 0x18}, {0x25, 0x08}, {0x26, 0x61}},
    {{0xff, 0x01}, {0x24, 0x28}, {0x25, 0x18}, {0x26, 0x71}},
    {{0xff, 0x01}, {0x24, 0x38}, {0x25, 0x28}, {0x26, 0x92}},
    {{0xff, 0x01}, {0x24, 0x48}, {0x25, 0x38}, {0x26, 0xb2}},
    {{0xff, 0x01}, {0x24, 0x70}, {0x25, 0x60}, {0x26, 0xc4}},
    {{0xff, 0x01}, {0x24, 0x90}, {0x25, 0x80}, {0x26, 0xd6}},
    {{0xff, 0x01}, {0x24, 0xc0}, {0x25, 0xa8}, {0x26, 0xd6}}    //Pan Lixiang   2006-10-11   
};



LOCAL uint32 set_ov2640_ev(uint32 level)
{
    uint16 i;    
    SENSOR_REG_T* sensor_reg_ptr =  PNULL;


    if(DCAMERA_ENVIRONMENT_NORMAL == s_preview_mode)
    {
        sensor_reg_ptr = (SENSOR_REG_T*)ov2640_ev_tab[level];
    }
    else
    {
        sensor_reg_ptr = (SENSOR_REG_T*)ov2640_ev_tab_night[level];
    }

    SCI_ASSERT(PNULL != sensor_reg_ptr);   /*assert verified*/
    SCI_ASSERT(level < 7);  /*assert to do*/

    for(i = 0; i < 4 /*(0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value)*/; i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    SCI_Sleep(200); 

    //SCI_TRACE_LOW:"SENSOR: set_ev: level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_OV2640_1115_112_2_18_0_31_29_1019,(uint8*)"d", level);

    return 0;
}

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//      
/******************************************************************************/
LOCAL uint32 OV2640_set_hmirror_enable(uint32 enable)
{
    uint16 reg04;

    Sensor_WriteReg(0xff, 0x01);
    reg04 = Sensor_ReadReg(0x04);
    
    if (enable)
    {
        reg04 = (reg04 | 0x80);
        Sensor_WriteReg(0xff, 0x01);
        Sensor_WriteReg(0x04, reg04);
    }
    else
    {
        reg04 = (reg04 & (~(0x80)));
        Sensor_WriteReg(0xff, 0x01);
        Sensor_WriteReg(0x04, reg04);
    }
    return 0;
}

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//      
/******************************************************************************/
LOCAL uint32 OV2640_set_vmirror_enable(uint32 enable)
{
    uint16 reg04;
    
    Sensor_WriteReg(0xff, 0x01);
    reg04 = Sensor_ReadReg(0x04);
    
    if (enable)
    {
        reg04 = (reg04 | 0x50);
        Sensor_WriteReg(0xff, 0x01);
        Sensor_WriteReg(0x04, reg04);
    }
    else
    {
        reg04 = (reg04 & (~(0x50)));
        Sensor_WriteReg(0xff, 0x01);
        Sensor_WriteReg(0x04, reg04);
    }
    return 0;
}

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//      
/******************************************************************************/
LOCAL uint32 OV2640_set_preview_mode(uint32 preview_mode)
{
    //SCI_TRACE_LOW:"set_preview_mode: preview_mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_OV2640_1181_112_2_18_0_31_29_1020,(uint8*)"d", preview_mode);
    
    s_preview_mode = preview_mode;
    
    switch (preview_mode)
    {
        case DCAMERA_ENVIRONMENT_NORMAL: 
        {
            OV2640_set_work_mode(0);
            break;
        }
        case DCAMERA_ENVIRONMENT_NIGHT:
        {
            OV2640_set_work_mode(1);
            break;
        }
        case DCAMERA_ENVIRONMENT_SUNNY:
        {
            OV2640_set_work_mode(0);
            break;
        }
        default:
        {
            break;
        }
            
    }
    
//  SCI_Sleep(250);
    
    return 0;
}

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//      
/******************************************************************************/
LOCAL uint32 OV2640_Identify(uint32 param)
{
#define OV2640_PID_VALUE    0x26    
#define OV2640_PID_ADDR     0x0A
#define OV2640_VER_VALUE    0x40    
#define OV2640_VER_ADDR     0x0B
        
    uint8 pid_value = 0;
    uint8 ver_value = 0;
    BOOLEAN ret_value = 0XFF;

    pid_value = Sensor_ReadReg(OV2640_PID_ADDR);

    if(OV2640_PID_VALUE == pid_value)
    {
        // Get Device Code
        ver_value = Sensor_ReadReg(OV2640_VER_ADDR);
        
        if(OV2640_VER_VALUE == ver_value)
        {
            //SCI_TRACE_LOW:"That is OV2640 sensor !"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_OV2640_1239_112_2_18_0_31_29_1021,(uint8*)"");
        }
        else
        {
            //SCI_TRACE_LOW:"OV2640_Identify: That is OV%x%x sensor !"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_OV2640_1243_112_2_18_0_31_29_1022,(uint8*)"dd", pid_value, ver_value);
        }
        ret_value = 0;
    }

    //SCI_TRACE_LOW:"OV2640_Identify: PID = %x, VER = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_OV2640_1249_112_2_18_0_31_29_1023,(uint8*)"dd",pid_value, ver_value);
    
    return (uint32)ret_value;
}

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//      
/******************************************************************************/
LOCAL void OV2640_CalculateExposureGain(SENSOR_MODE_E sensor_preview_mode, SENSOR_MODE_E sensor_snapshot_mode)

{
/*  uint16  PrvFps = 14.28, TgtFps = 10,*/
    uint16  TgtMaxExp =1248;
/*  uint16  PrvMaxExp = 1248;*/
    uint16  PrvExp,TgtExp = 0;
    uint8   PrvGain,val45,val10,val04;
    
    uint32  preview_xclk = s_OV2640_resolution_Tab_YUV[sensor_preview_mode].xclk_to_sensor;
    uint32  snapshot_xclk = s_OV2640_resolution_Tab_YUV[sensor_snapshot_mode].xclk_to_sensor;

    SCI_ASSERT(preview_xclk && snapshot_xclk);   /*assert verified*/

 // 1. Close AE/AG(0x13 register).

     Sensor_WriteReg(0xff,0x01);
     Sensor_WriteReg(0x13,0xe0);
     
 // 2. Read out gain/expsoure values, 0x00 and 0x10,0x04,0x45 register, (PrvGain, PrvExp)

 // 3. Change resolution and frame to capture mode

 // 4 . Calculate exposure/gain

     PrvGain = Sensor_ReadReg(0x00);
     PrvExp = Sensor_ReadReg(0x10) <<2;
     PrvExp +=  (Sensor_ReadReg(0x04) & 0x03); 
     PrvExp += (Sensor_ReadReg(0x45) & 0x3f) << 10;

       //SCI_TRACE_LOW:"ov2640: prvGain %d, prvExp %d"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_OV2640_1287_112_2_18_0_31_29_1024,(uint8*)"dd", PrvGain, PrvExp);
    
       TgtExp = (PrvExp * snapshot_xclk / preview_xclk ) * (2* 4 * 1190) / (3 * 1922); /*lint !e414*/
    
                
    if(TgtExp <= (TgtMaxExp/2))
    {
        if((PrvGain&0x10) == 0x10)
        {
            TgtExp = TgtExp * 2;
            PrvGain = PrvGain & 0xef;
        }
    }
        
    if(TgtExp <= (TgtMaxExp/2))
    {
        if((PrvGain&0x0f) >= 0x0b)
        {
            TgtExp = TgtExp * 2;
            PrvGain = PrvGain & 0xf0;
        }
    }

       //SCI_TRACE_LOW:"ov2640: PrvGain %d, TgtExp %d"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_OV2640_1310_112_2_18_0_31_29_1025,(uint8*)"dd", PrvGain, TgtExp);

    val45 =( (TgtExp>>10) | ((Sensor_ReadReg(0x45) & 0xc0)));   //   val45 = (TgtExp>>10);
    val10 = (TgtExp>>2);
    val04 =( Sensor_ReadReg(0x04)&0xfc);
    val04= (val04|(TgtExp&0x03));
        
    Sensor_WriteReg(0x45,val45);
    Sensor_WriteReg(0x10,val10);
    Sensor_WriteReg(0x04,val04);
    Sensor_WriteReg(0x00,PrvGain);


    

}

LOCAL uint32 OV2640_BeforeSnapshot(uint32 param)
{
    uint32  preview_mode = (param >= SENSOR_MODE_PREVIEW_TWO) ? \
                            SENSOR_MODE_PREVIEW_TWO:SENSOR_MODE_PREVIEW_ONE;


    if(param > SENSOR_MODE_PREVIEW_ONE)
    {
        OV2640_CalculateExposureGain(preview_mode, param);
    }

    SCI_Sleep(200);
    
    return 0;
}

/******************************************************************************/
// Description: set brightness 
// Global resource dependence: 
// Author:
// Note:
//      level  must smaller than 8
/******************************************************************************/
__align(4) LOCAL const SENSOR_REG_T ov2640_brightness_tab[][6]=
{
    {//level 1
        {0xff, 0x00},{0x7c, 0x00},{0x7d, 0x04},{0x7c, 0x09},{0x7d, 0x24},{0x7d, 0x0a}   
    },
    {//level 2
        {0xff, 0x00},{0x7c, 0x00},{0x7d, 0x04},{0x7c, 0x09},{0x7d, 0x18},{0x7d, 0x0a}
    },
    {//level 3
        {0xff, 0x00},{0x7c, 0x00},{0x7d, 0x04},{0x7c, 0x09},{0x7d, 0x0c},{0x7d, 0x0a}   
    },
    {//level 4
        {0xff, 0x00},{0x7c, 0x00},{0x7d, 0x04},{0x7c, 0x09},{0x7d, 0x00},{0x7d, 0x0a}   
    },
    {//level 5
        {0xff, 0x00},{0x7c, 0x00},{0x7d, 0x04},{0x7c, 0x09},{0x7d, 0x10},{0x7d, 0x02}
    },
    {//level 6
        {0xff, 0x00},{0x7c, 0x00},{0x7d, 0x04},{0x7c, 0x09},{0x7d, 0x20},{0x7d, 0x02}
    },                             
    {//level 7
        {0xff, 0x00},{0x7c, 0x00},{0x7d, 0x04},{0x7c, 0x09},{0x7d, 0x30},{0x7d, 0x02}   
        
    }        
};


__align(4) LOCAL const SENSOR_REG_T ov2640_brightness_tab_unnormal[][6]= //in case of current effect equals unnormal except for negative
{
    {//level 1
        {0xff, 0x00},{0x7c, 0x00},{0x7d, 0x1c},{0x7c, 0x09},{0x7d, 0x24},{0x7d, 0x0a}   
    },
    {//level 2
        {0xff, 0x00},{0x7c, 0x00},{0x7d, 0x1c},{0x7c, 0x09},{0x7d, 0x18},{0x7d, 0x0a}
    },
    {//level 3
        {0xff, 0x00},{0x7c, 0x00},{0x7d, 0x1c},{0x7c, 0x09},{0x7d, 0x0c},{0x7d, 0x0a}   
    },
    {//level 4
        {0xff, 0x00},{0x7c, 0x00},{0x7d, 0x1c},{0x7c, 0x09},{0x7d, 0x00},{0x7d, 0x0a}   
    },
    {//level 5
        {0xff, 0x00},{0x7c, 0x00},{0x7d, 0x1c},{0x7c, 0x09},{0x7d, 0x10},{0x7d, 0x02}
    },
    {//level 6
        {0xff, 0x00},{0x7c, 0x00},{0x7d, 0x1c},{0x7c, 0x09},{0x7d, 0x20},{0x7d, 0x02}
    },                             
    {//level 7
        {0xff, 0x00},{0x7c, 0x00},{0x7d, 0x1c},{0x7c, 0x09},{0x7d, 0x30},{0x7d, 0x02}   
        
    }        
};

__align(4) LOCAL const SENSOR_REG_T ov2640_brightness_tab_negative[][6]= //in case of  current effect equels  negative
{
    {//level 1
        {0xff, 0x00},{0x7c, 0x00},{0x7d, 0x44},{0x7c, 0x09},{0x7d, 0x24},{0x7d, 0x0a}   
    },
    {//level 2
        {0xff, 0x00},{0x7c, 0x00},{0x7d, 0x44},{0x7c, 0x09},{0x7d, 0x18},{0x7d, 0x0a}
    },
    {//level 3
        {0xff, 0x00},{0x7c, 0x00},{0x7d, 0x44},{0x7c, 0x09},{0x7d, 0x0c},{0x7d, 0x0a}   
    },
    {//level 4
        {0xff, 0x00},{0x7c, 0x00},{0x7d, 0x44},{0x7c, 0x09},{0x7d, 0x00},{0x7d, 0x0a}   
    },
    {//level 5
        {0xff, 0x00},{0x7c, 0x00},{0x7d, 0x44},{0x7c, 0x09},{0x7d, 0x10},{0x7d, 0x02}
    },
    {//level 6
        {0xff, 0x00},{0x7c, 0x00},{0x7d, 0x44},{0x7c, 0x09},{0x7d, 0x20},{0x7d, 0x02}
    },                             
    {//level 7
        {0xff, 0x00},{0x7c, 0x00},{0x7d, 0x44},{0x7c, 0x09},{0x7d, 0x30},{0x7d, 0x02}   
        
    }        
};

LOCAL uint32 OV2640_set_brightness(uint32 level)
{
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr;

    SCI_ASSERT(level < 7);   /*assert verified*/

    if(DCAMERA_EFFECT_NORMAL == s_image_effect)//normal effect
    {
        sensor_reg_ptr = (SENSOR_REG_T*)ov2640_brightness_tab[level];
    }
    else if(DCAMERA_EFFECT_NEGATIVE == s_image_effect) //negative effect
    {
        sensor_reg_ptr = (SENSOR_REG_T*)ov2640_brightness_tab_negative[level];
    }
    else // unnormal effect except for negative
    {
        sensor_reg_ptr = (SENSOR_REG_T*)ov2640_brightness_tab_unnormal[level];
    }

    SCI_ASSERT(PNULL != sensor_reg_ptr);   /*assert verified*/
    
    for(i = 0; i < 6 ; i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    SCI_Sleep(20); 
    //SCI_TRACE_LOW:"set_brightness: level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_OV2640_1454_112_2_18_0_31_29_1026,(uint8*)"d", level);
    return 0;
}

/******************************************************************************/
// Description: set contrast
// Global resource dependence: 
// Author:
// Note:
//      level must smaller than 9
/******************************************************************************/
__align(4) LOCAL const SENSOR_REG_T ov2640_contrast_tab_negative[][5]=
{
    {//level 1
        {0xff, 0x00}, {0x7c, 0x00}, {0x7d, 0x44}, {0x7c, 0x08},{0x7d, 0x0e}
    },
    {//level 2
        {0xff, 0x00}, {0x7c, 0x00}, {0x7d, 0x44}, {0x7c, 0x08},{0x7d, 0x14}
    },
    {//level 3
        {0xff, 0x00}, {0x7c, 0x00}, {0x7d, 0x44}, {0x7c, 0x08},{0x7d, 0x1a}
    },
    {//level 4
        {0xff, 0x00}, {0x7c, 0x00}, {0x7d, 0x44}, {0x7c, 0x08},{0x7d, 0x20}
    },
    {//level 5
        {0xff, 0x00}, {0x7c, 0x00}, {0x7d, 0x44}, {0x7c, 0x08},{0x7d, 0x28}
    },
    {//level 6
        {0xff, 0x00}, {0x7c, 0x00}, {0x7d, 0x44}, {0x7c, 0x08},{0x7d, 0x30}
    },
    {//level 7
        {0xff, 0x00}, {0x7c, 0x00}, {0x7d, 0x44}, {0x7c, 0x08},{0x7d, 0x38}
    },
};
__align(4) LOCAL const SENSOR_REG_T ov2640_contrast_tab_unnormal[][5]=
{
    {//level 1
        {0xff, 0x00}, {0x7c, 0x00}, {0x7d, 0x1c}, {0x7c, 0x08},{0x7d, 0x0e}
    },
    {//level 2
        {0xff, 0x00}, {0x7c, 0x00}, {0x7d, 0x1c}, {0x7c, 0x08},{0x7d, 0x14}
    },
    {//level 3
        {0xff, 0x00}, {0x7c, 0x00}, {0x7d, 0x1c}, {0x7c, 0x08},{0x7d, 0x1a}
    },
    {//level 4
        {0xff, 0x00}, {0x7c, 0x00}, {0x7d, 0x1c}, {0x7c, 0x08},{0x7d, 0x20}
    },
    {//level 5
        {0xff, 0x00}, {0x7c, 0x00}, {0x7d, 0x1c}, {0x7c, 0x08},{0x7d, 0x28}
    },
    {//level 6
        {0xff, 0x00}, {0x7c, 0x00}, {0x7d, 0x1c}, {0x7c, 0x08},{0x7d, 0x30}
    },
    {//level 7
        {0xff, 0x00}, {0x7c, 0x00}, {0x7d, 0x1c}, {0x7c, 0x08},{0x7d, 0x38}
    },
};
__align(4) LOCAL const SENSOR_REG_T ov2640_contrast_tab[][5]=
{
    {//level 1
        {0xff, 0x00}, {0x7c, 0x00}, {0x7d, 0x04}, {0x7c, 0x08},{0x7d, 0x0e}
    },
    {//level 2
        {0xff, 0x00}, {0x7c, 0x00}, {0x7d, 0x04}, {0x7c, 0x08},{0x7d, 0x14}
    },
    {//level 3
        {0xff, 0x00}, {0x7c, 0x00}, {0x7d, 0x04}, {0x7c, 0x08},{0x7d, 0x1a}
    },
    {//level 4
        {0xff, 0x00}, {0x7c, 0x00}, {0x7d, 0x04}, {0x7c, 0x08},{0x7d, 0x20}
    },
    {//level 5
        {0xff, 0x00}, {0x7c, 0x00}, {0x7d, 0x04}, {0x7c, 0x08},{0x7d, 0x28}
    },
    {//level 6
        {0xff, 0x00}, {0x7c, 0x00}, {0x7d, 0x04}, {0x7c, 0x08},{0x7d, 0x30}
    },
    {//level 7
        {0xff, 0x00}, {0x7c, 0x00}, {0x7d, 0x04}, {0x7c, 0x08},{0x7d, 0x38}
    },
};
LOCAL uint32 OV2640_set_contrast(uint32 level)
{
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr;// = (SENSOR_REG_T*)ov2640_contrast_tab[level];

    SCI_ASSERT(level < 7 );   /*assert verified*/

       if(DCAMERA_EFFECT_NORMAL == s_image_effect)
       {
           sensor_reg_ptr = (SENSOR_REG_T*)ov2640_contrast_tab[level];
       }
       else if(DCAMERA_EFFECT_NEGATIVE == s_image_effect)
       {
           sensor_reg_ptr = (SENSOR_REG_T*)ov2640_contrast_tab_negative[level];
       }
       else
       {
           sensor_reg_ptr = (SENSOR_REG_T*)ov2640_contrast_tab_unnormal[level];
       }
    SCI_ASSERT(PNULL != sensor_reg_ptr);   /*assert verified*/
    
    for(i = 0; i < 5; i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    SCI_Sleep(20);
    //SCI_TRACE_LOW:"set_contrast: level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_OV2640_1561_112_2_18_0_31_30_1027,(uint8*)"d", level);
    return 0;
}

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//      
/******************************************************************************/
__align(4) LOCAL const SENSOR_REG_T ov2640_sharpness_tab[][3]=
{
    //weakest
    {{0xff, 0x00}, {0x92, 0x01}, {0x93, 0xc0}},
    {{0xff, 0x00}, {0x92, 0x01}, {0x93, 0xc1}},
    {{0xff, 0x00}, {0x92, 0x01}, {0x93, 0xc3}},
    {{0xff, 0x00}, {0x92, 0x01}, {0x93, 0xc5}},
    //strongest
    {{0xff, 0x00}, {0x92, 0x01}, {0x93, 0xc7}}
};

LOCAL uint32 OV2640_set_sharpness(uint32 level)
{
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)ov2640_sharpness_tab[level];
    
    SCI_ASSERT(level <= 4 );   /*assert to do*/
//  SCI_ASSERT(PNULL != sensor_reg_ptr);   /*assert verified*/
    
    for(i = 0; i < 3/*(0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value)*/ ; i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    //SCI_TRACE_LOW:"set_sharpness: level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_OV2640_1593_112_2_18_0_31_30_1028,(uint8*)"d", level);
    return 0;
}

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//      
/******************************************************************************/
__align(4) LOCAL const SENSOR_REG_T ov2640_saturation_tab[][6]=
{
    // level 0
    { 
        {0xff, 0x00}, {0x7c, 0x00}, {0x7d, 0x02}, {0x7c, 0x03}, {0x7d, 0x00}, {0x7d, 0x00} 
    },
    //level 1
    { 
        {0xff, 0x00}, {0x7c, 0x00}, {0x7d, 0x02}, {0x7c, 0x03}, {0x7d, 0x10}, {0x7d, 0x10} 
    },
    //level 2
    { 
        {0xff, 0x00}, {0x7c, 0x00}, {0x7d, 0x02}, {0x7c, 0x03}, {0x7d, 0x20}, {0x7d, 0x20} 
    },
    //level 3
    { 
        {0xff, 0x00}, {0x7c, 0x00}, {0x7d, 0x02}, {0x7c, 0x03}, {0x7d, 0x30}, {0x7d, 0x30} 
    },
    //level 4
    { 
        {0xff, 0x00}, {0x7c, 0x00}, {0x7d, 0x02}, {0x7c, 0x03}, {0x7d, 0x40}, {0x7d, 0x40} 
    },
    //level 5
    { 
        {0xff, 0x00}, {0x7c, 0x00}, {0x7d, 0x02}, {0x7c, 0x03}, {0x7d, 0x50}, {0x7d, 0x50} 
    },
    //level 6
    { 
        {0xff, 0x00}, {0x7c, 0x00}, {0x7d, 0x02}, {0x7c, 0x03}, {0x7d, 0x60}, {0x7d, 0x60}
    },
    //level 7
    { 
        {0xff, 0x00}, {0x7c, 0x00}, {0x7d, 0x02}, {0x7c, 0x03}, {0x7d, 0x70}, {0x7d, 0x70} 
    },
    //level 8
    { 
        {0xff, 0x00}, {0x7c, 0x00}, {0x7d, 0x02}, {0x7c, 0x03}, {0x7d, 0x80}, {0x7d, 0x80} 
    }
};
LOCAL uint32 OV2640_set_saturation(uint32 level)
{
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)ov2640_saturation_tab[level];
    
    SCI_ASSERT(level <= 8 );   /*assert to do*/
//  SCI_ASSERT(PNULL != sensor_reg_ptr);   /*assert verified*/
    
    for(i = 0; i < 6/*(0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value)*/ ; i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    //SCI_TRACE_LOW:"set_saturation: level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_OV2640_1653_112_2_18_0_31_30_1029,(uint8*)"d", level);
    return 0;
}

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//      
/******************************************************************************/
__align(4) LOCAL const SENSOR_REG_T ov2640_image_effect_tab[][7]=
{
    // effect normal
    {
        {0xff, 0x00}, {0x7c, 0x00}, {0x7d, 0x04}, {0x7c, 0x05}, {0x7d, 0x80}, {0x7d, 0x80}, {0xff, 0xff}
    },
    //effect BLACKWHITE
    {
        {0xff, 0x00}, {0x7c, 0x00}, {0x7d, 0x1c}, {0x7c, 0x05}, {0x7d, 0x80}, {0x7d, 0x80}, {0xff, 0xff}
    },
    // effect RED
    {
    //  {0xff, 0x00}, {0x7c, 0x00}, {0x7d, 0x18}, {0x7c, 0x05}, {0x7d, 0x40}, {0x7d, 0xc0}
        {0xff, 0x00}, {0x7c, 0x00}, {0x7d, 0x1c}, {0x7c, 0x05}, {0x7d, 0x80}, {0x7d, 0xb0}, {0xff, 0xff}
    },
        
    // effect GREEN
    {
        {0xff, 0x00}, {0x7c, 0x00}, {0x7d, 0x1c}, {0x7c, 0x05}, {0x7d, 0x40}, {0x7d, 0x40}, {0xff, 0xff}
    },
        
    // effect  BLUE
    {
        {0xff, 0x00}, {0x7c, 0x00}, {0x7d, 0x1c}, {0x7c, 0x05}, {0x7d, 0xa0}, {0x7d, 0x40}, {0xff, 0xff}
    },
    //effect YELLOW
    {
        {0xff, 0x00}, {0x7c, 0x00}, {0x7d, 0x1c}, {0x7c, 0x05}, {0x7d, 0x20}, {0x7d, 0x90}, {0xff, 0xff}
    },
    // effect NEGATIVE
    {
        {0xff, 0x00}, {0x7c, 0x00}, {0x7d, 0x44}, {0x7c, 0x05}, {0x7d, 0x80}, {0x7d, 0x80}, {0xff, 0xff}
    },
    // effect CANVAS ANTIQUE
    {
        {0xff, 0x00}, {0x7c, 0x00}, {0x7d, 0x1c}, {0x7c, 0x05}, {0x7d, 0x40}, {0x7d, 0xa6}, {0xff, 0xff}
    },
    

};
LOCAL uint32 OV2640_set_image_effect(uint32 effect_type)
{
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)ov2640_image_effect_tab[effect_type];
    
    s_image_effect = effect_type;
    
    SCI_ASSERT(PNULL != sensor_reg_ptr);   /*assert verified*/
    
    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    //SCI_TRACE_LOW:"set_image_effect: effect_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_OV2640_1715_112_2_18_0_31_30_1030,(uint8*)"d", effect_type);
    return 0;
}

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//      mode 0:normal;   1:night 
/******************************************************************************/
__align(4) LOCAL const SENSOR_REG_T ov2640_work_mode_tab[][6]=
{
    
    {//normal fix 15fps
            {0xff, 0x01},
            {0x03, 0x0f},
            {0x0f, 0x43},
            {0x2d, 0x00},
            {0x2e, 0x00},
            {0xff, 0xff}
                
    },
    {//night 7.5fps-15fps
            {0xff, 0x01},
            {0x03, 0x4f},
            {0x0f, 0x4b},
            {0x2d, 0x00}, 
            {0x2e, 0x00},
            {0xff, 0xff}
    }
};
/*lint -restore */

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//      mode 0:normal;   1:night 
/******************************************************************************/
LOCAL uint32 OV2640_set_work_mode(uint32 mode)
{
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)ov2640_work_mode_tab[mode];
    
    SCI_ASSERT(mode <= 1);   /*assert to do*/
//  SCI_ASSERT(PNULL != sensor_reg_ptr);  /*assert verified*/
    
    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
//  SCI_Sleep(50);
    //SCI_TRACE_LOW:"set_work_mode: mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_OV2640_1764_112_2_18_0_31_30_1031,(uint8*)"d", mode);
    return 0;
}

LOCAL uint32 OV2640_after_snapshot(uint32 param)
{
    //Sensor_SendRegTabToSensor(&s_OV3640_resolution_Tab_YUV[0]);
    OV2640_chang_image_format(SENSOR_IMAGE_FORMAT_YUV422);
    
    return SCI_SUCCESS;
}

LOCAL uint32 OV2640_chang_image_format(uint32 param)
{
    SENSOR_REG_TAB_INFO_T st_yuv422_reg_table_info = { ADDR_AND_LEN_OF_ARRAY(ov2640_YUV_COMMON),0,0,0,0};
    uint32 ret_val = SCI_ERROR;

    switch(param){
    case SENSOR_IMAGE_FORMAT_YUV422:
        ret_val = Sensor_SendRegTabToSensor(&st_yuv422_reg_table_info);
        break;

    case SENSOR_IMAGE_FORMAT_JPEG:
        ret_val = SCI_ERROR;
        break;

    default:
        SCI_PASSERT(0, ("OV3640 only support SENSOR_IMAGE_FORMAT_JPEG & SENSOR_IMAGE_FORMAT_YUV422, input is %d", param));  /*assert to do*/
        break;
    }

    return ret_val;
}

LOCAL uint32 OV2640_check_image_format_support(uint32 param) 
{
    uint32 ret_val = SCI_ERROR;

    switch(param){
    case SENSOR_IMAGE_FORMAT_YUV422:
        ret_val = SCI_SUCCESS;
        break;

    case SENSOR_IMAGE_FORMAT_JPEG:
        ret_val = SCI_ERROR;//SCI_SUCCESS;
        break;

    default:
        SCI_PASSERT(0, ("OV3640 only support SENSOR_IMAGE_FORMAT_JPEG & SENSOR_IMAGE_FORMAT_YUV422, input is %d", param));  /*assert to do*/
        break;
    }

    return ret_val;
}

LOCAL uint32 OV2640_pick_out_jpeg_stream(uint32 param)
{
    uint8* p_frame = ((DCAMERA_SNAPSHOT_RETURN_PARAM_T*)param)->return_data_addr;
    uint32 buf_len = ((DCAMERA_SNAPSHOT_RETURN_PARAM_T*)param)->return_data_len;
    uint32 i = 0;
    
    //SCI_TRACE_LOW:"ov3640: jpeg capture head: 0x%x, 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_OV2640_1825_112_2_18_0_31_30_1032,(uint8*)"dd", *((uint8*)p_frame), *((uint8*)p_frame + 1));

    /* Find the tail position */
    for(i = 0; i<buf_len; i++){
        #define TAIL_VAL 0xffd9
        uint8* p_cur_val = (uint8*)p_frame;
        
        uint16 tail_val = ((p_cur_val[i]<<8) | p_cur_val[i+1]);
        
        if(TAIL_VAL == tail_val){
            i += 2;
            break;
        }
    }

    /* check if the tail is found */
    if(i < buf_len){
        //SCI_TRACE_LOW:"ov3640: Found the jpeg tail at %d: 0x%x 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_OV2640_1842_112_2_18_0_31_30_1033,(uint8*)"ddd", i+1, *((uint8*)p_frame + i), *((uint8*)p_frame + i + 1));
    }
    else{
        //SCI_TRACE_LOW:"ov3640: can not find the jpeg tail: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_OV2640_1845_112_2_18_0_31_30_1034,(uint8*)"d", i);
        i = 0;
    }

    return i;
}

__align(4) LOCAL const SENSOR_REG_T ov2640_zoom_tab[][16]=
{
    //1280x960
    {
         {0xff, 0x00},//level 2
         {0xc0,0xc8},
         {0xc1,0x96},
         {0x8c,0x00},
         {0x86,0x3d},
         {0x50,0x89},
         {0x51,0x5e},
         {0x52,0x07},
         {0x53,0x64},
         {0x54,0x4a},
         {0x55,0x88},
         {0x5a,0xa0},
         {0x5b,0x78},
         {0x5c,0x00},
         {0xd3,0x82},
         {0xff, 0xff}
                
    },
    {
         //level 3
         {0xff, 0x00},//level 3
         {0xc0,0xc8},
         {0xc1,0x96},
         {0x8c,0x00},
         {0x86,0x3d},
         {0x50,0x00},
         {0x51,0x2c},
         {0x52,0xe1},
         {0x53,0xc8},
         {0x54,0x96},
         {0x55,0x08},
         {0x5a,0xa0},
         {0x5b,0x7b},
         {0x5c,0x00},
         {0xd3,0x82},
         {0xff, 0xff}
        },
    {
         //level 4
           {0xff, 0x00},//level 3
         {0xc0,0xc8},
         {0xc1,0x96},
         {0x8c,0x00},
         {0x86,0x3d},
         {0x50,0x00},
         {0x51,0xfa},
         {0x52,0xbc},
         {0x53,0x2c},
         {0x54,0xe0},
         {0x55,0x01},
         {0x5a,0xa0},
         {0x5b,0x78},
         {0x5c,0x00},
         {0xd3,0x82},
         {0xff, 0xff}
    },
    {
         //level 5
             {0xff, 0x00},//level 3
         {0xc0,0xc8},
         {0xc1,0x96},
         {0x8c,0x00},
         {0x86,0x3d},
         {0x50,0x00},
         {0x51,0xc8},
         {0x52,0x96},
         {0x53,0x90},
         {0x54,0x2c},
         {0x55,0x11},
         {0x5a,0xa0},
         {0x5b,0x78},
         {0x5c,0x00},
         {0xd3,0x82},
         {0xff, 0xff}
    },

    //1600X1200
    {
         {0xff, 0x00},//level 2
         {0xc0,0xc8},
         {0xc1,0x96},
         {0x8c,0x00},
         {0x86,0x3d},
         {0x50,0x00},
         {0x51,0x5e},
         {0x52,0x07},
         {0x53,0x64},
         {0x54,0x4a},
         {0x55,0x88},
         {0x5a,0xc8},
         {0x5b,0x96},
         {0x5c,0x00},
         {0xd3,0x82},
         {0xff, 0xff}
                
    },
    {
         //level 3
         {0xff, 0x00},//level 3
         {0xc0,0xc8},
         {0xc1,0x96},
         {0x8c,0x00},
         {0x86,0x3d},
         {0x50,0x00},
         {0x51,0x2c},
         {0x52,0xe1},
         {0x53,0xc8},
         {0x54,0x96},
         {0x55,0x08},
         {0x5a,0xc8},
         {0x5b,0x96},
         {0x5c,0x00},
         {0xd3,0x82},
         {0xff, 0xff}
        },
    {
         //level 4
           {0xff, 0x00},//level 3
         {0xc0,0xc8},
         {0xc1,0x96},
         {0x8c,0x00},
         {0x86,0x3d},
         {0x50,0x00},
         {0x51,0xfa},
         {0x52,0xbc},
         {0x53,0x2c},
         {0x54,0xe0},
         {0x55,0x01},
         {0x5a,0xc8},
         {0x5b,0x96},
         {0x5c,0x00},
         {0xd3,0x82},
         {0xff, 0xff}
    },
    {
         //level 5
             {0xff, 0x00},//level 3
         {0xc0,0xc8},
         {0xc1,0x96},
         {0x8c,0x00},
         {0x86,0x3d},
         {0x50,0x00},
         {0x51,0xc8},
         {0x52,0x96},
         {0x53,0x90},
         {0x54,0x2c},
         {0x55,0x11},
         {0x5a,0xc8},
         {0x5b,0x96},
         {0x5c,0x00},
         {0xd3,0x82},
         {0xff, 0xff}
    }

};


LOCAL uint32 OV2640_zoom(uint32 param)
{
    uint16 i;
    uint32 zoom_level,sensor_mode;
    uint32* pint32 = (uint32 *)param;
    SENSOR_REG_T* sensor_reg_ptr =NULL;
    sensor_mode = pint32[0];
    zoom_level    = pint32[1];
    sensor_reg_ptr = (SENSOR_REG_T*)ov2640_zoom_tab[(zoom_level-1)+4*(sensor_mode-2)];
    
    SCI_ASSERT(zoom_level <= 4 && zoom_level>=1);       /*assert to do*/
    SCI_ASSERT(sensor_mode <= 3 && sensor_mode>=2);     /*assert to do*/
    SCI_ASSERT(PNULL != sensor_reg_ptr);   /*assert verified*/
    //SCI_TRACE_LOW:"OV2640_zoom: mode=%d,level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_OV2640_2026_112_2_18_0_31_30_1035,(uint8*)"dd", sensor_mode,zoom_level);
    
    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
//  SCI_Sleep(50);

    return 0;
}



