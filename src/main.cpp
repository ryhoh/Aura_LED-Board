#include <Arduino.h>
#include <time.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

#include "ESP8266TimerInterrupt.h"

#include "DotMatrixLED.h"
#include "StatusClient.h"

ESP8266Timer ITimer;

/**
 * EEPROM Mapping
 *
 * [0x00] - [0x1F] : SSID
 * [0x20] - [0x3F] : Password
 * [0x40] - [0x5F] : Device Name
 *
*/

char myName[31];

const char *AP_PASSWORD = "sample";
const char *AP_SSID = "Rycery LED-Board";

const char weekday[] = "日月火水木金土";

const uint32_t MY_LED = 13;
const uint32_t DAT = 5;
const uint32_t LAT = 4;
const uint32_t CLK = 14;
const uint32_t MODE = 16;
constexpr uint16_t MATRIX_N = 8;

ESP8266WebServer server(80);

Max7219 max7219;
MatrixLED matrixLEDs[MATRIX_N];
MatrixLED matrixLEDs_clock[MATRIX_N];
MatrixLED matrixLEDs_date[MATRIX_N];
MatrixLED matrixLEDs_msg[MATRIX_N];
uint8_t network_setupstate = 1;  // ネットワーク接続まで 1 にしておく
uint8_t setupstate = 1;  // 初回のマトリクス設定まで 1 にしておく
uint8_t msg_end = 0;

char WIFI_SSID[31];
char WIFI_PASSWORD[31];

StatusClientOption statusClientOption;
String lastMessage = "Hello World!";

void Main_Task(void);
void LED_Main_Task(void);
void Network_Main_Task(void);
void ep_root();
void ep_submit();

void setup() {
  EEPROM.begin(0x60);

  pinMode(MY_LED, OUTPUT);
  pinMode(DAT, OUTPUT);
  pinMode(LAT, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(MODE, INPUT_PULLDOWN_16);

  for (uint8_t j = 0; j < MATRIX_N; ++j) {
    matrixLEDs[j] = MatrixLED(8, 8);
    matrixLEDs_clock[j] = MatrixLED(8, 8);
    matrixLEDs_date[j] = MatrixLED(8, 8);
    matrixLEDs_msg[j] = MatrixLED(8, 8);
  }

  max7219 = Max7219(DAT, LAT, CLK, 1);
  max7219.flushMatrixLEDs(matrixLEDs, MATRIX_N);

  /* Setup Mode */
  if (digitalRead(MODE) == LOW) {
    writeJISsToMatrixLEDs(matrixLEDs, MATRIX_N, "Setup Mode", 0);
    max7219.flushMatrixLEDs(matrixLEDs, MATRIX_N);

    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(IPAddress(192, 168, 1, 1), IPAddress(192, 168, 1, 10), IPAddress(255, 255, 255, 0));
    WiFi.softAP(AP_SSID, AP_PASSWORD);
    IPAddress ip = WiFi.softAPIP();

    for (uint8_t j = 0; j < MATRIX_N; ++j)
      matrixLEDs[j].fill(false);
    writeJISsToMatrixLEDs(matrixLEDs, MATRIX_N, (String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3])).c_str(), 0);
    max7219.flushMatrixLEDs(matrixLEDs, MATRIX_N);

    server.on("/", HTTP_GET, ep_root);
    server.on("/submit", HTTP_POST, ep_submit);
    server.onNotFound([]() {
      server.send(404, "<html><body>404 Not Found</body></html>");
    });

    server.begin();

    while (true) {
      server.handleClient();
      delay(1);
    }
  }

  /* Run Mode */
  EEPROM.get(0x00, WIFI_SSID);
  EEPROM.get(0x20, WIFI_PASSWORD);
  EEPROM.get(0x40, myName);
  
  ITimer.attachInterruptInterval(16000, Main_Task);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while(WiFi.status() != WL_CONNECTED) {
    digitalWrite(MY_LED, HIGH);
    delay(200);
    digitalWrite(MY_LED, LOW);
    delay(200);
  }
  network_setupstate = 0;

  for (uint8_t j = 0; j < MATRIX_N; ++j)
    matrixLEDs[j].fill(false);

  writeJISsToMatrixLEDs(matrixLEDs, MATRIX_N, "Connected!", 0);
  max7219.flushMatrixLEDs(matrixLEDs, MATRIX_N);
}

