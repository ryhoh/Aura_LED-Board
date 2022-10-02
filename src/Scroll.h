#ifndef _SCROLL_H_
#define _SCROLL_H_

#include "LEDControllers.h"
#include "MatrixLED.h"
#include "AsciiMatrix.h"
#include "JISMatrix.h"
#include "DeviceInterface.h"


struct Scroller {
  Max7219 max7219;
  MatrixLED *matrixLEDs;
  uint8_t ledlen;
  const char *string;
  int16_t offset;
  bool isEnd;

/**
 * @brief Constractor for scrolling ascii string.
 * @param max7219 [in] Reference of Max7219.
 * @param matrixLEDs [in] Reference of MatrixLED-Array.
 * @param ledlen [in] Length of MatrixLED-Array.
*/
  Scroller(Max7219 &max7219, MatrixLED *matrixLEDs, uint8_t ledlen) {
    this->max7219 = max7219;
    this->matrixLEDs = matrixLEDs;
    this->ledlen = ledlen;
    this->reset();
  }

/**
 * @param string [in] Char-Array to write.
*/
  void setString(const char *string) {
    this->string = string;
    this->reset();
  }

  void reset();

  void step();
};

#endif  /* _SCROLL_H_ */
