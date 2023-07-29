#include <Arduino.h>
#include <Ticker.h>

#include "MW/MW_pub.h"
#include "App/App_pub.h"

// StatusClientOption statusClientOption;

Ticker ticker;

void setup() {
  SYSCTL_Init();
  ticker.attach_ms(MW_LEDTASK_DURATION_MS, Main_Task);
}

void loop() {
  Network_Task_Main();
  delay(MW_MAINTASK_DURATION_MS);
}
