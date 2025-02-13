/*******************************************************************************
*
* Copyright (c) 2013, 2014 Intel Corporation and others.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* and Eclipse Distribution License v1.0 which accompany this distribution.
*
* The Eclipse Public License is available at
*    http://www.eclipse.org/legal/epl-v10.html
* The Eclipse Distribution License is available at
*    http://www.eclipse.org/org/documents/edl-v10.php.
*
* Contributors:
*    David Navarro, Intel Corporation - initial API and implementation
*    Fabien Fleutot - Please refer to git log
*    Bosch Software Innovations GmbH - Please refer to git log
*
*******************************************************************************/
#include "lwm2msdk.h"

#include "internals.h"
#include <float.h>

#define _PRV_STR_LENGTH 32

// dataP array length is assumed to be 1.
static int prv_textSerialize(lwm2m_data_t * dataP,
                             uint8_t ** bufferP)
{
    size_t res;

    switch (dataP->type)
    {
    case LWM2M_TYPE_STRING:
        *bufferP = (uint8_t *)lwm2m_malloc(dataP->value.asBuffer.length);
        if (*bufferP == NULL) return 0;
        memcpy(*bufferP, dataP->value.asBuffer.buffer, dataP->value.asBuffer.length);
        return (int)dataP->value.asBuffer.length;

    case LWM2M_TYPE_INTEGER:
    {
        uint8_t intString[_PRV_STR_LENGTH];

        res = utils_intToText(dataP->value.asInteger, intString, _PRV_STR_LENGTH);
        if (res == 0) return -1;

        *bufferP = (uint8_t *)lwm2m_malloc(res);
        if (NULL == *bufferP) return -1;

        memcpy(*bufferP, intString, res);

        return (int)res;
    }

    case LWM2M_TYPE_FLOAT:
    {
        uint8_t floatString[_PRV_STR_LENGTH * 2];

        res = utils_floatToText(dataP->value.asFloat, floatString, _PRV_STR_LENGTH * 2);
        if (res == 0) return -1;

        *bufferP = (uint8_t *)lwm2m_malloc(res);
        if (NULL == *bufferP) return -1;

        memcpy(*bufferP, floatString, res);

        return (int)res;
    }

    case LWM2M_TYPE_BOOLEAN:
        *bufferP = (uint8_t *)lwm2m_malloc(1);
        if (NULL == *bufferP) return -1;

        *bufferP[0] = dataP->value.asBoolean ? '1' : '0';

        return 1;

    case LWM2M_TYPE_OBJECT_LINK:
    {
        char stringBuffer[11];
        size_t length;
        
        length = utils_intToText(dataP->value.asObjLink.objectId, (uint8_t*)stringBuffer, 5);
        if (length == 0) return -1;

        stringBuffer[5] = ':';
        res = length + 1;

        length = utils_intToText(dataP->value.asObjLink.objectInstanceId, (uint8_t*)stringBuffer + res, 5);
        if (length == 0) return -1;

        res += length;

        *bufferP = (uint8_t *)lwm2m_malloc(res);
        if (*bufferP == NULL) return -1;

        memcpy(*bufferP, stringBuffer, res);

        return res;
    }

    case LWM2M_TYPE_OPAQUE:
    {
        size_t length;

        length = utils_base64GetSize(dataP->value.asBuffer.length);
        *bufferP = (uint8_t *)lwm2m_malloc(length);
        if (*bufferP == NULL) return 0;
        length = utils_base64Encode(dataP->value.asBuffer.buffer, dataP->value.asBuffer.length, *bufferP, length);
        if (length == 0)
        {
            lwm2m_free(*bufferP);
            *bufferP = NULL;
            return 0;
        }
        return (int)length;
    }

    case LWM2M_TYPE_UNDEFINED:
    default:
        return -1;
    }
}

