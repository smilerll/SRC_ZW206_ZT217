/*****************************************************************************
** File Name:       app_tcp_if.h         
** Author:          hyman.wu           
** Date:            2003/04/15                                               
** Copyright:       2003 Spreadtrum, Incorporated. All Rights Reserved.      
** Description:     This file defines tcpip external interfaces for app use   
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE             NAME                DESCRIPTION
** 2003/04/15       Hyman.Wu            Create
** 2009/07/22       Yifeng.Wang         Amend
** 2009/12/29       Yifeng.Wang         Move app-tcpip interfaces except socket
******************************************************************************/

#ifndef APP_TCP_IF_H
#define APP_TCP_IF_H

/*---------------------------------------------------------------------------*
 *                          Include Files                                    *
 *---------------------------------------------------------------------------*/
#include "tcpip_types.h"
#include "socket_types.h"
#include "socket_api.h"


/*---------------------------------------------------------------------------*
 *                          MACRO DEFINITION                                 *
 *---------------------------------------------------------------------------*/
// the default net interface ID for GPRS use
#define TCPIP_DEFAULT_NETID_FOR_GPRS    1
#define TCPIP_PING_HANDLE_NULL          0   /* NULL ping handle */
#define TCPIP_DHCPS_HOST_NAME_LEN_MAX   32


/*---------------------------------------------------------------------------*
 *                          TYPE DEFINITION                                  *
 *---------------------------------------------------------------------------*/
// PING result type
typedef enum
{
    PINGRES_SUCCESS         = 0,    /* ping OK, received ping echo reply */
    PINGRES_DNS_TIMEOUT     = 1,    /* ping fail, DNS timeout */
    PINGRES_DNS_ERROR       = 2,    /* ping fail, DNS error */
    PINGRES_ICMP_TIMEOUT    = 3,    /* ping fail, icmp timeout */
    PINGRES_ICMP_ERROR      = 4,     /* ping fail, icmp error */
    PINGRES_TIMING_END      = 5    /* ping timing end */
}TCPIP_PING_RESULT_E;

// PING result message
typedef struct ping_result_ind_sig
{
    _SIGNAL_VARS
    uint32                  delay;
    TCPIP_PING_HANDLE       handle;
    TCPIP_PING_RESULT_E     result;
}TCPIP_PING_RESULT_IND_SIG_T;

// PING result callback function
typedef void (*TCPIP_PING_CALLBACK_FPTR)(
    TCPIP_PING_RESULT_E     res,            /* ping result, 0 - succeed; other - fail */
    uint32                  time_delay,     /* ping time delay, only valid when success, unit: ms */
    TCPIP_PING_HANDLE       ping_handle );  /* ping handle */
// PING result callback function
typedef void (*TCPIP_PING_CALLBACK_EX_FPTR)(
    TCPIP_PING_RESULT_E     res,            /* ping result, 0 - succeed; other - fail */
    uint32                              time_delay,     /* ping time delay, only valid when success, unit: ms */
    TCPIP_PING_HANDLE       ping_handle,  /* ping handle */
    uint8                                ttl,
    char*                               ipaddr,
    uint8                                time_is_out    /*is time runs out, 0 - run out, 1 - not run out*/
    /*uint16                          ipaddr_size*/);

// DHCP result type
typedef enum
{
    DHCPRES_SUCCESS         = 0,    /* DHCP OK, received DHCP answer */
    DHCPRES_TIMEOUT         = 1,    /* DHCP fail, timeout */
    DHCPRES_OTHER_ERROR     = 2,    /* DHCP fail, other error except timeout */
}TCPIP_DHCP_RESULT_E;

// DHCP result message
// NOTES : IP address is Big-Ending
typedef struct dhcp_result_ind_sig
{
    _SIGNAL_VARS
    TCPIP_DHCP_RESULT_E     result;
    TCPIP_NETIF_IPADDR_T    addrs;
    TCPIP_IPADDR_T          dhcp_addr;
    TCPIP_NETID_T           netid;
}TCPIP_DHCP_RESULT_IND_SIG_T;

#ifdef IP_V6
typedef struct dhcp6_result_ind_sig
{
    _SIGNAL_VARS
    TCPIP_DHCP_RESULT_E     result;
    TCPIP_NETIF_IPADDR6_T    addrs;
    TCPIP_NETID_T           netid;
}TCPIP_DHCP6_RESULT_IND_SIG_T;
#endif

