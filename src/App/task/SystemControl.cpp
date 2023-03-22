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
static uint8_t gsu8_SYSCTL_Blocking_Flags[m_SYSCTL_BLOCKING_LEVEL_NUM] = { 0 };  // 割り込み禁止フラグ

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
  // if (step % cu32_network_substep == 0) {
  //   Network_Task_Main();
  // }

  // LEDタスクは毎回実行
  LED_Task_Main();

  // step を進める
  step++;
  if (step >= cu32_network_substep) {
    step = 0;
  }
}

/**
 * @brief 割り込み禁止の確認と待機
 * @note 割り込み禁止レベルが指定レベルより緩和されるまで待機する
 * 
 * @param u8_level 
 */
void SYSCTL_WaitForBlockingLevel(uint8_t u8_level) {
  uint8_t u8_blocked = m_ON;

  // レベルが範囲外の場合は何もしない
  if (u8_level >= m_SYSCTL_BLOCKING_LEVEL_NUM) {
    return;
  }

  while (u8_blocked == m_ON) {
    // 割り込み禁止されているか確認
    u8_blocked = m_OFF;
    for (uint8_t i = 0; i <= u8_level; i++) {
      if (gsu8_SYSCTL_Blocking_Flags[i] == m_ON) {
        u8_blocked = m_ON;
        break;
      }
    }

    // 割り込み禁止されている場合は待機
    call_usleep(100);
  }
}

/**
 * @brief 割り込み禁止レベルを設定する
 * 
 * @param u8_level 禁止レベル
 */
void Set_SYSCTL_Blocking_Level(uint8_t u8_level) {
  if (u8_level < m_SYSCTL_BLOCKING_LEVEL_NUM) {
    gsu8_SYSCTL_Blocking_Flags[u8_level] = m_ON;
  }
}

/**
 * @brief 割り込み禁止レベルを解除する
 * 
 * @param u8_level 禁止レベル
 */
void Unset_SYSCTL_Blocking_Level(uint8_t u8_level) {
  if (u8_level < m_SYSCTL_BLOCKING_LEVEL_NUM) {
    gsu8_SYSCTL_Blocking_Flags[u8_level] = m_OFF;
  }
}

uint8_t *Get_SYSCTL_SetupState(void) {
    return &gsu8_is_setup_state;
}

uint8_t *Get_SYSCTL_NetworkSetupState(void) {
    return &gsu8_is_network_setup_state;
}
