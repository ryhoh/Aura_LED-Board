#ifndef _DOTMATRIXLED_H_
#define _DOTMATRIXLED_H_

/* Buffers */
#include "MatrixLED.h"

/* Support functions for buffer */
#include "AsciiMatrix.h"
#include "JISMatrix.h"

/* Devices */
#ifndef SIMULATOR  // Can't use for simulation on terminal.
#include "LEDControllers.h"

/* Wrapper functions */
#include "Scroll.h"

#endif  /* SIMULATOR */


#endif  /* _DOTMATRIXLED_H_ */
