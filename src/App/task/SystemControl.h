/**
 * @file SystemControl.h
 * @author ryhoh
 * @brief システム制御ヘッダファイル
 * 
 */

#ifndef _SYSTEMCONTROL_H_
#define _SYSTEMCONTROL_H_

// インクルード
#include "Driver/DeviceInterface.h"
#include "App/Common.h"
#include "App/task/LEDTask.h"
#include "App/task/NetworkTask.h"

// マクロ定義
#define m_SYSCTL_CALL_ITVL (16)  // [ms,1]
#define m_SYSCTL_CALL_ITVL_NETWORK (320)  // [ms,1]

#define m_SYSCTL_BLOCKING_LEVEL_ALL (0)
#define m_SYSCTL_BLOCKING_LEVEL_LED (1)
#define m_SYSCTL_BLOCKING_LEVEL_NETWORK (2)
#define m_SYSCTL_BLOCKING_LEVEL_NUM (3)

// プロトタイプ宣言
void SYSCTL_WaitForBlockingLevel(uint8_t u8_level);
void Set_SYSCTL_Blocking_Level(uint8_t u8_level);
void Unset_SYSCTL_Blocking_Level(uint8_t u8_level);
uint8_t *Get_SYSCTL_SetupState(void);
uint8_t *Get_SYSCTL_NetworkSetupState(void);

#endif  /* _SYSTEMCONTROL_H_ */