static int prv_setBuffer(lwm2m_data_t * dataP,
                         uint8_t * buffer,
                         size_t bufferLen)
{
    dataP->value.asBuffer.buffer = (uint8_t *)lwm2m_malloc(bufferLen+1);
    if (dataP->value.asBuffer.buffer == NULL)
    {
        return 0;
    }
    memset(dataP->value.asBuffer.buffer,0,bufferLen+1);
    dataP->value.asBuffer.length = bufferLen;
    memcpy(dataP->value.asBuffer.buffer, buffer, bufferLen);
    LOG_ARG(" debug,dataP->value.asBuffer.buffer :%s ",dataP->value.asBuffer.buffer);
    
    LOG_ARG(" debug, bufferLen:%d ",bufferLen);
    return 1;
}

lwm2m_data_t * lwm2m_data_new(int size)
{
    lwm2m_data_t * dataP;

    LOG_ARG("size: %d", size);
    if (size <= 0) return NULL;

    dataP = (lwm2m_data_t *)lwm2m_malloc(size * sizeof(lwm2m_data_t));

    if (dataP != NULL)
    {
        memset(dataP, 0, size * sizeof(lwm2m_data_t));
    }

    return dataP;
}

void lwm2m_data_free(int size,
                     lwm2m_data_t * dataP)
{
    int i;

    LOG_ARG("size: %d", size);
    if (size == 0 || dataP == NULL) return;

    for (i = 0; i < size; i++)
    {
        switch (dataP[i].type)
        {
        case LWM2M_TYPE_MULTIPLE_RESOURCE:
        case LWM2M_TYPE_OBJECT_INSTANCE:
        case LWM2M_TYPE_OBJECT:
            lwm2m_data_free(dataP[i].value.asChildren.count, dataP[i].value.asChildren.array);
            break;

        case LWM2M_TYPE_STRING:
        case LWM2M_TYPE_OPAQUE:
            if (dataP[i].value.asBuffer.buffer != NULL)
            {
                lwm2m_free(dataP[i].value.asBuffer.buffer);
            }

        default:
            // do nothing
            break;
        }
    }
    lwm2m_free(dataP);
}

void lwm2m_data_encode_string(const char * string,
                              lwm2m_data_t * dataP)
{
    size_t len;
    int res;

    LOG_ARG("\"%s\"", string);
    if (string == NULL)
    {
        len = 0;
    }
    else
    {
        for (len = 0; string[len] != 0; len++);
    }

    if (len == 0)
    {
        dataP->value.asBuffer.length = 0;
        dataP->value.asBuffer.buffer = NULL;
        res = 1;
    }
    else
    {
        res = prv_setBuffer(dataP, (uint8_t *)string, len);
    }

    if (res == 1)
    {
        dataP->type = LWM2M_TYPE_STRING;
    }
    else
    {
        dataP->type = LWM2M_TYPE_UNDEFINED;
    }
}

void lwm2m_data_encode_opaque(uint8_t * buffer,
                              size_t length,
                              lwm2m_data_t * dataP)
{
    int res;

    LOG_ARG("length: %d", length);
    if (length == 0)
    {
        dataP->value.asBuffer.length = 0;
        dataP->value.asBuffer.buffer = NULL;
        res = 1;
    }
    else
    {
        res = prv_setBuffer(dataP, buffer, length);
    }

    if (res == 1)
    {
        dataP->type = LWM2M_TYPE_OPAQUE;
    }
    else
    {
        dataP->type = LWM2M_TYPE_UNDEFINED;
    }
}

void lwm2m_data_encode_nstring(const char * string,
                               size_t length,
                               lwm2m_data_t * dataP)
{
    LOG_ARG("length: %d, string: \"%s\"", length, string);
    lwm2m_data_encode_opaque((uint8_t *)string, length, dataP);

    if (dataP->type == LWM2M_TYPE_OPAQUE)
    {
        dataP->type = LWM2M_TYPE_STRING;
    }
}

void lwm2m_data_encode_int(int64_t value,
                           lwm2m_data_t * dataP)
{
    LOG_ARG("value: %" PRId64 "", value);
    dataP->type = LWM2M_TYPE_INTEGER;
    dataP->value.asInteger = value;
}

