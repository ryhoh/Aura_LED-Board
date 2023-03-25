#include <Arduino.h>

#include "ESP8266TimerInterrupt.h"

#include "MW/MW_pub.h"
#include "App/App_pub.h"

ESP8266Timer ITimer;
// StatusClientOption statusClientOption;

void setup() {
  SYSCTL_Init();
  ITimer.attachInterruptInterval(16000, Main_Task);
}

void loop() {
  Network_Task_Main();
  delay(320);
}
