/**
 * @file SystemControl.cpp
 * @author ryhoh
 * @brief システム制御ソースファイル
 * 
 */

// インクルード
#include "SystemControl.h"

// 変数宣言
static uint8_t gsu8_is_setup_state = m_ON;

// プロトタイプ宣言
uint8_t Get_SYSCTL_SetupState(void) {
    return gsu8_is_setup_state;
}
