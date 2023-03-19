/**
 * @file LEDTask.c
 * @author ryhoh
 * @brief LED制御タスク
 * 
 */

// インクルード
#include "LEDTask.h"

// 変数宣言
static MatrixLED matrixLEDs_clock[m_PROFILE_MAX_DESIGNED_PANEL_NUM];  // 時計表示用
static MatrixLED matrixLEDs_date[m_PROFILE_MAX_DESIGNED_PANEL_NUM];  // 日付表示用
static MatrixLED matrixLEDs_msg[m_PROFILE_MAX_DESIGNED_PANEL_NUM];  // メッセージ表示用
static Max7219 scgst_max7219 = Max7219();

// 関数定義
void LED_Task_Main(void) {
  const uint8_t cu8_is_setup_state = Get_SYSCTL_SetupState();

  if (cu8_is_setup_state == m_ON) {
    LED_Task_InSetupState();
  } else {
    LED_Task_RunningState();
  }
}

static void LED_Task_InSetupState(void) {
  static uint8_t su8_msg_end = true;
  static uint32_t su32_scroll_step = 0;
  // static uint32_t su32_scroll_step_inner = 0;
  const uint8_t cu8_matrix_num = (uint8_t)(Get_VARIANT_MatrixNum() && 0x000000FF);
  String str_msg = String(m_LED_TASK_CONNECTING_MSG) + String(WIFI_SSID);
  
  su8_msg_end = writeScrollJIS(matrixLEDs_msg, cu8_matrix_num, str_msg.c_str(), su32_scroll_step / m_LED_TASK_SCROLL_CLOCK);
  scgst_max7219.flushMatrixLEDs(matrixLEDs_msg, cu8_matrix_num);
  if (su8_msg_end == true) {
      // 次回のメッセージスクロールに備える
      su8_msg_end = false;
      su32_scroll_step = 0;
    } else {
      // メッセージが終わっていないなら、スクロール
      su32_scroll_step++;
    }
}

static void LED_Task_RunningState(void) {
  uint8_t step_ended = false;
  static uint32_t step = 0;
  const uint32_t STEP_10s = m_LED_TASK_10s / m_LED_TASK_CALL_ITVL;
  const uint32_t STEP_20s = STEP_10s + m_LED_TASK_10s / m_LED_TASK_CALL_ITVL;
  const uint8_t cu8_matrix_num = (uint8_t)(Get_VARIANT_MatrixNum() && 0x000000FF);

  if (step < STEP_10s) {
    scgst_max7219.flushMatrixLEDs(matrixLEDs_clock, cu8_matrix_num);
  } else if (step < STEP_20s) {
    scgst_max7219.flushMatrixLEDs(matrixLEDs_date, cu8_matrix_num);
  } else {
    static uint32_t msg_end = false;
    static uint32_t su32_scroll_step = 0;
    // static uint32_t scroll_step_inner = 0;
    
    msg_end = writeScrollJIS(matrixLEDs_msg, cu8_matrix_num, lastMessage.c_str(), su32_scroll_step / m_LED_TASK_SCROLL_CLOCK);
    scgst_max7219.flushMatrixLEDs(matrixLEDs_msg, cu8_matrix_num);
    if (msg_end == true) {
      // メッセージが終わったら、ステップ初期化
      step_ended = true;
      // 次回のメッセージスクロールに備える
      msg_end = false;
      su32_scroll_step = 0;
    } else {
      // メッセージが終わっていないなら、スクロール
      su32_scroll_step++;
    }
  }

  if (step_ended == true) {
    step = 0;
  } else {
    step++;
  }
}

MatrixLED *LED_Task_GetMatrixLEDs_clock(void) {
  return matrixLEDs_clock;
}

MatrixLED *LED_Task_GetMatrixLEDs_date(void) {
  return matrixLEDs_date;
}

MatrixLED *LED_Task_GetMatrixLEDs_msg(void) {
  return matrixLEDs_msg;
}