// DHCP request callback function
// NOTES : IP address is Big-Ending
typedef void (*TCPIP_DHCP_CALLBACK_FPTR)(
    int32                       res,        /* DHCP result - 0: OK; else - error */
    const TCPIP_NETIF_IPADDR_T* addr_ptr,   /* ip addresses pointer */
    TCPIP_IPADDR_T              dhcp_addr,  /* DHCP server address pointer */
    TCPIP_NETID_T               netid );    /* net interface ID */

// IP conflict check result message
typedef struct ipconflict_result_ind_sig
{
    _SIGNAL_VARS
    TCPIP_IPCONFLICT_HANDLE     handle;
}TCPIP_IPCONFLICT_RESULT_IND_SIG_T;

// client info registered in DHCP server
typedef struct dhcps_host_info
{
    TCPIP_IPADDR_T  hostip;
    char            hostname[TCPIP_DHCPS_HOST_NAME_LEN_MAX];
}TCPIP_DHCPS_HOST_INFO_T;

typedef enum
{
    SETMTU_SUCCESS         = 0,
    SETMTU_INVALID_MTU     = 1,
    SETMTU_INVALID_NETIF   = 2,
}TCPIP_SETMTU_RESULT_E;

typedef struct ping_option_param
{
    uint32  data_len;              /*pyload of icmp*/
    uint32  spend_time;         /*expect ping spend time*/
}TCPIP_PING_OPTION_PARAM_T;
/*---------------------------------------------------------------------------*
 *                         FUNCTION DEFINITION                               *
 *---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description: get global spare dns addresses
// Global resource : 
// Author: yifeng.wang
// Note :  PARAM1: dns1_ptr (OUT) -- address pointer for primary spare dns
//         PARAM2: dns2_ptr (OUT) -- address pointer for secondary spare dns
//         Output IP addresses are in Big-Endian.       
/*****************************************************************************/
PUBLIC void sci_getsparedns( TCPIP_IPADDR_T* dns1_ptr, TCPIP_IPADDR_T* dns2_ptr );


/*****************************************************************************/
// Description: set global spare dns addresses
// Global resource : 
// Author: yifeng.wang
// Note :  PARAM1: dns1 (IN) -- primary dns
//         PARAM2: dns2 (IN) -- secondary dns
//         Input IP addresses should be in Big-Endian.
//         Spare DNS are used in net interface where no DNS is assigned
//         Global spare DNSs will be used if no spare DNSs set on net interface.
/*****************************************************************************/
PUBLIC void sci_setsparedns( TCPIP_IPADDR_T dns1, TCPIP_IPADDR_T dns2 );


/*****************************************************************************/
// Description: get spare dns addresses on given net interface
// Global resource : none
// Author: yifeng.wang
// Note :  PARAM1: dns1_ptr (OUT) -- address pointer for primary spare dns
//         PARAM2: dns2_ptr (OUT) -- address pointer for secondary spare dns
//         PARAM3: netid (IN) -- net interface ID
//         Output IP addresses are in Big-Endian.
/*****************************************************************************/
PUBLIC void sci_getsparedns_ext(
    TCPIP_IPADDR_T* dns1_ptr,
    TCPIP_IPADDR_T* dns2_ptr,
    TCPIP_NETID_T   netid);

/*****************************************************************************/
// Description: get ipv4 dns addresses on given net interface
// Global resource : none
// Author: yajun.shi
// Note :  PARAM1: dns1_ptr (OUT) -- address pointer for primary spare dns
//         PARAM2: dns2_ptr (OUT) -- address pointer for secondary spare dns
//         PARAM3: netid (IN) -- net interface ID
//         Output IP addresses are in Big-Endian. If the network dont provide any dns address,
//          we will give the spare dns addressess.
/*****************************************************************************/
PUBLIC void sci_getdnsbynetid(
    TCPIP_IPADDR_T* dns1_ptr,
    TCPIP_IPADDR_T* dns2_ptr,
    TCPIP_NETID_T   netid);

/*****************************************************************************/
// Description: set spare dns addresses on given net interface
// Global resource : none
// Author: yifeng.wang
// Note :  PARAM1: dns1 (IN) -- primary dns
//         PARAM2: dns2 (IN) -- secondary dns
//         PARAM3: netid (IN) -- net interface ID
//         Spare DNS is used in net interface where no DNS is assigned.
//         Input IP addresses should be in Big-Endian.
//         If no spare DNS set on net interface, global spare DNS
//         which are set through sci_setsparedns() will be used instead.
/*****************************************************************************/
PUBLIC void sci_setsparedns_ext(
    TCPIP_IPADDR_T  dns1,
    TCPIP_IPADDR_T  dns2,
    TCPIP_NETID_T   netid);


