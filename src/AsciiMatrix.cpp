#include <AsciiMatrix.h>

void writeAsciiToMatrixLED(MatrixLED &matrixLED, char c, int16_t offset_from_left)
{
  const uint8_t offset_from_top = (7 < matrixLED.height) ? matrixLED.height - 7 : 0;
  const uint8_t height_limit = (matrixLED.height < 7) ? matrixLED.height : 7;
  const int8_t left_end = 3 - offset_from_left;  // 文字を左端に移動させてオフセット

  for (uint8_t row_i = 0; row_i < height_limit; ++row_i) {
    const uint8_t row = getASCIIRow(c, row_i);
    if (-matrixLED.width <= left_end && left_end < matrixLED.width) {
      if (left_end < 0)
        *(matrixLED.buffer + offset_from_top + row_i) |= row >> -left_end;
      else
        *(matrixLED.buffer + offset_from_top + row_i) |= row << left_end;
    }
  }
}

size_t writeAsciisToMatrixLEDs(MatrixLED *matrixLEDs, uint8_t ledlen, const char *string, int16_t offset_from_left)
{
  if (matrixLEDs == nullptr || string == nullptr)
    return 0;

  int16_t left_cur = offset_from_left;
  uint8_t matrix_cur = 0;

  // Skipping Empty Matrix (for big `offset_from_left`)
  for (uint8_t matrix_i = 0; matrix_i < ledlen; ++matrix_i) {
    if (matrixLEDs[matrix_i].width <= left_cur) {
      left_cur -= matrixLEDs[matrix_i].width;
      ++matrix_cur;
    } else break;
  }
  if (ledlen <= matrix_cur)  // matrixLED ran out
    return ledlen;

  const char *p = string;
  while (*p != '\0') {
    writeAsciiToMatrixLED(*(matrixLEDs + matrix_cur), *p, left_cur);
    left_cur += 6;  // 文字幅 5 + スキマ幅 1

    if (matrixLEDs->width <= left_cur) {
      // move to next matrix...
      left_cur %= (matrixLEDs + matrix_cur++)->width;

      if (ledlen <= matrix_cur)  // matrixLED ran out
        return ledlen;

      // write rest of previous char
      writeAsciiToMatrixLED(*(matrixLEDs + matrix_cur), *p, left_cur - 6);
    }

    ++p;
  }

  if (left_cur < 0)
    return 0;
  return matrix_cur + 1;  // used matrix + initial matrix
}
