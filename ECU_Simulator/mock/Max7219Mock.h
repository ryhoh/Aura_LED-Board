#ifndef _MAX7219MOCK_H_
#define _MAX7219MOCK_H_

#include <cstdint>

#include "../../src/MW/MW_pub.h"

#define MOCK_MAX7219_MAX_NUM (12)
#define MOCK_MAX7219_ACTUAL_NUM (8)

#define MOCK_MAX7219_ON_CHAR ('*')
#define MOCK_MAX7219_OFF_CHAR (' ')

void Mock_Max7219_init(void);
void Mock_Max7219_flushMatrixLED(MatrixLED &matrixLED);
void Mock_Max7219_flushMatrixLEDs(MatrixLED *matrixLEDs, uint8_t length);
void Mock_Max7219_flushMatrixLEDs(MatrixLED *matrixLEDs, uint8_t length, uint8_t vertical_invert);
void Mock_Max7219_PrintToStdout(void);

#endif  /* _MAX7219MOCK_H_ */
