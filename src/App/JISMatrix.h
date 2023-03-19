#ifndef _JISMATRIX_H_
#define _JISMATRIX_H_

#include <cstdint>

#ifdef ARDUINO
#include <avr/pgmspace.h>
#endif

#include "AsciiMatrix.h"
#include "UTF8toMatrix.h"

/**
 * @brief Get a row of indicated JIS char.
 * @param utf8_s [in] JIS char (Char-Array) to get row.
 * @param out_buf [out] Pointer to store row data.
 * @param row_i [in] Row number between [0, 7].
 * @retval size_t number of read char in utf8_s on success.
 * @retval 0 on failure.
 * @note *out_buf will not be changed when failure.
         It caches most recent result. Serving for char same to previous is fast.
*/
static inline size_t getJISRow(const char *utf8_s, uint8_t *out_buf, uint8_t row_i);

/**
 * @brief Write single JIS char to single matrixLED.
 * @param matrixLED [in] Reference of MatrixLED.
 * @param c [in] Char to write.
 * @param offset_from_left [in] Offset size from left end.
*/
size_t writeJISToMatrixLED(MatrixLED &matrixLED, const char *c, int16_t offset_from_left);

/**
 * @brief Write JIS string to multiple matrixLED.
 * @param matrixLEDs [in] Pointer of MatrixLED-Array.
 * @param ledlen [in] Length of MatrixLED-Array.
 * @param string [in] Char-Array to write.
 * @param offset_from_left [in] Offset size from left end.
 * @retval Number of used matrix panels (from left).
 * @note Ascii char has 5px width, and JIS char has 7px. If you like monospace, you should NOT use ascii chars.
*/
size_t writeJISsToMatrixLEDs(MatrixLED *matrixLEDs, uint8_t ledlen, const char *string, int16_t offset_from_left);

/* binarySearch in _UTF_CODES */
const uint8_t *_binarySearchForJISMatrix(uint32_t target);

static inline size_t getJISRow(const char *utf8_s, uint8_t *out_buf, uint8_t row_i)
{
  if (7 < row_i) {  // can't serve
    *out_buf = *(_NA_CHAR + row_i) << 3;  // 左端に寄せる
    return 0;
  }

  if ((uint8_t)*utf8_s <= 0x7e) {  // Ascii compatible
    *out_buf = getASCIIRow(*utf8_s, row_i) << 3;  // 左端に寄せる
    return 1;
  }

  uint32_t target = 0;
  size_t utf_char_len = 0;
  if ((uint8_t)*utf8_s == 0x5C) {  // 1byte char -> 3byte
    target = 0xEFBCBC;
    utf_char_len = 3;
  } else if ((uint8_t)*utf8_s < 0xD2) {  // 2byte char
    target = ((uint8_t)*utf8_s << 8) + (uint8_t)*(utf8_s + 1);
    utf_char_len = 2;
  } else if ((uint8_t)*utf8_s < 0xF0) {  // 3byte char
    target = ((uint8_t)*utf8_s << 16) + ((uint8_t)*(utf8_s + 1) << 8) + (uint8_t)*(utf8_s + 2);
    utf_char_len = 3;
  } else {  // not found
    *out_buf = *(_NA_CHAR + row_i) << 3;  // 左端に寄せる
    return 0;
  }

  // check cache
  static uint32_t utf_code_cache = 0xFFFFFFFF;
  static const uint8_t *matrix_cache = nullptr;
  static size_t utf_char_len_cache = 0xFFFFFFFF;
  if (target == utf_code_cache) {
    #ifdef ARDUINO
    *out_buf = pgm_read_byte(matrix_cache + row_i);
    #else
    *out_buf = *(matrix_cache + row_i);
    #endif
    return utf_char_len_cache;
  }

  const uint8_t *matrix = _binarySearchForJISMatrix(target);
  if (matrix == nullptr) {  // not found
    *out_buf = *(_NA_CHAR + row_i) << 3;  // 左端に寄せる
    return 0;
  }
  #ifdef ARDUINO
  *out_buf = pgm_read_byte(matrix + row_i);
  #else
  *out_buf = *(matrix + row_i);
  #endif

  // caching
  utf_code_cache = target;
  matrix_cache = matrix;
  utf_char_len_cache = utf_char_len;
  return utf_char_len;
}

#endif  /* _JISMATRIX_H_ */
