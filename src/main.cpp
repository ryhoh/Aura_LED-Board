#include <Arduino.h>

// #include "ESP8266TimerInterrupt.h"

#include "MW/MW_pub.h"
#include "App/App_pub.h"

// ESP8266Timer ITimer;
// StatusClientOption statusClientOption;

hw_timer_t *hw_timer_0 = NULL;

void setup() {
  Serial.begin(115200);
  SYSCTL_Init();
  // ITimer.attachInterruptInterval(16000, Main_Task);

  // hw_timer_0 = timerBegin(0, 80, true);
  // timerAttachInterrupt(hw_timer_0, &Main_Task, true);
  // timerAlarmWrite(hw_timer_0, 16000, true);
  // timerAlarmEnable(hw_timer_0);
}

void loop() {
  // Network_Task_Main();
  // delay(320);

  Main_Task();
  delay(16);
}
