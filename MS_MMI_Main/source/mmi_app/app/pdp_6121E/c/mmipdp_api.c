/****************************************************************************************
** File Name:      mmipdp_api.c                                                         *
** Author:         juan.zhang                                                           *
** Date:           2009.5.27                                                            *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.                  *
** Description:     for pdp management in MMI                                           *
** NOTE:            This PDP manager is not suitable for multitask usage. All APP using *
this manager must in one task, 'cause the manager instance is a     *
global variable. And the global variable must be protected by mutex *
in multitask usage                                                  *
*****************************************************************************************
**                         Important Edit History                                       *
** -------------------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                          *
** 2009.5        juan.zhang         Create                                              *
**                                                                                      *
*****************************************************************************************/
/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_pdp_trc.h"
#if defined(MMI_GPRS_SUPPORT) || defined(MMI_WIFI_SUPPORT)
#ifdef WIN32
#include "std_header.h"
#endif
#include "os_api.h"
#include "mmk_type.h"
#include "mmk_app.h"
#include "mmipdp_internal.h"
#include "mmipdp_list.h"
#include "sig_code.h"
#include "mmi_signal.h"
#include "mmi_signal_ext.h"
#include "mn_type.h"
#include "mn_api.h"
#include "mmi_nv.h"
#include "mmk_timer.h"
//#include "mmipdp_export.h"
#include "mmi_modu_main.h"
#include "mmiphone_export.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmiidle_export.h"
#include "mmipdp_export.h"
#include "sci_api.h"
#include "mmisms_export.h"

#ifdef ENGTD_SUPPORT
#include "mmiengtd_export.h"
#include "mmieng_dynamic_main.h"
#endif

#ifdef MMI_WIFI_SUPPORT
#include "cfl_list.h"
#include "mmiwifi_export.h"
#endif
#include "mmicc_export.h"
#ifdef FDN_SUPPORT
#include "mmifdn_export.h"
#endif
#include "mmiidle_statusbar.h"
//#include "mmicom_statusarea.h"
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
#include "mmiplmn_export.h"
#endif
#include "mmipub.h"
#ifdef DATACOUNTER_SUPPORT
#include "mmidatacounter_export.h"
#endif

#ifdef UAL_TELE_SUPPORT
#include "ual_tele_common.h"
#include "ual_common.h"
#include "ual_tele_radio.h"
#endif

/**-------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMIPDP_ACTIVE_TIMOUT        50000
#define MMIPDP_REACTIVE_MAX_COUNT   3
#define MMIPDP_REACTIVE_INTERVAL    5000
#define MMIPDP_DEFAULT_ACTIVE_PRO   3       //激活默认pdp优先级
#define MMI_MODULE_PDP             (0x03 << 16)

/**-------------------------------------------------------------------------*
**                         ENUM DEFINITION                                  *
**--------------------------------------------------------------------------*/
/**-------------------------------------------------------------------------*
**                         TYPE DEFINITION                                  *
**--------------------------------------------------------------------------*/
/**-------------------------------------------------------------------------*
**                         STRUCTURE DEFINITION                             *
**--------------------------------------------------------------------------*/

typedef struct 
{
    MMIPDP_APP_MSG_E    msg_id;
    uint32              msg_size;
    void                *msg_body;
}MMIPDP_FSM_MSG_T;

typedef struct 
{
    MMIPDP_ACTIVE_INFO_T    active_info;
    MMIPDP_APP_MSG_E        msg_id_to_callback;
}MMIPDP_LIST_APP_INFO_T;

typedef struct 
{
    MMIPDP_LINK_INFO_T          link_arr[MMIPDP_MAX_LINK_NUM];
    MMIPDP_LIST_HANDLE          pending_list;
    BOOLEAN                     is_release_all;
    MMIPDP_RELEASE_ALL_CALLBACK release_all_cb;
}MMIPDP_MANAGER_INSTANCE_T;

typedef struct 
{
    uint32                  module_id;
    uint32                  event_num;
    MMIPDP_NOTIFY_FUNC      notify_func;
    MMIPDP_NOTIFY_EVENT_E   *event_arr_ptr; 
}MMIPDP_SUBSCRIBER_T;

typedef struct
{
    MMIPDP_ACTIVE_INFO_T active_info;
    uint32                             link_id;
    uint32                  pdp_id;
    uint8                   nsapi;
}MMIPDP_ACTIVE_APN_SIM_SAME_INFO_T;

typedef void    (*FSM_DISPATCH)(MMIPDP_FSM_T *fsm_ptr, MMIPDP_FSM_MSG_T *msg_ptr);

extern int stricmp (const char *a, const char *b);

/**-------------------------------------------------------------------------*
**                         GLOBAL VARIABLE DEFINITION                       *
**--------------------------------------------------------------------------*/

MMIPDP_MANAGER_INSTANCE_T   g_mmipdp_manager_instance;
MMI_APPLICATION_T           g_pdp_app = {0};
uint32                      g_cur_send_flowrate = 0;
uint32                      g_cur_recv_flowrate = 0;
MMIPDP_LIST_HANDLE          g_subscriber_list = 0;
LOCAL MMIPDP_ACTIVE_APN_SIM_SAME_INFO_T *s_active_info_ptr = PNULL;

#ifdef MMI_WIFI_SUPPORT
MMIPDP_MEM_T stMmipdpMem = {0};
#endif

MMIPDP_INJECTION_FUNC       s_p_injiection_func = PNULL;
LOCAL BOOLEAN               s_public_link_is_active = {0};
LOCAL uint32                s_public_link_net_id = 0;

/**-------------------------------------------------------------------------*
**                         STATIC CLASS DEFINITION                          *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         STATIC VARIABLE DEFINITION                       *
**--------------------------------------------------------------------------*/
LOCAL const MMIPDP_NOTIFY_EVENT_E s_pdp_event[MMIPDP_NOTIFY_EVENT_MAX] = {
    MMIPDP_NOTIFY_EVENT_ACTIVED,
    MMIPDP_NOTIFY_EVENT_DEACTIVED,
    MMIPDP_NOTIFY_EVENT_ATTACH,
    MMIPDP_NOTIFY_EVENT_DETTACH
};

LOCAL MMIPDP_NOTIFY_EVENT_E s_last_pdp_event[MMI_DUAL_SYS_MAX] = {0};/*@CR255469 modify 2011.08.23*/
/**-------------------------------------------------------------------------*
**                         LOCAL FUNCTION DECLARE                           *
**--------------------------------------------------------------------------*/
LOCAL void FsmDeactived(MMIPDP_FSM_T *fsm_ptr, MMIPDP_FSM_MSG_T *msg_ptr);
LOCAL void FsmActived(MMIPDP_FSM_T *fsm_ptr, MMIPDP_FSM_MSG_T *msg_ptr);
LOCAL void FsmDeactiving(MMIPDP_FSM_T *fsm_ptr, MMIPDP_FSM_MSG_T *msg_ptr);
LOCAL void FsmActiving(MMIPDP_FSM_T *fsm_ptr, MMIPDP_FSM_MSG_T *msg_ptr);

LOCAL const FSM_DISPATCH  fsm_dispatch[MMIPDP_LINK_STATE_MAX] = {
    FsmDeactived,
    FsmActived,
    FsmDeactiving,
    FsmActiving
};

LOCAL BOOLEAN CheckIfCanUseTheSameLink(
                                       MMIPDP_ACTIVE_INFO_T *app_info_ptr,
                                       MMIPDP_LINK_INFO_T   *link_info_ptr
                                       );
LOCAL BOOLEAN PdpStringCopy(char **dest_ptr,char *src_ptr);
LOCAL void FreeMemAllocInListNode(MMIPDP_LIST_NODE_HANDLE node_handler);
LOCAL void FreeMemAllocInLinkInfo(uint32 link_id);
LOCAL BOOLEAN SetLinkAppInfo(MMIPDP_ACTIVE_INFO_T *app_info_ptr, uint32 link_id);
LOCAL void SetLinkReqChapForPapOrChap(uint32 link_id, BOOLEAN flag);
LOCAL uint32 RequestLinkId(MMIPDP_ACTIVE_INFO_T *app_info_ptr);
LOCAL BOOLEAN AddToList(
                        MMIPDP_LIST_HANDLE *list_handler_ptr, 
                        MMIPDP_ACTIVE_INFO_T *app_info_ptr,
                        MMIPDP_APP_MSG_E msg_id
                        );
LOCAL void Callback(
                    MMIPDP_ACTIVE_INFO_T *callback_info_ptr,
                    uint32 link_id,
                    MMIPDP_APP_MSG_E msg_id,
                    MMIPDP_RESULT_E result);
LOCAL MMIPDP_FSM_MSG_T* CreatMsg(MMIPDP_APP_MSG_E msg_id, void *msg_body_ptr);
LOCAL void DestroyMsg(MMIPDP_FSM_MSG_T *msg_ptr);
LOCAL MMIPDP_FSM_T* CreatFsmInstance(void);
LOCAL BOOLEAN FindNodeByApphandlerCb(MMIPDP_NODE_OBJECT data, void* app_handler);
LOCAL uint32 FindLinkIdByApphandler(uint32 app_handler);
LOCAL BOOLEAN ActiveFromPendingList(uint32 link_id);
LOCAL void SuspendAllAppInList(MMIPDP_LIST_HANDLE list_handler);
LOCAL BOOLEAN FsmDispatch(uint32 link_id,MMIPDP_FSM_MSG_T *msg_to_dispatch_ptr);
LOCAL void FsmStateTrans(MMIPDP_FSM_T *fsm_ptr, MMIPDP_LINK_STATE_E state);
LOCAL MMI_RESULT_E HandlePdpPsMsg(PWND app_ptr, uint16 msg_id, DPARAM param);
LOCAL uint32 FindLinkIdByDeactiveTimer(uint8 timer_id);
LOCAL uint32 FindLinkIdByReactiveTimer(uint8 timer_id);
LOCAL BOOLEAN FindNodeByActiveTimerCb(MMIPDP_NODE_OBJECT data, uint8 *timer_id_ptr);
LOCAL uint32 FindNodeByActiveTimer(
                                   uint8                *timer_id_ptr,      //in
                                   MMIPDP_LIST_HANDLE *list_handler_ptr, //out
                                   MMIPDP_LIST_NODE_HANDLE *node_handler_ptr //out
                                   );
LOCAL BOOLEAN ReActive(uint32 link_id);
LOCAL void ChangeFlowRate(uint32 link_id);

