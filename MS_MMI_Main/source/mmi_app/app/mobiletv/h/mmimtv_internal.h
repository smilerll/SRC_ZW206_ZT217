/*****************************************************************************
** File Name:      mmimtv_internal.h                                         *
** Author:                                                                   *
** Date:           2008-12	                                                 *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe mtv app                     *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2008        xin.li           Create									 *
*****************************************************************************/
#ifdef CMMB_SUPPORT
#ifndef  _MMIMTV_INTERNAL_H_    
#define  _MMIMTV_INTERNAL_H_   


/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmimtv_export.h"

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                     *
**---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
    
/**---------------------------------------------------------------------------*
**                         Macro Declaration    							  *
**---------------------------------------------------------------------------*/
   
/**---------------------------------------------------------------------------*
**                         Function Declare                                  *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : open MTV player
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMTV_OpenMTVPlayer(void);

/*****************************************************************************/
// 	Description : ��MBBMSʹ��
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMTV_OpenMTVPlayerForMBBMS(void);
			          
/**---------------------------------------------------------------------------*
**                         Compiler Flag                                     *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif /* END _MMIMTV_INTERNAL_H_  */
#endif