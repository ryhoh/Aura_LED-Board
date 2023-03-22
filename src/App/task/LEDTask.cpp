/**
 * @file LEDTask.c
 * @author ryhoh
 * @brief LED制御タスク
 * 
 */

// インクルード
#include "App/task/LEDTask.h"

// 変数宣言
static DisplayInfo_t gsst_displayInfo_clock = DisplayInfo_t {
  .u32_offset_from_left = 0,
  .str_to_display = String(""),
};
static DisplayInfo_t gsst_displayInfo_date = DisplayInfo_t {
  .u32_offset_from_left = 0,
  .str_to_display = String(""),
};
static DisplayInfo_t gsst_displayInfo_msg = DisplayInfo_t {
  .u32_offset_from_left = 0,
  .str_to_display = String(""),
};
static MatrixLED matrixLEDs_clock[m_PROFILE_MAX_DESIGNED_PANEL_NUM];  // 時計表示用
static MatrixLED matrixLEDs_date[m_PROFILE_MAX_DESIGNED_PANEL_NUM];  // 日付表示用
static MatrixLED matrixLEDs_msg[m_PROFILE_MAX_DESIGNED_PANEL_NUM];  // メッセージ表示用
static Max7219 gsst_max7219;

// プロトタイプ宣言
static void LED_Task_FirstTimeToRunningState(void);
static void LED_Task_InSetupState(void);
static void LED_Task_RunningState(void);
static void LED_Task_SubTaskClock(void);
static void LED_Task_SubTaskDate(void);
static uint8_t LED_Task_SubTaskMsg(const String str_msg, uint8_t u8_reset_required);
static uint8_t LED_Task_SubTaskMsg_SubRoutine(const String str_msg, uint32_t su32_scroll_step);

// 関数定義
/**
 * @brief LED初期化
 * @note 起動時に1度だけ呼び出し
 * 
 */
void LED_Task_Init(void) {
  // マトリクスLED初期化
  for (uint8_t u8_i = 0; u8_i < m_PROFILE_MAX_DESIGNED_PANEL_NUM; u8_i++) {
    matrixLEDs_clock[u8_i].height = 8;
    matrixLEDs_clock[u8_i].width = 8;
    matrixLEDs_clock[u8_i].fill(0);
    matrixLEDs_date[u8_i].height = 8;
    matrixLEDs_date[u8_i].width = 8;
    matrixLEDs_date[u8_i].fill(0);
    matrixLEDs_msg[u8_i].height = 8;
    matrixLEDs_msg[u8_i].width = 8;
    matrixLEDs_msg[u8_i].fill(0);
  }

  // MAX7219初期化
  gsst_max7219 = Max7219(m_LED_TASK_SPI_PIN_DAT, m_LED_TASK_SPI_PIN_LAT, m_LED_TASK_SPI_PIN_CLK, m_LED_TASK_BRIGHTNESS_MIN);

  // ディスプレイ表示初期化
  const uint8_t cu8_matrix_num = (uint8_t)(Get_VARIANT_MatrixNum() & 0xFF);
  gsst_max7219.flushMatrixLEDs(matrixLEDs_msg, cu8_matrix_num);
}


/**
 * @brief LED制御タスク
 * @note 16ms周期で呼び出し
 * 
 */
void LED_Task_Main(void) {
  // SYSCTL_WaitForBlockingLevel(m_SYSCTL_BLOCKING_LEVEL_LED);

  const uint8_t cu8_is_setup_state = *Get_SYSCTL_SetupState();
  static uint8_t cu8_is_setup_state_old = m_ON;

  if (cu8_is_setup_state != cu8_is_setup_state_old) {
    // 駆動モードに入った最初のタイミング
    LED_Task_FirstTimeToRunningState();
  }

  if (cu8_is_setup_state == m_ON) {
    LED_Task_InSetupState();
  } else {
    LED_Task_RunningState();
  }

  cu8_is_setup_state_old = cu8_is_setup_state;
}

/**
 * @brief LED制御タスクの駆動モード移行処理
 * @note 16ms周期で呼び出し
 * 
 */
static void LED_Task_FirstTimeToRunningState(void) {
  LED_Task_SubTaskMsg("", true);  // メッセージ表示用の状態を初期化
}

/**
 * @brief LED制御タスクのセットアップモード処理
 * @note 16ms周期で呼び出し
 * 
 */
static void LED_Task_InSetupState(void) {
  static uint8_t su8_msg_end = true;
  const String str_msg = String(m_LED_TASK_CONNECTING_MSG) + GET_Network_WiFi_SSID();
  su8_msg_end = LED_Task_SubTaskMsg(str_msg, false);
  if (su8_msg_end == true) {
    LED_Task_SubTaskMsg("", true);  // メッセージ表示用の状態を初期化
    su8_msg_end = false;
  }
}

/**
 * @brief LED制御タスクの駆動モード処理
 * @note 16ms周期で呼び出し
 * 
 */
