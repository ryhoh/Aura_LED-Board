#ifndef _SCROLL_H_
#define _SCROLL_H_

#include "LEDControllers.h"
#include "MatrixLED.h"
#include "AsciiMatrix.h"
#include "JISMatrix.h"
#include "DeviceInterface.h"


/**
 * @brief Function for scrolling ascii string.
 * @param max7219 [in] Reference of Max7219.
 * @param tick_ms [in] Interval time[ms]. When this gets bigger, then scroll will be slower.
 * @param matrixLEDs [in] Reference of MatrixLED-Array.
 * @param ledlen [in] Length of MatrixLED-Array.
 * @param string [in] Char-Array to write.
*/
void scrollAscii(Max7219 &max7219, uint32_t tick_ms, MatrixLED *matrixLEDs, uint8_t ledlen, const char *string);

/**
 * @brief Function for scrolling jis string.
 * @param max7219 [in] Reference of Max7219.
 * @param tick_ms [in] Interval time[ms]. When this gets bigger, then scroll will be slower.
 * @param matrixLEDs [in] Reference of MatrixLED-Array.
 * @param ledlen [in] Length of MatrixLED-Array.
 * @param string [in] Char-Array to write.
*/
void scrollJIS(Max7219 &max7219, uint32_t tick_ms, MatrixLED *matrixLEDs, uint8_t ledlen, const char *string);

#endif  /* _SCROLL_H_ */
