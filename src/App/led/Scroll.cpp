#include "Scroll.h"

uint8_t writeScrollAscii(
  MatrixLED *matrixLEDs,
  uint8_t ledlen,
  const char *string,
  uint32_t step
) {
  // 右端に寄せる
  int16_t offset = -1;
  for (uint8_t i = 0; i < ledlen; ++i)
    offset += matrixLEDs[i].width;

  // step によるスクロール
  offset -= step;

  for (uint8_t i = 0; i < ledlen; ++i)
    matrixLEDs[i].fill(false);

  if(writeAsciisToMatrixLEDs(matrixLEDs, ledlen, string, offset) == 0)
    return 1;
  return 0;
}

uint8_t writeScrollJIS(
  MatrixLED *matrixLEDs,
  uint8_t ledlen,
  const char *string,
  uint32_t step
) {
  // 右端に寄せる
  int16_t offset = -1;
  for (uint8_t i = 0; i < ledlen; ++i)
    offset += matrixLEDs[i].width;

  // step によるスクロール
  offset -= step;
  
  for (uint8_t i = 0; i < ledlen; ++i)
    matrixLEDs[i].fill(false);

  if(writeJISsToMatrixLEDs(matrixLEDs, ledlen, string, offset) == 0)
    return 1;
  return 0;
}