void loop() {
}

void Main_Task(void) {
  // 16msごとに呼ばれる
  static uint32_t step = 0;

  // ネットワークタスクは320msごとなので、分周で実行
  if (step % 20 == 0) {
    Network_Main_Task();
  }

  // LEDタスクは毎回実行
  LED_Main_Task();

  // step を進める
  step++;
  if (step >= 20) {
    step = 0;
  }
}

void LED_Main_Task(void) {
  static uint32_t step = 0;
  const uint32_t STEP_10s =  625;
  const uint32_t STEP_20s = 1250;
  uint8_t step_end = false;

  if (setupstate == 1) {
    static uint32_t msg_end = 0;
    static uint32_t scroll_step = 0;
    static uint32_t scroll_step_inner = 0;
    String msg = "Connecting..." + String(WIFI_SSID);
    
    msg_end = writeScrollJIS(matrixLEDs_msg, MATRIX_N, "Connecting...", scroll_step);
    max7219.flushMatrixLEDs(matrixLEDs_msg, MATRIX_N);
    if (msg_end == 1) {
      // 次回のメッセージスクロールに備える
      msg_end = 0;
      scroll_step = 0;
      scroll_step_inner = 0;
    } else {
      // メッセージが終わっていないなら、スクロール
      scroll_step_inner++;  // 1ステップ内のカウンタ
      if (scroll_step_inner == 4) {
        scroll_step++;
        scroll_step_inner = 0;
      }
    }
  } else {
    if (step < STEP_10s) {
      max7219.flushMatrixLEDs(matrixLEDs_clock, MATRIX_N);
    } else if (step < STEP_20s) {
      max7219.flushMatrixLEDs(matrixLEDs_date, MATRIX_N);
    } else {
      static uint32_t msg_end = 0;
      static uint32_t scroll_step = 0;
      static uint32_t scroll_step_inner = 0;
      
      msg_end = writeScrollJIS(matrixLEDs_msg, MATRIX_N, lastMessage.c_str(), scroll_step);
      max7219.flushMatrixLEDs(matrixLEDs_msg, MATRIX_N);
      if (msg_end == 1) {
        // メッセージが終わったら、ステップ初期化
        step_end = true;
        // 次回のメッセージスクロールに備える
        msg_end = 0;
        scroll_step = 0;
        scroll_step_inner = 0;
      } else {
        // メッセージが終わっていないなら、スクロール
        scroll_step_inner++;  // 1ステップ内のカウンタ
        if (scroll_step_inner == 4) {
          scroll_step++;
          scroll_step_inner = 0;
        }
      }
    }

    step++;
    if (step_end == true) {
      step = 0;
    }
  }
}

