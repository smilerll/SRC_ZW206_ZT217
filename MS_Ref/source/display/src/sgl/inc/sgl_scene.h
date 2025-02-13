/******************************************************************************
 ** File Name:      sgl_shape.h		    							  		  *
 ** Author:          			                                              *
 ** DATE:           2012-01-30                                                *
 ** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    SGL scene                					              *
 ** Note:                                                      				  *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE              NAME            DESCRIPTION                             *    
 ** 2012-01-30        Shan.He		  Create                                  *
 *****************************************************************************/
#ifndef __SGL_SCENE_H__
#define __SGL_SCENE_H__

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
        
/**---------------------------------------------------------------------------*
 **                         Marcos	                                          *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Data Types                                        *
 **---------------------------------------------------------------------------*/
 typedef struct _scene_cxt_tag
{
	const char     *mark;
	SGL_CAMERA_T	camera;
}_SCENE_CXT_T;
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif
        
#endif
