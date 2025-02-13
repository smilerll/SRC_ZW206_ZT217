/*****************************************************************************
** File Name:      zmt_gpt_id.h                                            *
** Author:           fys                                                        *
** Date:           2024/05/16                                                 *
** Copyright:       *
** Description:                       *
******************************************************************************

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_module.h"
#include "mmk_type.h"
#include "os_api.h"
#include "sci_types.h"


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

#define WIN_ID_DEF(win_id) win_id

// window ID
typedef enum {
    ZMT_GPT_WIN_ID_START = (MMI_MODULE_ZMT_GPT << 16),

    ZMT_GPT_MAIN_WIN_ID,

    ZMT_GPT_ZUOWEN_WIN_ID,

    ZMT_GPT_KOUYU_TALK_WIN_ID,
    ZMT_GPT_KOUYU_TOPIC_WIN_ID,
  
    ZMT_GPT_MAX_WIN_ID

} ZMT_GPT_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum {
    ZMT_GPT_CTRL_ID_START = ZMT_GPT_MAX_WIN_ID,

    ZMT_GPT_MAIN_LIST_CTRL_ID,

    ZMT_GPT_ZUOWEN_LIST_CTRL_ID,

    ZMT_GPT_KOUYU_TOPIC_LIST_CTRL_ID,
    ZMT_GPT_KOUYU_LIST_CTRL_ID,

    ZMT_GPT_FORM_CTRL_ID,
    ZMT_GPT_FORM_ANIM_CTRL_ID,
    ZMT_GPT_FORM_TEXT_1_CTRL_ID,
    ZMT_GPT_FORM_TEXT_MAX_CTRL_ID = ZMT_GPT_FORM_TEXT_1_CTRL_ID + 30,
  
    ZMT_GPT_MAX_CTRL_ID

} ZMT_GPT_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