LOCAL uint32 FindLinkIdByPdpId(uint32 pdp_id, MN_DUAL_SYS_E dual_sys);
LOCAL void AddToSubscriber(MMIPDP_SUBSCRIBER_T *subscriber_info_ptr);
LOCAL void DeleteFromSubscriber(uint16 module_id);
LOCAL BOOLEAN FindSubscriberByModuleIdCb(MMIPDP_NODE_OBJECT data, uint32 module_id);
LOCAL MMIPDP_LIST_NODE_HANDLE FindSubscriberByModuleId(uint16 module_id);
LOCAL BOOLEAN CheckIfNotifySubscriber(MMIPDP_SUBSCRIBER_T *subscriber_info_ptr, MMIPDP_NOTIFY_EVENT_E event);
LOCAL void NotifySubscriber(MN_DUAL_SYS_E dual_sys, MMIPDP_NOTIFY_EVENT_E event);
LOCAL void StopAllActiveTimerInUsingList(uint32 link_id);
LOCAL void RestartAllActiveTimerInUsingList(uint32 link_id);
LOCAL void HandlePdpEvent(MN_DUAL_SYS_E dual_sys,  MMIPDP_NOTIFY_EVENT_E event);
LOCAL BOOLEAN MMIPDP_IsPdpIdInLinkArray(uint32 pdp_id, MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// Description : set gprs detach
// Global resource dependence : 
// Author: 
// Note:
/*****************************************************************************/
LOCAL void MMIPDP_SetGPRSDetach(MN_DUAL_SYS_E dual_sys);


/*****************************************************************************
// Description : MMIPDP_WifiDeactiveCnf
// Global resource dependence : 
// Author: George.Liu 01526
// Note:
*****************************************************************************/
#ifdef MMI_WIFI_SUPPORT
PUBLIC BOOLEAN MMIPDP_WifiDeactiveCnf(uint32 app_handler);
#endif
/****************************************************************************/
//Description : Get PDP Time Out
//Global resource dependence : 
//Author: dave.ruan
//Note:	
/****************************************************************************/
LOCAL uint32 GetPdpTimeOut(MN_DUAL_SYS_E dual_sys);

/**-------------------------------------------------------------------------*
**                         IMPLEMENTATION                                   *
**--------------------------------------------------------------------------*/
/****************************************************************************/
//Description : to check if the active app info is the same with current link
//              info
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL BOOLEAN CheckIfCanUseTheSameLink(
                                       MMIPDP_ACTIVE_INFO_T *app_info_ptr,
                                       MMIPDP_LINK_INFO_T   *link_info_ptr
                                       )
{
    BOOLEAN                 ret_val = FALSE;
    // MN_GPRS_EXT_QOS_INFO_T  link_qos_info = {0};
    
    if(app_info_ptr!=0 && link_info_ptr!=0)
    {
        if(app_info_ptr->apn_ptr!=0 && link_info_ptr->apn_ptr!=0)
        {
            //apn and dual sys is the same
            if(0==stricmp(app_info_ptr->apn_ptr,link_info_ptr->apn_ptr) &&
                app_info_ptr->dual_sys == link_info_ptr->dual_sys)
            {
#if 0
                //qos profile is the same
                if(app_info_ptr->peak_throughput==link_info_ptr->peak_throughput &&
                    app_info_ptr->reliability==link_info_ptr->reliability)
                {
                    SCI_MEMCPY(&link_qos_info, &link_info_ptr->qos_info, sizeof(MN_GPRS_EXT_QOS_INFO_T));
                    link_qos_info.pdp_id = 0;
                    //qos is the same
                    if(0==memcmp(
                        &app_info_ptr->qos_info,
                        &link_qos_info,
                        sizeof(MN_GPRS_EXT_QOS_INFO_T))
                        )
                    {
                        ret_val = TRUE;
                    }
                }           
#endif
                ret_val = TRUE;
            }
        }
    }
    return ret_val;
}


/****************************************************************************/
//Description : string copy
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL BOOLEAN PdpStringCopy(char **dest_ptr,char *src_ptr)
{
    uint32 len = 0;
    
    if(src_ptr==0)
    {
        return FALSE;
    }
    
    len = strlen(src_ptr);
    *dest_ptr = SCI_ALLOCA(len+1);
    if(*dest_ptr == 0)
    {
        //SCI_TRACE_LOW:"PdpStringCopy, ALLOC FAIL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_291_112_2_18_2_44_48_0,(uint8*)"");
        return FALSE;
    }
    SCI_MEMSET(*dest_ptr,0,(len+1));
    strcpy(*dest_ptr,src_ptr);
    return TRUE;
}

/****************************************************************************/
//Description : to free the memory allocated in list node
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL void FreeMemAllocInListNode(MMIPDP_LIST_NODE_HANDLE node_handler)
{
    MMIPDP_LIST_APP_INFO_T *node_data_ptr = 0;
    
    if(node_handler==0)
    {
        return;
    }
    node_data_ptr = MMIPDP_ListGetNodeData(node_handler);
    if(node_data_ptr == 0)
    {
        return;
    }
    if(node_data_ptr->active_info.apn_ptr !=0)
    {
        SCI_FREE(node_data_ptr->active_info.apn_ptr);
    }
    if(node_data_ptr->active_info.user_name_ptr !=0)
    {
        SCI_FREE(node_data_ptr->active_info.user_name_ptr);
    }
    if(node_data_ptr->active_info.psw_ptr!=0)
    {
        SCI_FREE(node_data_ptr->active_info.psw_ptr);
    }
}

/****************************************************************************/
//Description : to free the memory allocated in link info
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL void FreeMemAllocInLinkInfo(uint32 link_id)
{
    if(link_id==0 || link_id>MMIPDP_MAX_LINK_NUM)
    {
        return;
    }
    if(g_mmipdp_manager_instance.link_arr[link_id-1].apn_ptr!=0)
    {
        SCI_FREE(g_mmipdp_manager_instance.link_arr[link_id-1].apn_ptr);
    }
    if(g_mmipdp_manager_instance.link_arr[link_id-1].user_name_ptr!=0)
    {
        SCI_FREE(g_mmipdp_manager_instance.link_arr[link_id-1].user_name_ptr);
    }
    if(g_mmipdp_manager_instance.link_arr[link_id-1].psw_ptr!=0)
    {
        SCI_FREE(g_mmipdp_manager_instance.link_arr[link_id-1].psw_ptr);
    }
}
/****************************************************************************/
//Description : Set up the link with the APP_related info
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL BOOLEAN SetLinkAppInfo(MMIPDP_ACTIVE_INFO_T *app_info_ptr, uint32 link_id)
{
    //uint32 len = 0;
    
    if(app_info_ptr==0 || link_id==0 || link_id>MMIPDP_MAX_LINK_NUM)
    {
        return FALSE;
    }
    g_mmipdp_manager_instance.link_arr[link_id-1].dual_sys = app_info_ptr->dual_sys;
    g_mmipdp_manager_instance.link_arr[link_id-1].ps_service_type = app_info_ptr->ps_service_type;
    g_mmipdp_manager_instance.link_arr[link_id-1].ps_service_rat = app_info_ptr->ps_service_rat;
    g_mmipdp_manager_instance.link_arr[link_id-1].auth_type = app_info_ptr->auth_type;
    g_mmipdp_manager_instance.link_arr[link_id-1].qos_info = app_info_ptr->qos_info;
    g_mmipdp_manager_instance.link_arr[link_id-1].reliability = app_info_ptr->reliability;
    g_mmipdp_manager_instance.link_arr[link_id-1].peak_throughput = app_info_ptr->peak_throughput;
#ifdef IPVERSION_SUPPORT_V4_V6
    g_mmipdp_manager_instance.link_arr[link_id-1].ip_type = app_info_ptr->ip_type;
#endif
    if(app_info_ptr->apn_ptr == 0)
    {
        return FALSE;
    }
    
    FreeMemAllocInLinkInfo(link_id);
    
    if(!PdpStringCopy(&g_mmipdp_manager_instance.link_arr[link_id-1].apn_ptr,app_info_ptr->apn_ptr))
    {
        return FALSE;
    }
    
    if(app_info_ptr->user_name_ptr!=0)
    {
        if(!PdpStringCopy(&g_mmipdp_manager_instance.link_arr[link_id-1].user_name_ptr,app_info_ptr->user_name_ptr))
        {
            return FALSE;
        }
    }
    
    if(app_info_ptr->psw_ptr!=0)
    {
        if(!PdpStringCopy(&g_mmipdp_manager_instance.link_arr[link_id-1].psw_ptr,app_info_ptr->psw_ptr))
        {
            return FALSE;
        }
    }
    return TRUE;
}

/****************************************************************************/
//Description : SetLinkReqChapForPapOrChap
//Global resource dependence : 
//Author: 
//Note: 
/****************************************************************************/
LOCAL void SetLinkReqChapForPapOrChap(uint32 link_id, BOOLEAN flag)
{
    if(link_id <= MMIPDP_MAX_LINK_NUM && link_id != 0)
    {
        g_mmipdp_manager_instance.link_arr[link_id-1].reqChapForPapOrChap = flag;
    }
}

/****************************************************************************/
//Description : to request a link id, if no link avalible, return 0
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL uint32 RequestLinkId(MMIPDP_ACTIVE_INFO_T *app_info_ptr)
{
    uint32 i = 1;
    MMIPDP_LIST_APP_INFO_T *node_data_ptr = 0;
    
    //get the free link id
    while(i<=MMIPDP_MAX_LINK_NUM)
    {
        if(MMIPDP_LINK_STATE_DEACTIVED==g_mmipdp_manager_instance.link_arr[i-1].state)
        {
            break;
        }
        else
        {
            if(CheckIfCanUseTheSameLink(app_info_ptr,&(g_mmipdp_manager_instance.link_arr[i-1])))
            {
                break;
            }
            else
            {
                i++;
            }
        }
    }
    //no free link got
    if(i>MMIPDP_MAX_LINK_NUM)
    {
        i = 1;
        //find the lowest priority link
        while(i<=MMIPDP_MAX_LINK_NUM)
        {
            node_data_ptr = MMIPDP_ListGetNodeData(MMIPDP_ListGetHead(g_mmipdp_manager_instance.link_arr[0].using_list));
            if(node_data_ptr==0)
            {
                return 0;
            }
            if(node_data_ptr->active_info.priority>app_info_ptr->priority)
            {
                break;
            }
            else
            {
                i++;
            }
        }
        //the links' lowest priority is stiller higher than the requested app
        if(i>MMIPDP_MAX_LINK_NUM)
        {
            i = 0;
        }
    }
    return i;
}

/****************************************************************************/
//Description : add node to a list by active info
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL BOOLEAN AddToList(
                        MMIPDP_LIST_HANDLE *list_handler_ptr, 
                        MMIPDP_ACTIVE_INFO_T *app_info_ptr,
                        MMIPDP_APP_MSG_E msg_id
                        )
{
    MMIPDP_LIST_NODE_HANDLE node_handler = 0;
    MMIPDP_LIST_APP_INFO_T *data_ptr = 0;
    MMIPDP_LIST_APP_INFO_T inserted_data = {0};
    
    if(app_info_ptr==0 || list_handler_ptr==0)
    {
        return FALSE;
    }
    if(*list_handler_ptr == 0)
    {
        *list_handler_ptr = MMIPDP_ListCreat(sizeof(MMIPDP_LIST_APP_INFO_T));
    }
    if(*list_handler_ptr!=0)
    {
        node_handler = MMIPDP_ListGetHead(*list_handler_ptr);
        while(node_handler!=0)
        {
            data_ptr = MMIPDP_ListGetNodeData(node_handler);
            if(data_ptr==0)
            {
                return FALSE;
            }
            if(data_ptr->active_info.priority < app_info_ptr->priority)
            {
                node_handler = MMIPDP_ListGetNextNode(node_handler);
            }
            else
            {
                break;
            }
        }
        
        inserted_data.active_info = *app_info_ptr;
        if(!PdpStringCopy(&inserted_data.active_info.apn_ptr,app_info_ptr->apn_ptr))
        {
            return FALSE;
        }
        
        if(app_info_ptr->user_name_ptr!=0)
        {
            if(!PdpStringCopy(&inserted_data.active_info.user_name_ptr,app_info_ptr->user_name_ptr))
            {
                return FALSE;
            }
        }
        
        if(app_info_ptr->psw_ptr!=0)
        {
            if(!PdpStringCopy(&inserted_data.active_info.psw_ptr,app_info_ptr->psw_ptr))
            {
                return FALSE;
            }
        }
        if(msg_id != 0)
        {
            inserted_data.msg_id_to_callback = msg_id;
        }
        if(node_handler!=0)
        {
            node_handler = MMIPDP_ListGetPrevNode(node_handler);
        }
        else
        {
            node_handler = MMIPDP_ListGetTail(*list_handler_ptr);
        }
        if (0 == MMIPDP_ListInsertNode(*list_handler_ptr,
            node_handler,
            &inserted_data,
            sizeof(MMIPDP_LIST_APP_INFO_T)))
        {
            return FALSE;
        }
        return TRUE;
    }
    return FALSE;
}

/****************************************************************************/
//Description : the callback function
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL void Callback(
                    MMIPDP_ACTIVE_INFO_T *callback_info_ptr,
                    uint32 link_id,
                    MMIPDP_APP_MSG_E msg_id,
                    MMIPDP_RESULT_E result)
{
    MMIPDP_CNF_INFO_T *cnf_msg_ptr = 0;
    
    if(callback_info_ptr==0)
    {
        return;
    }
    //SCI_TRACE_LOW:"PDP MANAGER callback,app_handler:%d,msg:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_563_112_2_18_2_44_49_1,(uint8*)"dd",callback_info_ptr->app_handler,msg_id);
    if(callback_info_ptr->active_timer_id != 0)
    {
        MMIPDP_StopTimer(&callback_info_ptr->active_timer_id);
    }
    cnf_msg_ptr = SCI_ALLOCA(sizeof(MMIPDP_CNF_INFO_T));
    if(cnf_msg_ptr!=0)
    {
        cnf_msg_ptr->msg_id = msg_id;
        if(result!=MMIPDP_RESULT_SUCC && msg_id==MMIPDP_RESUME_IND)
        {
            cnf_msg_ptr->msg_id = MMIPDP_DEACTIVE_IND;
        }
        cnf_msg_ptr->app_handler = callback_info_ptr->app_handler;
        cnf_msg_ptr->result = result;
        cnf_msg_ptr->ps_interface = callback_info_ptr->ps_interface;
        if(msg_id==MMIPDP_ACTIVE_CNF && result==MMIPDP_RESULT_SUCC)
        {
            if(link_id>0 && link_id<=MMIPDP_MAX_LINK_NUM)
            {
                cnf_msg_ptr->nsapi = g_mmipdp_manager_instance.link_arr[link_id-1].nsapi;
            }
        }
        else
        {
            cnf_msg_ptr->nsapi = 0;
        }
        if(callback_info_ptr->handle_msg_callback != 0)
        {
            (callback_info_ptr->handle_msg_callback)(cnf_msg_ptr);
        }
        SCI_FREE(cnf_msg_ptr);
    }
    else
    {
        //SCI_TRACE_LOW:"Callback, ALLOC FAIL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_598_112_2_18_2_44_49_2,(uint8*)"");
    }
}

/****************************************************************************/
//Description : to creat a fsm message
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL MMIPDP_FSM_MSG_T* CreatMsg(MMIPDP_APP_MSG_E msg_id, void *msg_body_ptr)
{
    MMIPDP_FSM_MSG_T *msg_ptr = 0;
    
    switch(msg_id)
    {
    case MMIPDP_ACTIVE_REQ://msg_body = MMIPDP_ACTIVE_INFO_T
        {
            if(msg_body_ptr==0)
            {
                break;
            }
            msg_ptr = SCI_ALLOCA(sizeof(MMIPDP_FSM_MSG_T)+sizeof(MMIPDP_ACTIVE_INFO_T));
            if(msg_ptr!=0)
            {
                SCI_MEMSET(msg_ptr,0,(sizeof(MMIPDP_FSM_MSG_T)+sizeof(MMIPDP_ACTIVE_INFO_T)));
                msg_ptr->msg_id = msg_id;
                msg_ptr->msg_size = sizeof(MMIPDP_FSM_MSG_T)+sizeof(MMIPDP_ACTIVE_INFO_T);
                msg_ptr->msg_body = (uint8*)msg_ptr+sizeof(MMIPDP_FSM_MSG_T);
                SCI_MEMCPY(msg_ptr->msg_body,msg_body_ptr,sizeof(MMIPDP_ACTIVE_INFO_T));
                
                if(!PdpStringCopy(&((MMIPDP_ACTIVE_INFO_T*)msg_ptr->msg_body)->apn_ptr,
                    ((MMIPDP_ACTIVE_INFO_T*)msg_body_ptr)->apn_ptr))
                {
                    return 0;
                }
                if(((MMIPDP_ACTIVE_INFO_T*)msg_body_ptr)->user_name_ptr!=0)
                {
                    if(!PdpStringCopy(&((MMIPDP_ACTIVE_INFO_T*)msg_ptr->msg_body)->user_name_ptr,
                        ((MMIPDP_ACTIVE_INFO_T*)msg_body_ptr)->user_name_ptr))
                    {
                        return 0;
                    }
                }
                if(((MMIPDP_ACTIVE_INFO_T*)msg_body_ptr)->psw_ptr!=0)
                {
                    if(!PdpStringCopy(&((MMIPDP_ACTIVE_INFO_T*)msg_ptr->msg_body)->psw_ptr,
                        ((MMIPDP_ACTIVE_INFO_T*)msg_body_ptr)->psw_ptr))
                    {
                        return 0;
                    }
                }
            }
            else
            {
                //SCI_TRACE_LOW:"CreatMsg, MMIPDP_ACTIVE_REQ ALLOC FAIL"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_651_112_2_18_2_44_49_3,(uint8*)"");
            }
        }
        break;
    case MMIPDP_DEACTIVE_REQ://msg_body = app_handler
        {
            msg_ptr = SCI_ALLOCA(sizeof(MMIPDP_FSM_MSG_T));
            if(msg_ptr!=0)
            {
                SCI_MEMSET(msg_ptr,0,sizeof(MMIPDP_FSM_MSG_T));
                msg_ptr->msg_id = msg_id;
                msg_ptr->msg_size = sizeof(MMIPDP_FSM_MSG_T);
                msg_ptr->msg_body = msg_body_ptr;
            }
            else
            {
                //SCI_TRACE_LOW:"CreatMsg, MMIPDP_DEACTIVE_REQ ALLOC FAIL"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_667_112_2_18_2_44_49_4,(uint8*)"");
            }
        }
        break;
    case MMIPDP_PS_ACTIVE_CNF:
        msg_ptr = SCI_ALLOCA(sizeof(MMIPDP_FSM_MSG_T)+sizeof(APP_MN_GPRS_EXT_T));
        if(msg_ptr!=0)
        {
            SCI_MEMSET(msg_ptr,0,(sizeof(MMIPDP_FSM_MSG_T)+sizeof(APP_MN_GPRS_EXT_T)));
            msg_ptr->msg_id = msg_id;
            msg_ptr->msg_size = sizeof(MMIPDP_FSM_MSG_T);
            msg_ptr->msg_body = (uint8*)msg_ptr+sizeof(MMIPDP_FSM_MSG_T);
            SCI_MEMCPY(msg_ptr->msg_body,msg_body_ptr,sizeof(APP_MN_GPRS_EXT_T));
        }
        else
        {
            //SCI_TRACE_LOW:"CreatMsg, MMIPDP_PS_ACTIVE_CNF ALLOC FAIL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_683_112_2_18_2_44_49_5,(uint8*)"");
        }
        break;
    case MMIPDP_ACTIVE_TIMOUT_IND: //msg_body = using list node handler
        msg_ptr = SCI_ALLOCA(sizeof(MMIPDP_FSM_MSG_T));
        if(msg_ptr!=0)
        {
            SCI_MEMSET(msg_ptr,0,sizeof(MMIPDP_FSM_MSG_T));
            msg_ptr->msg_id = msg_id;
            msg_ptr->msg_size = sizeof(MMIPDP_FSM_MSG_T);
            msg_ptr->msg_body = msg_body_ptr;
        }
        else
        {
            //SCI_TRACE_LOW:"CreatMsg, msg_id=%d ALLOC FAIL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_697_112_2_18_2_44_49_6,(uint8*)"d",msg_id);
        }
        break;
    case MMIPDP_PS_DEACTIVE_CNF:
    case MMIPDP_PS_DEACTIVE_IND:
    case MMIPDP_REACTIVE_TIMER_IND:
        msg_ptr = SCI_ALLOCA(sizeof(MMIPDP_FSM_MSG_T));
        if(msg_ptr!=0)
        {
            SCI_MEMSET(msg_ptr,0,sizeof(MMIPDP_FSM_MSG_T));
            msg_ptr->msg_id = msg_id;
            msg_ptr->msg_size = sizeof(MMIPDP_FSM_MSG_T);
            msg_ptr->msg_body = 0;
        }
        else
        {
            //SCI_TRACE_LOW:"CreatMsg, msg_id=%d ALLOC FAIL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_713_112_2_18_2_44_49_7,(uint8*)"d",msg_id);
        }
        break;
    default:
        break;
    }
    return msg_ptr;
}
/****************************************************************************/
//Description : to creat a fsm message
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL void DestroyMsg(MMIPDP_FSM_MSG_T *msg_ptr)
{
    if(msg_ptr==0)
    {
        return;
    }
    switch(msg_ptr->msg_id)
    {
    case MMIPDP_ACTIVE_REQ://msg_body = MMIPDP_ACTIVE_INFO_T
        if(((MMIPDP_ACTIVE_INFO_T*)msg_ptr->msg_body)->apn_ptr !=0)
        {
            SCI_FREE(((MMIPDP_ACTIVE_INFO_T*)msg_ptr->msg_body)->apn_ptr);
        }
        if(((MMIPDP_ACTIVE_INFO_T*)msg_ptr->msg_body)->user_name_ptr !=0)
        {
            SCI_FREE(((MMIPDP_ACTIVE_INFO_T*)msg_ptr->msg_body)->user_name_ptr);
        }
        if(((MMIPDP_ACTIVE_INFO_T*)msg_ptr->msg_body)->psw_ptr !=0)
        {
            SCI_FREE(((MMIPDP_ACTIVE_INFO_T*)msg_ptr->msg_body)->psw_ptr);
        }
        break;
    default:
        break;
    }
    SCI_FREE(msg_ptr);
}
/****************************************************************************/
//Description : to creat a fsm instance
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL MMIPDP_FSM_T* CreatFsmInstance(void)
{
    MMIPDP_FSM_T *fsm_ptr = 0;
    
    fsm_ptr = SCI_ALLOCA(sizeof(MMIPDP_FSM_T));
    if(0 != fsm_ptr)
    {
        SCI_MEMSET(fsm_ptr,0,sizeof(MMIPDP_FSM_T));
    }
    else
    {
        //SCI_TRACE_LOW:"CreatFsmInstance, ALLOC FAIL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_767_112_2_18_2_44_49_8,(uint8*)"");
    }
    return fsm_ptr;
}

/****************************************************************************/
//Description : to find node by app handler
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL BOOLEAN FindNodeByApphandlerCb(MMIPDP_NODE_OBJECT data, void* app_handler)
{
    if(data==0 || app_handler==0)
    {
        return FALSE;
    }
    if(((MMIPDP_LIST_APP_INFO_T*)data)->active_info.app_handler == (uint32)app_handler)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/****************************************************************************/
//Description : to find the app handler's using link
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL uint32 FindLinkIdByApphandler(uint32 app_handler)
{
    uint32 link_id = 1;
    MMIPDP_LIST_NODE_HANDLE node_handler = 0;
    
    while(link_id<=MMIPDP_MAX_LINK_NUM)
    {
        if(g_mmipdp_manager_instance.link_arr[link_id-1].using_list!=0)
        {
            node_handler = MMIPDP_ListFindNode(
                g_mmipdp_manager_instance.link_arr[link_id-1].using_list,
                (void*)app_handler,
                FindNodeByApphandlerCb
                );
        }
        if(node_handler!=0)
        {
            break;
        }
        else
        {
            link_id++;
        }
    }
    if(link_id>MMIPDP_MAX_LINK_NUM)
    {
        link_id = 0;
    }
    return link_id;
}
/****************************************************************************/
//Description : to find the link using deactive timer
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL uint32 FindLinkIdByDeactiveTimer(uint8 timer_id)
{
    uint32 link_id = 1;
    if(timer_id == 0)
    {
        return 0;
    }
    while(link_id<=MMIPDP_MAX_LINK_NUM)
    {
        if(g_mmipdp_manager_instance.link_arr[link_id-1].deactive_protect_timer == timer_id)
        {
            break;
        }
        else
        {
            link_id++;
        }
    }
    if(link_id<=MMIPDP_MAX_LINK_NUM)
    {
        return link_id;
    }
    else
    {
        return 0;
    }
}

/****************************************************************************/
//Description : to find the link using deactive timer
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL uint32 FindLinkIdByReactiveTimer(uint8 timer_id)
{
    uint32 link_id = 1;
    if(timer_id == 0)
    {
        return 0;
    }
    while(link_id<=MMIPDP_MAX_LINK_NUM)
    {
        if(g_mmipdp_manager_instance.link_arr[link_id-1].reactive_timer_id == timer_id)
        {
            break;
        }
        else
        {
            link_id++;
        }
    }
    if(link_id<=MMIPDP_MAX_LINK_NUM)
    {
        return link_id;
    }
    else
    {
        return 0;
    }
}

/****************************************************************************/
//Description : FindNodeByActiveTimerCb
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL BOOLEAN FindNodeByActiveTimerCb(MMIPDP_NODE_OBJECT data, uint8 *timer_id_ptr)
{
    if(data==0 || *timer_id_ptr==0)
    {
        return FALSE;
    }
    if(((MMIPDP_LIST_APP_INFO_T*)data)->active_info.active_timer_id == *timer_id_ptr)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/****************************************************************************/
//Description : FindNodeByActiveTimer
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL uint32 FindNodeByActiveTimer(
                                   uint8                *timer_id_ptr,      //in
                                   MMIPDP_LIST_HANDLE *list_handler_ptr, //out
                                   MMIPDP_LIST_NODE_HANDLE *node_handler_ptr //out
                                   )
{
    uint32 link_id = 1;
    
    if(list_handler_ptr!=0 && node_handler_ptr!=0)
    {
        if(g_mmipdp_manager_instance.pending_list!=0)
        {
            *node_handler_ptr = MMIPDP_ListFindNode(g_mmipdp_manager_instance.pending_list,
                timer_id_ptr,
                FindNodeByActiveTimerCb
                );/*lint !e64*/
            if(*node_handler_ptr != 0)
            {
                *list_handler_ptr = g_mmipdp_manager_instance.pending_list;
                return 0;
            }
        }
        while(link_id>0 && link_id<=MMIPDP_MAX_LINK_NUM)
        {
            *node_handler_ptr = MMIPDP_ListFindNode(g_mmipdp_manager_instance.link_arr[link_id-1].using_list,
                timer_id_ptr,
                FindNodeByActiveTimerCb
                );/*lint !e64*/
            if(*node_handler_ptr != 0)
            {
                *list_handler_ptr = g_mmipdp_manager_instance.link_arr[link_id-1].using_list;
                break;
            }
            else
            {
                link_id++;
            }
        }
        if(link_id>MMIPDP_MAX_LINK_NUM)
        {
            link_id = 0;
        }
        return link_id;
    }
    return 0;
}

/****************************************************************************/
//Description : StopAllActiveTimerInUsingList
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL void StopAllActiveTimerInUsingList(uint32 link_id)
{
    MMIPDP_ACTIVE_INFO_T    *active_info_ptr = 0;
    MMIPDP_LIST_NODE_HANDLE node_handler = 0;
    
    if(link_id==0 || link_id>MMIPDP_MAX_LINK_NUM)
    {
        return ;
    }
    node_handler = MMIPDP_ListGetHead(g_mmipdp_manager_instance.link_arr[link_id-1].using_list);
    while(node_handler!=0)
    {
        active_info_ptr = MMIPDP_ListGetNodeData(node_handler);
        MMIPDP_StopTimer(&active_info_ptr->active_timer_id);
        node_handler = MMIPDP_ListGetNextNode(node_handler);
    }
}

/****************************************************************************/
//Description : RestartAllActiveTimerInUsingList
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL void RestartAllActiveTimerInUsingList(uint32 link_id)
{
    MMIPDP_ACTIVE_INFO_T    *active_info_ptr = 0;
    MMIPDP_LIST_NODE_HANDLE node_handler = 0;
    
    if(link_id==0 || link_id>MMIPDP_MAX_LINK_NUM)
    {
        return ;
    }
    node_handler = MMIPDP_ListGetHead(g_mmipdp_manager_instance.link_arr[link_id-1].using_list);
    while(node_handler!=0)
    {
        active_info_ptr = MMIPDP_ListGetNodeData(node_handler);
        if(active_info_ptr->active_timer_id!=0)
        {
            MMIPDP_StopTimer(&active_info_ptr->active_timer_id);
        }
        active_info_ptr->active_timer_id = MMIPDP_CreatTimer(GetPdpTimeOut(active_info_ptr->dual_sys));
        node_handler = MMIPDP_ListGetNextNode(node_handler);
    }
}

/****************************************************************************/
//Description : ReActive
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL BOOLEAN ReActive(uint32 link_id)
{
    if(link_id==0 || link_id>MMIPDP_MAX_LINK_NUM)
    {
        SCI_TRACE_LOW("[ReActive] link id error");
        return FALSE;
    }
    MMIPDP_StopTimer(&g_mmipdp_manager_instance.link_arr[link_id-1].reactive_timer_id);
    StopAllActiveTimerInUsingList(link_id);
    SCI_TRACE_LOW("[ReActive] reactive_count = %d", g_mmipdp_manager_instance.link_arr[link_id-1].reactive_count);
    if(g_mmipdp_manager_instance.link_arr[link_id-1].reactive_count<MMIPDP_REACTIVE_MAX_COUNT)
    {
        g_mmipdp_manager_instance.link_arr[link_id-1].reactive_timer_id = 
                                                MMIPDP_CreatTimer(MMIPDP_REACTIVE_INTERVAL);
        return TRUE;
    }
    else
    {
        g_mmipdp_manager_instance.link_arr[link_id-1].reactive_count = 0;
        return FALSE;
    }
}
/****************************************************************************/
//Description : to active pdp using the pending list's info
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL BOOLEAN ActiveFromPendingList(uint32 link_id)
{
    //BOOLEAN is_succ = FALSE;
    MMIPDP_LIST_NODE_HANDLE node_handler = 0;
    MMIPDP_LIST_APP_INFO_T  *node_data_ptr = 0;
    if(g_mmipdp_manager_instance.pending_list==0||
        link_id==0||
        link_id>MMIPDP_MAX_LINK_NUM)
    {
        return FALSE;
    }
    node_handler = MMIPDP_ListGetHead(g_mmipdp_manager_instance.pending_list);
    if(node_handler!=PNULL)
    {
        node_data_ptr = MMIPDP_ListGetNodeData(node_handler);
        if(node_data_ptr!=PNULL)
        {
            SetLinkAppInfo(&(node_data_ptr->active_info),link_id);
            // modified by Tong Yongli. According to Coverity CID 10095
            while((!MMIPDP_PsActivePdp(link_id)) && (PNULL != node_data_ptr))
            {
                FreeMemAllocInListNode(node_handler);
                MMIPDP_ListDeleteNode(g_mmipdp_manager_instance.pending_list,node_handler);
                Callback(&(node_data_ptr->active_info),link_id,node_data_ptr->msg_id_to_callback,MMIPDP_RESULT_FAIL);
                FreeMemAllocInLinkInfo(link_id);

                node_handler = MMIPDP_ListGetHead(g_mmipdp_manager_instance.pending_list);
                if(node_handler==PNULL)
                {
                    return FALSE;
                }
                node_data_ptr = MMIPDP_ListGetNodeData(node_handler);
                if(node_data_ptr!=PNULL)
                {
                    SetLinkAppInfo(&(node_data_ptr->active_info),link_id);
                }
            }
            if(node_data_ptr == PNULL)
            {
                return FALSE;
            }
            AddToList(&g_mmipdp_manager_instance.link_arr[link_id-1].using_list,&node_data_ptr->active_info,node_data_ptr->msg_id_to_callback);
            FreeMemAllocInListNode(node_handler);
            MMIPDP_ListDeleteNode(g_mmipdp_manager_instance.pending_list,node_handler);
            return TRUE;
        }
    }
    return FALSE;
}

/****************************************************************************/
//Description : to suspend all using app
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL void SuspendAllAppInList(MMIPDP_LIST_HANDLE list_handler)
{
    MMIPDP_LIST_NODE_HANDLE node_handler = 0;
    MMIPDP_LIST_APP_INFO_T  *node_data_ptr = 0;
    
    if(list_handler==0)
    {
        return;
    }
    node_handler = MMIPDP_ListGetHead(list_handler);
    while(node_handler!=0)
    {
        node_data_ptr = MMIPDP_ListGetNodeData(node_handler);
        if(node_data_ptr!=0)
        {
            node_data_ptr->msg_id_to_callback = MMIPDP_SUSPEND_IND;
        }
        node_handler = MMIPDP_ListGetNextNode(node_handler);
    }
}



/****************************************************************************/
//Description : fsm Dispatch
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL BOOLEAN FsmDispatch(uint32 link_id,MMIPDP_FSM_MSG_T *msg_to_dispatch_ptr)
{
    MMIPDP_FSM_T *fsm_ptr = 0;
    MMIPDP_LINK_STATE_E state = MMIPDP_LINK_STATE_MAX;
    
    if(link_id>MMIPDP_MAX_LINK_NUM || link_id==0 || msg_to_dispatch_ptr==0)
    {
        DestroyMsg(msg_to_dispatch_ptr);
        return FALSE;
    }
    
    fsm_ptr = g_mmipdp_manager_instance.link_arr[link_id-1].fsm_ptr;
    state = g_mmipdp_manager_instance.link_arr[link_id-1].state;
    if(fsm_ptr==0 || state>=MMIPDP_LINK_STATE_MAX)
    {
        DestroyMsg(msg_to_dispatch_ptr);
        return FALSE;
    }
    
    if(!fsm_ptr->is_busy)
    {
        (*fsm_dispatch[state])(fsm_ptr,msg_to_dispatch_ptr);
        DestroyMsg(msg_to_dispatch_ptr);
        return TRUE;
    }
    
    DestroyMsg(msg_to_dispatch_ptr);
    return FALSE;
}

/****************************************************************************/
//Description : fsm state transition
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL void FsmStateTrans(MMIPDP_FSM_T *fsm_ptr, MMIPDP_LINK_STATE_E state)
{
    if(fsm_ptr==0 || state>=MMIPDP_LINK_STATE_MAX)
    {
        return;
    }
    if(fsm_ptr->link_id>0 && fsm_ptr->link_id<=MMIPDP_MAX_LINK_NUM)
    {
        //SCI_TRACE_LOW:"PDP FsmStateTrans: cur:%d, trans to:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_1160_112_2_18_2_44_50_9,(uint8*)"dd",g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].state,state);
        g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].state = state;
    }
}

/****************************************************************************/
//Description : the deactived fsm
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL void FsmDeactived(MMIPDP_FSM_T *fsm_ptr, MMIPDP_FSM_MSG_T *msg_ptr)
{
    if(0==fsm_ptr || 0==msg_ptr)
    {
        return;
    }
    if(fsm_ptr->link_id==0||fsm_ptr->link_id>MMIPDP_MAX_LINK_NUM)
    {
        return;
    }
    fsm_ptr->is_busy = TRUE;
    //SCI_TRACE_LOW:"PDP MANAGER FsmDeactived,msg_id:%d,link_id:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_1180_112_2_18_2_44_50_10,(uint8*)"dd",msg_ptr->msg_id,fsm_ptr->link_id);
    switch(msg_ptr->msg_id)
    {
    case MMIPDP_ACTIVE_REQ:
        {
            MMIPDP_ACTIVE_INFO_T *active_info_ptr = 0;
            
            active_info_ptr = (MMIPDP_ACTIVE_INFO_T*)(msg_ptr->msg_body);
            SCI_TRACE_LOW("FsmDeactived handle_msg_callback = %x",active_info_ptr->handle_msg_callback);
            SetLinkAppInfo(active_info_ptr,fsm_ptr->link_id);
            if(MMIPDP_PsActivePdp(fsm_ptr->link_id))
            {
                FsmStateTrans(fsm_ptr,MMIPDP_LINK_STATE_ACTIVING);
                AddToList(&(g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].using_list),active_info_ptr,MMIPDP_ACTIVE_CNF);
            }
            else
            {
                SCI_TRACE_LOW("FsmDeactived Callback handle_msg_callback%x",active_info_ptr->handle_msg_callback);
                Callback(active_info_ptr,fsm_ptr->link_id,MMIPDP_ACTIVE_CNF,MMIPDP_RESULT_FAIL);
            }
        }
        break;
    default:
        //SCI_TRACE_LOW:"DEACTIVED, got wrong msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_1201_112_2_18_2_44_50_11,(uint8*)"");
        break;
    }
    fsm_ptr->is_busy = FALSE;
    //  SCI_FREE(msg_ptr);
}

/****************************************************************************/
//Description : the actived fsm
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL void FsmActived(MMIPDP_FSM_T *fsm_ptr, MMIPDP_FSM_MSG_T *msg_ptr)
{
    if(0==fsm_ptr || 0==msg_ptr)
    {
        return;
    }
    if(fsm_ptr->link_id==0||fsm_ptr->link_id>MMIPDP_MAX_LINK_NUM)
    {
        return;
    }
    fsm_ptr->is_busy = TRUE;
    //SCI_TRACE_LOW:"PDP MANAGER FsmActived,msg_id:%d,link_id:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_1223_112_2_18_2_44_50_12,(uint8*)"dd",msg_ptr->msg_id,fsm_ptr->link_id);
    switch(msg_ptr->msg_id)
    {
    case MMIPDP_ACTIVE_REQ:
        {
            MMIPDP_ACTIVE_INFO_T *active_info_ptr = 0;
            
            active_info_ptr = (MMIPDP_ACTIVE_INFO_T*)(msg_ptr->msg_body);
            //if app already exist in using list
            if(0!=MMIPDP_ListFindNode(
                g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].using_list,
                (void*)active_info_ptr->app_handler,
                FindNodeByApphandlerCb
                ))
            {
                Callback(active_info_ptr,fsm_ptr->link_id,MMIPDP_ACTIVE_CNF,MMIPDP_RESULT_SUCC);
            }
            else
            {
                if(CheckIfCanUseTheSameLink(active_info_ptr,&(g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1])))
                {                                
                    APP_MN_GPRS_EXT_T    *sig_ptr = PNULL;
                    uint16               sig_size = 0;
                    
                    if(PNULL != s_active_info_ptr)
                    {
                        SCI_FREE(s_active_info_ptr);
                        s_active_info_ptr = PNULL;
                    }
                    s_active_info_ptr = SCI_ALLOCA(sizeof(MMIPDP_ACTIVE_APN_SIM_SAME_INFO_T));
                    if(PNULL != s_active_info_ptr)
                    {
                        sig_size = sizeof(APP_MN_GPRS_EXT_T);
                        SCI_CREATE_SIGNAL(sig_ptr,APP_MN_ACTIVATE_PDP_CONTEXT_CNF,sig_size,P_APP);   /*lint !e64*/
                        sig_ptr->dual_sys = g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].dual_sys;
                        sig_ptr->result = MN_GPRS_ERR_SUCCESS;
                        sig_ptr->pdp_id = g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].pdp_id;
                        sig_ptr->nsapi = g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].nsapi;
                        SCI_MEMSET(s_active_info_ptr, 0x00, sizeof(MMIPDP_ACTIVE_APN_SIM_SAME_INFO_T));
                        SCI_MEMCPY(&(s_active_info_ptr->active_info), active_info_ptr, sizeof(MMIPDP_ACTIVE_INFO_T));
                        s_active_info_ptr->link_id = fsm_ptr->link_id;
                        s_active_info_ptr->nsapi = g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].nsapi;
                        s_active_info_ptr->pdp_id = g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].pdp_id;
                        SCI_SEND_SIGNAL(sig_ptr,P_APP);
                    }
                }
                else
                {
                    MMIPDP_PsDeactivePdp(fsm_ptr->link_id);
                    SuspendAllAppInList(g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].using_list);
                    AddToList(&g_mmipdp_manager_instance.pending_list,active_info_ptr,MMIPDP_ACTIVE_CNF);
                    FsmStateTrans(fsm_ptr,MMIPDP_LINK_STATE_DEACTIVING);
                }
            }
        }
        break;
    case MMIPDP_DEACTIVE_REQ:
        {
            MMIPDP_LIST_NODE_HANDLE to_deactive_node_handler = 0, highest_prio_node = 0;
            MMIPDP_LIST_APP_INFO_T *using_list_data_ptr=0, *pending_list_data_ptr=0, *to_deactive_data_ptr = 0;
            
            to_deactive_node_handler = MMIPDP_ListFindNode(
                g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].using_list,
                (void*)(msg_ptr->msg_body),
                FindNodeByApphandlerCb
                );
            to_deactive_data_ptr = MMIPDP_ListGetNodeData(to_deactive_node_handler);
            if(0 == to_deactive_data_ptr)
            {
                return;
            }
            to_deactive_data_ptr->msg_id_to_callback = MMIPDP_DEACTIVE_CNF;
            if(1 == MMIPDP_ListGetMemberCount(g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].using_list))
            {
                MMIPDP_PsDeactivePdp(fsm_ptr->link_id);
                FsmStateTrans(fsm_ptr,MMIPDP_LINK_STATE_DEACTIVING);
            }
            else
            {
                //no app is waiting in pending list
                if(0 == MMIPDP_ListGetMemberCount(g_mmipdp_manager_instance.pending_list))
                {   
                    ChangeFlowRate(fsm_ptr->link_id);
                    Callback(&(to_deactive_data_ptr->active_info),fsm_ptr->link_id,to_deactive_data_ptr->msg_id_to_callback,MMIPDP_RESULT_SUCC);
                    FreeMemAllocInListNode(to_deactive_node_handler);
                    MMIPDP_ListDeleteNode(g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].using_list,to_deactive_node_handler);
                }
                else
                {
                    //find the highest priority app except the one be requested to deactive in using list.
                    if(to_deactive_node_handler==MMIPDP_ListGetHead(g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].using_list))
                    {
                        highest_prio_node = MMIPDP_ListGetNextNode(to_deactive_node_handler);
                    }
                    else
                    {
                        highest_prio_node = MMIPDP_ListGetHead(g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].using_list);
                    }
                    using_list_data_ptr = MMIPDP_ListGetNodeData(highest_prio_node);
                    
                    pending_list_data_ptr = MMIPDP_ListGetNodeData(MMIPDP_ListGetHead(g_mmipdp_manager_instance.pending_list));
                    if(using_list_data_ptr!=0 && pending_list_data_ptr!=0)
                    {
                        //if the priority of app in using list is higher than that in pending list
                        if(using_list_data_ptr->active_info.priority < pending_list_data_ptr->active_info.priority)
                        {
                            ChangeFlowRate(fsm_ptr->link_id);
                            Callback(&(to_deactive_data_ptr->active_info),fsm_ptr->link_id,to_deactive_data_ptr->msg_id_to_callback,MMIPDP_RESULT_SUCC);
                            FreeMemAllocInListNode(to_deactive_node_handler);
                            MMIPDP_ListDeleteNode(g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].using_list,to_deactive_node_handler);
                        }
                        //if the priority of app in pending list is higher than that in using list, to suspend the using list
                        else
                        {
                            MMIPDP_PsDeactivePdp(fsm_ptr->link_id);
                            SuspendAllAppInList(g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].using_list);
                            FsmStateTrans(fsm_ptr,MMIPDP_LINK_STATE_DEACTIVING);
                        }
                    }
                }
            }
        }
        break;
    case MMIPDP_PS_DEACTIVE_IND:
        {
            MMIPDP_LIST_NODE_HANDLE node_handler = 0;
            MMIPDP_LIST_APP_INFO_T  *node_data_ptr = 0;
            
            node_handler = MMIPDP_ListGetHead(g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].using_list);
            while(node_handler!=0)
            {
                node_data_ptr = MMIPDP_ListGetNodeData(node_handler);
                if(node_data_ptr!=0)
                {
                    //callback
                    Callback(&(node_data_ptr->active_info),fsm_ptr->link_id,MMIPDP_DEACTIVE_IND,MMIPDP_RESULT_SUCC);
                    FreeMemAllocInListNode(node_handler);
                    MMIPDP_ListDeleteNode(g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].using_list,node_handler);
                }
                node_handler = MMIPDP_ListGetHead(g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].using_list);
            }
            //if no app was waiting for activing pdp
            if(0 == MMIPDP_ListGetMemberCount(g_mmipdp_manager_instance.pending_list))
            {
                FreeMemAllocInLinkInfo(fsm_ptr->link_id);
                FsmStateTrans(fsm_ptr,MMIPDP_LINK_STATE_DEACTIVED);
#ifndef MMI_MULTI_SIM_SYS_SINGLE
                MMIAPISET_BgSetIgStatusParam(MMIAPISET_GetAnotherSim(g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].dual_sys));
