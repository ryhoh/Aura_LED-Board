/**
 * @file NVM.h
 * @author ryhoh
 * @brief Non-Volatile Memory manager in App layer.
 * 
 */

#ifndef _NVM_H_
#define _NVM_H_

/**
 * EEPROM Mapping
 *
 * [0x00] - [0x1F] : SSID
 * [0x20] - [0x3F] : Password
 * [0x40] - [0x5F] : Device Name
 * [0x60] - [0x60] : Variant Index
 *
*/

// インクルード
#include "Driver/DeviceInterface.h"

// マクロ定義
#define m_NVM_BUFFER_SIZE (31)  // バッファサイズ
#define m_NVM_ADDR_SSID        (0x00)  // SSIDのアドレス
#define m_NVM_ADDR_PASSWD      (0x20)  // PASSWDのアドレス
#define m_NVM_ADDR_HOST_NAME   (0x40)  // ネットワーク上での名前のアドレス
#define m_NVM_ADDR_VARIANT_IDX (0x60)  // バリアントインデックスのアドレス
#define m_NVM_SIZE             (0x61)  // EEPROMのサイズ

// 関数宣言
void NVM_Init(void);
String Get_NVM_SSID();
void Set_NVM_SSID(String str_SSID);
String Get_NVM_PASSWD();
void Set_NVM_PASSWD(String str_PASSWD);
String Get_NVM_HostName();
void Set_NVM_HostName(String str_hostname);
uint8_t Get_NVM_Variant_idx();

#endif /* _NVM_H_ */