/*****************************************************************************/
// Description: get data throughput length on given net interface.
// Global resource : none
// Author : yifeng.wang
// Note :   PARAM1: total_sent (OUT) -- pointer to total sent length in byte. 
//                  If NULL, sent length will not return.
//          PARAM2: total_recv (OUT) -- pointer to total received length in byte. 
//                  If NULL, recv length will not return.
//          PARAM3: netid (IN) -- net interface ID.
//          RETURN: 0 for get throughput succesfully; -1 for failure.
//          1) Both sent and received length include protocol header legnth.
//          2) throughput will be automatically cleared when network is deactived.
//          So caller should get valid throughput before network deactivate.
/*****************************************************************************/
PUBLIC int sci_getthroughput_ext(
    uint64*         total_sent,
    uint64*         total_recv,
    TCPIP_NETID_T   netid);


/*****************************************************************************/
// Description: reset throughput record on given net interface.
// Global resource : none
// Author : yifeng.wang
// Note :   PARAM1: netid (IN) -- net interface ID.
//          RETURN: 0 if reset successfully; else -1 for failure. 
/*****************************************************************************/
PUBLIC int sci_resetthroughput_ext( TCPIP_NETID_T netid );


/*****************************************************************************/
// Description: asynchronous ping echo request
// Global resource : none
// Author : yifeng.wang
// Note :   PARAM1: faddr_ptr (IN) -- ping test address, it can be an IP or URL
//                  e.g. "172.16.14.136" or "www.google.com"
//          PARAM2: data_size (IN) -- ping payload size, unit: byte, range: 0
//                  max data length that tcpip buffer can hold,
//                  e.g. when tcpip buffer is 1536, the Rang_max is 1492.
//                  if input is out of range, it will be confined automatically
//                  0 means default payload size 64 bytes
//          PARAM3: time_out (IN) -- ping echo request time out value, unit: ms
//                  0 means default time out value 4 seconds;
//          PARAM4: task_id (IN) -- task which to receive ping result message
//          PARAM5: net_id (IN) -- net interface ID
//          RETURN: non-0 - PING request handle; 0 - failure
/*****************************************************************************/
PUBLIC TCPIP_PING_HANDLE sci_async_ping( 
    char*           faddr_ptr,
    uint32          data_size,
    uint32          time_out,
    BLOCK_ID        task_id,
    TCPIP_NETID_T   net_id );


/*****************************************************************************/
// Description: send ping6 or ping4 echo request
// Global resource : none
// Author : yifeng.wang
// Note :   PARAM1: faddr_ptr (IN) -- ping test address, it can be an IP or URL
//                  e.g. "172.16.14.136" or "www.google.com"
//          PARAM2: data_size (IN) -- ping payload size, unit: byte, range: 0
//                  max data length that tcpip buffer can hold,
//                  e.g. when tcpip buffer is 1536, the Rang_max is 1492.
//                  if input is out of range, it will be confined automatically
//                  0 means default payload size 64 bytes
//          PARAM3: time_out (IN) -- ping echo request time out value, unit: ms
//                  0 means default time out value 4 seconds;
//          PARAM4: callback_fptr (IN) -- ping echo reply callback pointer
//          PARAM5: net_id (IN) -- net interface ID
//          RETURN: non-0 - PING request handle; 0 - failure
/*****************************************************************************/
PUBLIC TCPIP_PING_HANDLE sci_ping_request( 
    char*           faddr_ptr,
    uint32          data_size,
    uint32          time_out,
    TCPIP_PING_CALLBACK_EX_FPTR    callback_fptr,
    TCPIP_NETID_T   net_id );

