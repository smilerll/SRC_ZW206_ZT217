/*==============================================================================
File Name: mmiim_sp_py26key.h
Author: Lianxiang.Zhou
Date: 2010/06/30
Copyright: 2010 Spreadtrum, Incorporated. All Rights Reserved.
Description:
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2010/06/30   Lianxiang.Zhou  Create.
==============================================================================*/
#if (!defined KEYPAD_TYPE_TYPICAL_KEYPAD ||defined  MMI_IM_PDA_SUPPORT)

#ifndef _MMIIM_SP_PY26KEY_H_
#define _MMIIM_SP_PY26KEY_H_


#include "mmiim_sp_tp_basic.h"

#if defined(MMIIM_USE_SPIM_QWERT)


#include "spim_ch_py26k.h"


//ui data
typedef struct
{
    GUIIM_INIT_KERNEL_T init_param;

    SPIM_HANDLE_T core_handle;
    MMIIM_SP_STATE_T state;

    GUI_RECT_T rect;

    //��ʾ��ص�
    MMIIM_SP_TEXT_BAR_T bar_edit;
    MMIIM_SP_TEXT_BAR_T bar_cand;

    wchar asso_chr[MMIIM_SP_ASSO_CNT_MAX];
    MMIIM_SP_BAR_DATA_T asso;

    //draw flag
    BOOLEAN is_rect_change;
    BOOLEAN is_bar_edit_dirty;
    BOOLEAN is_bar_cand_dirty;
} MMIIM_SP_PY26KEY_DATA_T;


#endif //MMIIM_USE_SPIM_QWERT

#endif /*_MMIIM_SP_PY26KEY_H_*/

#endif


