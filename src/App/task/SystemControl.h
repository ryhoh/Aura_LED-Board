/**
 * @file SystemControl.h
 * @author ryhoh
 * @brief システム制御ヘッダファイル
 * 
 */

#ifndef _SYSTEMCONTROL_H_
#define _SYSTEMCONTROL_H_

// インクルード
#include "App/Common.h"
#include "App/task/LEDTask.h"
#include "App/task/NetworkTask.h"

// マクロ定義
#define m_SYSCTL_CALL_ITVL (16)  // [ms,1]
#define m_SYSCTL_CALL_ITVL_NETWORK (320)  // [ms,1]

// プロトタイプ宣言
uint8_t *Get_SYSCTL_SetupState(void);
uint8_t *Get_SYSCTL_NetworkSetupState(void);

#endif  /* _SYSTEMCONTROL_H_ */