/*****************************************************************************/
// Description: send ping6 or ping4 echo request
// Global resource : none
// Author : yifeng.wang
// Note :   PARAM1: faddr_ptr (IN) -- ping test address, it can be an IP or URL
//                  e.g. "172.16.14.136" or "www.google.com"
//          PARAM2: data_size (IN) -- ping payload size, unit: byte, range: 0
//                  max data length that tcpip buffer can hold,
//                  e.g. when tcpip buffer is 1536, the Rang_max is 1492.
//                  if input is out of range, it will be confined automatically
//                  0 means default payload size 64 bytes
//          PARAM3: time_out (IN) -- ping echo request time out value, unit: ms
//                  0 means default time out value 4 seconds;
//          PARAM4: times (IN) -- number of times to ping request
//          PARAM5: callback_fptr (IN) -- ping echo reply callback pointer
//          PARAM6: net_id (IN) -- net interface ID
//          RETURN: non-0 - PING request handle; 0 - failure
/*****************************************************************************/
PUBLIC TCPIP_PING_HANDLE sci_ping_request_ext( 
    char*           faddr_ptr,
    uint32          data_size,
    uint32          time_out,
    uint32			times,
    TCPIP_PING_CALLBACK_EX_FPTR    callback_fptr,
    TCPIP_NETID_T   net_id );

/*****************************************************************************/
// Description: send ping4 echo request
// Global resource : none
// Author : yifeng.wang
// Note :   PARAM1: faddr_ptr (IN) -- ping test address, it can be an IP or URL
//                  e.g. "172.16.14.136" or "www.google.com"
//          PARAM2: data_size (IN) -- ping payload size, unit: byte, range: 0
//                  max data length that tcpip buffer can hold,
//                  e.g. when tcpip buffer is 1536, the Rang_max is 1492.
//                  if input is out of range, it will be confined automatically
//                  0 means default payload size 64 bytes
//          PARAM3: time_out (IN) -- ping echo request time out value, unit: ms
//                  0 means default time out value 4 seconds;
//          PARAM4: times (IN) -- number of times to ping request
//          PARAM5: callback_fptr (IN) -- ping echo reply callback pointer
//          PARAM6: net_id (IN) -- net interface ID
//          RETURN: non-0 - PING request handle; 0 - failure
/*****************************************************************************/
PUBLIC TCPIP_PING_HANDLE sci_ping4_request(
    char*           faddr_ptr,
    TCPIP_PING_OPTION_PARAM_T optvalue,
    uint32          time_out,
    uint32          times,
    TCPIP_PING_CALLBACK_EX_FPTR    callback_fptr,
    TCPIP_NETID_T   net_id );

/*****************************************************************************/
// Description: send ping6 echo request
// Global resource : none
// Author : yifeng.wang
// Note :   PARAM1: faddr_ptr (IN) -- ping test address, it can be an IP or URL
//                  e.g. "172.16.14.136" or "www.google.com"
//          PARAM2: data_size (IN) -- ping payload size, unit: byte, range: 0
//                  max data length that tcpip buffer can hold,
//                  e.g. when tcpip buffer is 1536, the Rang_max is 1492.
//                  if input is out of range, it will be confined automatically
//                  0 means default payload size 64 bytes
//          PARAM3: time_out (IN) -- ping echo request time out value, unit: ms
//                  0 means default time out value 4 seconds;
//          PARAM4: times (IN) -- number of times to ping request
//          PARAM5: callback_fptr (IN) -- ping echo reply callback pointer
//          PARAM6: net_id (IN) -- net interface ID
//          RETURN: non-0 - PING request handle; 0 - failure
/*****************************************************************************/
PUBLIC TCPIP_PING_HANDLE sci_ping6_request( 
    char*           faddr_ptr,
    TCPIP_PING_OPTION_PARAM_T optvalue,
    uint32          time_out,
    uint32          times,
    TCPIP_PING_CALLBACK_EX_FPTR    callback_fptr,
    TCPIP_NETID_T   net_id );

/*****************************************************************************/
// Description: cancel and stop ping request
// Global resource : none
// Author : yifeng.wang
// Note :   PARAM1: ping_handle (IN) -- ping handle
//          this function is valid for both normal and async ping request.
/*****************************************************************************/
PUBLIC void sci_ping_cancel( TCPIP_PING_HANDLE ping_handle );


/*****************************************************************************/
// Description : asynchronous DHCP request
// Global resource : none
// Author: yifeng.wang
// Note :   for unified multi-bearer use, this function tries to get local IP 
//          through DHCP, and the caller should make sure the low bearer 
//          (e.g. GPRS, WIFI) is up already, else there will be error anyway.
//          PARAM1: time_out (IN) -- DHCP time out value (unit: ms)
//          PARAM2: task_id (IN) -- task that to receive DHCP result message
//          PARAM3: net_id (IN) -- net interface ID
//          RETURN: 0 - send DHCP request OK ; non-0 - send DHCP request fail
/*****************************************************************************/
PUBLIC int32 sci_async_dhcp(
    uint32          time_out,
    BLOCK_ID        task_id,
    TCPIP_NETID_T   net_id );


