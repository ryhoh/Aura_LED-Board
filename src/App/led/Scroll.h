#ifndef _SCROLL_H_
#define _SCROLL_H_

#include "Driver/DeviceInterface.h"
#include "MW/MW_pub.h"
#include "App/Common.h"
#include "AsciiMatrix.h"
#include "JISMatrix.h"


/**
 * @brief Function for writing scrolling ascii string.
 * @param matrixLEDs [in] Reference of MatrixLED-Array.
 * @param ledlen [in] Length of MatrixLED-Array.
 * @param string [in] Char-Array to write.
 * @param step [in] Step of scroll.
 * @return 0: scrolling, 1: end.
*/
uint8_t writeScrollAscii(MatrixLED *matrixLEDs, uint8_t ledlen, const char *string, uint32_t step);

/**
 * @brief Function for writing scrolling jis string.
 * @param matrixLEDs [in] Reference of MatrixLED-Array.
 * @param ledlen [in] Length of MatrixLED-Array.
 * @param string [in] Char-Array to write.
 * @param step [in] Step of scroll.
 * @return 0: scrolling, 1: end.
*/
uint8_t writeScrollJIS(MatrixLED *matrixLEDs, uint8_t ledlen, const char *string, uint32_t step);

#endif  /* _SCROLL_H_ */
