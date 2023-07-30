/**
 * @file NetworkTask.c
 * @author ryhoh
 * @brief Network task source file.
 * 
 */

// インクルード
#include "App/network/NetworkSTAControl.h"

// マクロ宣言
#define m_NETWORK_TASK_WEEKDAY_STR_SIZE (22)  // 曜日文字列サイズ： 7曜日 * 3文字 + 1終端文字
#define m_NETWORK_TASK_TIME_STR_SIZE (9)  // 時刻文字列サイズ
#define m_NETWORK_TASK_DATE_STR_SIZE (16)  // 日付文字列サイズ
#define m_NETWORK_TASK_TIME_LEFT_OFFSET (9)
#define m_NETWORK_TASK_DATE_LEFT_OFFSET (1)

// 変数宣言
static const char gscc_weekday[m_NETWORK_TASK_WEEKDAY_STR_SIZE] = "日月火水木金土";
static Network_Config_t gsst_Network_Config;

// プロトタイプ宣言
static void Network_Task_Make_Connection(void);
static void Network_Task_Select_NTP_Server_Randomly(void);
static void Network_Task_SubTaskClock(const struct tm *tm);
static void Network_Task_SubTaskDate(const struct tm *tm);
static void Network_Task_SubTaskMsg(void);

// 関数定義
/**
 * @brief ネットワークタスク初期化関数
 * @note 起動時に1回だけ実行される
 * 
 */
void Network_Task_Init(void) {
  // ssid はNVMから読み込む
  char u8_buffer[31] = { 0 };
  gsst_Network_Config = Get_NVM_Network_Config();
  WiFi.mode(WIFI_STA);
  WiFi.begin(gsst_Network_Config.str_ssid.c_str(), gsst_Network_Config.str_passwd.c_str());
}

/**
 * @brief ネットワークタスクメイン関数
 * @note 320ms周期で実行される
 * 
 */
void Network_Task_Main(void) {
  static time_t t;
  static struct tm *tm;
  static int32_t last_mday = -1;
  uint8_t u8_system_State = Get_SYSCTL_SystemState();

  if (u8_system_State != m_SYSCTL_STATE_CONFIGURE) {
    Network_Task_Make_Connection();
  }

  if ((u8_system_State == m_SYSCTL_STATE_NETWORK_READY)
   || (u8_system_State == m_SYSCTL_STATE_DRIVE)) {
    /* Check Date,Time */
    if (last_mday == -1 || last_mday != tm->tm_mday) {  // Date changed
      // Routines which run only one time each day
      Network_Task_Select_NTP_Server_Randomly();
    }

    t = time(NULL);
    tm = localtime(&t);

    /* Clock */
    Network_Task_SubTaskClock(tm);

    /* Date */
    Network_Task_SubTaskDate(tm);
    
    /* Message From StatusBoard */
    Network_Task_SubTaskMsg();

    last_mday = tm->tm_mday;
  }
}

static void Network_Task_Make_Connection(void) {
  if (WiFi.status() == WL_CONNECTED) {
    Set_SYSCTL_NetworkSetupState(m_ON);
  }
}

static void Network_Task_Select_NTP_Server_Randomly(void) {
  uint8_t u8_random = call_randint(3);
  switch (u8_random) {
    case 0:
      configTime(9 * 3600L, 0, "time.cloudflare.com");
      break;
    case 1:
      configTime(9 * 3600L, 0, "ntp.nict.jp");
      break;
    case 2:
      configTime(9 * 3600L, 0, "ntp.jst.mfeed.ad.jp");
      break;
    default:
      break;
  }
}

/**
 * @brief 時刻設定関数
 * @note 320ms周期で実行される
 * 
 * @param tm 時刻構造体
 */
static void Network_Task_SubTaskClock(const struct tm *tm) {
  char now_s[m_NETWORK_TASK_TIME_STR_SIZE] = "12:34:56";
  DisplayInfo_t *pst_matrixLEDs_clock = GET_LED_Task_DisplayInfoClock();

  now_s[0] = '0' + tm->tm_hour / 10;
  now_s[1] = '0' + tm->tm_hour % 10;
  // now_s[2] = (tm->tm_sec % 2 != 0) ? ':' : ' ';  // 1秒おきにコロンを点滅
  now_s[3] = '0' + tm->tm_min / 10;
  now_s[4] = '0' + tm->tm_min % 10;
  now_s[6] = '0' + tm->tm_sec / 10;
  now_s[7] = '0' + tm->tm_sec % 10;

  noInterrupts();
  pst_matrixLEDs_clock->str_to_display = String(now_s);
  pst_matrixLEDs_clock->u32_offset_from_left = m_NETWORK_TASK_TIME_LEFT_OFFSET;
  interrupts();
}

/**
 * @brief 日付設定関数
 * @note 320ms周期で実行される
 * 
 * @param tm 時刻構造体
 */
static void Network_Task_SubTaskDate(const struct tm *tm) {
  static char date_s[m_NETWORK_TASK_DATE_STR_SIZE] = "11月11日(火)";
  DisplayInfo_t *pst_matrixLEDs_date = GET_LED_Task_DisplayInfoDate();

  date_s[0] = ((tm->tm_mon+1) / 10 == 0) ? ' ' : '1';
  date_s[1] = '0' + ((tm->tm_mon+1) % 10);
  date_s[5] = (tm->tm_mday / 10 == 0) ? ' ' : '0' + (tm->tm_mday / 10);
  date_s[6] = '0' + (tm->tm_mday % 10);
  date_s[11] = gscc_weekday[tm->tm_wday * 3];  // utf-8 1文字分を3byteで
  date_s[12] = gscc_weekday[tm->tm_wday * 3 + 1];
  date_s[13] = gscc_weekday[tm->tm_wday * 3 + 2];

  noInterrupts();
  pst_matrixLEDs_date->str_to_display = String(date_s);
  pst_matrixLEDs_date->u32_offset_from_left = m_NETWORK_TASK_DATE_LEFT_OFFSET;
  interrupts();
}

/**
 * @brief メッセージ設定関数
 * @note 320ms周期で実行される
 * 
 */
static void Network_Task_SubTaskMsg(void) {
  DisplayInfo_t *pst_matrixLEDs_msg = GET_LED_Task_DisplayInfoMsg();
  // statusClientOption = postStatusToBoard(myName);
  // if (!statusClientOption.skipped()) {
  //   lastMessage = statusClientOption.retval;
  //   lastMessage.replace("\n", " ");
  //   lastMessage.replace("\t", " ");
  //   lastMessage.replace("\r", " ");
  //   lastMessage.trim();
  // }
  // Set_SYSCTL_Blocking_Level(m_SYSCTL_BLOCKING_LEVEL_LED);
  noInterrupts();
  pst_matrixLEDs_msg->str_to_display = "Hello world!";  // @@暫定
  interrupts();
  // Unset_SYSCTL_Blocking_Level(m_SYSCTL_BLOCKING_LEVEL_LED);
}


String GET_Network_WiFi_SSID(void) {
  return gsst_Network_Config.str_ssid;
}