/*****************************************************************************/
// Description : start DHCP request
// Global resource : none
// Author: yifeng.wang
// Note :   for unified multi-bearer use, this function tries to get local IP 
//          through DHCP, and the caller should make sure the low bearer 
//          (e.g. GPRS, WIFI) is up, else there will be error anyway.
//          PARAM1: callback_fptr (IN) -- DHCP call back function to caller
//          PARAM2: time_out (IN) -- DHCP time out value (unit: ms)
//          PARAM3: net_id (IN) -- net interface ID
//          RETURN: 0 - send DHCP request OK ; non-0 - send DHCP request fail
/*****************************************************************************/
PUBLIC int32 sci_dhcp_request(
    TCPIP_DHCP_CALLBACK_FPTR callback_fptr,
    uint32          time_out,
    TCPIP_NETID_T   net_id );


/*****************************************************************************/
// Description : cancel DHCP request
// Global resource : 
// Author: yifeng.wang
// Note :   for unified multi-bearer use
//          PARAM1: net_id (IN) -- net interface ID
//          this function is valid for both normal and async ping request.
/*****************************************************************************/
PUBLIC void sci_dhcp_cancel( TCPIP_NETID_T net_id );

/*****************************************************************************/
// Description : Set Dhcp Host Name
// Global resource : none
// Author: qin.he
// Note : for unified multi-bearer use
/*****************************************************************************/
PUBLIC void sci_dhcp_sethostname(char* hostname);

/*****************************************************************************/
// Description : set EAPOL data receive up-call
// Global resource : 
// Author : yifeng.wang
// Note :   TCPIP doesn't understand EAPOL data, so when it received EAPOL data,
//          TCPIP take them up to APP, this function is used to register EAPOL 
//          data up-call function.
//          PARAM1: callback_fptr (IN) -- receive callback function pointer
//          PARAM2: net_id (IN) -- net interface ID
//          RETURN: 0 - set OK; other - set failed    
/*****************************************************************************/
PUBLIC int32 sci_eapol_setrxcallback(
    TCPIP_EAPOL_RX_CALLBACK_FPTR callback_fptr,
    TCPIP_NETID_T   net_id);


/*****************************************************************************/
// Description : send EAPOL data
// Global resource : 
// Author : yifeng.wang
// Note :   PARAM1: data_ptr (IN) -- EAPOL data pointer
//          PARAM2: data_len (IN) -- EAPOL data length
//          PARAM3: dst_mac_ptr (IN) -- dest mac address
//          PARAM4: dst_mac_len (IN) -- dest mac length
//          PARAM5: net_id (IN) -- net interface ID
//          RETURN: 0 - send OK; other - send failed
/*****************************************************************************/
PUBLIC int32 sci_eapol_tx(
    const uint8*    data_ptr, 
    uint32          data_len,
    const uint8*    dst_mac_ptr,
    uint32          dst_mac_len,
    TCPIP_NETID_T   net_id );


/*****************************************************************************/
// Description : register and start IP conflict check notify
// Global resource : 
// Author : yifeng.wang
// Note :   PARAM1: task_id (IN) -- task which to receive check result
//          PARAM2: net_id (IN) -- net interface ID
//          RETURN: OK - IP conflict check handle; Fail - 0
/*****************************************************************************/
PUBLIC TCPIP_IPCONFLICT_HANDLE sci_ipconflict_reg(
    BLOCK_ID        task_id,
    TCPIP_NETID_T   net_id );


/*****************************************************************************/
// Description : cancel IP conflict nodify
// Global resource : 
// Author : yifeng.wang
// Note :   PARAM1: handle (IN) -- IP conflict check handle
/*****************************************************************************/
PUBLIC void sci_ipconflict_dereg( TCPIP_IPCONFLICT_HANDLE handle ); 


/*****************************************************************************/
// Description : get client info which registered at DHCP server.
// Global :
// Author : yifeng.wang
// Note :   PARAM1: mac_ptr (IN) -- MAC address, length should be 6 bytes
//          PARAM2: info_ptr (OUT) -- host info struct pointer
//          PARAM3: netid (IN) -- net interface ID
//          RETURN: TCPIP_ERROR_OK - successful; else - failed
/*****************************************************************************/
PUBLIC TCPIP_ERROR_E sci_dhcps_gethostinfo(
    uint8 *mac_ptr, TCPIP_DHCPS_HOST_INFO_T* info_ptr, TCPIP_NETID_T netid);