int lwm2m_data_decode_int(const lwm2m_data_t * dataP,
                          int64_t * valueP)
{
    int result;

    LOG("Entering");
    switch (dataP->type)
    {
    case LWM2M_TYPE_INTEGER:
        *valueP = dataP->value.asInteger;
        result = 1;
        break;

    case LWM2M_TYPE_STRING:
        result = utils_textToInt(dataP->value.asBuffer.buffer, dataP->value.asBuffer.length, valueP);
        break;

    case LWM2M_TYPE_OPAQUE:
        switch (dataP->value.asBuffer.length)
        {
        case 1:
            *valueP = (int8_t)dataP->value.asBuffer.buffer[0];
            result = 1;
            break;

        case 2:
        {
            int16_t value;

            utils_copyValue(&value, dataP->value.asBuffer.buffer, dataP->value.asBuffer.length);

            *valueP = value;
            result = 1;
            break;
        }

        case 4:
        {
            int32_t value;

            utils_copyValue(&value, dataP->value.asBuffer.buffer, dataP->value.asBuffer.length);

            *valueP = value;
            result = 1;
            break;
        }

        case 8:
            utils_copyValue(valueP, dataP->value.asBuffer.buffer, dataP->value.asBuffer.length);
            result = 1;
            break;

        default:
            result = 0;
        }
        break;

    default:
        return 0;
    }
    LOG_ARG("result: %d, value: %" PRId64, result, *valueP);

    return result;
}

void lwm2m_data_encode_float(double value,
                             lwm2m_data_t * dataP)
{
    LOG_ARG("value: %f", value);
    dataP->type = LWM2M_TYPE_FLOAT;
    dataP->value.asFloat = value;
}

int lwm2m_data_decode_float(const lwm2m_data_t * dataP,
                            double * valueP)
{
    int result;

    LOG("Entering");
    switch (dataP->type)
    {
    case LWM2M_TYPE_FLOAT:
        *valueP = dataP->value.asFloat;
        result = 1;
        break;

    case LWM2M_TYPE_INTEGER:
        *valueP = (double)dataP->value.asInteger;
        result = 1;
        break;

    case LWM2M_TYPE_STRING:
#ifdef LWM2M_SUPPORT_4
        result = utils_textToFloat(dataP->value.asBuffer.buffer, dataP->value.asBuffer.length, valueP,false);
#else
        result = utils_textToFloat(dataP->value.asBuffer.buffer, dataP->value.asBuffer.length, valueP);
#endif
        break;

    case LWM2M_TYPE_OPAQUE:
        switch (dataP->value.asBuffer.length)
        {
        case 4:
        {
            float temp;

            utils_copyValue(&temp, dataP->value.asBuffer.buffer, dataP->value.asBuffer.length);

            *valueP = temp;
            result = 1;
        }
        break;

        case 8:
            utils_copyValue(valueP, dataP->value.asBuffer.buffer, dataP->value.asBuffer.length);
            result = 1;
            break;

        default:
            result = 0;
        }
        break;

    default:
        result = 0;
    }

    LOG_ARG("result: %d, value: %f", result, *valueP);

    return result;
}

void lwm2m_data_encode_bool(bool value,
                            lwm2m_data_t * dataP)
{
    LOG_ARG("value: %s", value?"true":"false");
    dataP->type = LWM2M_TYPE_BOOLEAN;
    dataP->value.asBoolean = value;
}

int lwm2m_data_decode_bool(const lwm2m_data_t * dataP,
                           bool * valueP)
{
    int result;

    LOG("Entering");
    switch (dataP->type)
    {
    case LWM2M_TYPE_BOOLEAN:
        *valueP = dataP->value.asBoolean;
        result = 1;
        break;

    case LWM2M_TYPE_STRING:
        if (dataP->value.asBuffer.length != 1) return 0;

        switch (dataP->value.asBuffer.buffer[0])
        {
        case '0':
            *valueP = false;
            result = 1;
            break;
        case '1':
            *valueP = true;
            result = 1;
            break;
        default:
            result = 0;
            break;
        }
        break;

    case LWM2M_TYPE_OPAQUE:
        if (dataP->value.asBuffer.length != 1) return 0;

        switch (dataP->value.asBuffer.buffer[0])
        {
        case 0:
            *valueP = false;
            result = 1;
            break;
        case 1:
            *valueP = true;
            result = 1;
            break;
        default:
            result = 0;
            break;
        }
        break;

    default:
        result = 0;
        break;
    }

    LOG_ARG("result: %d, value: %s", result, *valueP ? "true" : "false");

    return result;
}