void Network_Main_Task(void) {
  static time_t t;
  static struct tm *tm;
  static char date_s[] = "11月11日(火)";
  static char now_s[] = "12:34:56";
  static int32_t last_mday = -1;

  if (network_setupstate == 0) {
    /* Check Date,Time */
    if (last_mday == -1 || last_mday == tm->tm_mday) {  // Date changed
      // Routines which run only one time each day
      configTzTime("JST-9", "time.cloudflare.com", "ntp.nict.jp", "ntp.jst.mfeed.ad.jp");
    }

    /* Time */
    t = time(NULL);
    tm = localtime(&t);

    now_s[0] = '0' + tm->tm_hour / 10;
    now_s[1] = '0' + tm->tm_hour % 10;
    // now_s[2] = (tm->tm_sec % 2 != 0) ? ':' : ' ';  // 1秒おきにコロンを点滅
    now_s[3] = '0' + tm->tm_min / 10;
    now_s[4] = '0' + tm->tm_min % 10;
    now_s[6] = '0' + tm->tm_sec / 10;
    now_s[7] = '0' + tm->tm_sec % 10;

    for (uint8_t j = 0; j < MATRIX_N; ++j) {
      matrixLEDs_clock[j].fill(false);
    }
    writeJISsToMatrixLEDs(matrixLEDs_clock, MATRIX_N, now_s, 9);

    /* Date */
    date_s[0] = ((tm->tm_mon+1) / 10 == 0) ? ' ' : '1';
    date_s[1] = '0' + ((tm->tm_mon+1) % 10);
    date_s[5] = (tm->tm_mday / 10 == 0) ? ' ' : '0' + (tm->tm_mday / 10);
    date_s[6] = '0' + (tm->tm_mday % 10);
    date_s[11] = weekday[tm->tm_wday * 3];  // utf-8 1文字分を3byteで
    date_s[12] = weekday[tm->tm_wday * 3 + 1];
    date_s[13] = weekday[tm->tm_wday * 3 + 2];
    
    for (uint8_t j = 0; j < MATRIX_N; ++j) {
      matrixLEDs_date[j].fill(false);
    }
    writeJISsToMatrixLEDs(matrixLEDs_date, MATRIX_N, date_s, 1);

    /* Message From StatusBoard */
    // statusClientOption = postStatusToBoard(myName);
    // if (!statusClientOption.skipped()) {
    //   lastMessage = statusClientOption.retval;
    //   lastMessage.replace("\n", " ");
    //   lastMessage.replace("\t", " ");
    //   lastMessage.replace("\r", " ");
    //   lastMessage.trim();
    // }
    lastMessage = "Hello world!";  // @@暫定

    setupstate = 0;
  }
}

void ep_root() {
  server.send(200, "text/html",
"<!DOCTYPE html>"
"<html lang=\"en\">"
"<head>"
"  <meta charset=\"UTF-8\">"
"  <title>Rycery LED-Board Setup</title>"
"</head>"
"<body>"
"  <h1>Rycery LED-Board Setup</h1>"
"  <p>Please Register ssid and password.</p>"
"  <form method=\"POST\" action=\"/submit\">"
"    SSID: <input type=\"text\" name=\"ssid\" maxlength=\"30\"><br>"
"    Password: <input type=\"password\" name=\"password\" maxlength=\"30\"><br>"
"    Device Name: <input type=\"text\" name=\"dev_name\" maxlength=\"30\"><br>"
"    <input type=\"submit\" value=\"Submit\">"
"  </form>"
"</body>"
"</html>");
}

void ep_submit() {
  if (!(server.hasArg("ssid") && server.hasArg("password") && server.hasArg("dev_name"))) {
      server.send(400, "text/html", "<html><body>400 Bad Request</body></html>");
      return;
  }

  char buffer[31];
  buffer[30] = '\0';
  strncpy(buffer, server.arg("ssid").c_str(), 30);
  EEPROM.put(0x00, buffer);
  strncpy(buffer, server.arg("password").c_str(), 30);
  EEPROM.put(0x20, buffer);
  strncpy(buffer, server.arg("dev_name").c_str(), 30);
  EEPROM.put(0x40, buffer);
  EEPROM.commit();

  server.send(200, "text/html", "<html><body>Registered!<br>Please set run-mode and reboot.</body></html>");

  for (uint8_t j = 0; j < MATRIX_N; ++j)
    matrixLEDs[j].fill(false);
  
  uint32_t scrollstep = 0;
  uint8_t scrollend = 0;
  while (scrollend == 0) {
    scrollend = writeScrollJIS(
      matrixLEDs,
      MATRIX_N,
      (String("Registered: ") + server.arg("ssid")).c_str(),
      scrollstep);
    ++scrollstep;
    delay(80);
  }

  writeAsciisToMatrixLEDs(matrixLEDs, MATRIX_N, "Registered.", 0);
  max7219.flushMatrixLEDs(matrixLEDs, MATRIX_N);
}
