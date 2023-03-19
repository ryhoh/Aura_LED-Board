#ifndef _DOTMATRIXLED_H_
#define _DOTMATRIXLED_H_

/* Buffers */
#include "MW/MatrixLED.h"

/* Support functions for buffer */
#include "App/AsciiMatrix.h"
#include "App/JISMatrix.h"

/* Devices */
#ifndef SIMULATOR  // Can't use for simulation on terminal.
#include "MW/LEDControllers.h"

/* Wrapper functions */
#include "App/Scroll.h"

#endif  /* SIMULATOR */


#endif  /* _DOTMATRIXLED_H_ */