static void LED_Task_RunningState(void) {
  const uint32_t STEP_10s = m_LED_TASK_10s / m_LED_TASK_CALL_ITVL;
  const uint32_t STEP_20s = STEP_10s + m_LED_TASK_10s / m_LED_TASK_CALL_ITVL;
  const String cstr_lastMessage = gsst_displayInfo_msg.str_to_display;
  static uint32_t step = 0;
  uint8_t u8_step_ended = false;

  if (step < STEP_10s) {
    LED_Task_SubTaskClock();
  } else if (step < STEP_20s) {
    LED_Task_SubTaskDate();
  } else {
    u8_step_ended = LED_Task_SubTaskMsg(cstr_lastMessage, false);
  }

  if (u8_step_ended == true) {
    step = 0;
  } else {
    step++;
  }
}

/**
 * @brief 時計表示サブタスク
 * @note 16ms周期で呼び出し
 * 
 */
static void LED_Task_SubTaskClock(void) {
  const uint8_t cu8_matrix_num = (uint8_t)(Get_VARIANT_MatrixNum() & 0xFF);

  for (uint8_t j = 0; j < cu8_matrix_num; ++j) {
    matrixLEDs_clock[j].fill(false);
  }
  writeJISsToMatrixLEDs(
    matrixLEDs_clock,
    cu8_matrix_num,
    gsst_displayInfo_clock.str_to_display.c_str(),
    gsst_displayInfo_clock.u32_offset_from_left
  );
  gsst_max7219.flushMatrixLEDs(matrixLEDs_clock, cu8_matrix_num);
}

/**
 * @brief 日付表示サブタスク
 * @note 16ms周期で呼び出し
 * 
 */
static void LED_Task_SubTaskDate(void) {
  const uint8_t cu8_matrix_num = (uint8_t)(Get_VARIANT_MatrixNum() & 0xFF);

  for (uint8_t j = 0; j < cu8_matrix_num; ++j) {
    matrixLEDs_date[j].fill(false);
  }
  writeJISsToMatrixLEDs(
    matrixLEDs_date,
    cu8_matrix_num,
    gsst_displayInfo_date.str_to_display.c_str(),
    gsst_displayInfo_date.u32_offset_from_left
  );
  gsst_max7219.flushMatrixLEDs(matrixLEDs_date, cu8_matrix_num);
}

/**
 * @brief メッセージ表示サブタスク
 * @param u8_reset_required リセット要求フラグ（その場合はスクロール実行しない）
 * @note 16ms周期で呼び出し
 * 
 * @return uint8_t スクロール完了時にtrueを返す
 */
static uint8_t LED_Task_SubTaskMsg(const String str_msg, uint8_t u8_reset_required) {
  static uint32_t u32_msg_ended = false;
  static uint32_t su32_scroll_step = 0;
  
  // SetupState -> RunningStateに移行したときに、スクロールをリセットすることを想定
  if (u8_reset_required == true) {
    u32_msg_ended = false;
    su32_scroll_step = 0;
  } else {
    u32_msg_ended = LED_Task_SubTaskMsg_SubRoutine(str_msg, su32_scroll_step);
    su32_scroll_step++;
  }

  return u32_msg_ended;
}

/**
 * @brief メッセージ表示サブタスクのサブルーチン
 * 
 * @param u32_msg_ended メッセージ終了フラグ
 * @param su32_scroll_step スクロールステップ
 * @note 16ms周期で呼び出し
 * @return uint8_t スクロール完了時にtrueを返す
 */
static uint8_t LED_Task_SubTaskMsg_SubRoutine(const String str_msg, uint32_t su32_scroll_step) {
  const uint8_t cu8_matrix_num = (uint8_t)(Get_VARIANT_MatrixNum() & 0xFF);
  uint8_t u8_step_ended;

  u8_step_ended = writeScrollJIS(matrixLEDs_msg, cu8_matrix_num, str_msg.c_str(), su32_scroll_step / m_LED_TASK_SCROLL_CLOCK);
  gsst_max7219.flushMatrixLEDs(matrixLEDs_msg, cu8_matrix_num);
  return u8_step_ended;
}

DisplayInfo_t *GET_LED_Task_DisplayInfoClock(void) {
  return &gsst_displayInfo_clock;
}

DisplayInfo_t *GET_LED_Task_DisplayInfoDate(void) {
  return &gsst_displayInfo_date;
}

DisplayInfo_t *GET_LED_Task_DisplayInfoMsg(void) {
  return &gsst_displayInfo_msg;
}

// MatrixLED *LED_Task_GetMatrixLEDs_clock(void) {
//   return matrixLEDs_clock;
// }

// MatrixLED *LED_Task_GetMatrixLEDs_date(void) {
//   return matrixLEDs_date;
// }

// MatrixLED *LED_Task_GetMatrixLEDs_msg(void) {
//   return matrixLEDs_msg;
// }
