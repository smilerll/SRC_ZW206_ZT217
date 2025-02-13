/****************************************************************************
** File Name:      mmiemail_view.h                                         *
** Author:                                                          			 *
** Date:           05/13/2011                                              *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 05/2011                           create
** 
****************************************************************************/

#ifndef _MMIEMAIL_VIEW_H_
#define _MMIEMAIL_VIEW_H_

/**---------------------------------------------------------------------------*
 **                         Include Files                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"
#include "mMmsApi.h"
#include "guianim.h"
#include "mmifmm_export.h"
#include "mmiemail_def.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern  "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/
       
/**---------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                 *
**---------------------------------------------------------------------------*/

typedef enum
{ 
    MMIEMAIL_VIEW_FROM_INBOX,   
	MMIEMAIL_VIEW_FROM_OUTBOX, 
	MMIEMAIL_VIEW_FROM_SENTBOX,                                         
	MMIEMAIL_VIEW_FROM_MAX                          
} MMIEMAIL_VIEW_REASON_TYPE_E;

/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description: create view window
//	Global resource dependence:
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIEMAIL_CreateViewWin(MMIEMAIL_VIEW_REASON_TYPE_E view_type, void *data_ptr);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif


#endif  /* _MMIEMAIL_VIEW_H_*/
