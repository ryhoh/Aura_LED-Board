#include "Scroll.h"

void scrollAscii(Max7219 &max7219, uint32_t tick_ms, MatrixLED *matrixLEDs, uint8_t ledlen, const char *string) {
  int16_t offset = -1;
  for (uint8_t i = 0; i < ledlen; ++i)
    offset += matrixLEDs[i].width;

  while (true) {
    for (uint8_t i = 0; i < ledlen; ++i)
      matrixLEDs[i].fill(false);

    if(writeAsciisToMatrixLEDs(matrixLEDs, 8, string, offset) == 0)
      return;
    max7219.flushMatrixLEDs(matrixLEDs, ledlen);
    call_sleep(tick_ms);
    --offset;
  }
}

void scrollJIS(Max7219 &max7219, uint32_t tick_ms, MatrixLED *matrixLEDs, uint8_t ledlen, const char *string) {
  int16_t offset = -1;
  for (uint8_t i = 0; i < ledlen; ++i)
    offset += matrixLEDs[i].width;

  while (true) {
    for (uint8_t i = 0; i < ledlen; ++i)
      matrixLEDs[i].fill(false);

    if(writeJISsToMatrixLEDs(matrixLEDs, 8, string, offset) == 0)
      return;
    max7219.flushMatrixLEDs(matrixLEDs, ledlen);
    call_sleep(tick_ms);
    --offset;
  }
}