#endif
            }
            else
            {
                MMIPDP_LIST_APP_INFO_T *temp_node_data_ptr = 0;
                MMIPDP_LIST_NODE_HANDLE temp_node_handler = 0;
                
                if(ActiveFromPendingList(fsm_ptr->link_id))
                {
                    //find the same apn app left in pending list
                    temp_node_handler = MMIPDP_ListGetHead(g_mmipdp_manager_instance.pending_list);
                    while(temp_node_handler!=0)
                    {
                        temp_node_data_ptr = MMIPDP_ListGetNodeData(temp_node_handler);
                        if(temp_node_data_ptr!=0)
                        {
                            if(CheckIfCanUseTheSameLink(&temp_node_data_ptr->active_info,&g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1]))
                            {
                                
                                AddToList(&(g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].using_list),&temp_node_data_ptr->active_info,temp_node_data_ptr->msg_id_to_callback);
                                FreeMemAllocInListNode(temp_node_handler);
                                MMIPDP_ListDeleteNode(g_mmipdp_manager_instance.pending_list,temp_node_handler);
                            }
                        }
                        temp_node_handler = MMIPDP_ListGetNextNode(node_handler);
                    }
                    FsmStateTrans(fsm_ptr,MMIPDP_LINK_STATE_ACTIVING);
                }
                else
                {
                    FreeMemAllocInLinkInfo(fsm_ptr->link_id);
                    FsmStateTrans(fsm_ptr,MMIPDP_LINK_STATE_DEACTIVED);
#ifndef MMI_MULTI_SIM_SYS_SINGLE
                    MMIAPISET_BgSetIgStatusParam(MMIAPISET_GetAnotherSim(g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].dual_sys));
#endif
                }
            }
        }
        break;
    default:
        //SCI_TRACE_LOW:"ACTIVED, got wrong msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_1411_112_2_18_2_44_51_13,(uint8*)"");
        break;
    }
    //  SCI_FREE(msg_ptr);
    fsm_ptr->is_busy = FALSE;
}
/****************************************************************************/
//Description : the deactiving fsm
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL void FsmDeactiving(MMIPDP_FSM_T *fsm_ptr, MMIPDP_FSM_MSG_T *msg_ptr)
{
    MMIPDP_LIST_APP_INFO_T *node_data_ptr = 0;
    MMIPDP_LIST_NODE_HANDLE node_handler = 0;
    
    if(0==fsm_ptr || 0==msg_ptr)
    {
        return;
    }
    if(fsm_ptr->link_id==0||fsm_ptr->link_id>MMIPDP_MAX_LINK_NUM)
    {
        return;
    }
    //SCI_TRACE_LOW:"PDP MANAGER FsmDeactiving,msg_id:%d,link_id:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_1434_112_2_18_2_44_51_14,(uint8*)"dd",msg_ptr->msg_id,fsm_ptr->link_id);
    fsm_ptr->is_busy = TRUE;
    switch(msg_ptr->msg_id)
    {
    case MMIPDP_ACTIVE_REQ:
        if(!g_mmipdp_manager_instance.is_release_all)
        {
            AddToList(&g_mmipdp_manager_instance.pending_list,(MMIPDP_ACTIVE_INFO_T*)(msg_ptr->msg_body),MMIPDP_ACTIVE_CNF);
        }
        break;
    case MMIPDP_DEACTIVE_REQ:
        //do nothing
        break;
    case MMIPDP_ACTIVE_TIMOUT_IND:
        {
            node_handler = (uint32)msg_ptr->msg_body;
            node_data_ptr = MMIPDP_ListGetNodeData(node_handler);
            if(node_data_ptr!=0)
            {
                Callback(&node_data_ptr->active_info,fsm_ptr->link_id,MMIPDP_ACTIVE_CNF,MMIPDP_RESULT_TIMEOUT);
                FreeMemAllocInListNode(node_handler);
                MMIPDP_ListDeleteNode(g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].using_list,node_handler);
            }
        }
        break;
    case MMIPDP_PS_DEACTIVE_CNF:
        {
            node_handler = MMIPDP_ListGetHead(g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].using_list);
            while(node_handler!=0)
            {
                node_data_ptr = MMIPDP_ListGetNodeData(node_handler);
                if(node_data_ptr!=0)
                {
                    if(g_mmipdp_manager_instance.is_release_all)
                    {
                        Callback(&(node_data_ptr->active_info),fsm_ptr->link_id,MMIPDP_DEACTIVE_IND,MMIPDP_RESULT_SUCC);
                    }
                    else
                    {
                        //callback
                        Callback(&(node_data_ptr->active_info),fsm_ptr->link_id,node_data_ptr->msg_id_to_callback,MMIPDP_RESULT_SUCC);
                        //the app been suspended must add to pending list for resuming
                        if(node_data_ptr->msg_id_to_callback == MMIPDP_SUSPEND_IND)
                        {
                            AddToList(&g_mmipdp_manager_instance.pending_list,&node_data_ptr->active_info,MMIPDP_RESUME_IND);
                        }
                    }
                    FreeMemAllocInListNode(node_handler);
                    MMIPDP_ListDeleteNode(g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].using_list,node_handler);
                }
                node_handler = MMIPDP_ListGetHead(g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].using_list);
            }
            //if no app was waiting for activing pdp
            if(0 == MMIPDP_ListGetMemberCount(g_mmipdp_manager_instance.pending_list))
            {
                FreeMemAllocInLinkInfo(fsm_ptr->link_id);
                FsmStateTrans(fsm_ptr,MMIPDP_LINK_STATE_DEACTIVED);
#ifndef MMI_MULTI_SIM_SYS_SINGLE
                MMIAPISET_BgSetIgStatusParam(MMIAPISET_GetAnotherSim(g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].dual_sys));
#endif
            }
            else
            {
                MMIPDP_LIST_APP_INFO_T *temp_node_data_ptr = 0;
                MMIPDP_LIST_NODE_HANDLE temp_node_handler = 0;
                
                if(ActiveFromPendingList(fsm_ptr->link_id))
                {
                    //find the same apn app left in pending list
                    temp_node_handler = MMIPDP_ListGetHead(g_mmipdp_manager_instance.pending_list);
                    while(temp_node_handler!=0)
                    {
                        temp_node_data_ptr = MMIPDP_ListGetNodeData(temp_node_handler);
                        if(temp_node_data_ptr!=0)
                        {
                            if(CheckIfCanUseTheSameLink(&temp_node_data_ptr->active_info,&g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1]))
                            {
                                
                                AddToList(&(g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].using_list),&temp_node_data_ptr->active_info,temp_node_data_ptr->msg_id_to_callback);
                                FreeMemAllocInListNode(temp_node_handler);
                                MMIPDP_ListDeleteNode(g_mmipdp_manager_instance.pending_list,temp_node_handler);
                            }
                        }
                        temp_node_handler = MMIPDP_ListGetNextNode(node_handler);
                    }
                    FsmStateTrans(fsm_ptr,MMIPDP_LINK_STATE_ACTIVING);
                }
                else
                {
                    FreeMemAllocInLinkInfo(fsm_ptr->link_id);
                    FsmStateTrans(fsm_ptr,MMIPDP_LINK_STATE_DEACTIVED);
#ifndef MMI_MULTI_SIM_SYS_SINGLE
                    MMIAPISET_BgSetIgStatusParam(MMIAPISET_GetAnotherSim(g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].dual_sys));
#endif
                }
            }
        }
        break;
    default:
        //SCI_TRACE_LOW:"DEACTIVING, got wrong msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_1533_112_2_18_2_44_51_15,(uint8*)"");
        break;
    }
    //  SCI_FREE(msg_ptr);
    fsm_ptr->is_busy = FALSE;
}

/****************************************************************************/
//Description : Try reactiving pdp
//Global resource dependence :
//Author: shuting.ma
//Note:
/****************************************************************************/
LOCAL BOOLEAN TryReactivePdp(MMIPDP_LINK_INFO_T link_info, uint32 link_id)
{
    BOOLEAN ret = FALSE;

    if(link_id == 0 || link_id > MMIPDP_MAX_LINK_NUM)
    {
        SCI_TRACE_LOW("[TryReactivePdp] link id error");
        return FALSE;
    }

    // if auth type is PAP_OR_CHAP, request PAP and CHAP alternately
    SCI_TRACE_LOW("[TryReactivePdp] auth_type = %d", link_info.auth_type);
    if(MN_PAP_CHAP == link_info.auth_type)
    {
        SCI_TRACE_LOW("[TryReactivePdp] reqChapForPapOrChap = %d", link_info.reqChapForPapOrChap);
        if(FALSE == link_info.reqChapForPapOrChap)
        {
            SetLinkReqChapForPapOrChap(link_id, TRUE);
            MMIPDP_PsActivePdp(link_id);
            ret = TRUE;
        }
        else
        {
            SCI_TRACE_LOW("[TryReactivePdp] reactive_count = %d", g_mmipdp_manager_instance.link_arr[link_id-1].reactive_count);
            if(MMIPDP_REACTIVE_MAX_COUNT > g_mmipdp_manager_instance.link_arr[link_id-1].reactive_count)
            {
                SetLinkReqChapForPapOrChap(link_id, FALSE);
                if (TRUE == ReActive(link_id))
                {
                    ret = TRUE;
                }
            }
            else
            {
                SetLinkReqChapForPapOrChap(link_id, FALSE);
                g_mmipdp_manager_instance.link_arr[link_id-1].reactive_count = 0;
                ret = FALSE;
            }
        }
    }
    else if(TRUE == ReActive(link_id))
    {
        ret = TRUE;
    }
    else
    {
        SCI_TRACE_LOW("[TryReactivePdp] do nothing");
        ret = FALSE;
    }
    return ret;
}

