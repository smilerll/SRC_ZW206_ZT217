/******************************************************************************
 ** File Name:      usc15241_state.h                                               *
 ** Author:         ken.kuang                                                 *
 ** DATE:           12/12/2011                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    Rigister definition for physical layer.                   *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/12/2011     ken.kuang          Create.                                 *
 *****************************************************************************/
#ifndef _USC15241_STATE_H_
#define _USC15241_STATE_H_

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Macro defination                                  *
 **---------------------------------------------------------------------------*/
enum{
    USC15241_FUN_PLAYBACK        = 0x00000001,
    USC15241_FUN_CAPTURE         = 0x00000002,
    USC15241_FUN_LINEIN          = 0x00000004,
    USC15241_FUN_LINEIN_REC      = 0x00000008,
    USC15241_FUN_HP              = 0x00000010,
    USC15241_FUN_LOUT            = 0x00000020,
    USC15241_FUN_DAC_UNMUTE      = 0x00000040,
};


/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
PUBLIC void     USC15241_SM_Process(void);
PUBLIC void     USC15241_ChangeSt(uint32 func_flag,uint32 is_close);
PUBLIC BOOLEAN  USC15241_IsReady(void);
PUBLIC BOOLEAN  USC15241_IsRuning(void);
PUBLIC BOOLEAN  USC15241_IsOnStop(void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif //_USC15241_STATE_H_


