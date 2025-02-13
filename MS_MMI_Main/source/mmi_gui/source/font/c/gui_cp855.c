#ifdef IM_RUSSIAN_SUPPORT
/*****************************************************************************
** File Name:      gui_cp855.c                                              *
** Author:                                                                   *
** Date:           18/02/2012                                                *
** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe conversion function         *
**                 from CP1251 to unicode                                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 18/02/2012    dafeng.xu         Create                                  *
******************************************************************************/

#ifndef _GUI_CP855_C_
#define _GUI_CP855_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "caf.h"
#include "sci_codec.h"
#include "gui_ucs2b_converter.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
/*
 * CP855
 */
LOCAL const uint16 s_cp855_2uni[128] = {
  /* 0x80 */
  0x0452, 0x0402, 0x0453, 0x0403, 0x0451, 0x0401, 0x0454, 0x0404,
  0x0455, 0x0405, 0x0456, 0x0406, 0x0457, 0x0407, 0x0458, 0x0408,
  /* 0x90 */
  0x0459, 0x0409, 0x045a, 0x040a, 0x045b, 0x040b, 0x045c, 0x040c,
  0x045e, 0x040e, 0x045f, 0x040f, 0x044e, 0x042e, 0x044a, 0x042a,
  /* 0xa0 */
  0x0430, 0x0410, 0x0431, 0x0411, 0x0446, 0x0426, 0x0434, 0x0414,
  0x0435, 0x0415, 0x0444, 0x0424, 0x0433, 0x0413, 0x00ab, 0x00bb,
  /* 0xb0 */
  0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x0445, 0x0425, 0x0438,
  0x0418, 0x2563, 0x2551, 0x2557, 0x255d, 0x0439, 0x0419, 0x2510,
  /* 0xc0 */
  0x2514, 0x2534, 0x252c, 0x251c, 0x2500, 0x253c, 0x043a, 0x041a,
  0x255a, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256c, 0x00a4,
  /* 0xd0 */
  0x043b, 0x041b, 0x043c, 0x041c, 0x043d, 0x041d, 0x043e, 0x041e,
  0x043f, 0x2518, 0x250c, 0x2588, 0x2584, 0x041f, 0x044f, 0x2580,
  /* 0xe0 */
  0x042f, 0x0440, 0x0420, 0x0441, 0x0421, 0x0442, 0x0422, 0x0443,
  0x0423, 0x0436, 0x0416, 0x0432, 0x0412, 0x044c, 0x042c, 0x2116,
  /* 0xf0 */
  0x00ad, 0x044b, 0x042b, 0x0437, 0x0417, 0x0448, 0x0428, 0x044d,
  0x042d, 0x0449, 0x0429, 0x0447, 0x0427, 0x00a7, 0x25a0, 0x00a0,
};

LOCAL const uint8 s_cp855_page00[32] = {
  0xff, 0x00, 0x00, 0x00, 0xcf, 0x00, 0x00, 0xfd, /* 0xa0-0xa7 */
  0x00, 0x00, 0x00, 0xae, 0x00, 0xf0, 0x00, 0x00, /* 0xa8-0xaf */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0xb0-0xb7 */
  0x00, 0x00, 0x00, 0xaf, 0x00, 0x00, 0x00, 0x00, /* 0xb8-0xbf */
};

LOCAL const uint8 s_cp855_page04[96] = {
  0x00, 0x85, 0x81, 0x83, 0x87, 0x89, 0x8b, 0x8d, /* 0x00-0x07 */
  0x8f, 0x91, 0x93, 0x95, 0x97, 0x00, 0x99, 0x9b, /* 0x08-0x0f */
  0xa1, 0xa3, 0xec, 0xad, 0xa7, 0xa9, 0xea, 0xf4, /* 0x10-0x17 */
  0xb8, 0xbe, 0xc7, 0xd1, 0xd3, 0xd5, 0xd7, 0xdd, /* 0x18-0x1f */
  0xe2, 0xe4, 0xe6, 0xe8, 0xab, 0xb6, 0xa5, 0xfc, /* 0x20-0x27 */
  0xf6, 0xfa, 0x9f, 0xf2, 0xee, 0xf8, 0x9d, 0xe0, /* 0x28-0x2f */
  0xa0, 0xa2, 0xeb, 0xac, 0xa6, 0xa8, 0xe9, 0xf3, /* 0x30-0x37 */
  0xb7, 0xbd, 0xc6, 0xd0, 0xd2, 0xd4, 0xd6, 0xd8, /* 0x38-0x3f */
  0xe1, 0xe3, 0xe5, 0xe7, 0xaa, 0xb5, 0xa4, 0xfb, /* 0x40-0x47 */
  0xf5, 0xf9, 0x9e, 0xf1, 0xed, 0xf7, 0x9c, 0xde, /* 0x48-0x4f */
  0x00, 0x84, 0x80, 0x82, 0x86, 0x88, 0x8a, 0x8c, /* 0x50-0x57 */
  0x8e, 0x90, 0x92, 0x94, 0x96, 0x00, 0x98, 0x9a, /* 0x58-0x5f */
};

