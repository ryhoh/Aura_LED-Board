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
#include "App/task/LEDTask.h"

// プロトタイプ宣言
void Network_Task_Init(void);
void Network_Task_Main(void);
String GET_Network_WiFi_SSID(void);

#endif  /* _NETWORKSTACONTROL_H_ */