/*****************************************************************************/
// Description : get DHCP server open state.
// Global :
// Author : yifeng.wang
// Note :   PARAM : netid (IN) -- net interface ID
//          RETURN: TRUE - open; FALSE - not open
/*****************************************************************************/
PUBLIC BOOLEAN sci_dhcps_isopen( TCPIP_NETID_T netid );


/*****************************************************************************/
// Description : set DHCP server parameter.
// Global :
// Author : yifeng.wang
// Note :   PARAM1: cfg_ptr (IN) -- DHCP server config parameters
//          PARAM2: netid (IN) -- net interface ID
//          RETURN: TCPIP_ERROR_OK - successful; else - failed
//          If DHCP server is running, this function will close server first,
//          then set new parameters.
//          Parameters should be in NetEndian( Big-Endian );
/*****************************************************************************/
PUBLIC TCPIP_ERROR_E sci_dhcps_setparam(
    const TCPIP_DHCP_SERVER_CONFIG_T* cfg_ptr, TCPIP_NETID_T netid );

/*****************************************************************************/
// Description : DHCPS Add static leases
// Global : none
// Author : yajun.shi
// Note :   PARAM1: mac_ptr (IN) -- mac address pointer
//          PARAM2: ip_addr (IN) -- new ip address
//          RETURN: TRUE - success; FALSE - fail
/*****************************************************************************/
PUBLIC BOOLEAN sci_dhcps_addstaticlease(uint8 *mac_ptr,TCPIP_IPADDR_T ip_addr);

/*****************************************************************************/
// Description : DHCPS delete static leases
// Global : none
// Author : yajun.shi
// Note :   PARAM: mac_ptr (IN) -- mac address pointer
//          RETURN: TRUE - success; FALSE - fail
/*****************************************************************************/
PUBLIC BOOLEAN sci_dhcps_delstaticlease(uint8 *mac_ptr);

/*****************************************************************************/
// Description : start DHCP server.
// Global :
// Author : yifeng.wang
// Note :   PARAM1: cfg_ptr (IN) -- DHCP server config parameters
//          PARAM2: netid (IN) -- net interface ID
//          RETURN: TCPIP_ERROR_OK - open successful; else - open failed
//          If DHCP server is running, this function will close server first,
//          then set new parameters and restart the DHCP server.
//          Parameters should be in NetEndian( Big-Endian );
/*****************************************************************************/
PUBLIC TCPIP_ERROR_E sci_dhcps_open(
    const TCPIP_DHCP_SERVER_CONFIG_T* cfg_ptr, TCPIP_NETID_T netid );


/*****************************************************************************/
// Description : close DHCP server.
// Global :
// Author : yifeng.wang
// Note :   PARAM : netid (IN) -- net interface on which DHCP server runs.
/*****************************************************************************/
PUBLIC void sci_dhcps_close( TCPIP_NETID_T netid );

#ifdef IP_V6
/*****************************************************************************/
// Description : set enable/disable  get ipv6 auto ip.
// Global :
// Author : qin.he
// Note :   PARAM : netid (IN) -- net interface.
//          PARAM : autoip (IN) -- ture/false -- enable/disable.
//          PARAM : task_id (IN) -- task that to receive DHCP6 result message
//          PARAM : time_out (IN) -- DHCP6 time out value (unit: ms)
/*****************************************************************************/
PUBLIC TCPIP_ERROR_E sci_setip6autoip(TCPIP_NETID_T netid, BOOLEAN is_enable_autoip, BLOCK_ID task_id, uint32 time_out);;

#endif

/*****************************************************************************/
// Description: reverses the string. 
// Global resource : none
// Note : PARAM1: str (IN): the string to reversess.
//        RETURN: 0 - success; 
//                -1 - fail. 
/*****************************************************************************/
int sci_strrev(unsigned char *str);

/*****************************************************************************/
// Description: convert the number the string. 
// Global resource : none
// Note : PARAM1: num (IN): the number to convert.
//        PARAM2: buffer(OUT): the buffer to store result.
//        PARAM3: len(IN): the buffer len of PARAM2.
//        PARAM4: base(IN): the base to convert(eg 10 or 16 base etc)
//        RETURN: 0 - success; 
//                -1 - fail. 
/*****************************************************************************/
int sci_itoa(uint64 num, unsigned char* buff, int len, int base);
///////////////////////////////////////////////////////////////////////////////
#endif /* APP_TCP_IF_H */