LOCAL const uint8 s_cp855_page25[168] = {
  0xc4, 0x00, 0xb3, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x00-0x07 */
  0x00, 0x00, 0x00, 0x00, 0xda, 0x00, 0x00, 0x00, /* 0x08-0x0f */
  0xbf, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, /* 0x10-0x17 */
  0xd9, 0x00, 0x00, 0x00, 0xc3, 0x00, 0x00, 0x00, /* 0x18-0x1f */
  0x00, 0x00, 0x00, 0x00, 0xb4, 0x00, 0x00, 0x00, /* 0x20-0x27 */
  0x00, 0x00, 0x00, 0x00, 0xc2, 0x00, 0x00, 0x00, /* 0x28-0x2f */
  0x00, 0x00, 0x00, 0x00, 0xc1, 0x00, 0x00, 0x00, /* 0x30-0x37 */
  0x00, 0x00, 0x00, 0x00, 0xc5, 0x00, 0x00, 0x00, /* 0x38-0x3f */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x40-0x47 */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x48-0x4f */
  0xcd, 0xba, 0x00, 0x00, 0xc9, 0x00, 0x00, 0xbb, /* 0x50-0x57 */
  0x00, 0x00, 0xc8, 0x00, 0x00, 0xbc, 0x00, 0x00, /* 0x58-0x5f */
  0xcc, 0x00, 0x00, 0xb9, 0x00, 0x00, 0xcb, 0x00, /* 0x60-0x67 */
  0x00, 0xca, 0x00, 0x00, 0xce, 0x00, 0x00, 0x00, /* 0x68-0x6f */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x70-0x77 */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x78-0x7f */
  0xdf, 0x00, 0x00, 0x00, 0xdc, 0x00, 0x00, 0x00, /* 0x80-0x87 */
  0xdb, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x88-0x8f */
  0x00, 0xb0, 0xb1, 0xb2, 0x00, 0x00, 0x00, 0x00, /* 0x90-0x97 */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x98-0x9f */
  0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0xa0-0xa7 */
};



/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : cp855 to wchar
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN CP855ToWchar(
                           uint8 ch,
                           wchar* wch_ptr
                           )
{
    SCI_ASSERT( PNULL != wch_ptr ); /*assert verified*/

    if (ch < 0x80) {
        *wch_ptr = ch;
    }
    else {
        *wch_ptr = s_cp855_2uni[ch - 0x80];
    }
    return TRUE;
}

/*****************************************************************************/
// 	Description : wchar to cp855 
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN WcharToCP855(
                            uint8 *ch_ptr, //out
                            wchar w_char
                            )
{
    uint8 c = 0;
    if (w_char < 0x0080) {
	      *ch_ptr = w_char;
        return TRUE;
    }
    else if (w_char >= 0x00a0 && w_char < 0x00c0)
        c = s_cp855_page00[w_char-0x00a0];
    else if (w_char >= 0x0400 && w_char < 0x0460)
        c = s_cp855_page04[w_char-0x0400];
    else if (w_char == 0x2116)
        c = 0xef;
    else if (w_char >= 0x2500 && w_char < 0x25a8)
        c = s_cp855_page25[w_char-0x2500];
    if (c != 0) {
        *ch_ptr = c;
        return TRUE;
    }
    return FALSE; 
}

/*****************************************************************************/
// 	Description : cp855 to wstr
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
PUBLIC uint32 GUI_CP855ToWstr(//return the number of UCS2 code that have been converted.
                               wchar       *wstr_ptr, 	   //destination: string pointer to UCS2 string.
                               const uint8 *cp855_ptr,    //source: string pointer to cp855 string
                               uint32      str_len         //the length to convert from cp855 to UCS2.
                               )
{
    uint32 i = 0;
    uint32 len = str_len;
    
    if ( PNULL == cp855_ptr || PNULL == wstr_ptr || 0 == str_len )
    {
        return 0;
    }

    for ( i = 0; i < str_len; i++ )
    {
        if ( !CP855ToWchar( cp855_ptr[i], &wstr_ptr[i]) )
        {
            len = i;
            break;
        }
    }

    return len;
}

/*****************************************************************************/
// 	Description : wstr to cp855 
//	Global resource dependence : none
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC uint32 GUI_WstrToCP855(//return the number of UCS2 code that have been converted.
                               uint8 *cp855_ptr,        //destination: string pointer to cp855 string
                               const wchar *wstr_ptr,    //source: string pointer to UCS2 string.
                               uint32 wstr_len           //the length to convert from UCS2 to cp855.
                               )
{
    uint32 i = 0;
    uint32 len = wstr_len;
    
    if ( PNULL == cp855_ptr || PNULL == wstr_ptr || 0 == wstr_len )
    {
        return 0;
    }
   
    for ( i = 0; i < wstr_len; i++ )
    {
        if ( !WcharToCP855( &cp855_ptr[i], wstr_ptr[i] ) )
        {
            len = i;
            break;
        }
    }
    
    return len;
}

PUBLIC uint32 GUI_CP855ToWstrWithEndPos(//return the number of UCS2 code that have been converted.
									  wchar *wstr_ptr, 	     //destination: string pointer to UCS2 string.
									  const uint8 *cp855_ptr,  //source: string pointer to CP855 string
									  uint32 str_len,        //the length to convert from CP855 to UCS2.
									  uint32 *src_end_pos    //the end pos of src string
									  )
{
    uint32 ret = 0;
    if (PNULL == wstr_ptr || PNULL == cp855_ptr || 0 == str_len) {
        return 0;
    }

    ret = GUI_CP855ToWstr(wstr_ptr, cp855_ptr, str_len);
    if (NULL != src_end_pos) {
        *src_end_pos = (uint32)cp855_ptr + ret;
    }
    return ret;
}

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


#endif
#endif