void lwm2m_data_encode_objlink(uint16_t objectId,
                           uint16_t objectInstanceId,
                           lwm2m_data_t * dataP)
{
    LOG_ARG("value: %d/%d", objectId, objectInstanceId);
    dataP->type = LWM2M_TYPE_OBJECT_LINK;
    dataP->value.asObjLink.objectId = objectId;
    dataP->value.asObjLink.objectInstanceId = objectInstanceId;
}

void lwm2m_data_include(lwm2m_data_t * subDataP,
                        size_t count,
                        lwm2m_data_t * dataP)
{
    LOG_ARG("count: %d", count);
    if (subDataP == NULL || count == 0) return;

    switch (subDataP[0].type)
    {
    case LWM2M_TYPE_STRING:
    case LWM2M_TYPE_OPAQUE:
    case LWM2M_TYPE_INTEGER:
    case LWM2M_TYPE_FLOAT:
    case LWM2M_TYPE_BOOLEAN:
    case LWM2M_TYPE_OBJECT_LINK:
    case LWM2M_TYPE_MULTIPLE_RESOURCE:
        dataP->type = LWM2M_TYPE_OBJECT_INSTANCE;
        break;
    case LWM2M_TYPE_OBJECT_INSTANCE:
        dataP->type = LWM2M_TYPE_OBJECT;
        break;
    default:
        return;
    }
    dataP->value.asChildren.count = count;
    dataP->value.asChildren.array = subDataP;
}

void lwm2m_data_encode_instances(lwm2m_data_t * subDataP,
                                 size_t count,
                                 lwm2m_data_t * dataP)
{
    LOG_ARG("count: %d", count);
    lwm2m_data_include(subDataP, count, dataP);
    dataP->type = LWM2M_TYPE_MULTIPLE_RESOURCE;
}

#ifdef LWM2M_SUPPORT_4
//4.0 add
extern struct deviceinfo g_deviceinfo[];
extern struct ruleConfig g_ruleConfig[];
extern int g_deviceinfo_need_report;

int lwm2m_sdk_dm_make_json_string(lwm2m_data_t * dataP, char * *json_string_out)
{
    int length = 0;
    int length_temp = 0;
    int i = 0;
    int j = 0;
    int index = 0;
    char temp[128] = {0};
    char * json_string;
    int has_been_report_deviceinfo =0;

    if(g_deviceinfo_need_report == 0)
    {
        LOG("g_deviceinfo_need_report is 0,nothing to report !\n");
        index = 0;
        return index;
    }
    //json_string = lwm2m_malloc(1024);
    json_string = lwm2m_malloc(g_deviceinfo_need_report*128);
    LOG_ARG("json_string: 0x%x\n",json_string);
    *json_string_out = json_string;
    if(json_string == NULL)
    {
        LOG("MALLOC FAIL");
        return 0;
    }
    memset(json_string,0,g_deviceinfo_need_report*128);
    index = 0;
    sprintf(temp,"%s","{");
    length_temp = strlen(temp);
    LOG_ARG("length_temp: %d ,temp :%s.\n",length_temp,temp);

    memcpy(json_string+index,temp,length_temp);

    //snprintf(json_string+index,strlen("{"),"%s","{");
    index = index+length_temp;
    LOG_ARG("index: %d ,json_string :%s.\n",index,json_string);

    LOG_ARG("g_deviceinfo_need_report num: %d \n",g_deviceinfo_need_report);
    for(j = 0; j < deviceinfo_num ;j++)
    {
        LOG_ARG("g_deviceinfo[%d].resid: %d,g_deviceinfo[j].choice: %d",j,g_deviceinfo[j].resid,g_deviceinfo[j].choice);
        for(i = 0;i < deviceinfo_num; i++)
        {
            if(g_deviceinfo[j].choice == TRUE)
            {
                 if(g_deviceinfo[j].resid == dataP[i].id)
                 {
                    int name_length = 0;
                    int value_length = 0;
                    name_length = strlen(g_deviceinfo[j].name);
                    value_length = dataP[i].value.asBuffer.length;
                    if(has_been_report_deviceinfo == (g_deviceinfo_need_report-1))
                    {
                        memset(temp,0,128);
                        sprintf(temp,"\"%s\":\"%s\"}",g_deviceinfo[j].name,dataP[i].value.asBuffer.buffer);
                        length_temp = strlen(temp);
                        memcpy(json_string+index,temp,length_temp);
                        index +=length_temp;
                        //LOG_ARG("index = %d ,json_string is:%s.\n",index,json_string);
                    }
                    else
                    {
                        memset(temp,0,128);
                        sprintf(temp,"\"%s\":\"%s\",",g_deviceinfo[j].name,dataP[i].value.asBuffer.buffer);
                        length_temp = strlen(temp);
                        //LOG_ARG("length_temp = %d ,temp is:%s.\n",length_temp,temp);
                        //LOG_ARG("length = %d \n",(name_length+value_length+6));
                        memcpy(json_string+index,temp,length_temp);
                        index +=length_temp;
                        has_been_report_deviceinfo++;
                        //LOG_ARG("index = %d ,json_string is:%s.\n",index,json_string);
                    }
                    break;
                }
            }
            else
            {
                LOG_ARG(" break,g_deviceinfo[%d].choice: %d, not report !",j,g_deviceinfo[j].choice);
                break;
            }
        }
    }

    LOG_ARG("total length  = %d ,json_string is:%s.\n",index,json_string);
    return index;
}
#endif

