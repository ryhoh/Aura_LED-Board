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

// プロトタイプ宣言
void Network_Task_Init_WebServer(void);
void Network_Task_RunAPMode(void);
Network_Config_t Get_NetworkAP_Network_Config(void);

#endif  /* _NETWORKAPCONTROL_H_ */
