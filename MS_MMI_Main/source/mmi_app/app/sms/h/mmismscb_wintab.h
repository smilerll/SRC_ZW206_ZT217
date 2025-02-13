/******************************************************************************
**
**
**
**  小区广播模块
**
**
**
**
**
**
*******************************************************************************/
#if defined(MMI_SMSCB_SUPPORT)

#ifndef  _MMISMS_CB_WINTAB_H_    
#define  _MMISMS_CB_WINTAB_H_   
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "dal_time.h"
#include "mmk_type.h"
#include "mmisms_export.h"
#include "mmismscb_control.h"
#include "mn_type.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif

#define MMISMSCB_VIBRATE_INTERVAL 500

/******************************************************************************************************************/
//
//
//                        小区广播模块对外接口函数声明
//
//
/******************************************************************************************************************/

/*****************************************************************************/
//     Description : MMISMSCB_OpenCBList
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMSCB_OpenCBList(void);

/*****************************************************************************/
//     Description : MMISMSCB_SaveCBEmergenceChannel
//    Global resource dependence :
//  Author: Tong Yongli
//    Note:
/*****************************************************************************/
PUBLIC void MMISMSCB_SaveCBEmergenceChannel(MN_DUAL_SYS_E dual_sys);

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         NV_LoadSmsCBChannelList
//  功能描述:        读取NV中小区广播信道列表结构
//  全局变量使用:    无
//  返回值:          NV中保存的小区广播信道列表结构
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC MMISMS_CB_CHANNEL_T MMISMS_NV_LoadSmsCBChannelList(MN_DUAL_SYS_E dual_sys);


#ifdef MMI_ETWS_SUPPORT
//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMS_NV_LoadSmsCBMessageList
//  功能描述:        读取NV中小区广播信息数目
//  全局变量使用:    无
//  返回值:          NV中保存的小区广播信息
//  作者:
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC uint8 MMISMS_NV_LoadSmsCBMessageListNum(void);

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMSNV_SaveSmsCBMessageList
//  功能描述:        保存小区广播信息数目
//  全局变量使用:    无
//  返回值:          无
//  作者:
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC void MMISMSNV_SaveSmsCBMessageListNum( uint8 message_num);

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMS_NV_LoadSmsCBMessageList
//  功能描述:        读取NV中小区广播信息
//  全局变量使用:    无
//  返回值:          NV中保存的小区广播信息
//  作者:
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC MMISMS_CB_MESSAGE_ARRAY_T* MMISMS_NV_LoadSmsCBMessageList(uint8 index);

 //----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMSNV_SaveSmsCBMessageList
//  功能描述:        保存小区广播信息
//  全局变量使用:    无
//  返回值:          无
//  作者:
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC void MMISMSNV_SaveSmsCBMessageList(MMISMS_CB_MESSAGE_ARRAY_T*  message_array, uint8 index);
#endif

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         GetCBChannelNameByMsgId
//  功能描述:        由小区信道ID获得相应的信道名称
//  全局变量使用:    无
//  返回值:          信道名称
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC MMISMS_CB_CHANNEL_NAME_T MMISMS_GetCBChannelNameByMsgId(                                        //RETURN: 
                                                       MMISMS_CB_CHANNEL_T*    channel_list,    //IN: 信道列表结构
                                                       uint16                msg_id            //IN: 小区信道ID
                                                       );

/*****************************************************************************/
//  Description : open SMSCB CLASS0 window      
//  Global resource dependence : 
//  Author: jixin.xu
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMSCB_OpenCLASS0Win(void);

/*****************************************************************************/
//     Description : to show the new SMSCB prompt
//    Global resource dependence : 
//  Author:Tracy Zhang
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_ShowNewSMSCBPrompt(void);

#ifdef MMI_ETWS_SUPPORT
/*****************************************************************************/
//     Description : to show the new ETWS prompt
//    Global resource dependence :
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_ShowNewETWSPrompt(MMI_STRING_T* etws_prompt_str, MMI_STRING_T* etws_title_string,BOOLEAN is_enable);
#endif
//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMSCB_DirectDisplayNewCBMsg
//  功能描述:        立即显示收到的新小区广播内容
//  全局变量使用:    无
//  返回值:          无
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC void MMISMSCB_DirectDisplayNewCBMsg(
                                 APP_MN_SMSCB_MSG_IND_T *cb_msg_ptr    //广播信息信号指针  (输入参数)

                                 );

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMSCB_DirectDisplayNewMarcoCBMsg
//  功能描述:        立即显示收到的新小区广播内容
//  全局变量使用:    无
//  返回值:          无
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC void MMISMSCB_DirectDisplayNewMarcoCBMsg(
                                 APP_MN_SMSCB_MACRO_MSG_IND_T *cb_msg_ptr    //广播信息信号指针  (输入参数)
                                 );
#ifdef MODEM_SUPPORT_SPRD
//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMSCB_DirectDisplayNewLteWarningMsg
//  功能描述:        立即显示收到的新小区广播内容
//  全局变量使用:    无
//  返回值:          无
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC void MMISMSCB_DirectDisplayNewLteWarningMsg(
                                 APP_MN_LTE_WARNING_IND_T *cb_msg_ptr  //广播信息信号指针  (输入参数)
                                 );
#endif

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         ConvertCBMsgContextToMsgItem
//  功能描述:        将小区广播内容按照其编码方式解压为小区广播信息项结构
//  全局变量使用:    无
//  返回值:          小区广播信息
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC uint16 ConvertCBMsgContextToMsgItem(                                           //RETURN: 
                                    MN_CB_MSG_ARR_T           msg_arr,         //IN: 小区广播消息内容数据指针
                                    uint16                    msg_len,         //IN: 小区广播消息内容数据长度
                                    MN_SMS_ALPHABET_TYPE_E    alphabet_type,   //IN: 小区广播消息内容编码方式
                                    BOOLEAN                   is_macro_msg,    //IN: 是否为长小区广播
                                    MMISMS_CB_MESSAGE_ITEM_T  *msg_item_ptr    //IN: 小区广播信息项指针
                                    );

#if 0
/*****************************************************************************/
// 	Description : enter cb msg list win
//	Global resource dependence : 
//  Author: jixin.xu
//	Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISMS_EnterMsgListWin(void);
#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif
#endif

