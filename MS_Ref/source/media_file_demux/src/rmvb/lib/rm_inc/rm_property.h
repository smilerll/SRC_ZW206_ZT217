

#ifndef RM_PROPERTY_H
#define RM_PROPERTY_H

#include "helix_types.h"

#define RM_PROPERTY_TYPE_UINT32  0
#define RM_PROPERTY_TYPE_BUFFER  1
#define RM_PROPERTY_TYPE_CSTRING 2

#ifdef __cplusplus
extern "C" {
#endif  /* #ifdef __cplusplus */

/*
 * Property struct
 *
 * This struct can hold a UINT32 property, a CString
 * property, or a buffer property. The members
 * of this struct are as follows:
 *
 * pName:        NULL-terminated string which holds the name
 * ulType:       This can be either RM_PROPERTY_TYPE_UINT32,
 *                 RM_PROPERTY_TYPE_BUFFER, or RM_PROPERTY_TYPE_CSTRING.
 * pValue and ulValueLen:
 *     1) For type RM_PROPERTY_TYPE_UINT32, the value is held in
 *        the pValue pointer itself and ulValueLen is 0.
 *     2) For type RM_PROPERTY_TYPE_BUFFER, the value is held in
 *        the buffer pointed to by pValue and the length of
 *        the buffer is ulValueLen.
 *     3) For type RM_PROPERTY_TYPE_CSTRING, the value is a
 *        NULL terminated string in pValue. Therefore the pValue
 *        pointer can be re-cast as a (const char*) and the value
 *        string read from it. ulValueLen holds a length that
 *        is AT LEAST strlen((const char*) pValue) + 1. It may
 *        be more than that, so do not rely on ulValueLen being
 *        equal to strlen((const char*) pValue) + 1.
 */

typedef struct rm_property_struct
{
    char*  pName;
    UINT32 ulType;
    BYTE*  pValue;
    UINT32 ulValueLen;
} rm_property;

/*
 * rm_property Accessor Functions
 *
 * Users are strongly encouraged to use these accessor
 * functions to retrieve information from the 
 * rm_property struct, since the definition of rm_property
 * may change in the future.
 *
 * When retrieving the property name, the user should first
 * call rm_property_get_name_length() to see that the name
 * length is greater than 0. After that, the user can get
 * read-only access to the name by calling rm_property_get_name().
 */
const char* rm_property_get_name(rm_property* prop);
UINT32      rm_property_get_type(rm_property* prop);
UINT32      rm_property_get_value_uint32(rm_property* prop);
const char* rm_property_get_value_cstring(rm_property* prop);
UINT32      rm_property_get_value_buffer_length(rm_property* prop);
BYTE*       rm_property_get_value_buffer(rm_property* prop);

#ifdef __cplusplus
}
#endif  /* #ifdef __cplusplus */

#endif /* #ifndef RM_PROPERTY_H */
