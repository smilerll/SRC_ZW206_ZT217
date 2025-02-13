/*****************************************************************************
** File Name:      cfl_mem.h                                           *
** Author:         juan.zhang                                                *
** Date:           10/02/2009                                                *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    DAPS MEM FUNCTION                                         *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/02/2009     juan.zhang          Create                                 *
******************************************************************************/

#ifndef _CFL_MEM_H_
#define _CFL_MEM_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"

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
#define CFL_ALLOC(_SIZE)    CFL_AllocEx(_SIZE, __FILE__, __LINE__)
#define CFL_FREE            SCI_FREE

#define CFL_REALLOC         CFL_ReAlloc
#define CFL_MEMCPY          SCI_MEMCPY
#define CFL_MEMSET          SCI_MEMSET

#define CFL_AllocEx         SCI_MallocApp_Zero//SCI_MallocApp

#define CFL_ReAlloc(_OLD_MEM_PTR, _OLD_SIZE, _NEW_SIZE) \
        CFL_ReAllocEx(_OLD_MEM_PTR, _OLD_SIZE, _NEW_SIZE, __FILE__, __LINE__)
/*---------------------------------------------------------------------------*/
/*                          Enum                                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          Struct                                           */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          Message                                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/

/****************************************************************************/
// 	Description : realloc memory
//	Global resource dependence : 
//  Author:juan.zhang 
//	Note:	
/****************************************************************************/
PUBLIC void* CFL_ReAllocEx(
                           void   *old_mem_ptr, 
                           uint32 old_size,
                           uint32 new_size,
                           char   *file_ptr,
                           uint32 line
                           );

#ifdef   __cplusplus
    }
#endif


#endif 

