/**
 * @file LEDTask.c
 * @author ryhoh
 * @brief LED制御タスク
 * 
 */

// インクルード
#include "App/task/LEDTask.h"

// 定数

/* 以下は暫定で、本来はスクロール1周完了を検知すべき */
#define m_LED_TASK_IPADDRESS_DISPLAY_TIME (800)  // IPアドレス表示時間 [ms,16] 12.8秒

// 変数宣言
static DisplayInfo_t gsst_displayInfo_clock = DisplayInfo_t {
  .u32_offset_from_left = 0,
  .u8_is_updated = m_OFF,
  .str_to_display = String(""),
};
static DisplayInfo_t gsst_displayInfo_date = DisplayInfo_t {
  .u32_offset_from_left = 0,
  .u8_is_updated = m_OFF,
  .str_to_display = String(""),
};
static DisplayInfo_t gsst_displayInfo_msg = DisplayInfo_t {
  .u32_offset_from_left = 0,
  .u8_is_updated = m_OFF,
  .str_to_display = String(""),
};

static MatrixLED matrixLEDs_clock[m_PROFILE_MAX_DESIGNED_PANEL_NUM];  // 時計表示用
static MatrixLED matrixLEDs_date[m_PROFILE_MAX_DESIGNED_PANEL_NUM];  // 日付表示用
static MatrixLED matrixLEDs_msg[m_PROFILE_MAX_DESIGNED_PANEL_NUM];  // メッセージ表示用
static MatrixLED matrixLEDs_output[m_PROFILE_MAX_DESIGNED_PANEL_NUM];  // 出力用
static Max7219 gsst_max7219;
static uint8_t gsu8_is_LED_setup_done = m_OFF;  // LEDセットアップ完了フラグ
static uint8_t gsu8_is_LED_DisplayUpdateRequiredFlg = m_OFF;  // LED表示更新要求フラグ

// プロトタイプ宣言
static void LED_Task_ConfigureDisplayData(void);
static void LED_Task_ScrollLoop(const String str_msg);
static void LED_Task_RunningState(void);
static void LED_Task_SubTaskClock(void);
static void LED_Task_copyMatrixLEDs(MatrixLED *dst, MatrixLED *src);
static void LED_Task_SubTaskDate(void);
static void LED_Task_OutputMain(void);
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
    matrixLEDs_output[u8_i].height = 8;
    matrixLEDs_output[u8_i].width = 8;
    matrixLEDs_output[u8_i].fill(0);
  }

  // MAX7219初期化
  const uint8_t cu8_data_pin = Get_VARIANT_SPIDataPin();
  const uint8_t cu8_cs_pin = Get_VARIANT_SPICSPin();
  const uint8_t cu8_clk_pin = Get_VARIANT_SPIClockPin();
  const uint8_t cu8_brightness = Get_VARIANT_Brightness();
  const uint32_t cu32_cascading_num = Get_VARIANT_MatrixNum();
  gsst_max7219 = Max7219(cu8_data_pin, cu8_cs_pin, cu8_clk_pin, cu8_brightness, cu32_cascading_num);

  // ディスプレイ表示初期化
  const uint8_t cu8_matrix_num = (uint8_t)(Get_VARIANT_MatrixNum() & 0xFF);
  gsst_max7219.flushMatrixLEDs(matrixLEDs_msg, cu8_matrix_num);

  // LEDセットアップ完了を報告
  gsu8_is_LED_setup_done = m_ON;
}

/**
 * @brief LED制御タスクの駆動モード移行処理
 * @note 状態遷移時1度だけ呼び出し
 * 
 */
void LED_Task_FirstTimeToRunningState(void) {
  LED_Task_SubTaskMsg("", true);  // メッセージ表示用の状態を初期化
}

/**
 * @brief LED制御タスク
 * @note 16ms周期で呼び出し
 * 
 */
void LED_Task_Main(void) {
  // 出力データセット処理など
  LED_Task_ConfigureDisplayData();

  // 出力処理
  LED_Task_OutputMain();
}

/**
 * @brief LED制御タスクにおける表示データのセット処理
 * @note 16ms周期で呼び出し
 * 
 */
