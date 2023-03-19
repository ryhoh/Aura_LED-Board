#ifndef _MATRIXLED_H_
#define _MATRIXLED_H_

#include <cstdio>
#include <cstdint>
#include <cstring>

/**
 * @brief Buffer for matrix LED panel.
 * @note Supports mono-color, binary and up to 8x8 size panel.
*/
struct MatrixLED {
  uint8_t width;   ///< width  <= 8
  uint8_t height;  ///< height <= 8
  uint8_t buffer[8];  ///< 8*8 bit = 64 bit = 8 byte

  /**
   * @brief Initialize MatrixLED.
   * @note The width and height is 0.
  */
  MatrixLED(): width(0), height(0) {}

  /**
   * @brief Initialize MatrixLED.
   * @param width [in] Number of LED on the panel (horizontal).
   * @param height [in] Number of LED on the panel (vertical).
   * @note It fails when the panel is larger than 8x8 and set 0 to width and height.
  */
  MatrixLED(uint8_t width, uint8_t height);

  /**
   * @brief Destroy MatrixLED.
  */
  ~MatrixLED() {}

  /**
   * @brief Invert all LEDs' state (on or off).
  */
  void flip();

  /**
   * @brief Fill all LEDs with 0 or 1.
   * @param value [in] Value for filling (0 or 1).
  */
  void fill(bool value);

  /**
   * @brief Shift all LEDs to left by one bit.
   * @param padding [in] Bits for inserting right end.
   * @retval uint8_t Pack of overflowed bits
  */
  uint8_t leftShift(uint8_t padding);
};

#endif  /* _MATRIXLED_H_ */
