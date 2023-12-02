/**
 * @file SystemControl.cpp
 * @author ryhoh
 * @brief システム制御ソースファイル
 * 
 */

// インクルード
#include "SystemControl.h"

// 変数宣言
static uint8_t gsu8_SYSCTL_SystemState = m_SYSCTL_STATE_POWER_ON;  // システム制御状態

static uint8_t gsu8_SYSCTL_Blocking_Flags[m_SYSCTL_BLOCKING_LEVEL_NUM] = { 0 };  // 割り込み禁止フラグ

// プロトタイプ宣言
static void SYSCTL_State_Control(void);
static void SYSCTL_Judge_LED_Ready(void);
static void SYSCTL_Entry_LED_Ready(void);
static void SYSCTL_Judge_Configure(void);
static void SYSCTL_Entry_Configure(void);
static void SYSCTL_Do_Configure(void);
static void SYSCTL_Judge_Network_Ready(void);
static void SYSCTL_Entry_Network_Ready(void);
static void SYSCTL_Judge_Drive(void);
static void SYSCTL_Entry_Drive(void);
static void SYSCTL_Do_Drive(void);

// 状態遷移テーブル
static TransitionTable_t gsst_SYSCTL_StateTransition_Tbl[m_SYSCTL_STATE_TRANSITION_NUM] = {
  /* Judge                       Entry                        Do                    Exit */
  { NULL,                        NULL,                        NULL,                 NULL },  // (PowerOn)
  { &SYSCTL_Judge_LED_Ready,     &SYSCTL_Entry_LED_Ready,     NULL,                 NULL },  // PowerOn      -> LEDReady
  { &SYSCTL_Judge_Network_Ready, &SYSCTL_Entry_Network_Ready, NULL,                 NULL },  // LEDReady     -> NetworkReady
};

// 関数定義
/**
 * @brief システム制御初期化
 * 
 */
void SYSCTL_Init(void) {
  // NVMの初期化
  NVM_Init();

  // バリアント設定
  Variant_Init();

  // ピンモード設定
  call_pinMode(Get_VARIANT_LampPin(), OUTPUT);
  call_pinMode(Get_VARIANT_SPIDataPin(), OUTPUT);
  call_pinMode(Get_VARIANT_SPICSPin(), OUTPUT);
  call_pinMode(Get_VARIANT_SPIClockPin(), OUTPUT);

  // LED のセットアップを行う
  LED_Task_Init();

  // ネットワークのセットアップを行う
  Network_Task_Init();

  // Webサーバのセットアップを行う
  Network_Task_Init_WebServer();
}

/**
 * @brief メインタスク
 * @note 1ms周期で呼び出し
 * 
 */
void SYSCTL_Priority_Task_Main(void) {
  // 出力処理
  LED_PrimeTask_OutputMain();
}

/**
 * @brief システム制御状態遷移判定
 * @note 状態遷移テーブルを参照し、状態遷移判定を行う
 * 16ms周期で呼び出し
 * 
 */
static void SYSCTL_State_Control(void) {
  const uint8_t cu8_old_state = gsu8_SYSCTL_SystemState;
  // 状態遷移判定
  // テーブルは優先度の高い順で定義されており、チェックは低いものから順に実施する
  for (int8_t i8_i = m_SYSCTL_STATE_TRANSITION_NUM - 1; i8_i >= 0; --i8_i) {
    if (gsst_SYSCTL_StateTransition_Tbl[i8_i].p_judge != NULL) {
      gsst_SYSCTL_StateTransition_Tbl[i8_i].p_judge();
    }
  }

  const uint8_t cu8_state = gsu8_SYSCTL_SystemState;
  if (cu8_old_state != cu8_state) {
    // 状態遷移時の処理
    // Exit処理
    if (gsst_SYSCTL_StateTransition_Tbl[cu8_old_state].p_exit != NULL) {
      gsst_SYSCTL_StateTransition_Tbl[cu8_old_state].p_exit();
    }

    // Entry処理
    if (gsst_SYSCTL_StateTransition_Tbl[cu8_state].p_entry != NULL) {
      gsst_SYSCTL_StateTransition_Tbl[cu8_state].p_entry();
    }
  }

  // Do処理
  if (gsst_SYSCTL_StateTransition_Tbl[cu8_state].p_do != NULL) {
    gsst_SYSCTL_StateTransition_Tbl[cu8_state].p_do();
  }
}

static void SYSCTL_Judge_LED_Ready(void) {
  const uint8_t cu8_is_LED_setup_done = GET_LED_Task_Setup_Done();

  // 遷移条件は、(現在PowerOn状態 && LEDタスクがセットアップ完了であること)
  if ((gsu8_SYSCTL_SystemState == m_SYSCTL_STATE_POWER_ON)
    && (cu8_is_LED_setup_done == m_ON)) {
    // 遷移先状態を設定
    gsu8_SYSCTL_SystemState = m_SYSCTL_STATE_LED_READY;
  }
}

static void SYSCTL_Entry_LED_Ready(void) {
  GET_LED_Task_DisplayInfoMsg()->str_to_display = "LED Ready";
}

static void SYSCTL_Do_Configure(void) {
  // wip
}

static void SYSCTL_Judge_Network_Ready(void) {
  uint8_t u8_is_network_setup_done = GET_Network_Task_WiFi_Connected();

  // 遷移条件は、(現在LEDReady状態 && ネットワークのセットアップが完了していること)
  if ((gsu8_SYSCTL_SystemState == m_SYSCTL_STATE_LED_READY)
    && (u8_is_network_setup_done == m_ON)) {
    // 遷移先状態を設定
    gsu8_SYSCTL_SystemState = m_SYSCTL_STATE_NETWORK_READY;
  }
}

static void SYSCTL_Entry_Network_Ready(void) {
  // wip
}

/**
 * @brief バックグラウンドタスク
 * @note 16ms周期で呼び出し
 * 
 */
void SYSCTL_Background_Task_Main(void) {
  static uint32_t su32_cnt = 0;
  const uint8_t cu8_system_state = gsu8_SYSCTL_SystemState;
  uint8_t cu8_wifi_connected_flg = GET_Network_Task_WiFi_Connected();

  // 状態遷移
  SYSCTL_State_Control();

  // NVMタスク
  NVM_Main();
  
  // ネットワークタスク
  if (su32_cnt % m_SYSCTL_SUBTASK_INVL_NETWORK == 0) {
    Network_Task_Main();
  }

  // WebServer
  if (cu8_wifi_connected_flg = m_ON) {  /* WiFiが接続できている場合 */
    Network_Task_WebServer_Main();
  }

  // LEDメインタスク
  LED_Task_Main();

  // カウンタ更新
  if (su32_cnt == m_SYSCTL_SUBTASK_INVL_NETWORK) {
    su32_cnt = 0;
  } else {
    su32_cnt += m_SYSCTL_CALL_ITVL_BACKGROUND;
  }
}

uint8_t Get_SYSCTL_SystemState(void) {
  return gsu8_SYSCTL_SystemState;
}