int lwm2m_data_parse(lwm2m_uri_t * uriP,
                     uint8_t * buffer,
                     size_t bufferLen,
                     lwm2m_media_type_t format,
                     lwm2m_data_t ** dataP)
{
    int res;

    LOG_ARG("format: %s, bufferLen: %d", STR_MEDIA_TYPE(format), bufferLen);
    LOG_URI(uriP);
    switch (format)
    {
    case LWM2M_CONTENT_TEXT:
        if (!LWM2M_URI_IS_SET_RESOURCE(uriP)) return 0;
        *dataP = lwm2m_data_new(1);
        if (*dataP == NULL) return 0;
        (*dataP)->id = uriP->resourceId;
        (*dataP)->type = LWM2M_TYPE_STRING;
        res = prv_setBuffer(*dataP, buffer, bufferLen);
        if (res == 0)
        {
            lwm2m_data_free(1, *dataP);
            *dataP = NULL;
        }
        return res;

    case LWM2M_CONTENT_OPAQUE:
        if (!LWM2M_URI_IS_SET_RESOURCE(uriP)) return 0;
        *dataP = lwm2m_data_new(1);
        if (*dataP == NULL) return 0;
        (*dataP)->id = uriP->resourceId;
        (*dataP)->type = LWM2M_TYPE_OPAQUE;
        res = prv_setBuffer(*dataP, buffer, bufferLen);
        if (res == 0)
        {
            lwm2m_data_free(1, *dataP);
            *dataP = NULL;
    }
        return res;

#ifdef LWM2M_OLD_CONTENT_FORMAT_SUPPORT
    case LWM2M_CONTENT_TLV_OLD:
#endif
    case LWM2M_CONTENT_TLV:
        return tlv_parse(buffer, bufferLen, dataP);

#ifdef LWM2M_SUPPORT_JSON
#ifdef LWM2M_OLD_CONTENT_FORMAT_SUPPORT
    case LWM2M_CONTENT_JSON_OLD:
#endif
    case LWM2M_CONTENT_JSON:
        return json_parse(uriP, buffer, bufferLen, dataP);
#endif

    default:
        return 0;
    }
}

