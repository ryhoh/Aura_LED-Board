#include "JISMatrix.h"

size_t writeJISToMatrixLED(MatrixLED &matrixLED, const char *c, int16_t offset_from_left)
{
  const uint8_t offset_from_top = 1;
  const uint8_t height_limit = (matrixLED.height < 7) ? matrixLED.height : 7;
  const int16_t left_end = -offset_from_left;  // 文字を左端に移動させてオフセット

  size_t read_char_size = 0;
  uint8_t row = 0;
  for (uint8_t row_i = 0; row_i < height_limit; ++row_i) {
    read_char_size = getJISRow(c, &row, row_i);
    if (-matrixLED.width <= left_end && left_end < matrixLED.width) {
      if (left_end < 0)
        *(matrixLED.buffer + offset_from_top + row_i) |= row >> -left_end;
      else
        *(matrixLED.buffer + offset_from_top + row_i) |= row << left_end;
    }
  }

  return read_char_size;
}

size_t writeJISsToMatrixLEDs(MatrixLED *matrixLEDs, uint8_t ledlen, const char *string, int16_t offset_from_left)
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
    size_t read_char_n = writeJISToMatrixLED(*(matrixLEDs + matrix_cur), p, left_cur);
    if (read_char_n == 0) {
      // not found (got "n/a char")
      read_char_n = 1;
    }

    // (文字幅 5 + スキマ幅 1) または (文字幅 7 + スキマ幅 1)
    const uint8_t char_width = (read_char_n == 1) ? 6 : 8;
    left_cur += char_width;

    if (matrixLEDs->width <= left_cur) {
      // move to next matrix...
      left_cur %= (matrixLEDs + matrix_cur)->width;
      ++matrix_cur;

      if (ledlen <= matrix_cur)  // matrixLED ran out
        return ledlen;

      // write rest of previous char
      writeJISToMatrixLED(*(matrixLEDs + matrix_cur), p, left_cur - char_width);
    }

    p += read_char_n;
  }

  if (left_cur < 0)
    return 0;
  return matrix_cur + 1;  // used matrix + initial matrix
}

const uint8_t *_binarySearchForJISMatrix(uint32_t target)
{
  uint32_t imin = 0;
  uint32_t imax = _UTF_CODES_LEN - 1;
  while (imin <= imax) {
    // linear search for small area
    if (imax - imin < 50) {
      for (uint16_t entry_i = imin; entry_i < imax + 1; ++entry_i) {
        #ifdef ARDUINO
        if (pgm_read_dword(_UTF8_CODES + entry_i) == target)
        #else
        if (*(_UTF8_CODES + entry_i) == target)
        #endif
          return *(_UTF8_MATRIX + entry_i);
      }
      // not found
      return nullptr;
    }

    const uint32_t imid = imin + (imax - imin) / 2;
    #ifdef ARDUINO
    const uint32_t looking_code = (uint32_t)pgm_read_dword(_UTF8_CODES + imid);
    #else
    const uint32_t looking_code = *(_UTF8_CODES + imid);
    #endif

    if (looking_code > target)
      imax = imid - 1;
    else if (looking_code < target)
      imin = imid + 1;
    else
      return *(_UTF8_MATRIX + imid);
  }
  return nullptr;
}
