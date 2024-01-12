/**
 * @file NetworkTask.h
 * @author ryhoh
 * @brief Network task header file.
 * 
 */

#ifndef _NETWORKSTACONTROL_H_
#define _NETWORKSTACONTROL_H_


// インクルード
#include "Driver/DeviceInterface.h"
#include "App/Common.h"
#include "App/Type.h"
#include "App/task/SystemControl.h"
#include "App/task/LEDControl.h"

// 構造体定義
typedef struct _IPAddress {
  uint8_t u8_octet1;
  uint8_t u8_octet2;
  uint8_t u8_octet3;
  uint8_t u8_octet4;
} IPAddress_t;

// プロトタイプ宣言
void Network_Task_Init(void);
void Network_Task_Main(void);
uint8_t GET_Network_Task_WiFi_Connected(void);

// I/F
IPAddress_t GET_Network_Local_IPAddress();

#endif  /* _NETWORKSTACONTROL_H_ */