/****************************************************************************/
//Description : the activing fsm
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL void FsmActiving(MMIPDP_FSM_T *fsm_ptr, MMIPDP_FSM_MSG_T *msg_ptr)
{
    MMIPDP_LIST_NODE_HANDLE node_handler = 0;
    MMIPDP_LIST_APP_INFO_T *node_data_ptr = 0;
    
    
    if(0==fsm_ptr || 0==msg_ptr)
    {
        return;
    }
    if(fsm_ptr->link_id==0||fsm_ptr->link_id>MMIPDP_MAX_LINK_NUM)
    {
        return;
    }
    //SCI_TRACE_LOW:"PDP MANAGER FsmActiving,msg_id:%d,link_id:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_1558_112_2_18_2_44_51_16,(uint8*)"dd",msg_ptr->msg_id,fsm_ptr->link_id);
    fsm_ptr->is_busy = TRUE;
    switch(msg_ptr->msg_id)
    {
    case MMIPDP_ACTIVE_REQ:
        {
            MMIPDP_ACTIVE_INFO_T *active_info_ptr = 0;
            
            active_info_ptr = (MMIPDP_ACTIVE_INFO_T*)(msg_ptr->msg_body);
            //if app already exist in using list
            if(0!=MMIPDP_ListFindNode(
                g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].using_list,
                (void*)active_info_ptr->app_handler,
                FindNodeByApphandlerCb
                ))
            {
                ;//do nothing
            }
            else
            {
                if(CheckIfCanUseTheSameLink(active_info_ptr,&(g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1])))
                {
                    AddToList(&(g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].using_list),active_info_ptr,MMIPDP_ACTIVE_CNF);
                }
                else
                {
                    MMIPDP_PsDeactivePdp(fsm_ptr->link_id);
                    SuspendAllAppInList(g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].using_list);
                    AddToList(&g_mmipdp_manager_instance.pending_list,active_info_ptr,MMIPDP_ACTIVE_CNF);
                    FsmStateTrans(fsm_ptr,MMIPDP_LINK_STATE_DEACTIVING);
                }
            }
        }
        break;
    case MMIPDP_DEACTIVE_REQ:
        {
            MMIPDP_LIST_NODE_HANDLE to_deactive_node_handler = 0, highest_prio_node = 0;
            MMIPDP_LIST_APP_INFO_T *using_list_data_ptr=0, *pending_list_data_ptr=0, *to_deactive_data_ptr = 0;
            
            to_deactive_node_handler = MMIPDP_ListFindNode(
                g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].using_list,
                (void*)(msg_ptr->msg_body),
                FindNodeByApphandlerCb
                );
            to_deactive_data_ptr = MMIPDP_ListGetNodeData(to_deactive_node_handler);
            if(0 == to_deactive_data_ptr)
            {
                return;
            }
            to_deactive_data_ptr->msg_id_to_callback = MMIPDP_DEACTIVE_CNF;
            if(1 == MMIPDP_ListGetMemberCount(g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].using_list))
            {
                MMIPDP_PsDeactivePdp(fsm_ptr->link_id);
                FsmStateTrans(fsm_ptr,MMIPDP_LINK_STATE_DEACTIVING);
            }
            else
            {
                //no app is waiting in pending list
                if(0 == MMIPDP_ListGetMemberCount(g_mmipdp_manager_instance.pending_list))
                {
                    to_deactive_data_ptr->msg_id_to_callback = MMIPDP_DEACTIVE_CNF;
                    Callback(&(to_deactive_data_ptr->active_info),fsm_ptr->link_id,to_deactive_data_ptr->msg_id_to_callback,MMIPDP_RESULT_SUCC);
                    FreeMemAllocInListNode(to_deactive_node_handler);
                    MMIPDP_ListDeleteNode(
                        g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].using_list,
                        to_deactive_node_handler);
                }
                //find the highest priority app except the one be requested to deactive in using list.
                if(to_deactive_node_handler==MMIPDP_ListGetHead(g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].using_list))
                {
                    highest_prio_node = MMIPDP_ListGetNextNode(to_deactive_node_handler);
                }
                else
                {
                    highest_prio_node = MMIPDP_ListGetHead(g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].using_list);
                }
                using_list_data_ptr = MMIPDP_ListGetNodeData(highest_prio_node);
                
                pending_list_data_ptr = MMIPDP_ListGetNodeData(MMIPDP_ListGetHead(g_mmipdp_manager_instance.pending_list));
                if(using_list_data_ptr!=0 && pending_list_data_ptr!=0)
                {
                    //if the priority of app in using list is higher than that in penging list
                    if(using_list_data_ptr->active_info.priority < pending_list_data_ptr->active_info.priority)
                    {
                        to_deactive_data_ptr->msg_id_to_callback = MMIPDP_DEACTIVE_CNF;
                        Callback(&(to_deactive_data_ptr->active_info),fsm_ptr->link_id,to_deactive_data_ptr->msg_id_to_callback,MMIPDP_RESULT_SUCC);
                        FreeMemAllocInListNode(to_deactive_node_handler);
                        MMIPDP_ListDeleteNode(
                            g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].using_list,
                            to_deactive_node_handler);
                    }
                    //if the priority of app in pending list is higher than that in using list, to suspend the using list
                    else
                    {
                        MMIPDP_PsDeactivePdp(fsm_ptr->link_id);
                        SuspendAllAppInList(g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].using_list);
                        to_deactive_data_ptr->msg_id_to_callback = MMIPDP_DEACTIVE_CNF;
                        FsmStateTrans(fsm_ptr,MMIPDP_LINK_STATE_DEACTIVING);
                    }
                }
            }
        }
        break;
    case MMIPDP_ACTIVE_TIMOUT_IND:
        {
            node_handler = (uint32)msg_ptr->msg_body;
            node_data_ptr = MMIPDP_ListGetNodeData(node_handler);
            if(node_data_ptr!=0)
            {
                Callback(&node_data_ptr->active_info,fsm_ptr->link_id,node_data_ptr->msg_id_to_callback,MMIPDP_RESULT_TIMEOUT);
                FreeMemAllocInListNode(node_handler);
                MMIPDP_ListDeleteNode(g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].using_list,node_handler);
                if(0 == MMIPDP_ListGetMemberCount(g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].using_list))
                {
                    g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].reactive_count = 0;
                    MMIPDP_StopTimer(&g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].reactive_timer_id);
                    MMIPDP_PsDeactivePdp(fsm_ptr->link_id);
                    FsmStateTrans(fsm_ptr,MMIPDP_LINK_STATE_DEACTIVING);
                }
            }
        }
        break;
    case MMIPDP_PS_ACTIVE_CNF:
        {
             MMIPDP_LINK_INFO_T link_info = {0};

            MMIPDP_GetLinkInfo(fsm_ptr->link_id, &link_info);            
            //((MMI_GPRS_EXT_T*)msg_ptr->msg_body)->result = MN_GPRS_ERR_RETRYING; // test always fail case for simulator

            if(((APP_MN_GPRS_EXT_T*)msg_ptr->msg_body)->result!=MN_GPRS_ERR_SUCCESS
                && ((APP_MN_GPRS_EXT_T*)msg_ptr->msg_body)->result!=MN_GPRS_ERR_PDP_CONTEXT_ACTIVATED
                && ((APP_MN_GPRS_EXT_T*)msg_ptr->msg_body)->result!=MN_GPRS_ERR_SUCCESS_IPV4_ONLY
                && ((APP_MN_GPRS_EXT_T*)msg_ptr->msg_body)->result!=MN_GPRS_ERR_SUCCESS_IPV6_ONLY
                && ((APP_MN_GPRS_EXT_T*)msg_ptr->msg_body)->result!=MN_GPRS_ERR_SUCCESS_SINGLE_ONLY
                )
            {   
                //to retry
                if(MN_GPRS_ERR_TEMPORARILY_BLOCKED == ((APP_MN_GPRS_EXT_T*)msg_ptr->msg_body)->result||
                    MN_GPRS_ERR_RETRYING == ((APP_MN_GPRS_EXT_T*)msg_ptr->msg_body)->result)
                {
                    if(TRUE == TryReactivePdp(link_info, fsm_ptr->link_id))
                    {
                        break;
                    }
                }
                FsmStateTrans(fsm_ptr,MMIPDP_LINK_STATE_DEACTIVED);
#ifndef MMI_MULTI_SIM_SYS_SINGLE
                MMIAPISET_BgSetIgStatusParam(MMIAPISET_GetAnotherSim(g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].dual_sys));
#endif
                node_handler = MMIPDP_ListGetHead(g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].using_list);
                while(node_handler!=0)
                {
                    node_data_ptr = MMIPDP_ListGetNodeData(node_handler);
                    if(node_data_ptr!=0)
                    {
                        //SCI_TRACE_LOW:"PDP MANAGER Active fail, ps err=%d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_1703_112_2_18_2_44_51_17,(uint8*)"d",((APP_MN_GPRS_EXT_T*)msg_ptr->msg_body)->result);
                        Callback(&(node_data_ptr->active_info),fsm_ptr->link_id,node_data_ptr->msg_id_to_callback,MMIPDP_RESULT_FAIL);
                    }
                    FreeMemAllocInListNode(node_handler);
                    MMIPDP_ListDeleteNode(g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].using_list,node_handler);
                    node_handler = MMIPDP_ListGetHead(g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].using_list);
                }

#if 0
                if (!MMIPDP_PsIsPdpLinkExist(g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].dual_sys))
                {
                    MMIPDP_SetGPRSDetach(g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].dual_sys);
                }
#endif
                FreeMemAllocInLinkInfo(fsm_ptr->link_id);
            }
            else
            {
                FsmStateTrans(fsm_ptr,MMIPDP_LINK_STATE_ACTIVED);
                g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].reactive_count= 0;
                if (TRUE == link_info.reqChapForPapOrChap) // if Chap succeed for PAP+CHAP type, reset the flag
                {
                    SetLinkReqChapForPapOrChap(fsm_ptr->link_id, FALSE);
                }
                node_handler = MMIPDP_ListGetHead(g_mmipdp_manager_instance.link_arr[fsm_ptr->link_id-1].using_list);
                while(node_handler!=0)
                {
                    node_data_ptr = MMIPDP_ListGetNodeData(node_handler);
                    if(node_data_ptr!=0)
                    {
                        Callback(&(node_data_ptr->active_info),fsm_ptr->link_id,node_data_ptr->msg_id_to_callback,MMIPDP_RESULT_SUCC);
                    }
                    node_handler = MMIPDP_ListGetNextNode(node_handler);
                }
            }
        }
        break;
    default:
        //SCI_TRACE_LOW:"ACTIVING, got wrong msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_1736_112_2_18_2_44_51_18,(uint8*)"");
        break;
    }
    //  SCI_FREE(msg_ptr);
    fsm_ptr->is_busy = FALSE;
}

/****************************************************************************/
//Description : FindLinkIdByPdpId
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL uint32 FindLinkIdByPdpId(uint32 pdp_id, MN_DUAL_SYS_E dual_sys)
{
    uint32 link_id = 1;
    
    while(link_id<=MMIPDP_MAX_LINK_NUM)
    {
        if(g_mmipdp_manager_instance.link_arr[link_id-1].pdp_id==pdp_id &&
            g_mmipdp_manager_instance.link_arr[link_id-1].dual_sys==dual_sys)
        {
            break;
        }
        else
        {
            link_id++;
        }
    }
    if(link_id>MMIPDP_MAX_LINK_NUM)
    {
        link_id=0;
    }
    return link_id;
}
//add clear flowrate
LOCAL void clearFowrate(MMIPDP_FLOWRATE_T* flowrate_to_write_arr, MN_DUAL_SYS_E dual_sys, SCI_DATE_T* cur_date)
{
    //跨年或跨月将清除旧数据
    if ( !(flowrate_to_write_arr[dual_sys].nYear == cur_date->year
        && flowrate_to_write_arr[dual_sys].nMonth == cur_date->mon ) )
    {
        SCI_MEMSET(flowrate_to_write_arr[dual_sys].month_recv_data_array, 0x00, sizeof(uint32)*31);
        SCI_MEMSET(flowrate_to_write_arr[dual_sys].month_send_data_array, 0x00, sizeof(uint32)*31);
        flowrate_to_write_arr[dual_sys].nYear = cur_date->year;
        flowrate_to_write_arr[dual_sys].nMonth = cur_date->mon;
    }
}
//End
/****************************************************************************/
//Description : change the flow rate record in NV
//Global resource dependence : 
//Author: juan.zhang
//Note: 注意，在多pdp的情况下，获取的上次连接的流量是错误的，原因是下层的流量
//      计算放在socket那边计算的，无法区分各个链路的流量。需要下层修改计算方法
/****************************************************************************/
LOCAL void ChangeFlowRate(uint32 link_id)
{
#ifdef MMI_GPRS_SUPPORT
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_MAX;
    uint32  total_send = 0;
    uint32  total_recv = 0;
    MMIPDP_FLOWRATE_T   flowrate_to_write_arr[MMI_DUAL_SYS_MAX] = {0};
    MN_RETURN_RESULT_E  read_result = MN_RETURN_SUCCESS; 
    SCI_DATE_T cur_date = {0};
    BOOLEAN bSent = FALSE;
    BOOLEAN bReceiv = FALSE;
    
    if(link_id>MMIPDP_MAX_LINK_NUM)
    {
        return;
    }
    dual_sys = g_mmipdp_manager_instance.link_arr[link_id-1].dual_sys;
    if(dual_sys>=MMI_DUAL_SYS_MAX)
    {
        return;
    }
    
    MMINV_READ(MMINV_PDP_GPRS_FLOW_RATE,flowrate_to_write_arr,read_result);
    //get current day
    TM_GetSysDate(&cur_date);

    clearFowrate(flowrate_to_write_arr, dual_sys, &cur_date);
 
    MMIPDP_PsGetFlowRate(&total_send,&total_recv);
    if(g_cur_recv_flowrate<total_recv)
    {
        //add storage date data
        flowrate_to_write_arr[dual_sys].month_recv_data_array[cur_date.mday - 1] += total_recv-g_cur_recv_flowrate;
        //End
        flowrate_to_write_arr[dual_sys].total_recv += total_recv-g_cur_recv_flowrate;
        flowrate_to_write_arr[dual_sys].last_recv = total_recv-g_cur_recv_flowrate;
        g_cur_recv_flowrate = total_recv;
        bReceiv = TRUE;
    }
    if(g_cur_send_flowrate<total_send)
    {
        //add storage date data
        flowrate_to_write_arr[dual_sys].month_send_data_array[cur_date.mday - 1] += total_send-g_cur_send_flowrate;
        //End
        flowrate_to_write_arr[dual_sys].total_send += total_send-g_cur_send_flowrate;
        flowrate_to_write_arr[dual_sys].last_send = total_send-g_cur_send_flowrate;
        g_cur_send_flowrate = total_send;
        bSent = TRUE;
    }
    MMINV_WRITE(MMINV_PDP_GPRS_FLOW_RATE,flowrate_to_write_arr);

    //add dataCounter threshold tips
#ifdef DATACOUNTER_SUPPORT
   if ( bSent )
   {
        MMIAPIDataCounter_dataAlertNotifaction(dual_sys, DATACOUNTER_SENT_DATATYPE);
    }

   if ( bReceiv )
   {
        MMIAPIDataCounter_dataAlertNotifaction(dual_sys, DATACOUNTER_RECV_DATATYPE);
   }
#endif
    //End
#endif
}
/****************************************************************************/
//Description : to handle ps message
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E HandlePdpPsMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    uint32              link_id = 0;
    MMIPDP_FSM_MSG_T    *fsm_msg_ptr = 0;
    BOOLEAN             is_all_released = TRUE;
    
    if(0==app_ptr||0==param)
    {
        return MMI_RESULT_FALSE;
    }
   // SCI_TRACE_ID(TRACE_TOOL_CONVERT,"HandlePdpPsMsg msg_id %d",msg_id);
    switch(msg_id)
    {
    case APP_MN_ACTIVATE_PDP_CONTEXT_CNF:
	if(
#ifdef MMI_COMPILE_E
		(((APP_MN_GPRS_EXT_T*)param)->result == MN_GPRS_ERR_UNKNOWN_APN) || 
#endif
		(((APP_MN_GPRS_EXT_T*)param)->result == MN_GPRS_ERR_MISSING_OR_UNKOWN_APN))
	{
		//	SCI_TRACE_ID(TRACE_TOOL_CONVERT,"HandlePdpPsMsg  result %x",((APP_MN_GPRS_EXT_T*)param)->result);
            MMIPUB_OpenAlertWinByTextId(PNULL, STR_APN_WRONG,
                                            TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
	 }
#ifdef MMI_WIFI_SUPPORT        
        if((0 == ((APP_MN_GPRS_EXT_T*)param)->pdp_id) && (MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus()))
        {
            MMIAPIPDP_WifiActiveCnf(MMIPDP_RESULT_SUCC);    
        }
        else
#endif        
        {
            if((PNULL != s_active_info_ptr) && (s_active_info_ptr->nsapi == ((APP_MN_GPRS_EXT_T*)param)->nsapi)
                &&(s_active_info_ptr->pdp_id == ((APP_MN_GPRS_EXT_T*)param)->pdp_id))
            {
                MMIPDP_ACTIVE_INFO_T *active_info_ptr = &(s_active_info_ptr->active_info);
                //SCI_TRACE_LOW:"PDP MANAGER HandlePdpPsMsg,APP_MN_ACTIVATE_PDP_CONTEXT_CNF apn sim same!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_1840_112_2_18_2_44_52_19,(uint8*)"");
                link_id = s_active_info_ptr->link_id;
                AddToList(&(g_mmipdp_manager_instance.link_arr[link_id-1].using_list),active_info_ptr,MMIPDP_ACTIVE_CNF);
                Callback(active_info_ptr,link_id,MMIPDP_ACTIVE_CNF,MMIPDP_RESULT_SUCC);
                SCI_FREE(s_active_info_ptr);
                s_active_info_ptr = PNULL;
            }
            else
            {
                //SCI_TRACE_LOW:"PDP MANAGER HandlePdpPsMsg,APP_MN_ACTIVATE_PDP_CONTEXT_CNF,result:%d,nsapi:%d,pdp_id:%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_1850_112_2_18_2_44_52_20,(uint8*)"ddd",((APP_MN_GPRS_EXT_T*)param)->result,((APP_MN_GPRS_EXT_T*)param)->nsapi,((APP_MN_GPRS_EXT_T*)param)->pdp_id);
                link_id = FindLinkIdByPdpId((uint32)((APP_MN_GPRS_EXT_T*)param)->pdp_id,((APP_MN_GPRS_EXT_T*)param)->dual_sys);
                //SCI_TRACE_LOW:"PDP MANAGER HandlePdpPsMsg,APP_MN_ACTIVATE_PDP_CONTEXT_CNF,link_id:%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_1852_112_2_18_2_44_52_21,(uint8*)"d",link_id);
                if(0<link_id && MMIPDP_MAX_LINK_NUM>=link_id)
                {
                    g_mmipdp_manager_instance.link_arr[link_id-1].nsapi = ((APP_MN_GPRS_EXT_T*)param)->nsapi;
#ifdef WIN32
                        /* under WIN simulator NSAPI is meanless, so set as default value 0 */
                        g_mmipdp_manager_instance.link_arr[link_id-1].nsapi = 0;
#endif /* WIN32 */
                    MMIPDP_StopTimer(&g_mmipdp_manager_instance.link_arr[link_id-1].reactive_timer_id);
                    fsm_msg_ptr = CreatMsg(MMIPDP_PS_ACTIVE_CNF,(void *)param);
                    FsmDispatch(link_id,fsm_msg_ptr);
                }
                else
                {
                    //SCI_TRACE_LOW:"APP_MN_ACTIVATE_PDP_CONTEXT_CNF err link_id=0"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_1866_112_2_18_2_44_52_22,(uint8*)"");
                }
                if(((APP_MN_GPRS_EXT_T*)param)->result == MN_GPRS_ERR_SUCCESS)
                {
                    NotifySubscriber(((APP_MN_GPRS_EXT_T*)param)->dual_sys, MMIPDP_NOTIFY_EVENT_ACTIVED);
                }

            }
        }
        break;
    case APP_MN_DEACTIVATE_PDP_CONTEXT_CNF:
        {
            uint8   pdp_num = 0;
            uint8   pdp_id = 0, i = 0;
            MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
            
            pdp_num = ((APP_MN_GPRS_DEACTIVATE_CNF_T*)param)->pdp_num;
            dual_sys = ((APP_MN_GPRS_DEACTIVATE_CNF_T*)param)->dual_sys;
            //SCI_TRACE_LOW:"PDP MANAGER HandlePdpPsMsg,APP_MN_DEACTIVATE_PDP_CONTEXT_CNF,pdp_num:%d,dual_sys:%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_1883_112_2_18_2_44_52_23,(uint8*)"dd",pdp_num, dual_sys);

#ifdef MMI_WIFI_SUPPORT
            if((0 == pdp_num) && (MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus()))
            {
                MMIPDP_WifiDeactiveCnf(MMIPDP_RESULT_SUCC);   
                //SCI_TRACE_LOW:"APP_MN_DEACTIVATE_PDP_CONTEXT_CNF deactive wifi"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_1889_112_2_18_2_44_52_24,(uint8*)"");
                return MMI_RESULT_TRUE;
            }
#endif      

            if (0 == pdp_num)
            {
                for(link_id=1; link_id<=MMIPDP_MAX_LINK_NUM; link_id++)
                {
                    if (g_mmipdp_manager_instance.link_arr[link_id-1].dual_sys == dual_sys
                        && g_mmipdp_manager_instance.link_arr[link_id-1].state != MMIPDP_LINK_STATE_DEACTIVED)
                    {
                        ChangeFlowRate(link_id);
                        MMIPDP_StopTimer(&g_mmipdp_manager_instance.link_arr[link_id-1].reactive_timer_id);
                        MMIPDP_StopTimer(&g_mmipdp_manager_instance.link_arr[link_id-1].deactive_protect_timer);
                        fsm_msg_ptr = CreatMsg(MMIPDP_PS_DEACTIVE_CNF,0);
                        FsmDispatch(link_id,fsm_msg_ptr);
                    }
                }
            }
            else
            {
                for(i=0;i<pdp_num;i++)
                {
                    pdp_id = ((APP_MN_GPRS_DEACTIVATE_CNF_T*)param)->pdp_id[i];
                    //SCI_TRACE_LOW:"PDP MANAGER HandlePdpPsMsg,APP_MN_DEACTIVATE_PDP_CONTEXT_CNF,pdp_id:%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_1914_112_2_18_2_44_52_25,(uint8*)"d",pdp_id);
                    link_id = FindLinkIdByPdpId((uint32)pdp_id,dual_sys);
                    //SCI_TRACE_LOW:"PDP MANAGER HandlePdpPsMsg,APP_MN_DEACTIVATE_PDP_CONTEXT_CNF,link_id:%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_1916_112_2_18_2_44_52_26,(uint8*)"d",link_id);
                    if(0<link_id && MMIPDP_MAX_LINK_NUM>=link_id)
                    {
                        ChangeFlowRate(link_id);
                        MMIPDP_StopTimer(&g_mmipdp_manager_instance.link_arr[link_id-1].reactive_timer_id);
                        MMIPDP_StopTimer(&g_mmipdp_manager_instance.link_arr[link_id-1].deactive_protect_timer);
                        fsm_msg_ptr = CreatMsg(MMIPDP_PS_DEACTIVE_CNF,0);
                        FsmDispatch(link_id,fsm_msg_ptr);
                    }
                    else
                    {
                        //SCI_TRACE_LOW:"APP_MN_DEACTIVATE_PDP_CONTEXT_CNF err link_id=0"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_1927_112_2_18_2_44_52_27,(uint8*)"");
                    }
                }
            }

            for(link_id=1; link_id<=MMIPDP_MAX_LINK_NUM; link_id++)
            {

			if(g_mmipdp_manager_instance.link_arr[link_id-1].state != MMIPDP_LINK_STATE_DEACTIVED)
			{
				is_all_released = FALSE;
				break;
			}
            }
            if(is_all_released)
            {
                if(!MMIPDP_PsIsPdpLinkExist(dual_sys))
                {
                    NotifySubscriber(dual_sys, MMIPDP_NOTIFY_EVENT_DEACTIVED);
#if 0
                    for(i=0;i<pdp_num;i++)
                    {
                        pdp_id = ((APP_MN_GPRS_DEACTIVATE_CNF_T*)param)->pdp_id[i];
                        if(MMIPDP_IsPdpIdInLinkArray((uint32)pdp_id,dual_sys))
                        {
                            //SCI_TRACE_LOW:"[YKTEST] pdp detatch!!!"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_1950_112_2_18_2_44_52_28,(uint8*)"");
                            MMIPDP_SetGPRSDetach(dual_sys);
                            break;
                        }
                    }
#endif
                }
                
                if(g_mmipdp_manager_instance.is_release_all)
                {
                    g_mmipdp_manager_instance.is_release_all = FALSE;
                    if(g_mmipdp_manager_instance.release_all_cb)
                    {
                        g_mmipdp_manager_instance.release_all_cb();
                        g_mmipdp_manager_instance.release_all_cb = 0;
                    }
                }
            }   
            
            for(link_id=1; link_id<=MMIPDP_MAX_LINK_NUM; link_id++)
            {
                if(g_mmipdp_manager_instance.link_arr[link_id-1].state == MMIPDP_LINK_STATE_DEACTIVED)
                {
                    g_mmipdp_manager_instance.link_arr[link_id-1].pdp_id = 0;
                }
            }
        }
        break;
    case APP_MN_DEACTIVATE_PDP_CONTEXT_IND:
        {
            uint8   pdp_num = 0;
            uint8   pdp_id = 0, i = 0;
             MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
            //SCI_TRACE_LOW:"PDP MANAGER HandlePdpPsMsg,APP_MN_DEACTIVATE_PDP_CONTEXT_IND,pdp_num:%d"
            
		pdp_num = ((APP_MN_GPRS_DEACTIVATE_IND_T*)param)->pdp_num;
		dual_sys = ((APP_MN_GPRS_DEACTIVATE_IND_T*)param)->dual_sys;	
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_1982_112_2_18_2_44_52_29,(uint8*)"d",pdp_num);            
		for(i=0; i<pdp_num; i++)
            {
                pdp_id = ((APP_MN_GPRS_DEACTIVATE_IND_T*)param)->pdp_id[i];

                link_id = FindLinkIdByPdpId((uint32)pdp_id,dual_sys);
                if(0<link_id && MMIPDP_MAX_LINK_NUM>=link_id)
                {
                    fsm_msg_ptr = CreatMsg(MMIPDP_PS_DEACTIVE_IND,0);
                    FsmDispatch(link_id,fsm_msg_ptr);
                }
                else
                {
                    //SCI_TRACE_LOW:"APP_MN_DEACTIVATE_PDP_CONTEXT_IND err link_id=0"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_1995_112_2_18_2_44_52_30,(uint8*)"");
                }
            }
            for(link_id=1; link_id<=MMIPDP_MAX_LINK_NUM; link_id++)
            {
			if(g_mmipdp_manager_instance.link_arr[link_id-1].state != MMIPDP_LINK_STATE_DEACTIVED)
			{
				is_all_released = FALSE;
				break;
			}
            }
            if(is_all_released)
            {
                if(!MMIPDP_PsIsPdpLinkExist(dual_sys))
                {
                    NotifySubscriber(dual_sys, MMIPDP_NOTIFY_EVENT_DEACTIVED);
#if 0
                    for(i=0;i<pdp_num;i++)
                    {
                        pdp_id = ((APP_MN_GPRS_DEACTIVATE_IND_T*)param)->pdp_id[i];
                        if(MMIPDP_IsPdpIdInLinkArray((uint32)pdp_id,((APP_MN_GPRS_DEACTIVATE_IND_T*)param)->dual_sys))
                        {
                            MMIPDP_SetGPRSDetach(((APP_MN_GPRS_DEACTIVATE_IND_T*)param)->dual_sys);
                            break;
                        }
                    }
#endif
                }
            }
        }
        break;
    case APP_MN_GPRS_ATTACH_RESULT:
        NotifySubscriber(((APP_MN_GPRS_ATTACH_RESULT_T*)param)->dual_sys, MMIPDP_NOTIFY_EVENT_ATTACH);
         return MMI_RESULT_FALSE;
    case APP_MN_GPRS_DETACH_RESULT:
        NotifySubscriber(((APP_MN_GPRS_DETACH_RESULT_T*)param)->dual_sys, MMIPDP_NOTIFY_EVENT_DETTACH);
        break;
    default:
        break;
    }
    return MMI_RESULT_TRUE;
}

