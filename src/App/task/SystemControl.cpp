/**
 * @file SystemControl.cpp
 * @author ryhoh
 * @brief システム制御ソースファイル
 * 
 */

// インクルード
#include "SystemControl.h"

// 変数宣言
static uint8_t gsu8_is_setup_state = m_ON;  // システムセットアップ状態
static uint8_t gsu8_is_network_setup_state = m_ON;  // ネットワークセットアップ状態

// 関数定義
/**
 * @brief メインタスク
 * @note 16ms周期で呼び出し
 * 
 */
void Main_Task(void) {
  static uint32_t step = 0;
  const uint32_t cu32_network_substep = m_SYSCTL_CALL_ITVL_NETWORK / m_SYSCTL_CALL_ITVL;

  // ネットワークタスクは320msごとなので、分周で実行
  if (step % cu32_network_substep == 0) {
    Network_Task_Main();
  }

  // LEDタスクは毎回実行
  LED_Task_Main();

  // step を進める
  step++;
  if (step >= cu32_network_substep) {
    step = 0;
  }
}

uint8_t *Get_SYSCTL_SetupState(void) {
    return &gsu8_is_setup_state;
}

uint8_t *Get_SYSCTL_NetworkSetupState(void) {
    return &gsu8_is_network_setup_state;
}
