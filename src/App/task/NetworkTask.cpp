/**
 * @file NetworkTask.c
 * @author ryhoh
 * @brief Network task source file.
 * 
 */

// インクルード
#include "App/task/NetworkTask.h"

// 変数宣言
static ESP8266WebServer gsst_webserver(80); 
static const char gscc_weekday[m_NETWORK_TASK_WEEKDAY_STR_SIZE] = "日月火水木金土";
static String gsstr_wifi_ssid = "";
static String gsstr_wifi_passwd = "";
static String gsstr_wifi_device_name = "";

// プロトタイプ宣言
static void Network_Task_AP_EntryPoint_root(void);
static void Network_Task_AP_EntryPoint_submit(void);
static void Network_Task_Make_Connection(void);
static void Network_Task_RunAPMode(void);
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
  EEPROM.get(0x00, u8_buffer);
  gsstr_wifi_ssid = String(u8_buffer);
  EEPROM.get(0x20, u8_buffer);
  gsstr_wifi_passwd = String(u8_buffer);
  EEPROM.get(0x40, u8_buffer);
  gsstr_wifi_device_name = String(u8_buffer);

  WiFi.mode(WIFI_STA);
  WiFi.begin(gsstr_wifi_ssid, gsstr_wifi_passwd);
}

/**
 * @brief APモード初期化関数
 * @note 起動時に1回だけ実行される
 * 
 */
void Network_Task_Init_APMode(void) {
  const String cstr_ap_ssid = Get_VARIANT_MachineName();
  const String cstr_ap_passwd = m_NETWORK_TASK_AP_PASSWD;

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(m_NETWORK_TASK_AP_IP, m_NETWORK_TASK_AP_GATEWAY, m_NETWORK_TASK_AP_SUBNET);
  WiFi.softAP(cstr_ap_ssid, cstr_ap_passwd);
  const IPAddress ip = WiFi.softAPIP();

  GET_LED_Task_DisplayInfoMsg()->str_to_display
    = String("AP:") + cstr_ap_ssid
    + String(" ") + (String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]));

  gsst_webserver.on("/", HTTP_GET, Network_Task_AP_EntryPoint_root);
  gsst_webserver.on("/submit", HTTP_POST, Network_Task_AP_EntryPoint_submit);
  gsst_webserver.onNotFound([]() {
    gsst_webserver.send(404, "<html><body>404 Not Found</body></html>");
  });

  gsst_webserver.begin();
}

static void Network_Task_AP_EntryPoint_root(void) {
  #include "App/task/root.html";
  gsst_webserver.send(200, "text/html", );
}

static void Network_Task_AP_EntryPoint_submit(void) {
  if (!(gsst_webserver.hasArg("ssid") && gsst_webserver.hasArg("password") && gsst_webserver.hasArg("dev_name"))) {
      gsst_webserver.send(400, "text/html", "<html><body>Please fill forms</body></html>");
      return;
  }

  // 受け取ったパラメータをNVMに書き込む
  if ((gsst_webserver.hasArg("ssid"))
   && (gsst_webserver.arg("ssid").length() > 0)) {
    Set_NVM_SSID(gsst_webserver.arg("ssid"));
  }

  if ((gsst_webserver.hasArg("password"))
   && (gsst_webserver.arg("password").length() > 0)) {
    Set_NVM_PASSWD(gsst_webserver.arg("password"));
  }

  if ((gsst_webserver.hasArg("dev_name"))
   && (gsst_webserver.arg("dev_name").length() > 0)) {
    Set_NVM_HostName(gsst_webserver.arg("dev_name"));
  }

  // ブラウザに結果を表示
  gsst_webserver.send(200, "text/html", "<html><body>Registered!<br>Please set run-mode and reboot.</body></html>");

  // マトリクスLEDに結果を表示
  GET_LED_Task_DisplayInfoMsg()->str_to_display = String("Registered: ") + gsst_webserver.arg("ssid");
}

/**
 * @brief ネットワークタスクメイン関数
 * @note 320ms周期で実行される
 * 
 */
void Network_Task_Main(void) {
  // SYSCTL_WaitForBlockingLevel(m_SYSCTL_BLOCKING_LEVEL_NETWORK);

  static time_t t;
  static struct tm *tm;
  static int32_t last_mday = -1;
  uint8_t u8_system_State = Get_SYSCTL_SystemState();

  Network_Task_Make_Connection();

  if (u8_system_State == m_SYSCTL_STATE_CONFIGURE) {
    Network_Task_RunAPMode();
  } else if ((u8_system_State == m_SYSCTL_STATE_NETWORK_READY)
          || (u8_system_State == m_SYSCTL_STATE_DRIVE)) {
    /* Check Date,Time */
    if (last_mday == -1 || last_mday == tm->tm_mday) {  // Date changed
      // Routines which run only one time each day
      // configTzTime("JST-9", "time.cloudflare.com", "ntp.nict.jp", "ntp.jst.mfeed.ad.jp");
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

    // *Get_SYSCTL_SetupState() = m_OFF;  // システムセットアップ完了
  }
}

static void Network_Task_Make_Connection(void) {
  if (WiFi.status() == WL_CONNECTED) {
    Set_SYSCTL_NetworkSetupState(m_ON);
  }
}

/**
 * @brief ホストモード実行関数
 * @note SSIDなどの設定をwebブラウザ経由で提供する
 *       320ms周期で実行される
 * 
 */
static void Network_Task_RunAPMode(void) {
  gsst_webserver.handleClient();
}

static void Network_Task_Select_NTP_Server_Randomly(void) {
  uint8_t u8_random = call_randint(3);
  switch (u8_random) {
    case 0:
      configTime("JST-9", "time.cloudflare.com");
      break;
    case 1:
      configTime("JST-9", "ntp.nict.jp");
      break;
    case 2:
      configTime("JST-9", "ntp.jst.mfeed.ad.jp");
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
  return gsstr_wifi_ssid;
}