/****************************************************************************/
//Description : to find the app handler's using link
//Global resource dependence : 
//Author: weipeng.wang
//Note: 
/****************************************************************************/
PUBLIC uint32  MMIPDP_FindLinkIdByApphandler(uint32 app_handler)
{
    uint32 link_id =FindLinkIdByApphandler(app_handler);
    SCI_TRACE_LOW("MMIPDP_FindLinkIdByApphandler: link_id=%d, app_handler=%d", link_id, app_handler);
    return link_id;
}

/****************************************************************************/
//Description : MMIPDP_CreatTimer
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC uint8 MMIPDP_CreatTimer(uint32 timeout)
{
    uint8 timer_id = 0;
    
    if(timeout!=0)
    {
        timer_id = MMK_CreateTimerCallback(timeout, MMIPDP_HandleTimer, NULL, FALSE);
    }
    return timer_id;
}

/****************************************************************************/
//Description : MMIPDP_StopTimer
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC void MMIPDP_StopTimer(uint8 *timer_id_ptr)
{
    if(*timer_id_ptr!=0)
    {
        MMK_StopTimer(*timer_id_ptr);
        *timer_id_ptr = 0;
    }
}

/****************************************************************************/
//Description : get the link info
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC void MMIPDP_GetLinkInfo(uint32 link_id, MMIPDP_LINK_INFO_T *link_info_ptr)
{
    if(link_id<=MMIPDP_MAX_LINK_NUM && link_info_ptr!=0)
    {
        SCI_MEMCPY(link_info_ptr,&(g_mmipdp_manager_instance.link_arr[link_id-1]),sizeof(MMIPDP_LINK_INFO_T));
    }
}

/****************************************************************************/
//Description : MMIPDP_SetLinkPdpId
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC void MMIPDP_SetLinkPdpId(uint32 link_id, uint32 pdp_id)
{
    if(link_id<=MMIPDP_MAX_LINK_NUM && link_id!=0)
    {
        //SCI_TRACE_LOW:"PDP MANAGER MMIPDP_SetLinkPdpId,link_id:%d,pdp_id:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2085_112_2_18_2_44_52_31,(uint8*)"dd",link_id,pdp_id);
        g_mmipdp_manager_instance.link_arr[link_id-1].pdp_id = pdp_id;
        g_mmipdp_manager_instance.link_arr[link_id-1].qos_info.pdp_id = pdp_id;
    }
}

/****************************************************************************/
//Description : MMIPDP_SetLinkDeactiveTimer
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC void MMIPDP_SetLinkDeactiveTimer(uint32 link_id, uint8 timer_id)
{
    if(link_id<=MMIPDP_MAX_LINK_NUM && link_id!=0)
    {
        g_mmipdp_manager_instance.link_arr[link_id-1].deactive_protect_timer = timer_id;
    }
}

