/**
 * @file NetworkTask.c
 * @author ryhoh
 * @brief Network task source file.
 * 
 */

// インクルード
#include "App/network/NetworkWebServer.h"

// 変数宣言
static WebServer gsst_webserver(80);

// プロトタイプ宣言
static void Network_Task_WebServer_EntryPoint_root(void);
// static void Network_Task_AP_EntryPoint_submit(void);

// 関数定義
/**
 * @brief APモード初期化関数
 * @note 起動時に1回だけ実行される
 * 
 */
void Network_Task_Init_WebServer(void) {
  const String cstr_ap_ssid = Get_VARIANT_MachineName();


  gsst_webserver.on("/", HTTP_GET, Network_Task_WebServer_EntryPoint_root);
  // gsst_webserver.on("/submit", HTTP_POST, Network_Task_AP_EntryPoint_submit);
  gsst_webserver.onNotFound([]() {
    gsst_webserver.send(404, "<html><body>404 Not Found</body></html>");
  });

  gsst_webserver.begin();
}

static void Network_Task_WebServer_EntryPoint_root(void) {
  #include "App/task/root.html"
  gsst_webserver.send(200, "text/html", cstr_html);
}

// static void Network_Task_AP_EntryPoint_submit(void) {
  /* WiFiの設定は廃止だが、表示メッセージの設定機能を作る可能性があるため、残しておく */

  // uint8_t u8_updated = m_OFF;
  // Network_Config_t st_network_config = gsst_NetworkAP_Network_Config_In;

  // // 受け取ったパラメータをNVMに書き込む
  // if ((gsst_webserver.hasArg("ssid"))
  //  && (gsst_webserver.arg("ssid").length() > 0)) {
  //   st_network_config.str_ssid = gsst_webserver.arg("ssid");
  //   u8_updated = m_ON;
  // }

  // if ((gsst_webserver.hasArg("password"))
  //  && (gsst_webserver.arg("password").length() > 0)) {
  //   st_network_config.str_ssid = gsst_webserver.arg("password");
  //   u8_updated = m_ON;
  // }

  // if (u8_updated == m_OFF) {
  //   gsst_webserver.send(400, "text/html", "<html><body>Please fill forms</body></html>");
  // } else {
  //   // ブラウザに結果を表示
  //   gsst_webserver.send(200, "text/html", "<html><body>Registered!<br>Please set run-mode and reboot.</body></html>");
  // }

  // // NVMに書き込み
  // gsst_NetworkAP_Network_Config_Out = st_network_config;
// }

/**
 * @brief ホストモード実行関数
 * @note SSIDなどの設定をwebブラウザ経由で提供する
 *       128ms周期で実行される
 * 
 */
void Network_Task_WebServer_Main(void) {
  gsst_webserver.handleClient();
}