int lwm2m_data_serialize(lwm2m_uri_t * uriP,
                         int size,
                         lwm2m_data_t * dataP,
                         lwm2m_media_type_t * formatP,
                         uint8_t ** bufferP)
{
    LOG_URI(uriP);
    LOG_ARG("size: %d, formatP: %s", size, STR_MEDIA_TYPE(*formatP));

    // Check format
    if (*formatP == LWM2M_CONTENT_TEXT
     || *formatP == LWM2M_CONTENT_OPAQUE)
    {
        if (size != 1
         || (uriP != NULL && !LWM2M_URI_IS_SET_RESOURCE(uriP))
         || dataP->type == LWM2M_TYPE_OBJECT
         || dataP->type == LWM2M_TYPE_OBJECT_INSTANCE
         || dataP->type == LWM2M_TYPE_MULTIPLE_RESOURCE)
        {
#ifdef LWM2M_SUPPORT_JSON
            *formatP = LWM2M_CONTENT_JSON;
#else
            *formatP = LWM2M_CONTENT_TLV;
#endif
        }
    }

    if (*formatP == LWM2M_CONTENT_OPAQUE
     && dataP->type != LWM2M_TYPE_OPAQUE)
    {
        LOG("Opaque format is reserved to opaque resources.");
        return -1;
    }

    LOG_ARG("Final format: %s", STR_MEDIA_TYPE(*formatP));

    switch (*formatP)
    {
    case LWM2M_CONTENT_TEXT:
        return prv_textSerialize(dataP, bufferP);

    case LWM2M_CONTENT_OPAQUE:
        *bufferP = (uint8_t *)lwm2m_malloc(dataP->value.asBuffer.length);
        if (*bufferP == NULL) return -1;
        memcpy(*bufferP, dataP->value.asBuffer.buffer, dataP->value.asBuffer.length);
        return (int)dataP->value.asBuffer.length;

    case LWM2M_CONTENT_TLV:
    case LWM2M_CONTENT_TLV_OLD:
    {
            bool isResourceInstance;

            if (uriP != NULL && LWM2M_URI_IS_SET_RESOURCE(uriP)
             && (size != 1 || dataP->id != uriP->resourceId))
            {
                isResourceInstance = true;
            }
            else
            {
                isResourceInstance = false;
            }
#ifdef LWM2M_SUPPORT_4
            //add by 4.0
            LOG_ARG("uriP->objectId : %d", uriP->objectId );
            if(uriP->objectId == LWM2M_DM_UE_OBJECT_READ)
            {
                char * json_string =NULL;
                int json_string_length = 0;
                json_string_length = lwm2m_sdk_dm_make_json_string(dataP,&json_string);
                LOG_ARG("result json_string is : %s", json_string );
                if(json_string_length == 0)
                {
                    LOG("lwm2m_sdk_dm_make_json_string error");
                    return -1;
                }
                size = 1   ;//  only one json string
                isResourceInstance = FALSE;
                //remalloc
                if(1)//dataP[0].value.asBuffer.buffer !=NULL)
                {
                    if(dataP[0].value.asBuffer.buffer !=NULL)
                    {
                        LOG_ARG("last buffer is : %s", dataP[0].value.asBuffer.buffer );
                        lwm2m_free(dataP[0].value.asBuffer.buffer );
                    }
                     else
                    {
                        LOG("dataP[0].value.asBuffer.buffer ==NULL ,dont need free !");
                    }

                    dataP[0].value.asBuffer.buffer =lwm2m_malloc(json_string_length+1);
                    if(dataP[0].value.asBuffer.buffer ==NULL)
                    {
                        LOG("malloc error");
                        return -1;
                    }
                    memset(dataP[0].value.asBuffer.buffer,0,json_string_length+1);
                }

                memcpy(dataP[0].value.asBuffer.buffer,json_string,json_string_length) ;
                dataP[0].value.asBuffer.length = json_string_length;
                dataP[0].id = 1;  //only one json string

                if(json_string != NULL)
                {
                    lwm2m_free(json_string);
                }
            }
            //add by 4.0
#endif
            return tlv_serialize(isResourceInstance, size, dataP, bufferP);
        }

#ifdef LWM2M_CLIENT_MODE
    case LWM2M_CONTENT_LINK:
        return discover_serialize(NULL, uriP, NULL, size, dataP, bufferP);
#endif
#ifdef LWM2M_SUPPORT_JSON
    case LWM2M_CONTENT_JSON:
    case LWM2M_CONTENT_JSON_OLD:
        return json_serialize(uriP, size, dataP, bufferP);
#endif

    default:
        return -1;
    }
}