/****************************************************************************/
//Description : pdp app initiation
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC void MMIAPIPDP_AppInit (void)
{
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;

    g_pdp_app.ProcessMsg     = HandlePdpPsMsg;
    g_pdp_app.component_type = CT_APPLICATION;

    for(dual_sys = MN_DUAL_SYS_1; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
    {
        s_last_pdp_event[dual_sys] = MMIPDP_NOTIFY_EVENT_DETTACH;
    }

    #ifdef MMI_WIFI_SUPPORT
    if(0 == stMmipdpMem.dwWifiRegAppList)
    {
        /*创建一个无大小限制的List*/
        stMmipdpMem.dwWifiRegAppList = CFL_ListCreat(sizeof(MMIPDP_WIFI_APPINFO_T), 0);
        if (0 == stMmipdpMem.dwWifiRegAppList)
        {
            //SCI_TRACE_LOW:"mmipdp error %s: CFL_ListCreat failed!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2126_112_2_18_2_44_52_32,(uint8*)"s", __func__);
        }
    }
    #endif
    
    SCI_MEMSET(&g_mmipdp_manager_instance,0,sizeof(MMIPDP_MANAGER_INSTANCE_T));
}

/****************************************************************************/
//Description : to handle the pdp manager timer
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC void MMIPDP_HandleTimer(uint8 timer_id, uint32 param)
{
    uint32 link_id = 0;
    uint8 timer_id_to_find = timer_id;
    MMIPDP_LIST_NODE_HANDLE node_handler = 0;
    MMIPDP_LIST_HANDLE      list_handler = 0;
    MMIPDP_LIST_APP_INFO_T  *node_data_ptr = 0;
    MMIPDP_FSM_MSG_T        *msg_ptr = 0;
    
    if(timer_id==0)
    {
        return;
    }
    //check if deactive timer
    if(0 != (link_id=FindLinkIdByDeactiveTimer(timer_id)))
    {
        //SCI_TRACE_LOW:"PDP MANAGER MMIPDP_HandleTimer: deactive timer,link_id:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2154_112_2_18_2_44_52_33,(uint8*)"d",link_id);
        MMIPDP_StopTimer(&g_mmipdp_manager_instance.link_arr[link_id-1].deactive_protect_timer);
#if 0
        if (!MMIPDP_PsIsPdpLinkExist(g_mmipdp_manager_instance.link_arr[link_id-1].dual_sys))
        {
            MMIPDP_SetGPRSDetach(g_mmipdp_manager_instance.link_arr[link_id-1].dual_sys);
        }
#endif
        ChangeFlowRate(link_id);
        msg_ptr = CreatMsg(MMIPDP_PS_DEACTIVE_CNF,0);
        FsmDispatch(link_id,msg_ptr);
    }
    //check if reactive timer
    else if(0!= (link_id=FindLinkIdByReactiveTimer(timer_id)))
    {
        //SCI_TRACE_LOW:"PDP MANAGER MMIPDP_HandleTimer: reactive timer,link_id:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2169_112_2_18_2_44_52_34,(uint8*)"d",link_id);
        MMIPDP_StopTimer(&g_mmipdp_manager_instance.link_arr[link_id-1].reactive_timer_id);
        if(g_mmipdp_manager_instance.link_arr[link_id-1].state == MMIPDP_LINK_STATE_ACTIVING)
        {
            RestartAllActiveTimerInUsingList(link_id);
            g_mmipdp_manager_instance.link_arr[link_id-1].reactive_count++;
            MMIPDP_PsActivePdp(link_id);        
        }
        else
        {
            //SCI_TRACE_LOW:"PDP MANAGER MMIPDP_HandleTimer: reactive timer,wrong state"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2179_112_2_18_2_44_52_35,(uint8*)"");
            g_mmipdp_manager_instance.link_arr[link_id-1].reactive_count = 0;
        }
    }
    //check if active timer
    else
    {
        uint32 app_handler = 0;
        link_id = FindNodeByActiveTimer(&timer_id_to_find,&list_handler,&node_handler);
        if(node_handler!=0 && list_handler!=0)
        {
            node_data_ptr = MMIPDP_ListGetNodeData(node_handler);
            if(node_data_ptr==0)
            {
                return;
            }
            app_handler = node_data_ptr->active_info.app_handler;
            MMIPDP_StopTimer(&node_data_ptr->active_info.active_timer_id);
            //in the pending list
            if(list_handler == g_mmipdp_manager_instance.pending_list)
            {
                //SCI_TRACE_LOW:"PDP MANAGER MMIPDP_HandleTimer: active timer,pending list"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2200_112_2_18_2_44_52_36,(uint8*)"");
                //SCI_TRACE_LOW:"PDP MANAGER Active timeout callback, cannot get link resouce, other higher priority APP is using"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2201_112_2_18_2_44_52_37,(uint8*)"");
                Callback(&node_data_ptr->active_info,link_id,node_data_ptr->msg_id_to_callback,MMIPDP_RESULT_TIMEOUT);
                
                //to check if the node is still exist after callback
                node_handler = 0;
                node_handler = MMIPDP_ListFindNode(
                    g_mmipdp_manager_instance.pending_list,
                    (void*)app_handler,
                    FindNodeByApphandlerCb
                    );
                if(node_handler!=0)
                {
                    FreeMemAllocInListNode(node_handler);
                    MMIPDP_ListDeleteNode(list_handler,node_handler);
                }
            }
            //in using list
            else
            {
                if(link_id>0)
                {
                    //SCI_TRACE_LOW:"PDP MANAGER MMIPDP_HandleTimer: active timer,using list"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2222_112_2_18_2_44_52_38,(uint8*)"");
                    //SCI_TRACE_LOW:"PDP MANAGER Active timeout callback, no response from PS"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2223_112_2_18_2_44_52_39,(uint8*)"");
                    msg_ptr = CreatMsg(MMIPDP_ACTIVE_TIMOUT_IND,(void*)node_handler);
                    FsmDispatch(link_id,msg_ptr);
                }
            }
        }
    }
}

/****************************************************************************/
//Description : active internal pdp
//Global resource dependence : 
//Author: zhongping.shi
//Note: 
/****************************************************************************/
PUBLIC void MMIAPIPDP_ActiveInternalPdp(MN_DUAL_SYS_E dual_sys, MMICONNECTION_LINKSETTING_DETAIL_T *link_info)
{
#ifndef WIN32
    BOOLEAN                 context_param[7] = {TRUE,TRUE,TRUE,FALSE,FALSE,FALSE,FALSE};
    MN_GPRS_PDP_ADDR_T      pdp_addr = {0}; 
    ERR_MNGPRS_CODE_E       err = ERR_MNGPRS_NO_ERR;
    MN_INTERNAL_PDP_CTX_TYPE pdpctx_type = MN_PDP_CTX_TYPE_INVALID;
    
    SCI_TRACE_LOW("MMIPDP_ActiveInternalPdp dual_sys:%d, account_type:%d port:%d", dual_sys, link_info->account_type, link_info->port);
    if ((MMICONNECTION_ACCOUNT_XCAP == link_info->account_type) || (MMICONNECTION_ACCOUNT_BROWSER == link_info->account_type))
    {
        pdpctx_type = MN_PDP_CTX_TYPE_XCAP;
    }
    else if (MMICONNECTION_ACCOUNT_IA == link_info->account_type)
    {
        pdpctx_type = MN_PDP_CTX_TYPE_IA;
    }
    else if (MMICONNECTION_ACCOUNT_IMS == link_info->account_type)
    {
        pdpctx_type = MN_PDP_CTX_TYPE_IMS;
    }
    else
    {
        SCI_TRACE_LOW("MMIPDP_ActiveInternalPdp, link_info->access_type:%d", link_info->account_type);
        return;
    }
    
    if (MMICONNECTION_IP_V4 == link_info->ip_type)
    {
        if(ERR_MNGPRS_NO_ERR != (err=MNGPRS_SetInteralPdpContextEx(dual_sys,context_param,pdpctx_type,(uint8 *)"IP",(uint8 *)link_info->apn,pdp_addr,0,0,0,link_info->username, link_info->password, link_info->auth_type)))
        {
            SCI_TRACE_LOW("IPV4 MNGPRS_SetInteralPdpContextEx:%d", err);
        }
    } 
    else if (MMICONNECTION_IP_V6 == link_info->ip_type)
    {
        if (ERR_MNGPRS_NO_ERR != (err=MNGPRS_SetInteralPdpContextEx(dual_sys,context_param,pdpctx_type,(uint8 *)"IPV6",(uint8 *)link_info->apn,pdp_addr,0,0,0,link_info->username, link_info->password, link_info->auth_type)))
        {
            SCI_TRACE_LOW("IPV6 MNGPRS_SetInteralPdpContextEx:%d", err);
        }
    } 
    else
    {
        if (ERR_MNGPRS_NO_ERR != (err=MNGPRS_SetInteralPdpContextEx(dual_sys,context_param,pdpctx_type,(uint8 *)"IPV4V6",(uint8 *)link_info->apn,pdp_addr,0,0,0,link_info->username, link_info->password, link_info->auth_type)))
        {
            SCI_TRACE_LOW("IPV4/IPV6 MNGPRS_SetInteralPdpContextEx:%d", err);
        }
    }
#endif
    return;
}

/* BEGIN: Added by George.Liu 01526, 2010/9/27   PN:ms00203493 */
#ifdef MMI_WIFI_SUPPORT
/*****************************************************************************
 Prototype    : mmipdp_EqualWifiRegAppHandler
 Description  : 判断handler是否一致
 Input        : CFL_NODE_OBJECT data  
                void *find_data       
 Output       : None
 Return Value : LOCAL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2010/9/28
    Author       : George.Liu 01526
    Modification : Created function

*****************************************************************************/
LOCAL BOOLEAN mmipdp_EqualWifiRegAppHandler(CFL_NODE_OBJECT data, void *find_data)
{
    if(0 == data)
    {
        return FALSE;
    }
    //SCI_TRACE_LOW:"!!!!!! app_handler = %d,find_data = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2250_112_2_18_2_44_52_40,(uint8*)"dd",((MMIPDP_WIFI_APPINFO_T *)data)->app_handler,(uint32)find_data);

    return (((MMIPDP_WIFI_APPINFO_T *)data)->app_handler == (uint32)find_data);
}

/*****************************************************************************
 Prototype    : mmipdp_EqualWifiRegAppStatus
 Description  : 判断状态是否一致
 Input        : CFL_NODE_OBJECT data  
                void *find_data       
 Output       : None
 Return Value : LOCAL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2010/9/28
    Author       : George.Liu 01526
    Modification : Created function

*****************************************************************************/
LOCAL BOOLEAN mmipdp_EqualWifiRegAppStatus(CFL_NODE_OBJECT data, void *find_data)
{
    if(0 == data)
    {
        return FALSE;
    }
    
    return (((MMIPDP_WIFI_APPINFO_T *)data)->e_app_status == ((MMIPDP_LINK_STATE_E)(uint32)find_data));
}

/*****************************************************************************
 Prototype    : MMIAPIPDP_WifiActiveCnf
 Description  : ActiveCnf处理
 Input        : MMIPDP_RESULT_E eResult  
 Output       : None
 Return Value : PUBLIC
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2010/9/28
    Author       : George.Liu 01526
    Modification : Created function

*****************************************************************************/
PUBLIC void MMIAPIPDP_WifiActiveCnf(MMIPDP_RESULT_E eResult)
{
    CFL_LIST_NODE_HANDLE   node_found_handler = 0;
    uint32 i = 0;
    CFL_NODE_OBJECT        node_data;
    MMIPDP_WIFI_APPINFO_T  *pstAppRegInfo = PNULL;
    MMIPDP_CNF_INFO_T      stAppCnf = {0};
    MMIPDP_WIFI_APPINFO_T  temp_app_reg_info = {0};
    
    for (i = 0; i < 10000; i++)
    {
        node_found_handler = CFL_ListFindNode(stMmipdpMem.dwWifiRegAppList, (void *)MMIPDP_LINK_STATE_DEACTIVED, mmipdp_EqualWifiRegAppStatus);
        if (0 == node_found_handler)
        {
            break;
        }

        node_data = CFL_ListGetNodeData(node_found_handler);
        pstAppRegInfo = (MMIPDP_WIFI_APPINFO_T *)node_data;
        if (PNULL == pstAppRegInfo)
        {
            //SCI_TRACE_LOW:"mmipdp error %s: pstAppRegInfo is PNULL!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2320_112_2_18_2_44_53_41,(uint8*)"s", __func__);
            return;
        }

        /*向app发送ActiveCnf消息*/
        stAppCnf.app_handler = pstAppRegInfo->app_handler;
        stAppCnf.nsapi       = MMIAPIWIFI_GetNetId(); 
        stAppCnf.msg_id      = MMIPDP_ACTIVE_CNF;
        stAppCnf.ps_interface = MMIPDP_INTERFACE_WIFI;
        stAppCnf.result      = eResult;
        if (PNULL == pstAppRegInfo->handle_msg_callback)
        {
            //SCI_TRACE_LOW:"mmipdp error %s: handle_msg_callback is PNULL!app_handler is 0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2332_112_2_18_2_44_53_42,(uint8*)"sd", __func__, pstAppRegInfo->app_handler);
            return;
        }
        //SCI_TRACE_LOW:"mmipdp info %s: app_handler=%p,nsapi=0x%x,result=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2335_112_2_18_2_44_53_43,(uint8*)"sdd", __func__, stAppCnf.app_handler, stAppCnf.nsapi, stAppCnf.result);
        pstAppRegInfo->handle_msg_callback(&stAppCnf);
        //SCI_TRACE_LOW:"mmipdp info %s: handle_msg_callback exit"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2337_112_2_18_2_44_53_44,(uint8*)"s", __func__);
        temp_app_reg_info.app_handler = pstAppRegInfo->app_handler;
        temp_app_reg_info.handle_msg_callback = pstAppRegInfo->handle_msg_callback;

        /*更改app注册信息*/
        if (eResult != MMIPDP_RESULT_SUCC)
        {
            CFL_ListDeleteNode(stMmipdpMem.dwWifiRegAppList, node_found_handler);
        }
        else
        {
            temp_app_reg_info.e_app_status = MMIPDP_LINK_STATE_ACTIVED;
            CFL_ListUpdateNodeData(stMmipdpMem.dwWifiRegAppList, node_found_handler, (CFL_NODE_OBJECT)&temp_app_reg_info, sizeof(MMIPDP_WIFI_APPINFO_T));
        }
    }

    if (i >= 10000)
    {
        //SCI_TRACE_LOW:"mmipdp error %s: loop is overlap!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2355_112_2_18_2_44_53_45,(uint8*)"s", __func__);
    }

    //SCI_TRACE_LOW:"mmipdp info %s: final, i is %d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2358_112_2_18_2_44_53_46,(uint8*)"sd", __func__, i);

    return;
}

/*****************************************************************************
 Prototype    : MMIPDP_WifiDeactiveCnf
 Description  : DeactiveCnf处理
 Input        : uint32 app_handler  
 Output       : None
 Return Value : PUBLIC
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2010/9/28
    Author       : George.Liu 01526
    Modification : Created function

*****************************************************************************/
PUBLIC BOOLEAN MMIPDP_WifiDeactiveCnf(uint32 app_handler)
{
    CFL_LIST_NODE_HANDLE   node_found_handler = 0;
    uint32 i = 0;
    CFL_NODE_OBJECT        node_data;
    MMIPDP_WIFI_APPINFO_T  *pstAppRegInfo = PNULL;
    MMIPDP_CNF_INFO_T      stAppCnf = {0};

    for (i = 0; i < 10000; i++)
    {
        node_found_handler = CFL_ListFindNode(stMmipdpMem.dwWifiRegAppList, (void *)app_handler, mmipdp_EqualWifiRegAppHandler);
        if (0 == node_found_handler)
        {
            break;
        }

        node_data = CFL_ListGetNodeData(node_found_handler);
        pstAppRegInfo = (MMIPDP_WIFI_APPINFO_T *)node_data;
        if (PNULL == pstAppRegInfo)
        {
            //SCI_TRACE_LOW:"mmipdp error %s: pstAppRegInfo is PNULL!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2397_112_2_18_2_44_53_47,(uint8*)"s", __func__);
            return FALSE;
        }

        /*向app发送DeactiveCnf消息*/
        stAppCnf.app_handler = pstAppRegInfo->app_handler;
        stAppCnf.msg_id      = MMIPDP_DEACTIVE_CNF;
        stAppCnf.result      = MMIPDP_RESULT_SUCC;
        stAppCnf.ps_interface = MMIPDP_INTERFACE_WIFI;

        if (PNULL == pstAppRegInfo->handle_msg_callback)
        {
            //SCI_TRACE_LOW:"mmipdp error %s: handle_msg_callback is PNULL!app_handler is %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2409_112_2_18_2_44_53_48,(uint8*)"sd", __func__, pstAppRegInfo->app_handler);
            return FALSE;
        }
        //SCI_TRACE_LOW:"mmipdp info %s: handle_msg_callback entry %p"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2412_112_2_18_2_44_53_49,(uint8*)"s", __func__, pstAppRegInfo->handle_msg_callback);
        pstAppRegInfo->handle_msg_callback(&stAppCnf);
        //SCI_TRACE_LOW:"mmipdp info %s: handle_msg_callback exit"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2414_112_2_18_2_44_53_50,(uint8*)"s", __func__);

        /*更改app注册信息*/
        CFL_ListDeleteNode(stMmipdpMem.dwWifiRegAppList, node_found_handler);
    }

    if (i >= 10000)
    {
        //SCI_TRACE_LOW:"mmipdp error %s: loop is overlap!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2422_112_2_18_2_44_53_51,(uint8*)"s", __func__);
        return FALSE;
    }

    return TRUE;
}

/*****************************************************************************
 Prototype    : mmipdp_WifiDeactiveInd
 Description  : 主动通知应用wifi已经断开
 Input        : void * pdwData  
 Output       : None
 Return Value : PUBLIC
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2010/9/27
    Author       : George.Liu 01526
    Modification : Created function

*****************************************************************************/
PUBLIC void MMIAPIPDP_WifiDeactiveInd(void)
{
    CFL_LIST_NODE_HANDLE   node_found_handler = 0;
    uint32 i = 0;
    CFL_NODE_OBJECT        node_data;
    MMIPDP_WIFI_APPINFO_T  *pstAppRegInfo = PNULL;
    MMIPDP_CNF_INFO_T      stAppCnf = {0};

    for (i = 0; i < 10000; i++)
    {
        node_found_handler = CFL_ListFindNode(stMmipdpMem.dwWifiRegAppList, (void *)MMIPDP_LINK_STATE_ACTIVED, mmipdp_EqualWifiRegAppStatus);
        if (0 == node_found_handler)
        {
            break;
        }

        node_data = CFL_ListGetNodeData(node_found_handler);
        pstAppRegInfo = (MMIPDP_WIFI_APPINFO_T *)node_data;
        if (PNULL == pstAppRegInfo)
        {
            //SCI_TRACE_LOW:"mmipdp error %s: pstAppRegInfo is PNULL!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2463_112_2_18_2_44_53_52,(uint8*)"s", __func__);
            return;
        }

        /*向app发送DeactiveInd消息*/
        stAppCnf.app_handler = pstAppRegInfo->app_handler;
        stAppCnf.msg_id      = MMIPDP_DEACTIVE_IND;
        stAppCnf.result      = MMIPDP_RESULT_SUCC;
        stAppCnf.ps_interface = MMIPDP_INTERFACE_WIFI;

        if (PNULL == pstAppRegInfo->handle_msg_callback)
        {
            //SCI_TRACE_LOW:"mmipdp error %s: handle_msg_callback is PNULL!app_handler is %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2475_112_2_18_2_44_53_53,(uint8*)"sd", __func__, pstAppRegInfo->app_handler);
            return;
        }
        //SCI_TRACE_LOW:"mmipdp info %s: handle_msg_callback entry %p"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2478_112_2_18_2_44_53_54,(uint8*)"s", __func__, pstAppRegInfo->handle_msg_callback);
        pstAppRegInfo->handle_msg_callback(&stAppCnf);
        //SCI_TRACE_LOW:"mmipdp info %s: handle_msg_callback exit"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2480_112_2_18_2_44_53_55,(uint8*)"s", __func__);

        /*更改app注册信息*/
        CFL_ListDeleteNode(stMmipdpMem.dwWifiRegAppList, node_found_handler);
    }

    if (i >= 10000)
    {
        //SCI_TRACE_LOW:"mmipdp error %s: loop is overlap!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2488_112_2_18_2_44_53_56,(uint8*)"s", __func__);
        return ;
    }

    return;
}

/****************************************************************************/
//Description : Indicate the app connection of wifi
//Global resource dependence : 
//Author: Jiaoyouwu
//Note:
/****************************************************************************/
PUBLIC void MMIAPIPDP_WifiServiceChangeInd(void)
{
    MMIPDP_CNF_INFO_T      stAppCnf = {0};
    uint32 link_id = 1;
    MMIPDP_LIST_NODE_HANDLE node_handler = 0;
    MMIPDP_LIST_APP_INFO_T *node_data_ptr = 0;

    for(link_id = 1; link_id <= MMIPDP_MAX_LINK_NUM; link_id++)
    {
        node_handler = MMIPDP_ListGetHead(g_mmipdp_manager_instance.link_arr[link_id-1].using_list);

        while(node_handler != 0)
        {
            node_data_ptr = MMIPDP_ListGetNodeData(node_handler);
            if(node_data_ptr != 0)
            {
                stAppCnf.app_handler = node_data_ptr->active_info.app_handler;
                stAppCnf.msg_id      = MMIPDP_SERVICE_CHANGE_IND;
                stAppCnf.result      = MMIPDP_RESULT_SUCC;
                stAppCnf.ps_interface = MMIPDP_INTERFACE_WIFI;

                if (PNULL == node_data_ptr->active_info.handle_msg_callback)
                {
                    //SCI_TRACE_LOW:"mmipdp error %s: handle_msg_callback is PNULL!app_handler is %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2522_112_2_18_2_44_53_57,(uint8*)"sd", __func__, node_data_ptr->active_info.app_handler);
                    return;
                }
                //SCI_TRACE_LOW:"mmipdp info %s: handle_msg_callback entry %p"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2525_112_2_18_2_44_53_58,(uint8*)"s", __func__, node_data_ptr->active_info.handle_msg_callback);
                node_data_ptr->active_info.handle_msg_callback(&stAppCnf);
                //SCI_TRACE_LOW:"mmipdp info %s: handle_msg_callback exit"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2527_112_2_18_2_44_53_59,(uint8*)"s", __func__);
            }

            node_handler = MMIPDP_ListGetNextNode(node_handler);
        }
    }

    return;
}

/****************************************************************************/
//Description : Indicate Service change
//Global resource dependence : 
//Author: Jiaoyouwu
//Note:
/****************************************************************************/
PUBLIC void MMIAPIPDP_GPRSServiceChangeInd(void)
{
    MMIPDP_CNF_INFO_T      stAppCnf = {0};
    uint32 link_id = 1;
    MMIPDP_LIST_NODE_HANDLE node_handler = 0;
    MMIPDP_LIST_APP_INFO_T *node_data_ptr = 0;

    for(link_id = 1; link_id <= MMIPDP_MAX_LINK_NUM; link_id++)
    {
        node_handler = MMIPDP_ListGetHead(g_mmipdp_manager_instance.link_arr[link_id-1].using_list);

        while(node_handler != 0)
        {
            node_data_ptr = MMIPDP_ListGetNodeData(node_handler);
            if(node_data_ptr != 0)
            {
                stAppCnf.app_handler = node_data_ptr->active_info.app_handler;
                stAppCnf.msg_id      = MMIPDP_SERVICE_CHANGE_IND;
                stAppCnf.result      = MMIPDP_RESULT_SUCC;
                stAppCnf.ps_interface = MMIPDP_INTERFACE_GPRS;

                if (PNULL == node_data_ptr->active_info.handle_msg_callback)
                {
                    //SCI_TRACE_LOW:"mmipdp error %s: handle_msg_callback is PNULL!app_handler is %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2564_112_2_18_2_44_53_60,(uint8*)"sd", __func__, node_data_ptr->active_info.app_handler);
                    return;
                }
                //SCI_TRACE_LOW:"mmipdp info %s: handle_msg_callback entry %p"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2567_112_2_18_2_44_53_61,(uint8*)"s", __func__, node_data_ptr->active_info.handle_msg_callback);
                node_data_ptr->active_info.handle_msg_callback(&stAppCnf);
                //SCI_TRACE_LOW:"mmipdp info %s: handle_msg_callback exit"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2569_112_2_18_2_44_53_62,(uint8*)"s", __func__);
            }

            node_handler = MMIPDP_ListGetNextNode(node_handler);
        }
    }

    return;
}

/*****************************************************************************
 Prototype    : mmipdp_WifiRegApp
 Description  : 在pdp中注册app
 Input        : MMIPDP_WIFI_APPINFO_T *pstWifiAppInfo  
 Output       : None
 Return Value : LOCAL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2010/9/28
    Author       : George.Liu 01526
    Modification : Created function

*****************************************************************************/
LOCAL CFL_LIST_NODE_HANDLE mmipdp_WifiRegApp(MMIPDP_WIFI_APPINFO_T *pstAppRegInfo)
{
    CFL_LIST_NODE_HANDLE  node_found_handler = 0;

    if(PNULL == pstAppRegInfo)
    {
        //SCI_TRACE_LOW:"mmipdp error %s: pstWifiAppInfo is PNULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2599_112_2_18_2_44_53_63,(uint8*)"s", __func__);
        return 0;
    }
    
    if (0 == stMmipdpMem.dwWifiRegAppList)
    {
        //SCI_TRACE_LOW:"mmipdp error %s: dwWifiRegAppList is zeros!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2605_112_2_18_2_44_53_64,(uint8*)"s", __func__);
        return 0;
    }

    node_found_handler = CFL_ListFindNode(stMmipdpMem.dwWifiRegAppList, (void *)(pstAppRegInfo->app_handler), mmipdp_EqualWifiRegAppHandler);
    if(0 == node_found_handler)
    {
        //SCI_TRACE_LOW:"mmipdp info %s: CFL_ListInsertNode!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2612_112_2_18_2_44_53_65,(uint8*)"s", __func__);
        node_found_handler = CFL_ListInsertNode(stMmipdpMem.dwWifiRegAppList, (CFL_LIST_NODE_HANDLE)0, pstAppRegInfo, sizeof(MMIPDP_WIFI_APPINFO_T)); 
    }
    else
    {
        //SCI_TRACE_LOW:"mmipdp info %s: CFL_ListUpdateNodeData!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2617_112_2_18_2_44_53_66,(uint8*)"s", __func__);
        pstAppRegInfo->e_app_status = MMIPDP_LINK_STATE_DEACTIVED;
        CFL_ListUpdateNodeData(stMmipdpMem.dwWifiRegAppList, node_found_handler, pstAppRegInfo, sizeof(MMIPDP_WIFI_APPINFO_T));
    }
    return node_found_handler;
}


/*****************************************************************************
 Prototype    : mmipdp_ActiveWifi
 Description  : 激活wifi
 Input        : MMIPDP_ACTIVE_INFO_T *app_info_ptr  
 Output       : None
 Return Value : LOCAL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2010/9/28
    Author       : George.Liu 01526
    Modification : Created function

*****************************************************************************/
LOCAL BOOLEAN mmipdp_ActiveWifi(MMIPDP_ACTIVE_INFO_T *app_info_ptr)
{
    MMIPDP_WIFI_APPINFO_T  WifiAppInfo  = {0};
    CFL_LIST_NODE_HANDLE   qwAppRegNode = 0;
    APP_MN_GPRS_EXT_T    *sig_ptr = PNULL;
    uint16               sig_size = 0;  

    /*第一步，在pdp系统中注册, 初始状态都是DEACTIVE*/
    WifiAppInfo.app_handler         = app_info_ptr->app_handler;
    WifiAppInfo.handle_msg_callback = app_info_ptr->handle_msg_callback;
    WifiAppInfo.e_app_status        = MMIPDP_LINK_STATE_DEACTIVED;
    qwAppRegNode = mmipdp_WifiRegApp(&WifiAppInfo);
    if (0 == qwAppRegNode)
    {
        //SCI_TRACE_LOW:"mmipdp error %s: mmipdp_WifiRegApp is abnormal"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2653_112_2_18_2_44_53_67,(uint8*)"s", __func__);
        return FALSE;
    }

    /*如果当前是飞行模式，则不允许使用网络*/
    if(TRUE == MMIAPISET_GetFlyMode())
    {
        //SCI_TRACE_LOW:"mmipdp info %s: mmipdp_WifiRegApp is abnormal"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2660_112_2_18_2_44_53_68,(uint8*)"s", __func__);
        
        MMIAPIPDP_WifiActiveCnf(MMIPDP_RESULT_NOT_PERMIT);

        return TRUE; /*返回TRUE代表流程正确，FALSE代表程序本身出现错误*/
    }   

    /*第二步，激活WiFi连接*/
    /*wifi已经处于激活状态，直接返回成功*/
    if(MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus())
    {
        //SCI_TRACE_LOW:"mmipdp info %s: wifi already actived"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2671_112_2_18_2_44_53_69,(uint8*)"s", __func__);
        
        //MMIAPIPDP_WifiActiveCnf(MMIPDP_RESULT_SUCC);
        sig_size = sizeof(APP_MN_GPRS_EXT_T);
        SCI_CREATE_SIGNAL(sig_ptr,APP_MN_ACTIVATE_PDP_CONTEXT_CNF,sig_size,P_APP);   /*lint !e64*/
        sig_ptr->result = MN_GPRS_ERR_SUCCESS;
        sig_ptr->pdp_id = 0;
        SCI_SEND_SIGNAL(sig_ptr,P_APP);        
        return TRUE;
    }
    
    /*wifi尚未激活，需要先激活WiFi*/
    /*1、需要手动连接*/
    if (PNULL == app_info_ptr->apn_ptr)
    {
        //SCI_TRACE_LOW:"mmipdp info %s: call wifi list_win"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2686_112_2_18_2_44_53_70,(uint8*)"s", __func__);
    
        MMIAPIWIFI_OpenListWin();
        
        return TRUE;
    }

    /*2、自动连接(已经提供连接信息)*/
    /*
    根据应用提供的用户名密码直接进行连接。目前尚不支持。
    */
    
    //SCI_TRACE_LOW:"mmipdp error %s: app active info is wrong"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2698_112_2_18_2_44_53_71,(uint8*)"s", __func__);

    return FALSE;
}

/*****************************************************************************
 Prototype    : mmipdp_DeactiveWifi
 Description  : 去激活Wifi
 Input        : uint32 app_handler  
 Output       : None
 Return Value : LOCAL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2010/9/28
    Author       : George.Liu 01526
    Modification : Created function

*****************************************************************************/
LOCAL BOOLEAN mmipdp_DeactiveWifi(uint32 app_handler)
{
    //SCI_TRACE_LOW:"mmipdp info %s: app_handler=%d deactive Wifi!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2719_112_2_18_2_44_53_72,(uint8*)"sd", __func__, app_handler);
    return MMIPDP_WifiDeactiveCnf(app_handler);
}

/*****************************************************************************
 Prototype    : mmipdp_GetWifiLinkState
 Description  : 获取wifi链路的状态
 Input        : uint32 app_handler  
 Output       : None
 Return Value : LOCAL
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2010/10/3
    Author       : George.Liu 01526
    Modification : Created function

*****************************************************************************/
LOCAL MMIPDP_LINK_STATE_E mmipdp_GetWifiLinkState(uint32 app_handler)
{
    CFL_LIST_NODE_HANDLE   node_found_handler = 0;
    CFL_NODE_OBJECT        node_data;
    MMIPDP_WIFI_APPINFO_T  *pstAppRegInfo = PNULL;
    
    node_found_handler = CFL_ListFindNode(stMmipdpMem.dwWifiRegAppList, (void *)app_handler, mmipdp_EqualWifiRegAppHandler);
    if (0 == node_found_handler)
    {
        return MMIPDP_LINK_STATE_MAX;
    }

    node_data = CFL_ListGetNodeData(node_found_handler);
    pstAppRegInfo = (MMIPDP_WIFI_APPINFO_T *)node_data;
    if (PNULL == pstAppRegInfo)
    {
        //SCI_TRACE_LOW:"mmipdp error %s: pstAppRegInfo is PNULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2753_112_2_18_2_44_53_73,(uint8*)"s", __func__);
        return MMIPDP_LINK_STATE_MAX;
    }

    return pstAppRegInfo->e_app_status;
}

#endif
/* END:   Added by George.Liu 01526, 2010/9/27   PN:ms00203493 */

/****************************************************************************/
//Description : Active pdp link req
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIAPIPDP_Active(MMIPDP_ACTIVE_INFO_T *app_info_ptr)
{
    uint32                  link_id = 0;
    MMIPDP_LIST_NODE_HANDLE node_handler = 0;
    MMIPDP_FSM_T            *fsm_ptr = 0;
    MMIPDP_FSM_MSG_T        *msg_ptr = 0;
    BOOLEAN                 rf_ret = FALSE;

    SCI_TRACE_LOW("MMIAPIPDP_Active Enter");
    SCI_TRACE_LOW("MMIAPIPDP_Active ip_type : %d", app_info_ptr->ip_type);
    if (0 == app_info_ptr)
    {
        //SCI_TRACE_LOW:"mmipdp error MMIAPIPDP_Active: app_info_ptr is PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2776_112_2_18_2_44_53_74,(uint8*)"");
        return FALSE;
    }

    //SCI_TRACE_LOW:"mmipdp info MMIAPIPDP_Active: priority:%d, app_handler:0x%x, apn:%s, dual_sys:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2780_112_2_18_2_44_53_75,(uint8*)"ddsd", app_info_ptr->priority, app_info_ptr->app_handler, app_info_ptr->apn_ptr?app_info_ptr->apn_ptr:"NULL", app_info_ptr->dual_sys);
    if((0 == MNPHONE_GetAttState(app_info_ptr->dual_sys)) 
		&&(app_info_ptr->app_handler != MMI_MODULE_MMS)
		&&(app_info_ptr->app_handler != MMIAUT_GetModuleID())
    )
    {
        //MMIPUB_OpenAlertWarningWin(STR_SIM_SEARCH_FAIL);
        SCI_TRACE_LOW("mmipdp error MMIAPIPDP_Active: SIM%d is not attached ready!!", app_info_ptr->dual_sys);
        #if 0 //ndef WIN32 不能开启，在弱信号下会掉卡
        MMIAPIPHONE_DeactivePS(app_info_ptr->dual_sys); //在这里处理一下拔卡出来卡的状态还是SIM_STATUS_OK
        #endif
        return FALSE;
    }

    if (0 == app_info_ptr->handle_msg_callback)
    {
        //SCI_TRACE_LOW:"mmipdp error MMIAPIPDP_Active: handle_msg_callback is PNULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2784_112_2_18_2_44_53_76,(uint8*)"");
        return FALSE;
    }


    //inject rf
    if (PNULL != s_p_injiection_func)
    {
        rf_ret = (s_p_injiection_func)(MMIPDP_ACTIVE_REQ, (void*)app_info_ptr, sizeof(MMIPDP_ACTIVE_INFO_T));
        if (TRUE == rf_ret)
        {
            SCI_TRACE_LOW("[MMIAPIPDP_Active] rf inject success");
            return TRUE;
        }
    }

#ifdef MMI_WIFI_SUPPORT
    if (MMIPDP_INTERFACE_WIFI == app_info_ptr->ps_interface)
    {
        //SCI_TRACE_LOW:"mmipdp info %s: Request WiFi interface"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2800_112_2_18_2_44_54_78,(uint8*)"s", __func__);

        return mmipdp_ActiveWifi(app_info_ptr);
    }
#endif
    
#ifdef MMI_DUALMODE_ENABLE 
    //SCI_TRACE_LOW:"PDP MANAGER MMIAPISET_GetSwitchRatState() = %d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2807_112_2_18_2_44_54_79,(uint8*)"d", MMIAPISET_GetSwitchRatState());
    
    if(MMIAPISET_GetSwitchRatState())
    {
        return FALSE;
    }    
#else
#if 0
#ifdef MMI_CALL_MINIMIZE_SUPPORT
    if(MMIAPICC_IsInState(CC_IN_CALL_STATE))
    {
        //SCI_TRACE_LOW:"mmipdp error call is being"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2817_112_2_18_2_44_54_80,(uint8*)"");
        return FALSE;
    }
#endif
#endif
#endif

    if (0 == app_info_ptr->apn_ptr)
    {
        //SCI_TRACE_LOW:"mmipdp error MMIAPIPDP_Active: GPRS apn_ptr is PNULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2825_112_2_18_2_44_54_81,(uint8*)"");
        return FALSE;
    }
    
    if (app_info_ptr->dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"mmipdp error MMIAPIPDP_Active: GPRS dual_sys is wrong!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2831_112_2_18_2_44_54_82,(uint8*)"");
        return FALSE;
    }

#ifdef FDN_SUPPORT
    if(!MMIPDP_IsFDNPermitActivePdp(app_info_ptr->dual_sys))
    {
        //SCI_TRACE_LOW:"mmipdp error MMIAPIPDP_Active: GPRS FDN is forbbiden!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2791_112_2_18_2_44_53_77,(uint8*)"");
        Callback(app_info_ptr,link_id,MMIPDP_ACTIVE_CNF,MMIPDP_RESULT_FDN_NOT_PERMIT);
        return TRUE;//must be true, OR it will popup fail message of MMS
    }
#endif
    
#ifndef WIN32
#ifndef GPRSMPDP_SUPPORT
    if(!(MMIAPIPHONE_IsPsServiceAvailble()))//(ATC_MODEM_ONDIAL == ATC_GetModemStatus())
    {
        //SCI_TRACE_LOW:"PDP MANAGER MMIAPIPDP_Active,priority:MMIPDP_RESULT_AT_IS_ON"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2839_112_2_18_2_44_54_83,(uint8*)"");
        Callback(app_info_ptr,link_id,MMIPDP_ACTIVE_CNF,MMIPDP_RESULT_AT_IS_ON);
        return TRUE;
    }
#endif
#endif

    if (MMIAPICONNECTION_isGPRSSwitchedOff())
    {
        Callback(app_info_ptr, link_id, MMIPDP_ACTIVE_CNF, MMIPDP_RESULT_FAIL);
        return TRUE;
    }
//////////////////////////////////////////////////////////////////////////
// added by feng.xiao
#ifdef DATA_ROAMING_SUPPORT
    if ((MMIPHONE_ROAMING_STATUS_NATIONAL_ROMING == MMIAPIPHONE_GetRoamingStatus(app_info_ptr->dual_sys))
        ||(MMIPHONE_ROAMING_STATUS_NATIVE_ROMING == MMIAPIPHONE_GetRoamingStatus(app_info_ptr->dual_sys)))
    {
        if (MMICONNECTION_SETTING_DATA_ROAMING_DISABLE == MMAPIICONNECTION_Setting_GetDataRoamingStatus())
        {
            //SCI_TRACE_LOW:"PDP MANAGER MMIAPIPDP_Active,MMICONNECTION_SETTING_DATA_ROAMING_DISABLE"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2853_112_2_18_2_44_54_84,(uint8*)"");
            Callback(app_info_ptr, link_id, MMIPDP_ACTIVE_CNF, MMIPDP_RESULT_NOT_PERMIT_ROAMING);
            return TRUE;
        }

       if(MMICONNECTION_IsSupportDomesticRoaming())
       {
           if (MMICONNECTION_SETTING_DATA_ROAMING_DOMESTIC_ENABLE == MMAPIICONNECTION_Setting_GetDataRoamingStatus()
               && (MMIPHONE_ROAMING_STATUS_NATIONAL_ROMING == MMIAPIPHONE_GetRoamingStatus(app_info_ptr->dual_sys)))
           {
               //SCI_TRACE_LOW:"PDP MANAGER MMIAPIPDP_Active,MMICONNECTION_SETTING_DATA_ROAMING_DISABLE"
               SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2853_112_2_18_2_44_54_84,(uint8*)"");
               Callback(app_info_ptr, link_id, MMIPDP_ACTIVE_CNF, MMIPDP_RESULT_NOT_PERMIT_ROAMING);
               return TRUE;
           }
       }
    }
#endif

#ifndef MMI_MULTI_SIM_SYS_SINGLE
#ifndef WIN32
    if(MMIPDP_IsDifferdualsysActivedPdpLinkExist(app_info_ptr->dual_sys))
    {
        if (!app_info_ptr->is_forceChange)
            {
            //SCI_TRACE_LOW:"PDP MANAGER MMIAPIPDP_Active,MMIPDP_RESULT_DIFFER_DUALSYS"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2864_112_2_18_2_44_54_85,(uint8*)"");
            Callback(app_info_ptr,link_id,MMIPDP_ACTIVE_CNF,MMIPDP_RESULT_DIFFER_DUALSYS);
            return TRUE;
            }
        else
            {
            MMIAPIPDP_ReleaseAll(PNULL);
        }
    }

#if 0  //BUG 966930 fix no service issue
    {
        uint32 i = 0;

        //SCI_TRACE_LOW:"PDP MANAGER MMIAPIPDP_Active, GprsForce Detach!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2872_112_2_18_2_44_54_86,(uint8*)"");

        if (MMIAPISET_GetMultiSysSettingNum(PNULL, 0) > 1)
        {
            for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
            {
                if (MMIAPIPHONE_GetSimExistedStatus(i) && !MMIPDP_PsIsPdpLinkExist(i))
                {
                    MNPHONE_GprsForceDetachEx(i, MN_PHONE_GPRS_DETACH);
                }
            }
        }
    }
#endif

#endif
#endif
    
    //read qos for 3G
    MNGPRS_ReadStorageExtReqQosEx(app_info_ptr->dual_sys, 
                                                                    app_info_ptr->storage, 
                                                                    app_info_ptr->ps_service_type, 
                                                                    &app_info_ptr->qos_info);
    
    //find if already in pending list
    node_handler = MMIPDP_ListFindNode(
                                g_mmipdp_manager_instance.pending_list,
                                (void*)app_info_ptr->app_handler,
                                FindNodeByApphandlerCb
                                );
    if(node_handler!=0)
    {
        //SCI_TRACE_LOW:"PDP MANAGER MMIAPIPDP_Active, the app already exist in pending list."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2896_112_2_18_2_44_54_87,(uint8*)"");
        return TRUE;
    }
    //find if app already actived
    link_id=FindLinkIdByApphandler(app_info_ptr->app_handler);
    if(0==link_id)
    {
        //Creat active timeout timer
        app_info_ptr->active_timer_id = MMIPDP_CreatTimer(GetPdpTimeOut(app_info_ptr->dual_sys));
        link_id=RequestLinkId(app_info_ptr);
    }
    //get a link, dispatch to fsm
    if(0!=link_id)
    {   
        fsm_ptr = g_mmipdp_manager_instance.link_arr[link_id-1].fsm_ptr;
        //creat the fsm
        if(0 == fsm_ptr)
        {
            fsm_ptr = CreatFsmInstance();
            if(0 == fsm_ptr)
            {
                return FALSE;
            }
            fsm_ptr->link_id = link_id;
            g_mmipdp_manager_instance.link_arr[link_id-1].fsm_ptr = fsm_ptr;
        }
        
        SCI_TRACE_LOW("MMIAPIPDP_Active handle_msg_callback= %x",app_info_ptr->handle_msg_callback);
        msg_ptr = CreatMsg(MMIPDP_ACTIVE_REQ,app_info_ptr);
        
        //Dispatch Msg
        if(!FsmDispatch(link_id,msg_ptr))
        {
            //SCI_TRACE_LOW:"PDP MANAGER MMIAPIPDP_Active,fsm busy"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2927_112_2_18_2_44_54_88,(uint8*)"");
            return FALSE;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"PDP MANAGER MMIAPIPDP_Active, no link resource, add to pending list"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2933_112_2_18_2_44_54_89,(uint8*)"");
        AddToList(&g_mmipdp_manager_instance.pending_list,app_info_ptr,MMIPDP_ACTIVE_CNF);
    }
	
    SCI_TRACE_LOW("MMIAPIPDP_Active EXIT");
    return TRUE;
}

