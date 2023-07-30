#include <iostream>
#include <cstdint>
#include <unistd.h>

#include "MW/MW_pub.h"
#include "App/App_pub.h"

#include "mock/Max7219Mock.h"

const uint8_t matrix_length = 8;
const uint8_t height = 8;
const uint8_t width = 8;

int main(int argc, char **argv) {
  SYSCTL_Init();

  uint32_t count = 0;
  while (true) {
    if (count % MW_LEDTASK_DURATION_MS == 0) {
      SYSCTL_Priority_Task_Main();
    }

    if (count % MW_MAINTASK_DURATION_MS == 0) {
      SYSCTL_Background_Task_Main();
    }

    // 画面表示
    Mock_Max7219_PrintToStdout();

    count++;
    usleep(1000);
  }
}
