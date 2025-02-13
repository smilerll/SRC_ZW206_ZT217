/******************************************************************************
 ** File Name:      ubot_cfg_info.h                                           *
 ** Author:         Sunny.Fei                                                 *
 ** DATE:           2010.12.13                                                *
 ** Copyright:      2010 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 **                                                                           *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                               Edit History                                *
 ** ------------------------------------------------------------------------- *
 **     DATE          NAME             DESCRIPTION                            *
 **  2010.12.13   Sunny.Fei             Create.                               *
 ******************************************************************************/
/******************************************************************************/
#ifndef _UBOT_CFG_INFO_H
#define _UBOT_CFG_INFO_H

/**---------------------------------------------------------------------------*
 **                             Dependencies                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                            Compiler Flag                                  *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C" 
{
#endif
/**---------------------------------------------------------------------------*
 **                           Macros & Enums                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                           Data Structures                                 *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                             Variables                                     *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
PUBLIC uint32 UBOT_GetCOnfigDescriptor(uint8 ** para);

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Jing.Li
//	Note:           
/*****************************************************************************/
PUBLIC uint8* UBOT_GetVenderStr (uint8 *string_length);

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Jing.Li
//	Note:           
/*****************************************************************************/
PUBLIC uint8* UBOT_GetProductIDStrNand (uint8 *string_length);

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Jing.Li
//	Note:           
/*****************************************************************************/
PUBLIC uint8* UBOT_GetProductIDStrSD (uint8 *string_length);

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Jing.Li
//	Note:           
/*****************************************************************************/
PUBLIC char* UBOT_GetSerialNumStr(uint8 *string_length);

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Jing.Li
//	Note:           
/*****************************************************************************/
PUBLIC void UBOT_SetVenderStr (uint8 *vender_str, uint8 string_length);

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Jing.Li
//	Note:           
/*****************************************************************************/
PUBLIC void UBOT_SetProductIDStrNand (uint8 *productid_str, uint8 string_length);

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Jing.Li
//	Note:           
/*****************************************************************************/
PUBLIC void UBOT_SetProductIDStrSD (uint8 *productid_str, uint8 string_length);

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Jing.Li
//	Note:           
/*****************************************************************************/
PUBLIC void UBOT_SetSerialNumStr(char *serial_num, uint8 string_length);


#ifdef __cplusplus
}
#endif

/******************************************************************************/
#endif //#ifndef _UBOT_CFG_INFO_H