static void LED_Task_ConfigureDisplayData(void) {
  const uint8_t cu8_system_state = Get_SYSCTL_SystemState();
  const Network_Config_t cst_Network_Config = Get_NVM_Network_Config();
  const IPAddress_t cst_IPAddress = GET_Network_Local_IPAddress();

  static uint8_t su8_ipaddr_displayed_flg = m_OFF;
  static uint16_t su16_ipaddr_displayed_cnt = 0;

  if (cu8_system_state == m_SYSCTL_STATE_LED_READY) {
    // LEDは使えるがネットワークが準備中なら
    const String str_msg = String(m_LED_TASK_CONNECTING_MSG) + cst_Network_Config.str_ssid;
    LED_Task_ScrollLoop(str_msg);
  } else if ((cu8_system_state == m_SYSCTL_STATE_NETWORK_READY)
           || (cu8_system_state == m_SYSCTL_STATE_DRIVE)) {
    // 初めてネットワークが準備完了したときに、スクロールメッセージにIPアドレスを表示する
    if (su8_ipaddr_displayed_flg == m_OFF) {
      const String str_msg =
          DEC2STR(cst_IPAddress.u8_octet1) + "."
        + DEC2STR(cst_IPAddress.u8_octet2) + "."
        + DEC2STR(cst_IPAddress.u8_octet3) + "."
        + DEC2STR(cst_IPAddress.u8_octet4);
      LED_Task_ScrollLoop(str_msg);

      // 表示状態を維持する
      if (su16_ipaddr_displayed_cnt < m_LED_TASK_IPADDRESS_DISPLAY_TIME) {
        M_CLIP_INC(su16_ipaddr_displayed_cnt, UINT16_MAX);
      } else {
        su8_ipaddr_displayed_flg = m_ON;
        LED_Task_SubTaskMsg("", true);  // 暫定 メッセージスクロールをリセット
      }
    } else {
      // 通常モード
      LED_Task_RunningState();
    }
  }

  // @@暫定 スクロールではLEDの表示更新要求フラグは常にONにする
  gsu8_is_LED_DisplayUpdateRequiredFlg = m_ON;
}

/**
 * @brief スクロールをループ表示する
 * @note 16ms周期で呼び出し
 * 
 */
static void LED_Task_ScrollLoop(const String str_msg) {
  static uint8_t su8_msg_end = true;  /* メッセージスクロール終了フラグ */
  static String sstr_msg_old = String("");  /* メッセージ前回値 */
  
  // 表示内容が変わったらリセット
  if (sstr_msg_old != str_msg) {
    su8_msg_end = LED_Task_SubTaskMsg("", true);
  }
  sstr_msg_old = str_msg;

  // スクロール処理
  su8_msg_end = LED_Task_SubTaskMsg(str_msg, false);

  // スクロール終了時にリセット
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
    u8_step_ended = LED_Task_SubTaskMsg("", true);
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

  // 内容が更新された場合のみ再描画
  if (gsst_displayInfo_clock.u8_is_updated == m_ON) {
    for (uint8_t j = 0; j < cu8_matrix_num; ++j) {
      matrixLEDs_clock[j].fill(false);
    }
    writeJISsToMatrixLEDs(
      matrixLEDs_clock,
      cu8_matrix_num,
      gsst_displayInfo_clock.str_to_display.c_str(),
      gsst_displayInfo_clock.u32_offset_from_left
    );
    
    LED_Task_copyMatrixLEDs(matrixLEDs_output, matrixLEDs_clock);

    gsst_displayInfo_clock.u8_is_updated = m_OFF;
    gsu8_is_LED_DisplayUpdateRequiredFlg = m_ON;
  } else {
    gsu8_is_LED_DisplayUpdateRequiredFlg = m_OFF;
  }
}

/**
 * @brief matrixLEDs バッファのコピー
 * 
 */
static void LED_Task_copyMatrixLEDs(MatrixLED *dst, MatrixLED *src) {
  for (uint8_t u8_i = 0; u8_i < m_PROFILE_MAX_DESIGNED_PANEL_NUM; ++u8_i) {
    dst[u8_i] = src[u8_i];
  }
}

/**
 * @brief 日付表示サブタスク
 * @note 16ms周期で呼び出し
 * 
 */
static void LED_Task_SubTaskDate(void) {
  const uint8_t cu8_matrix_num = (uint8_t)(Get_VARIANT_MatrixNum() & 0xFF);

  // 内容が更新された場合のみ再描画
    for (uint8_t j = 0; j < cu8_matrix_num; ++j) {
      matrixLEDs_date[j].fill(false);
    }
    writeJISsToMatrixLEDs(
      matrixLEDs_date,
      cu8_matrix_num,
      gsst_displayInfo_date.str_to_display.c_str(),
      gsst_displayInfo_date.u32_offset_from_left
    );
    
    LED_Task_copyMatrixLEDs(matrixLEDs_output, matrixLEDs_date);

    gsst_displayInfo_date.u8_is_updated = m_OFF;
    gsu8_is_LED_DisplayUpdateRequiredFlg = m_ON;
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
  LED_Task_copyMatrixLEDs(matrixLEDs_output, matrixLEDs_msg);
  return u8_step_ended;
}

/**
 * @brief LEDの出力処理メイン
 * 
 */
static void LED_Task_OutputMain(void) {
  const uint8_t cu8_matrix_num = (uint8_t)(Get_VARIANT_MatrixNum() & 0xFF);

  // 表示要求があれば出力
  if (gsu8_is_LED_DisplayUpdateRequiredFlg == m_ON) {
    gsst_max7219.flushMatrixLEDs(matrixLEDs_output, cu8_matrix_num);
  }
}

/**
 * インタフェース
 * 
 */
uint8_t GET_LED_Task_Setup_Done(void) {
  return gsu8_is_LED_setup_done;
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
