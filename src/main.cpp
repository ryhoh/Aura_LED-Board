#include <Arduino.h>
#include <time.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

#include "DotMatrixLED.h"
#include "StatusClient.h"

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

StatusClientOption statusClientOption;
String lastMessage = "Hello World!";

void ep_root();
void ep_submit();

void setup() {
  EEPROM.begin(0x60);

  pinMode(MY_LED, OUTPUT);
  pinMode(DAT, OUTPUT);
  pinMode(LAT, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(MODE, INPUT_PULLDOWN_16);

  for (uint8_t j = 0; j < MATRIX_N; ++j)
    matrixLEDs[j] = MatrixLED(8, 8);

  max7219 = Max7219(DAT, LAT, CLK, 1);
  delay(10);
  max7219.testRun();
  delay(10);
  max7219.flushMatrixLEDs(matrixLEDs, MATRIX_N);
  delay(100);

  /* Setup Mode */
  if (digitalRead(MODE) == LOW) {
    writeJISsToMatrixLEDs(matrixLEDs, MATRIX_N, "Setup Mode", 0);
    max7219.flushMatrixLEDs(matrixLEDs, MATRIX_N);

    WiFi.mode(WIFI_AP);
    delay(100);
    WiFi.softAPConfig(IPAddress(192, 168, 1, 1), IPAddress(192, 168, 1, 10), IPAddress(255, 255, 255, 0));
    delay(100);
    WiFi.softAP(AP_SSID, AP_PASSWORD);
    delay(100);
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
  writeJISsToMatrixLEDs(matrixLEDs, MATRIX_N, "Connecting...", 0);
  max7219.flushMatrixLEDs(matrixLEDs, MATRIX_N);

  delay(1000);

  WiFi.mode(WIFI_STA);
  char WIFI_SSID[31];
  char WIFI_PASSWORD[31];
  EEPROM.get(0x00, WIFI_SSID);
  EEPROM.get(0x20, WIFI_PASSWORD);
  EEPROM.get(0x40, myName);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while(WiFi.status() != WL_CONNECTED) {
    digitalWrite(MY_LED, HIGH);
    delay(200);
    digitalWrite(MY_LED, LOW);
    delay(1000);
  }

  for (uint8_t j = 0; j < MATRIX_N; ++j)
    matrixLEDs[j].fill(false);

  writeJISsToMatrixLEDs(matrixLEDs, MATRIX_N, "Connected!", 0);
  max7219.flushMatrixLEDs(matrixLEDs, MATRIX_N);
  delay(5000);
}

void loop() {
  static time_t t;
  static struct tm *tm;
  static char date_s[] = "11月11日(火)";
  static char now_s[] = "12:34:56";
  static int32_t last_mday = -1;

  /* Check Date,Time */
  if (last_mday == -1 || last_mday == tm->tm_mday) {  // Date changed
    // Routines which run only one time each day
    configTzTime("JST-9", "time.cloudflare.com", "ntp.nict.jp", "ntp.jst.mfeed.ad.jp");
  }

  /* Time */
  for (uint8_t sec_i = 0; sec_i < 5; ++sec_i) {
    t = time(NULL);
    tm = localtime(&t);

    now_s[0] = '0' + tm->tm_hour / 10;
    now_s[1] = '0' + tm->tm_hour % 10;
    // now_s[2] = (tm->tm_sec % 2 != 0) ? ':' : ' ';  // 1秒おきにコロンを点滅
    now_s[3] = '0' + tm->tm_min / 10;
    now_s[4] = '0' + tm->tm_min % 10;
    now_s[6] = '0' + tm->tm_sec / 10;
    now_s[7] = '0' + tm->tm_sec % 10;

    for (uint8_t j = 0; j < MATRIX_N; ++j)
      matrixLEDs[j].fill(false);

    writeJISsToMatrixLEDs(matrixLEDs, MATRIX_N, now_s, 9);
    max7219.flushMatrixLEDs(matrixLEDs, MATRIX_N);
    delay(1000);
  }

  for (uint8_t j = 0; j < MATRIX_N; ++j)
    matrixLEDs[j].fill(false);

  /* Date */
  for (uint8_t sec_i = 0; sec_i < 5; ++sec_i) {
    t = time(NULL);
    tm = localtime(&t);

    date_s[0] = ((tm->tm_mon+1) / 10 == 0) ? ' ' : '1';
    date_s[1] = '0' + ((tm->tm_mon+1) % 10);
    date_s[5] = (tm->tm_mday / 10 == 0) ? ' ' : '0' + (tm->tm_mday / 10);
    date_s[6] = '0' + (tm->tm_mday % 10);
    date_s[11] = weekday[tm->tm_wday * 3];  // utf-8 1文字分を3byteで
    date_s[12] = weekday[tm->tm_wday * 3 + 1];
    date_s[13] = weekday[tm->tm_wday * 3 + 2];
    
    for (uint8_t j = 0; j < MATRIX_N; ++j)
      matrixLEDs[j].fill(false);
    
    writeJISsToMatrixLEDs(matrixLEDs, MATRIX_N, date_s, 1);
    max7219.flushMatrixLEDs(matrixLEDs, MATRIX_N);
    delay(1000);
  }

  /* Message From StatusBoard */
  statusClientOption = postStatusToBoard(myName);
  if (!statusClientOption.skipped()) {
    lastMessage = statusClientOption.retval;
    lastMessage.replace("\n", " ");
    lastMessage.replace("\t", " ");
    lastMessage.replace("\r", " ");
    lastMessage.trim();
  }
  scrollJIS(max7219, 90, matrixLEDs, MATRIX_N, lastMessage.c_str());

  delay(1000);
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
  scrollAscii(max7219, 90, matrixLEDs, MATRIX_N, (String("Registered: ") + server.arg("ssid")).c_str());

  writeAsciisToMatrixLEDs(matrixLEDs, MATRIX_N, "Registered.", 0);
  max7219.flushMatrixLEDs(matrixLEDs, MATRIX_N);
}
