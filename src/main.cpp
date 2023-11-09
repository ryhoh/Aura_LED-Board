#include <Arduino.h>
#include <Ticker.h>

#include "MW/MW_pub.h"
#include "App/App_pub.h"

// StatusClientOption statusClientOption;

Ticker ticker;

void setup() {
  SYSCTL_Init();
  ticker.attach_ms(m_SYSCTL_CALL_ITVL_PRIORITY, SYSCTL_Priority_Task_Main);
}

void loop() {
  SYSCTL_Background_Task_Main();
  call_sleep(m_SYSCTL_CALL_ITVL_BACKGROUND);
}
