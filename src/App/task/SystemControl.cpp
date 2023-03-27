/**
 * @file SystemControl.cpp
 * @author ryhoh
 * @brief システム制御ソースファイル
 * 
 */

// インクルード
#include "SystemControl.h"

// 変数宣言
static uint8_t gsu8_is_LED_setup_done = m_OFF;  // LEDセットアップ完了フラグ
static uint8_t gsu8_is_network_setup_done = m_OFF;  // ネットワークセットアップ完了フラグ
static uint8_t gsu8_SYSCTL_SystemState = m_SYSCTL_STATE_POWER_ON;  // システム制御状態

static uint8_t gsu8_SYSCTL_Blocking_Flags[m_SYSCTL_BLOCKING_LEVEL_NUM] = { 0 };  // 割り込み禁止フラグ

// プロトタイプ宣言
static void SYSCTL_SystemControl_Task_Main(void);
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

// 状態遷移テーブル
static TransitionTable_t gsst_SYSCTL_StateTransition_Tbl[m_SYSCTL_STATE_TRANSITION_NUM] = {
  /* Judge                       Entry                        Do                  Exit */
  { NULL,                        NULL,                        NULL,                 NULL },  // (PowerOn)
  { &SYSCTL_Judge_LED_Ready,     &SYSCTL_Entry_LED_Ready,     NULL,                 NULL },  // PowerOn      -> LEDReady
  { &SYSCTL_Judge_Configure,     &SYSCTL_Entry_Configure,     &SYSCTL_Do_Configure, NULL },  // LEDReady     -> Configure
  { &SYSCTL_Judge_Network_Ready, &SYSCTL_Entry_Network_Ready, NULL,                 NULL },  // LEDReady     -> NetworkReady
  { &SYSCTL_Judge_Drive,         &SYSCTL_Entry_Drive,         NULL,                 NULL },  // NetworkReady -> Drive
};

// 関数定義
/**
 * @brief システム制御初期化
 * 
 */
void SYSCTL_Init(void) {
  // NVMの初期化
  NVM_Init();

  // ピンモード設定
  call_pinMode(Get_VARIANT_LampPin(), OUTPUT);
  call_pinMode(Get_VARIANT_SPIDataPin(), OUTPUT);
  call_pinMode(Get_VARIANT_SPICSPin(), OUTPUT);
  call_pinMode(Get_VARIANT_SPIClockPin(), OUTPUT);
  call_pinMode(Get_VARIANT_ModePin(), INPUT_PULLDOWN);

  // LED のセットアップを行う
  LED_Task_Init();

  // ネットワークのセットアップを行う
  Network_Task_Init();
}

/**
 * @brief メインタスク
 * @note 16ms周期で呼び出し
 * 
 */
void Main_Task(void) {
  static uint32_t step = 0;
  const uint32_t cu32_network_substep = m_SYSCTL_CALL_ITVL_NETWORK / m_SYSCTL_CALL_ITVL;

  // システム制御タスク
  SYSCTL_SystemControl_Task_Main();
  
  // LEDタスクは毎回実行
  LED_Task_Main();

  // step を進める
  step++;
  if (step >= cu32_network_substep) {
    step = 0;
  }
}

/**
 * @brief システム制御タスク
 * @note 16ms周期で呼び出し
 * 
 */
static void SYSCTL_SystemControl_Task_Main(void) {
  // 状態遷移
  SYSCTL_State_Control();
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
  // 遷移条件は、(現在PowerOn状態 && LEDタスクがセットアップ完了であること)
  if ((gsu8_SYSCTL_SystemState == m_SYSCTL_STATE_POWER_ON)
    && (gsu8_is_LED_setup_done == m_ON)) {
    // 遷移先状態を設定
    gsu8_SYSCTL_SystemState = m_SYSCTL_STATE_LED_READY;
  }
}

static void SYSCTL_Entry_LED_Ready(void) {
  GET_LED_Task_DisplayInfoMsg()->str_to_display = "LED Ready";
}

static void SYSCTL_Judge_Configure(void) {
  // 遷移条件は、LED準備完了状態 && モードピン=LOWであること
  const uint8_t cu8_mode_pin = Get_VARIANT_ModePin();
  const int32_t i32_mode = call_digitalRead(cu8_mode_pin);
  if ((gsu8_SYSCTL_SystemState == m_SYSCTL_STATE_LED_READY)
    && (i32_mode == LOW)) {
    // 遷移先状態を設定
    gsu8_SYSCTL_SystemState = m_SYSCTL_STATE_CONFIGURE;
  }
}

static void SYSCTL_Entry_Configure(void) {
  Network_Task_Init_APMode();
}

static void SYSCTL_Do_Configure(void) {
  // wip
}

static void SYSCTL_Judge_Network_Ready(void) {
  // 遷移条件は、(現在LEDReady状態 && ネットワークのセットアップが完了していること)
  if ((gsu8_SYSCTL_SystemState == m_SYSCTL_STATE_LED_READY)
    && (gsu8_is_network_setup_done == m_ON)) {
    // 遷移先状態を設定
    gsu8_SYSCTL_SystemState = m_SYSCTL_STATE_NETWORK_READY;
  }
}

static void SYSCTL_Entry_Network_Ready(void) {
  // wip
}

static void SYSCTL_Judge_Drive(void) {
  // 遷移条件は、(現在NetworkReady状態)
  if (gsu8_SYSCTL_SystemState == m_SYSCTL_STATE_NETWORK_READY) {
    // 遷移先状態を設定
    gsu8_SYSCTL_SystemState = m_SYSCTL_STATE_DRIVE;
  }
}

static void SYSCTL_Entry_Drive(void) {
  // wip
  LED_Task_FirstTimeToRunningState();
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

void Set_SYSCTL_LEDSetupState(uint8_t u8_done) {
    gsu8_is_LED_setup_done = u8_done;
}

void Set_SYSCTL_NetworkSetupState(uint8_t u8_done) {
    gsu8_is_network_setup_done = u8_done;
}

uint8_t Get_SYSCTL_SystemState(void) {
    return gsu8_SYSCTL_SystemState;
}