/****************************************************************************/
//Description : Deactive pdp link req
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIAPIPDP_Deactive(uint32 app_handler)
{
    uint32              link_id = 0;
    MMIPDP_LIST_NODE_HANDLE node_handler = 0;
    MMIPDP_LIST_APP_INFO_T *node_data_ptr = 0;
    MMIPDP_FSM_MSG_T    *msg_ptr = 0;
#ifdef MMI_WIFI_SUPPORT  
    CFL_LIST_NODE_HANDLE   node_found_handler = 0;
    CFL_NODE_OBJECT        node_data;
    MMIPDP_WIFI_APPINFO_T  *pstAppRegInfo = PNULL;
    APP_MN_GPRS_DEACTIVATE_CNF_T    *sig_ptr = PNULL;
    uint16               sig_size = 0; 
#endif

    if(0==app_handler)
    {
        return FALSE;
    }
    
    //SCI_TRACE_LOW:"PDP MANAGER MMIAPIPDP_Deactive,app_handler:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2963_112_2_18_2_44_54_90,(uint8*)"d",app_handler);

/* BEGIN: Added by George.Liu 01526, 2010/9/27   PN:ms00203493 */
#ifdef MMI_WIFI_SUPPORT
    /*两种方案，现在使用不return的方案。*/
    node_found_handler = CFL_ListFindNode(stMmipdpMem.dwWifiRegAppList, (void *)app_handler, mmipdp_EqualWifiRegAppHandler);
    if (0 != node_found_handler)
    {
        node_data = CFL_ListGetNodeData(node_found_handler);
        pstAppRegInfo = (MMIPDP_WIFI_APPINFO_T *)node_data;
        if (PNULL == pstAppRegInfo)
        {
            //SCI_TRACE_LOW:"mmipdp error %s: pstAppRegInfo is PNULL!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2974_112_2_18_2_44_54_91,(uint8*)"s", __func__);
            //return FALSE;
        }
        else
        {
            sig_size = sizeof(APP_MN_GPRS_DEACTIVATE_CNF_T);
            SCI_CREATE_SIGNAL(sig_ptr,APP_MN_DEACTIVATE_PDP_CONTEXT_CNF,sig_size,P_APP);   /*lint !e64*/
            sig_ptr->result = MN_GPRS_ERR_SUCCESS;
            sig_ptr->pdp_num = 0;
            SCI_SEND_SIGNAL(sig_ptr,P_APP);
            //return TRUE;
        }
    }
#endif
/* END:   Added by George.Liu 01526, 2010/9/27   PN:ms00203493 */

    node_handler = MMIPDP_ListFindNode(
                                g_mmipdp_manager_instance.pending_list,
                                (void*)app_handler,
                                FindNodeByApphandlerCb
                                );
    if(node_handler!=0)
    {
        //SCI_TRACE_LOW:"PDP MANAGER MMIAPIPDP_Deactive, the app is in the pending list."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_2996_112_2_18_2_44_54_92,(uint8*)"");
        node_data_ptr = MMIPDP_ListGetNodeData(node_handler);
        if(node_data_ptr!=0)
        {
            Callback(&(node_data_ptr->active_info),link_id,MMIPDP_DEACTIVE_CNF,MMIPDP_RESULT_SUCC);
            //to check if the node is still exist after callback
            node_handler = MMIPDP_ListFindNode(
                g_mmipdp_manager_instance.pending_list,
                (void*)app_handler,
                FindNodeByApphandlerCb
                );
            if(node_handler!=0)
            {
                FreeMemAllocInListNode(node_handler);
                MMIPDP_ListDeleteNode(g_mmipdp_manager_instance.pending_list,node_handler);
            }
        }
        return TRUE;
    }
    link_id=FindLinkIdByApphandler(app_handler);
    if(0==link_id)
    {
        //SCI_TRACE_LOW:"PDP MANAGER MMIAPIPDP_Deactive, the app is not using any link resource!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_3018_112_2_18_2_44_54_93,(uint8*)"");
        return FALSE;
    }
    
    if(0 == g_mmipdp_manager_instance.link_arr[link_id-1].fsm_ptr)
    {
        return FALSE;
    }
    
    msg_ptr = CreatMsg(MMIPDP_DEACTIVE_REQ,(void*)app_handler);
    //Dispatch Msg
    if(!FsmDispatch(link_id,msg_ptr))
    {
        //SCI_TRACE_LOW:"PDP MANAGER MMIAPIPDP_Deactive,fsm busy"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_3031_112_2_18_2_44_54_94,(uint8*)"");
        return FALSE;
    }
    return TRUE;

}

/****************************************************************************/
//Description : To release all pdp
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC void MMIAPIPDP_ReleaseAll(MMIPDP_RELEASE_ALL_CALLBACK release_all_cb)
{
    uint32                  link_id = 0;
    MMIPDP_LIST_NODE_HANDLE node_handler = 0;
    MMIPDP_LIST_APP_INFO_T  *node_data_ptr = 0;
    MMIPDP_LIST_APP_INFO_T  temp_node_data = {0};
    
    for(link_id=1; link_id<=MMIPDP_MAX_LINK_NUM; link_id++)
    {
        if(g_mmipdp_manager_instance.link_arr[link_id-1].state==MMIPDP_LINK_STATE_ACTIVED||
            g_mmipdp_manager_instance.link_arr[link_id-1].state==MMIPDP_LINK_STATE_ACTIVING)
        {
            FsmStateTrans(
                g_mmipdp_manager_instance.link_arr[link_id-1].fsm_ptr,
                MMIPDP_LINK_STATE_DEACTIVING
                );
            MMIPDP_PsDeactivePdp(link_id);
            g_mmipdp_manager_instance.is_release_all = TRUE;
            g_mmipdp_manager_instance.release_all_cb = release_all_cb;
        }
    }
    //callback all app in pending list
    node_handler = MMIPDP_ListGetHead(g_mmipdp_manager_instance.pending_list);
    while(node_handler!=0)
    {
        node_data_ptr = MMIPDP_ListGetNodeData(node_handler);
        if(node_data_ptr!=0)
        {
            //backup active info for callback
            SCI_MEMCPY(&temp_node_data, node_data_ptr, sizeof(MMIPDP_LIST_APP_INFO_T));
            temp_node_data.active_info.apn_ptr = 0;
            temp_node_data.active_info.psw_ptr = 0;
            temp_node_data.active_info.user_name_ptr = 0;
            
            FreeMemAllocInListNode(node_handler);
            MMIPDP_ListDeleteNode(g_mmipdp_manager_instance.pending_list,node_handler);
            //SCI_TRACE_LOW:"PDP MANAGER MMIAPIPDP_ReleaseAll is called, all app are deactived or quit activing"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_3078_112_2_18_2_44_54_95,(uint8*)"");
            Callback(&(temp_node_data.active_info),0,temp_node_data.msg_id_to_callback,MMIPDP_RESULT_FAIL);
        }
        node_handler = MMIPDP_ListGetHead(g_mmipdp_manager_instance.pending_list);
    }
}

/****************************************************************************/
//Description : To attach GPRS
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIAPIPDP_AttachGprs(MN_DUAL_SYS_E dual_sys)
{
    return MMIPDP_PsGprsAttach(dual_sys, MN_PHONE_GPRS_ATTACH);
}

/****************************************************************************/
//Description : To dettach GPRS
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIAPIPDP_DettachGprs(MN_DUAL_SYS_E dual_sys)
{
    MMIPDP_SetGPRSDetach(dual_sys);
    
    return TRUE;
}

/****************************************************************************/
//Description : Get Link State
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC MMIPDP_LINK_STATE_E MMIAPIPDP_GetLinkState(uint32 app_handler)
{
    MMIPDP_LINK_STATE_E link_state = MMIPDP_LINK_STATE_MAX;
    uint32              link_id = 0;

#ifdef MMI_WIFI_SUPPORT
    link_state = mmipdp_GetWifiLinkState(app_handler);
    if (link_state != MMIPDP_LINK_STATE_MAX)
    {
        //SCI_TRACE_LOW:"mmipdp info %s: %d wifi Link state is %d!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_3118_112_2_18_2_44_54_96,(uint8*)"sdd", __func__, app_handler, link_state);
        return link_state;
    }
#endif    
    if(0!=app_handler)
    {
        link_id=FindLinkIdByApphandler(app_handler);
        if(0!=link_id)
        {
            link_state = g_mmipdp_manager_instance.link_arr[link_id-1].state;
        }
    }
    return link_state;
}

/****************************************************************************/
//Description : Get flowrate infomation
//Global resource dependence : 
//Author: juan.zhang
//Note: 注意，在多pdp的情况下，获取的上次连接的流量是错误的，原因是下层的流量
//      计算放在socket那边计算的，无法区分各个链路的流量。需要下层修改计算方法
/****************************************************************************/
PUBLIC BOOLEAN MMIAPIPDP_GetFlowrate(MMIPDP_FLOWRATE_T *flowrate_ptr, MN_DUAL_SYS_E dual_sys)
{
#ifdef MMI_GPRS_SUPPORT
    MMIPDP_FLOWRATE_T   flowrate_to_read_arr[MMI_DUAL_SYS_MAX] = {0};
    
    if(0==flowrate_ptr)
    {
        return FALSE;
    }
    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        dual_sys = MN_DUAL_SYS_1;
    }

    if(MN_RETURN_SUCCESS != MMI_ReadNVItem(MMINV_PDP_GPRS_FLOW_RATE,flowrate_to_read_arr))
    {
        //SCI_TRACE_LOW:"MMIAPIPDP_GetFlowrate nv read error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_3149_112_2_18_2_44_54_97,(uint8*)"");
        return FALSE;
    }
    SCI_MEMSET(flowrate_ptr,0,sizeof(MMIPDP_FLOWRATE_T));
    SCI_MEMCPY(flowrate_ptr,&flowrate_to_read_arr[dual_sys],sizeof(MMIPDP_FLOWRATE_T));
    return TRUE;
#else
    return FALSE;
#endif
}

/****************************************************************************/
//Description : Clear flowrate infomation
//Global resource dependence : 
//Author: juan.zhang
//Note: 注意，在多pdp的情况下，获取的上次连接的流量是错误的，原因是下层的流量
//      计算放在socket那边计算的，无法区分各个链路的流量。需要下层修改计算方法
/****************************************************************************/
PUBLIC void MMIAPIPDP_ClearFlowrate(MN_DUAL_SYS_E dual_sys)
{
#ifdef MMI_GPRS_SUPPORT
    MMIPDP_FLOWRATE_T   flowrate_to_write_arr[MMI_DUAL_SYS_MAX] = {0};
    MN_RETURN_RESULT_E  read_result = MN_RETURN_SUCCESS;
    
    if(dual_sys>=MMI_DUAL_SYS_MAX)
    {
        return;
    }
    MMINV_READ(MMINV_PDP_GPRS_FLOW_RATE,flowrate_to_write_arr,read_result);
    SCI_MEMSET(&flowrate_to_write_arr[dual_sys], 0, sizeof(MMIPDP_FLOWRATE_T));
    MMINV_WRITE(MMINV_PDP_GPRS_FLOW_RATE,flowrate_to_write_arr);
#endif
}

/****************************************************************************/
//Description : AddToSubscriber
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL void AddToSubscriber(MMIPDP_SUBSCRIBER_T *subscriber_info_ptr)
{
    if(0 == subscriber_info_ptr)
    {
        return;
    }
    if(0 == g_subscriber_list)
    {
        g_subscriber_list = MMIPDP_ListCreat(sizeof(MMIPDP_SUBSCRIBER_T));
    }
    MMIPDP_ListInsertNode(g_subscriber_list,0,subscriber_info_ptr,
        sizeof(MMIPDP_SUBSCRIBER_T)+sizeof(MMIPDP_NOTIFY_EVENT_E)*(subscriber_info_ptr->event_num-1));
}

/****************************************************************************/
//Description : DeleteFromSubscriber
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL void DeleteFromSubscriber(uint16 module_id)
{
    MMIPDP_LIST_NODE_HANDLE node_found = 0;
    
    node_found = FindSubscriberByModuleId(module_id);
    if(0!=node_found)
    {
        MMIPDP_ListDeleteNode(g_subscriber_list,node_found);
    }
}

/****************************************************************************/
//Description : FindSubscriberByModuleIdCb
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL BOOLEAN FindSubscriberByModuleIdCb(MMIPDP_NODE_OBJECT data, uint32 module_id)
{
    if(0==data)
    {
        return FALSE;
    }
    return (((MMIPDP_SUBSCRIBER_T*)data)->module_id == module_id);
}

/****************************************************************************/
//Description : FindSubscriberByModuleId
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL MMIPDP_LIST_NODE_HANDLE FindSubscriberByModuleId(uint16 module_id)
{
    MMIPDP_LIST_NODE_HANDLE node_found = 0;
    
    node_found = MMIPDP_ListFindNode(g_subscriber_list,module_id,FindSubscriberByModuleIdCb);/*lint !e64*/
    return node_found;
}

/****************************************************************************/
//Description : CheckIfNotifySubscriber
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL BOOLEAN CheckIfNotifySubscriber(MMIPDP_SUBSCRIBER_T *subscriber_info_ptr, MMIPDP_NOTIFY_EVENT_E event)
{
    uint32 i=0;
    
    if(0==subscriber_info_ptr)
    {
        return FALSE;
    }
    for(i=0; i<subscriber_info_ptr->event_num; i++)
    {
        if((uint32)((&subscriber_info_ptr->event_arr_ptr)[i]) == event)
        {
            return TRUE;
        }
    }
    return FALSE;
}

/****************************************************************************/
//Description : NotifySubscriber
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
LOCAL void NotifySubscriber(MN_DUAL_SYS_E dual_sys,  MMIPDP_NOTIFY_EVENT_E event)
{
    MMIPDP_LIST_NODE_HANDLE node_handler = MMIPDP_ListGetHead(g_subscriber_list);
    MMIPDP_SUBSCRIBER_T     *subscriber_info_ptr = 0;
    
    while(node_handler!=0)
    {
        subscriber_info_ptr = MMIPDP_ListGetNodeData(node_handler);
        if(CheckIfNotifySubscriber(subscriber_info_ptr, event))
        {
            if(subscriber_info_ptr->notify_func)
            {
                (subscriber_info_ptr->notify_func)(dual_sys, event);
            }
        }
        node_handler = MMIPDP_ListGetNextNode(node_handler);
    }

    if (MMIPDP_NOTIFY_EVENT_DETTACH == event)
    {
        MMIAPISMS_CheckTimerMSG();
    }
}
/****************************************************************************/
//Description : To Subscribe the notify event
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC void MMIAPIPDP_SubscribeEvent(
                                     uint32                 module_id,
                                     MMIPDP_NOTIFY_FUNC     notify_func,
                                     uint32                 notify_event_num,
                                     MMIPDP_NOTIFY_EVENT_E  *notify_event_arr_ptr
                                     )
{
    MMIPDP_SUBSCRIBER_T     *subscriber_info_ptr = 0;
    
    if(notify_func!=0 && notify_event_num>0 && notify_event_arr_ptr!=0)
    {
        subscriber_info_ptr = SCI_ALLOCA(sizeof(MMIPDP_SUBSCRIBER_T)+
            sizeof(MMIPDP_NOTIFY_EVENT_E)*(notify_event_num-1));
        if(subscriber_info_ptr!=0)
        {
            SCI_MEMSET(subscriber_info_ptr,0,
                (sizeof(MMIPDP_SUBSCRIBER_T)+sizeof(MMIPDP_NOTIFY_EVENT_E)*(notify_event_num-1)));
            subscriber_info_ptr->event_num = notify_event_num;
            subscriber_info_ptr->module_id = module_id;
            subscriber_info_ptr->notify_func = notify_func;
            SCI_MEMCPY(&subscriber_info_ptr->event_arr_ptr,
                notify_event_arr_ptr,sizeof(MMIPDP_NOTIFY_EVENT_E)*notify_event_num);/*lint !e419*/
            AddToSubscriber(subscriber_info_ptr);
        }
        else
        {
            //SCI_TRACE_LOW:"MMIAPIPDP_SubscribeEvent, ALLOC FAIL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_3318_112_2_18_2_44_55_98,(uint8*)"");
        }
    }
}
/****************************************************************************/
//Description : To Unsubscribe the notify event
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC void MMIAPIPDP_UnsubscribeEvent(
                                       uint32                   module_id
                                       )
{
    DeleteFromSubscriber(module_id);
}

