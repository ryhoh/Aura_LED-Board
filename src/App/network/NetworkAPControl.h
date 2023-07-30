/**
 * @file NetworkTask.h
 * @author ryhoh
 * @brief Network task header file.
 * 
 */

#ifndef _NETWORKAPCONTROL_H_
#define _NETWORKAPCONTROL_H_


// インクルード
#include "Driver/DeviceInterface.h"
#include "App/Common.h"
#include "App/Type.h"
#include "App/task/SystemControl.h"
#include "App/task/LEDTask.h"

// マクロ定義

// これらはProfile.hかどこかに移動するべき
#define m_NETWORK_TASK_AP_PASSWD (String("luminous-ryhoh"))
#define m_NETWORK_TASK_AP_IP (IPAddress(192, 168, 1, 1))
#define m_NETWORK_TASK_AP_GATEWAY (IPAddress(192, 168, 1, 10))
#define m_NETWORK_TASK_AP_SUBNET (IPAddress(255, 255, 255, 0))

// プロトタイプ宣言
void Network_Task_Init_APMode(void);
void Network_Task_RunAPMode(void);

#endif  /* _NETWORKAPCONTROL_H_ */
