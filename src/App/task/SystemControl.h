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

// プロトタイプ宣言
uint8_t Get_SYSCTL_SetupState(void);
void LED_Task_InSetupState(void);


#endif  /* _SYSTEMCONTROL_H_ */