/****************************************************************************/
//Description : To reset pdp
//Global resource dependence : 
//Author: juan.zhang
//Note:	此函数只能在底层PDP链路释放，但又不是经过MMIPDP释放时调用，以此通知MMIPDP
//改变状态。比如手动将网络从G网切换到TD，底层自行将网络接口释放了，而MMI就需要调用
//此函数复位MMIPDP的状态。
/****************************************************************************/
PUBLIC void MMIAPIPDP_ResetPdp(MN_DUAL_SYS_E dual_sys)
{
    uint32					link_id = 0;
    MMIPDP_LIST_NODE_HANDLE node_handler = 0;
    MMIPDP_LIST_APP_INFO_T	*node_data_ptr = 0;
    MMIPDP_LIST_APP_INFO_T	temp_node_data = {0};
    MMIPDP_FSM_MSG_T* fsm_msg_ptr = 0;
    BOOLEAN is_need_notify = FALSE;
    
    //SCI_TRACE_LOW:"PDP MANAGER, reset pdp dual_sys=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_3348_112_2_18_2_44_55_99,(uint8*)"d", dual_sys);
    //callback all app in using
    for(link_id=1; link_id<=MMIPDP_MAX_LINK_NUM; link_id++)
    {
        if((g_mmipdp_manager_instance.link_arr[link_id-1].state==MMIPDP_LINK_STATE_ACTIVED
            || g_mmipdp_manager_instance.link_arr[link_id-1].state==MMIPDP_LINK_STATE_ACTIVING)
            && g_mmipdp_manager_instance.link_arr[link_id-1].dual_sys == dual_sys)
        {
            FsmStateTrans(
                g_mmipdp_manager_instance.link_arr[link_id-1].fsm_ptr,
                MMIPDP_LINK_STATE_DEACTIVING
                );

            g_mmipdp_manager_instance.is_release_all = TRUE;
            fsm_msg_ptr = CreatMsg(MMIPDP_PS_DEACTIVE_CNF,0);
            FsmDispatch(link_id,fsm_msg_ptr);

            g_mmipdp_manager_instance.is_release_all = FALSE;

            is_need_notify = TRUE;
        }
    }

    //callback all app in pending list
    node_handler = MMIPDP_ListGetHead(g_mmipdp_manager_instance.pending_list);
    while(node_handler!=0)
    {
        node_data_ptr = MMIPDP_ListGetNodeData(node_handler);
        if (node_data_ptr != 0)
        {
            if (node_data_ptr->active_info.dual_sys == dual_sys)
            {
                //backup active info for callback
                SCI_MEMCPY(&temp_node_data, node_data_ptr, sizeof(MMIPDP_LIST_APP_INFO_T));
                temp_node_data.active_info.apn_ptr = 0;
                temp_node_data.active_info.psw_ptr = 0;
                temp_node_data.active_info.user_name_ptr = 0;
                
                FreeMemAllocInListNode(node_handler);
                MMIPDP_ListDeleteNode(g_mmipdp_manager_instance.pending_list,node_handler);
                //SCI_TRACE_LOW:"PDP MANAGER MMIAPIPDP_ResetPdp is called, all app are deactived or quit activing"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_3388_112_2_18_2_44_55_100,(uint8*)"");
                Callback(&(temp_node_data.active_info),0,temp_node_data.msg_id_to_callback,MMIPDP_RESULT_FAIL);
            }
            else
            {
                //SCI_TRACE_LOW:"PDP MANAGER MMIAPIPDP_ResetPdp dual_sys=%d, %d, break!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_3393_112_2_18_2_44_55_101,(uint8*)"dd", node_data_ptr->active_info.dual_sys, dual_sys);

                break;
            }
        }
        node_handler = MMIPDP_ListGetHead(g_mmipdp_manager_instance.pending_list);
    }

    if (is_need_notify)
    {
        NotifySubscriber(dual_sys, MMIPDP_NOTIFY_EVENT_DEACTIVED);
        NotifySubscriber(dual_sys, MMIPDP_NOTIFY_EVENT_DETTACH);
    }
}

/****************************************************************************/
//Description : To Unsubscribe the notify event
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC void MMIAPIPDP_HandleIdleEvent(void) 
{
    MMIAPIPDP_SubscribeEvent(MMI_MODULE_COMMON, 
        HandlePdpEvent,
        MMIPDP_NOTIFY_EVENT_MAX,
        (MMIPDP_NOTIFY_EVENT_E *)s_pdp_event);
}

/*****************************************************************************/
// Description : 接受pdp模块传来的相关event
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-3
/*****************************************************************************/
LOCAL void HandlePdpEvent(MN_DUAL_SYS_E dual_sys, MMIPDP_NOTIFY_EVENT_E event)
{
    //SCI_TRACE_LOW:"HandlePdpEvent : %d, dual_sys:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_3426_112_2_18_2_44_55_102,(uint8*)"dd", event, dual_sys);
    MMIAPIIDLE_SetGprsInfo(dual_sys, event, FALSE);
//TODO:PDP MMI上层适配MMIAPICOM_StatusAreaSetGprsInfo
#if 0
    MMIAPICOM_StatusAreaSetGprsInfo(dual_sys, event, FALSE);
    GUIWIN_UpdateStb();
#endif
}

/*****************************************************************************/
// Description : 当网络类型发生变化的时候，更新数据业务图标显示
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-3
/*****************************************************************************/
PUBLIC void MMIAPIPDP_SetGprsInfoWhenRatSwitch( void )
{
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    for(dual_sys = MN_DUAL_SYS_1; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
    {
        MMIAPIIDLE_SetGprsInfo(dual_sys, s_last_pdp_event[dual_sys], FALSE);
    }
//TODO:PDP MMI上层适配MMIAPICOM_StatusAreaSetGprsInfo
#if 0
    for(dual_sys = MN_DUAL_SYS_1; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
    {
        MMIAPICOM_StatusAreaSetGprsInfo(dual_sys, s_last_pdp_event[dual_sys], FALSE);
    }
#endif
}

/*****************************************************************************/
// Description : set last pdp event
// Global resource dependence : 
// Author: 
// Note:
/*****************************************************************************/
PUBLIC void MMIAPIPDP_SetPdpLastEvent(MN_DUAL_SYS_E dual_sys, MMIPDP_NOTIFY_EVENT_E last_pdp_event) /*@CR255469 modify 2011.08.23*/
{
    if (dual_sys < MMI_DUAL_SYS_MAX)
    {
        s_last_pdp_event[dual_sys] = last_pdp_event; /*@CR255469 modify 2011.08.23*/
    }
}

/*****************************************************************************/
// Description : Get last pdp event
// Global resource dependence : 
// Author: 
// Note:
/*****************************************************************************/
PUBLIC MMIPDP_NOTIFY_EVENT_E MMIAPIPDP_GetPdpLastEvent(MN_DUAL_SYS_E dual_sys) /*@CR255469 modify 2011.08.23*/
{
    if (dual_sys < MMI_DUAL_SYS_MAX)
    {
        SCI_TRACE_LOW("[MMIAPIPDP_GetPdpLastEvent] s_last_pdp_event[%d]=%d", dual_sys, s_last_pdp_event[dual_sys]);
        return s_last_pdp_event[dual_sys];  /*@CR255469 modify 2011.08.23*/
    }
    
    return MMIPDP_NOTIFY_EVENT_DETTACH;
}

/*****************************************************************************/
// Description : set gprs detach
// Global resource dependence : 
// Author: 
// Note:
/*****************************************************************************/
LOCAL void MMIPDP_SetGPRSDetach(MN_DUAL_SYS_E dual_sys)
{

#if 0//8910 do not need detach GPRS
#ifndef WIN32
#ifdef MMI_DUALMODE_ENABLE
#ifdef ENGTD_SUPPORT
    if (!MMIAPIENG_GetFtaFlag())
#endif
#endif
    {
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        if (MMIAPISET_GetMultiSysSettingNum(PNULL, 0) > 1)
        {
#ifndef WIN32 
            MMIAPIPHONE_SetGPRSDetachEx(dual_sys);
#endif
        }   
        else
        {
            if (!MMIAPICONNECTION_isGPRSNeedAutoAttach())
            {
                MMIAPIPHONE_SetGPRSDetachEx(dual_sys);
            }
        }
        
        //SCI_TRACE_LOW:"MMIPDP_SetGPRSStatus out"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_3498_112_2_18_2_44_55_103,(uint8*)"");
#else
        if (!MMIAPICONNECTION_isGPRSNeedAutoAttach())
        {
            MMIAPIPHONE_SetGPRSDetachEx(dual_sys);
        }
        
        //SCI_TRACE_LOW:"MMIPDP_SetGPRSStatus out"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_3505_112_2_18_2_44_55_104,(uint8*)"");
#endif
    }
#endif
#endif
}

/****************************************************************************/
//Description : To Check if any Actived Pdp Link Exist
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIAPIPDP_PsIsActivedPdpLinkExist(void)
{
    BOOLEAN is_link_exist = FALSE;
    uint32 i = 0;
    
    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        is_link_exist = MMIPDP_PsIsActivedPdpLinkExist((MN_DUAL_SYS_E)i);
        
        if (is_link_exist)
        {
            return TRUE;
        }
    }
    
    //SCI_TRACE_LOW:"MMIAPIPDP_PsIsActivedPdpLinkExist Not Pdp Link Now!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_3530_112_2_18_2_44_55_105,(uint8*)"");
    
    return FALSE;
}

/*+ @CR255469 2011.08.23*/
/****************************************************************************/
//Description : To Check if Actived Pdp Link Exist with the sim
//Global resource dependence : 
//Author: fen.xie
//Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIAPIPDP_PsIsActivedPdpLinkExistEx(MN_DUAL_SYS_E dual_sys)
{
    return MMIPDP_PsIsActivedPdpLinkExist(dual_sys);
}
/*- @CR255469 2011.08.23*/

/****************************************************************************/
//Description : To Check if any Pdp Link Exist
//Global resource dependence : 
//Author: juan.zhang
//Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIAPIPDP_PsIsPdpLinkExist(void)
{
    BOOLEAN is_link_exist = FALSE;
    uint32 i = 0;
    
    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        is_link_exist = MMIPDP_PsIsPdpLinkExist((MN_DUAL_SYS_E)i);
        
        if (is_link_exist)
        {
            return TRUE;
        }
    }
    
    //SCI_TRACE_LOW:"MMIAPIPDP_PsIsPdpLinkExist Not Pdp Link Now!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_3564_112_2_18_2_44_55_106,(uint8*)"");
    
    return FALSE;
}

/****************************************************************************/
//Description : To Check if any Pdp Link Exist with the sim
//Global resource dependence : 
//Author: fen.xie
//Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIAPIPDP_PsIsPdpLinkExistEx(MN_DUAL_SYS_E dual_sys)
{
    return MMIPDP_PsIsPdpLinkExist(dual_sys);
}

/****************************************************************************/
//Description : Get PS Service SIM(Not In PDP return MMI_DUAL_SYS_MAX)
//Global resource dependence : 
//Author: wancan.you
//Note: 
/****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPIPDP_GetPdpLinkSIM(void)
{
    BOOLEAN is_link_exist = FALSE;
    uint32 i = 0;
    
    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        is_link_exist = MMIPDP_PsIsPdpLinkExist((MN_DUAL_SYS_E)i);
        
        if (is_link_exist)
        {
            return (MN_DUAL_SYS_E)i;
        }
    }
    
    //SCI_TRACE_LOW:"MMIAPIPDP_GetPdpLinkSIM Not Pdp Link Now!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_3597_112_2_18_2_44_55_107,(uint8*)"");
    
    return MMI_DUAL_SYS_MAX;
}

/****************************************************************************/
//Description : To Check if deactive pdp id is in pdp manager link list
//Global resource dependence : 
//Author: kun.yu
//Note: 
/****************************************************************************/
LOCAL BOOLEAN MMIPDP_IsPdpIdInLinkArray(uint32 pdp_id, MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN ret = FALSE;
    uint32 link_id = 1;
    
    while(link_id<=MMIPDP_MAX_LINK_NUM)
    {
        if(g_mmipdp_manager_instance.link_arr[link_id-1].pdp_id==pdp_id &&
            g_mmipdp_manager_instance.link_arr[link_id-1].dual_sys==dual_sys)
        {
            ret = TRUE;
            break;
        }
        else
        {
            link_id++;
        }
    }
    return ret;
}

#ifndef MMI_MULTI_SIM_SYS_SINGLE
/****************************************************************************/
//Description : MMIPDP_IsDifferdualsysActivedPdpLinkExist
//Global resource dependence : 
//Author: kun.yu
//Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIPDP_IsDifferdualsysActivedPdpLinkExist(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN ret = FALSE;
    uint32 link_id = 1;
    
    while(link_id<=MMIPDP_MAX_LINK_NUM)
    {
        if(g_mmipdp_manager_instance.link_arr[link_id-1].state != MMIPDP_LINK_STATE_DEACTIVED &&
            g_mmipdp_manager_instance.link_arr[link_id-1].dual_sys!=dual_sys)
        {
            ret = TRUE;
            break;
        }
        else
        {
            link_id++;
        }
    }
    
    SCI_TRACE_LOW("MMIPDP_IsDifferdualsysActivedPdpLinkExist dual_sys=%d, link_id=%d, ret=%d", dual_sys, link_id, ret);
    return ret;
}
#endif

#ifdef FDN_SUPPORT
/*****************************************************************************/
//  Description : is forbit active pdp
//  Global resource dependence : 
//  Author:dave.ruan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPDP_IsFDNPermitActivePdp(MN_DUAL_SYS_E dual_sys)
{ 

    BOOLEAN is_allow_pdp = FALSE;
    MMIFDN_CONTEXT_T *fdn_context_ptr = PNULL;
    MMI_PARTY_NUMBER_T  party_num = {MN_NUM_TYPE_UNKNOW, 0, 0};
    MN_CALLED_NUMBER_T called_num= {0};
    uint16 tem_nunber_len = 0;
    char* gprs_fdn_gateway_string_ptr[] =
    {
        "*98#",           //CMNET
        "*99#",           //CMNET
        "*99****1#",  //CMNET
        "*99****2#",  //CMWAP
        "*99****3#",  //CMWAP
    };
    uint16  temp_fdn_string_num = 0;
    uint16  i =0;
    
    fdn_context_ptr = MMIAPIFDN_GetFDNContext(dual_sys);
    if (PNULL == fdn_context_ptr || !fdn_context_ptr->is_fdn_enable)//fdn没有开启
    {
        /*fdn is not enable, permit to active pdp*/
        return TRUE;
    }
    
    /*get total number of string*/
    temp_fdn_string_num = sizeof(gprs_fdn_gateway_string_ptr)/sizeof(char*);
    
    for(i =0; i < temp_fdn_string_num; i++)
    {  
        if(PNULL == gprs_fdn_gateway_string_ptr[i])
        {
            //SCI_TRACE_LOW:"MMIAPIPDP_IsPermitActivePdp gprs_fdn_gateway_string_ptr i =%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPDP_API_3693_112_2_18_2_44_55_108,(uint8*)"d",i);
            break;
        }
        
        tem_nunber_len = SCI_STRLEN(gprs_fdn_gateway_string_ptr[i]);
        if( MMIAPICOM_GenPartyNumber((uint8*)gprs_fdn_gateway_string_ptr[i], tem_nunber_len, &party_num)
            &&MMIAPICOM_IsPhoneNumber((uint8*)gprs_fdn_gateway_string_ptr[i], tem_nunber_len) 
            )
        {
            called_num.number_plan = MN_NUM_PLAN_ISDN_TELE;
            called_num.number_type = party_num.number_type;
            called_num.num_len = party_num.num_len;
            SCI_MEMCPY(called_num.party_num, party_num.bcd_num, party_num.num_len);
            if (PHONEBOOK_CheckFdnEx(dual_sys, &called_num))
            {
                is_allow_pdp = TRUE;
                break;
            }
        }
        
    }
    return is_allow_pdp;
}
#endif /*FDN_SUPPORT*/

/****************************************************************************/
//Description : Get PDP Time Out
//Global resource dependence : 
//Author: dave.ruan
//Note:	
/****************************************************************************/
LOCAL uint32 GetPdpTimeOut(MN_DUAL_SYS_E dual_sys)
{
    uint32 pdp_timeout = NULL;

#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
    if (dual_sys < MMI_DUAL_SYS_MAX)
    {
        pdp_timeout = (MMIPLMN_PDP_TIMEOUT_T)MMIAPIPLMN_GetLocalPDPTimeout(dual_sys);
    }

    if (NULL == pdp_timeout)
#endif
    {
        pdp_timeout = MMIPDP_ACTIVE_TIMOUT;
    }

    return pdp_timeout;
}


/*****************************************************************************/
//  Description: 注入函数
//  Parameter: [In] p_func
//外部模块向mmi pdp注入的函数
//             [Out] none
//             [Return] 错误码
//  Author: shuting.ma
//  Note: rf模块可调用此接口注入函数
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPDP_Inject_Function(MMIPDP_INJECTION_FUNC p_func)
{
    BOOLEAN   ret = TRUE;

    SCI_TRACE_LOW("[MMIAPIPDP_Inject_Function] Enter");
    if(PNULL == p_func)
    {
        SCI_TRACE_LOW("[MMIAPIPDP_Inject_Function] p_func is pnull!");
        return FALSE;
    }
    if(PNULL != s_p_injiection_func)
    {
        SCI_TRACE_LOW("[MMIAPIPDP_Inject_Function] have injected func!");
        return FALSE;
    }

    s_p_injiection_func = p_func;
    SCI_TRACE_LOW("[MMIAPIPDP_Inject_Function] injection_func : %0x", p_func);
    SCI_TRACE_LOW("[MMIAPIPDP_Inject_Function] Exit, ret : %d", ret);
    return ret;
}

/*****************************************************************************/
//  Description:取消注入函数
//  Parameter: [In] none
//             [Out] none
//             [Return] 错误码
//  Author: shuting.ma
//  Note: rf模块可调用此接口取消注入函数
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPDP_Uninject_Function(void)
{
    BOOLEAN  ret = TRUE;

    SCI_TRACE_LOW("[MMIAPIPDP_Uninject_Function] Enter");
    if(PNULL == s_p_injiection_func)
    {
        SCI_TRACE_LOW("[MMIAPIPDP_Uninject_Function] uninject function is PNULL!");
        return FALSE;
    }
    s_p_injiection_func = PNULL;
    SCI_TRACE_LOW("[MMIAPIPDP_Uninject_Function] uninject function success");
    return ret;
}

/*****************************************************************************/
//  Description:获取net id
//  Parameter: [In] none
//             [Out] none
//             [Return] net id
//  Author: shuting.ma
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIPDP_GetPublicPdpLinkNetid(void)
{
    SCI_TRACE_LOW("[MMIAPIPDP_GetPublicPdpLinkNetid] net_id:%d", s_public_link_net_id);
    return s_public_link_net_id;
}

/*****************************************************************************/
//  Description :处理公共PDP链路的callback
// Parameter: [In] p_msg : pdp msg info
//            [Out] none
//            [Return] none
//  Author:shuting.ma
//  Note:
/*****************************************************************************/
LOCAL void PublicPdpLinkCallback(MMIPDP_CNF_INFO_T *p_msg)
{
    if(PNULL == p_msg)
    {
        SCI_TRACE_LOW("[PublicPdpLinkCallback] msg_ptr is PNULL, return\n");
        return;
    }

    s_public_link_net_id = p_msg->nsapi;
    SCI_TRACE_LOW("[PublicPdpLinkCallback] msg_id:%d,, net_id:%d\n", p_msg->msg_id, p_msg->nsapi);
    switch (p_msg->msg_id)
    {
        case MMIPDP_ACTIVE_CNF:
        {
            if (MMIPDP_RESULT_SUCC == p_msg->result)
            {
                s_public_link_is_active = TRUE;
                SCI_TRACE_LOW("[PublicPdpLinkCallback] active pdp succeed, s_is_active = %d", s_public_link_is_active);
            }
            else
            {
                s_public_link_is_active = FALSE;
                SCI_TRACE_LOW("[PublicPdpLinkCallback] active pdp failed, s_is_active = %d", s_public_link_is_active);
            }
        }
        break;

        case MMIPDP_DEACTIVE_CNF:
        case MMIPDP_DEACTIVE_IND:
        {
            s_public_link_is_active = FALSE;
            SCI_TRACE_LOW("[PublicPdpLinkCallback] deactive pdp result: %d, s_is_active = %d", p_msg->result, s_public_link_is_active);
        }
        break;

        default:
            SCI_TRACE_LOW("[PublicPdpLinkCallback] msg_id:%d Need Not Process\n",p_msg->msg_id);
        break;
    }
}


/*****************************************************************************/
//  Description:开机默认激活一路长连接pdp
//  Parameter: [In] sim id
//             [Out] none
//             [Return]MMIPDP_RESULT_E
//  Author: shuting.ma
//  Note:
/*****************************************************************************/
PUBLIC MMIPDP_RESULT_E MMIAPIPDP_PublicPDPLinkActive(MN_DUAL_SYS_E sim_id)
{
    MMIPDP_RESULT_E                     ret = MMIPDP_RESULT_FAIL;
    MMIPDP_ACTIVE_INFO_T                app_info = {0};
    ual_tele_data_apn_info_t            internet_apn_info = {0};
    ual_tele_radio_result_e             radio_ret = UAL_TELE_RADIO_RES_FAIL;

    SCI_TRACE_LOW("[MMIAPIPDP_PublicPDPLinkActive] enter");

    if(sim_id > MMI_DUAL_SYS_MAX)
    {
        SCI_TRACE_LOW("[MMIAPIPDP_PublicPDPLinkActive] sim_id error: %d\n", sim_id);
        return ret;
    }

    if(TRUE == s_public_link_is_active)
    {
        SCI_TRACE_LOW("[MMIAPIPDP_PublicPDPLinkActive] link has been activated, no need to activate again.");
        return ret;
    }

    //get apn information
    radio_ret = ual_tele_radio_get_internet_apn_info((ual_tele_sim_id_e)sim_id, &internet_apn_info);
    if (UAL_TELE_RADIO_RES_SUCCESS == radio_ret)
    {
        app_info.app_handler = MMI_MODULE_PDP;
        app_info.apn_ptr = (char*)internet_apn_info.apn;
        app_info.user_name_ptr = (char*)internet_apn_info.username;
        app_info.psw_ptr = (char*)internet_apn_info.password;
        app_info.auth_type = internet_apn_info.auth_type;
        app_info.dual_sys = sim_id;
        app_info.priority = MMIPDP_DEFAULT_ACTIVE_PRO;
        app_info.ps_service_rat = MN_UNSPECIFIED;
        app_info.handle_msg_callback = PublicPdpLinkCallback;
        app_info.ps_service_type = BROWSER_E;
        app_info.storage = MN_GPRS_STORAGE_ALL;
#ifdef IPVERSION_SUPPORT_V4_V6
        app_info.ip_type = internet_apn_info.ip_type;
#endif
    }
    else
    {
        SCI_TRACE_LOW("[MMIAPIPDP_PublicPDPLinkActive] get apn information fail");
        return ret;
    }

    SCI_TRACE_LOW("[MMIAPIPDP_PublicPDPLinkActive] apn info:%s\n", app_info.apn_ptr);
    if(TRUE == MMIAPIPDP_Active(&app_info))
    {
        ret = MMIPDP_RESULT_SUCC;
    }
    else
    {
        ret = MMIPDP_RESULT_FAIL;
    }
    SCI_TRACE_LOW("[MMIAPIPDP_PublicPDPLinkActive] ret = %d", ret);
    return ret;
}

#endif
