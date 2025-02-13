/**
 * Copyright (2017) Baidu Inc. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
// Author: Gang Chen (chengang12@baidu.com)
//
// Description: The AP information APIs.

#ifndef BAIDU_IOT_TINYDU_IOT_OS_SRC_IOT_BAIDU_CA_SOURCE_BAIDU_CA_AP_INFO_H
#define BAIDU_IOT_TINYDU_IOT_OS_SRC_IOT_BAIDU_CA_SOURCE_BAIDU_CA_AP_INFO_H

#include "lightduer_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Obtain the wifi signal status
 *
 * @Return duer_status_t:
 *         DUER_OK, DUER_ERR_FAILED, DUER_ERR_WIFI_SIGNAL_WEAK OR DUER_ERR_WIFI_DISCONNECTED
 */
DUER_INT duer_status_t duer_wifi_status_get(void);

/*
 * Get the signal strength of wifi
 *
 * @Return duer_s8_t:
 *         the signal strength of wifi, unit is dBm
 */
DUER_INT duer_s8_t duer_wifi_rssi_get(void);

/*
 * Set the connected status of wifi
 *
 * @Param connected, the connected status of wifi
 */
DUER_EXT void duer_wifi_set_connected(duer_bool connected);

/*
 * Get the connected status of wifi
 *
 * @Return duer_bool, DUER_TRUE: connected, DUER_FALSE: disconnected
 */
DUER_EXT duer_bool duer_wifi_is_connected(void);

/*
 * The wifi signal status callbacks
 * Return the wifi signal status
 */
typedef duer_status_t (*duer_get_wifi_status_f)();
typedef duer_s8_t (*duer_get_wifi_rssi_f)();

/*
 * Initial the AP information callbacks for Baidu CA
 *
 * @Param f_wifi_status, in, the function obtain the wifi signal status
 */
DUER_EXT void baidu_ca_ap_info_init(duer_get_wifi_status_f f_wifi_status,
                                    duer_get_wifi_rssi_f f_wifi_rssi);

#ifdef __cplusplus
}
#endif

#endif // BAIDU_IOT_TINYDU_IOT_OS_SRC_IOT_BAIDU_CA_SOURCE_BAIDU_CA_AP_INFO_H