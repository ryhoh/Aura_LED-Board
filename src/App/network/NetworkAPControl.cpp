/**
 * @file NetworkTask.c
 * @author ryhoh
 * @brief Network task source file.
 * 
 */

// インクルード
#include "App/network/NetworkAPControl.h"

// 変数宣言
static WebServer gsst_webserver(80);
static Network_Config_t gsst_NetworkAP_Network_Config_In;
static Network_Config_t gsst_NetworkAP_Network_Config_Out = { "", "", "" };

// プロトタイプ宣言
static void Network_Task_AP_EntryPoint_root(void);
static void Network_Task_AP_EntryPoint_submit(void);

// 関数定義
/**
 * @brief APモード初期化関数
 * @note 起動時に1回だけ実行される
 * 
 */
void Network_Task_Init_APMode(void) {
  gsst_NetworkAP_Network_Config_In = Get_NVM_Network_Config();
  const String cstr_ap_ssid = Get_VARIANT_MachineName();
  const String cstr_ap_passwd = m_NETWORK_TASK_AP_PASSWD;
  DisplayInfo_t *pst_display_info_msg = GET_LED_Task_DisplayInfoMsg();

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(m_NETWORK_TASK_AP_IP, m_NETWORK_TASK_AP_GATEWAY, m_NETWORK_TASK_AP_SUBNET);
  WiFi.softAP(cstr_ap_ssid, cstr_ap_passwd);
  const IPAddress ip = WiFi.softAPIP();

  pst_display_info_msg->str_to_display
#ifdef SIMULATOR
    = std::string("AP:") + cstr_ap_ssid
    + std::string(" ") + (std::to_string(ip[0]) + "." + std::to_string(ip[1]) + "." + std::to_string(ip[2]) + "." + std::to_string(ip[3]));
#else
    = String("AP:") + cstr_ap_ssid
    + String(" ") + (String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]));
#endif
  pst_display_info_msg->u8_is_updated = m_ON;

  gsst_webserver.on("/", HTTP_GET, Network_Task_AP_EntryPoint_root);
  gsst_webserver.on("/submit", HTTP_POST, Network_Task_AP_EntryPoint_submit);
  gsst_webserver.onNotFound([]() {
    gsst_webserver.send(404, "<html><body>404 Not Found</body></html>");
  });

  gsst_webserver.begin();
}

static void Network_Task_AP_EntryPoint_root(void) {
  #include "App/task/root.html"
  gsst_webserver.send(200, "text/html", cstr_html);
}

static void Network_Task_AP_EntryPoint_submit(void) {
  uint8_t u8_updated = m_OFF;
  Network_Config_t st_network_config = gsst_NetworkAP_Network_Config_In;

  // 受け取ったパラメータをNVMに書き込む
  if ((gsst_webserver.hasArg("ssid"))
   && (gsst_webserver.arg("ssid").length() > 0)) {
    st_network_config.str_ssid = gsst_webserver.arg("ssid");
    u8_updated = m_ON;
  }

  if ((gsst_webserver.hasArg("password"))
   && (gsst_webserver.arg("password").length() > 0)) {
    st_network_config.str_ssid = gsst_webserver.arg("password");
    u8_updated = m_ON;
  }

  // if ((gsst_webserver.hasArg("dev_name"))
  //  && (gsst_webserver.arg("dev_name").length() > 0)) {
  //   Set_NVM_HostName(gsst_webserver.arg("dev_name"));
  //   u8_updated = m_ON;
  // }

  if (u8_updated == m_OFF) {
    gsst_webserver.send(400, "text/html", "<html><body>Please fill forms</body></html>");
  } else {
    // ブラウザに結果を表示
    gsst_webserver.send(200, "text/html", "<html><body>Registered!<br>Please set run-mode and reboot.</body></html>");

    // マトリクスLEDに結果を表示
    GET_LED_Task_DisplayInfoMsg()->str_to_display = String("Registered: ") + gsst_webserver.arg("ssid");
  }

  // NVMに書き込み
  gsst_NetworkAP_Network_Config_Out = st_network_config;
}

/**
 * @brief ホストモード実行関数
 * @note SSIDなどの設定をwebブラウザ経由で提供する
 *       128ms周期で実行される
 * 
 */
void Network_Task_RunAPMode(void) {
  gsst_webserver.handleClient();
}

// インタフェース関数
Network_Config_t Get_NetworkAP_Network_Config(void) {
  return gsst_NetworkAP_Network_Config_Out;
}
