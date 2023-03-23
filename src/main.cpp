#include <Arduino.h>
#include <time.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
// #include <EEPROM.h>

#include "ESP8266TimerInterrupt.h"

#include "MW/MW_pub.h"
#include "App/App_pub.h"

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

const uint32_t MY_LED = 13;
const uint32_t DAT = 5;
const uint32_t LAT = 4;
const uint32_t CLK = 14;
const uint32_t MODE = 16;
constexpr uint16_t MATRIX_N = 8;

ESP8266WebServer server(80);

char WIFI_SSID[31];
char WIFI_PASSWORD[31];

StatusClientOption statusClientOption;
String lastMessage = "Hello World!";

void ep_root();
void ep_submit();

void setup() {
  EEPROM.begin(0x60);

  // pinMode(MY_LED, OUTPUT);
  // pinMode(DAT, OUTPUT);
  // pinMode(LAT, OUTPUT);
  // pinMode(CLK, OUTPUT);
  // pinMode(MODE, INPUT_PULLDOWN_16);

  /* Setup Mode */
  if (digitalRead(MODE) == LOW) {
    // writeJISsToMatrixLEDs(matrixLEDs, MATRIX_N, "Setup Mode", 0);
    // max7219.flushMatrixLEDs(matrixLEDs, MATRIX_N);

    // WiFi.mode(WIFI_AP);
    // WiFi.softAPConfig(IPAddress(192, 168, 1, 1), IPAddress(192, 168, 1, 10), IPAddress(255, 255, 255, 0));
    // WiFi.softAP(AP_SSID, AP_PASSWORD);
    // IPAddress ip = WiFi.softAPIP();

    // for (uint8_t j = 0; j < MATRIX_N; ++j)
    //   matrixLEDs[j].fill(false);
    // writeJISsToMatrixLEDs(matrixLEDs, MATRIX_N, (String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3])).c_str(), 0);
    // max7219.flushMatrixLEDs(matrixLEDs, MATRIX_N);

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
  // Network_Task_Init();
  // LED_Task_Init();
  // Serial.begin(115200);
  SYSCTL_Init();
  ITimer.attachInterruptInterval(16000, Main_Task);
}

void loop() {
  Network_Task_Main();
  delay(320);
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

  // for (uint8_t j = 0; j < MATRIX_N; ++j)
  //   matrixLEDs[j].fill(false);
  
  // uint32_t scrollstep = 0;
  // uint8_t scrollend = 0;
  // while (scrollend == 0) {
  //   scrollend = writeScrollJIS(
  //     matrixLEDs,
  //     MATRIX_N,
  //     (String("Registered: ") + server.arg("ssid")).c_str(),
  //     scrollstep);
  //   ++scrollstep;
  //   delay(80);
  // }

  // writeAsciisToMatrixLEDs(matrixLEDs, MATRIX_N, "Registered.", 0);
  // max7219.flushMatrixLEDs(matrixLEDs, MATRIX_N);
}
