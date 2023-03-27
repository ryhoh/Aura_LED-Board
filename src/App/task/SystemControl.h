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
#include "App/nvm/NVM.h"

// マクロ定義
#define m_SYSCTL_CALL_ITVL (16)  // [ms,1]
#define m_SYSCTL_CALL_ITVL_NETWORK (320)  // [ms,1]

#define m_SYSCTL_BLOCKING_LEVEL_ALL     (0)
#define m_SYSCTL_BLOCKING_LEVEL_LED     (1)
#define m_SYSCTL_BLOCKING_LEVEL_NETWORK (2)
#define m_SYSCTL_BLOCKING_LEVEL_NUM     (3)

#define m_SYSCTL_STATE_POWER_ON       (0)  // 電源投入状態。
#define m_SYSCTL_STATE_LED_READY      (1)  // LED初期設定完了状態。LEDで任意の文字を自由に表示できる。
#define m_SYSCTL_STATE_CONFIGURE      (2)  // 設定モード。WiFiホストモードで、ドライブモード時の接続先などを設定する。
#define m_SYSCTL_STATE_NETWORK_READY  (3)  // ネットワーク初期設定完了状態。WiFiクライアントモードで、インターネットに接続する。
#define m_SYSCTL_STATE_DRIVE          (4)  // ドライブモード。WiFiを切る。
#define m_SYSCTL_STATE_TRANSITION_NUM (5)  // 状態遷移数

// プロトタイプ宣言
void SYSCTL_Init(void);
void Main_Task(void);
void SYSCTL_WaitForBlockingLevel(uint8_t u8_level);
void Set_SYSCTL_Blocking_Level(uint8_t u8_level);
void Unset_SYSCTL_Blocking_Level(uint8_t u8_level);
void Set_SYSCTL_LEDSetupState(uint8_t u8_done);
void Set_SYSCTL_NetworkSetupState(uint8_t u8_done);
uint8_t Get_SYSCTL_SystemState(void);

#endif  /* _SYSTEMCONTROL_H_ */
