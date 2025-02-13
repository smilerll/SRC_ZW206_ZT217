#ifndef __ONEOS_CONFIG_H__
#define __ONEOS_CONFIG_H__

#define OS_TASK_PRIORITY_MAX 32

/* OTA */

/* Fota by CMIOT */

#define FOTA_USING_CMIOT
#define CMIOT_USING_SINGLE
#define CMIOT_NETWORKING_MODE_HTTP
#define CMIOT_DEFAULT_NETWORK_PROTOCOL 2
#define CMIOT_ALGORITHM_LUCKPAND
#define CMIOT_DEFAULT_TRY_TIME 5000
#define CMIOT_FOTA_ALGORITHM 4
#define CMIOT_DEFAULT_SECTOR_SIZE 0x10000
#define CMIOT_FOTA_AVAILABLE_RAM_SIZE 0x80000
#define CMIOT_UPDATE_SIZE 0xA0000
#define CMIOT_FOTA_WOSUN_VERSION "IOT4.0_R42641"
#define CMIOT_FOTA_LUSUN_VERSION "IOT5.0_LUSUN11_R50426"
#define CMIOT_FOTA_LUCKSPAR_VERSION "IOT6.0_LUCKSPAR_R010100"
#define CMIOT_FOTA_LUCKPAND_VERSION "IOT7.0_LUCKPAND_R010100"
#define CMIOT_SEGMENT_SIZE_INDEX_START 3
#define CMIOT_DEFAULT_SEGMENT_SIZE_INDEX 6
#define CMIOT_FOTA_SERVICE_PRODUCT_ID "CsgoFkLZUB"
#define CMIOT_FOTA_SERVICE_PRODUCT_SEC "36D75B8686AEE2BAC9945CA23EAFA603"

#define CMIOT_FOTA_SERVICE_OEM "ZTE"
#define CMIOT_FOTA_SERVICE_MODEL "ZW35"
#define CMIOT_FOTA_SERVICE_LANG "CN"
#define CMIOT_FOTA_SERVER_HOST "iosfotacn.ztems.com:443" // max length 32
#define CMIOT_FIRMWARE_VERSION "3.0" // max length 128

#define CMIOT_FOTA_FILENAME1 "mmi_res_240x240_child_circular.bin"
#define CMIOT_FOTA_ADDRESS1 0x90000004
#define CMIOT_FOTA_FILE_MORE1
#define CMIOT_FOTA_FILENAME2 "UWS6121E_WC_1H00_USER_delta_nv.bin"
#define CMIOT_FOTA_ADDRESS2 0x90000100
#define CMIOT_FOTA_FILE_MORE2
#define CMIOT_FOTA_FILENAME3 "UWS6121E_UWS6121E_WC_1H00_USER_cp.bin"
#define CMIOT_FOTA_ADDRESS3 0x90000101
#define CMIOT_FOTA_FILE_MORE3
#define CMIOT_FOTA_FILENAME4 "UWS6121E_UWS6121E_WC_1H00_USER_stone.img.bin"
#define CMIOT_FOTA_ADDRESS4 0x80000003
#define CMIOT_FOTA_CONTRAST_BLOCK_SIZE 64

/* end of Fota by CMIOT */
/* end of OTA */

// #define OS_SERIAL_TX_BUFSZ (128)

#endif /* __ONEOS_CONFIG_H__ */
