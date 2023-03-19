/**
 * @file MW_pub.h
 * @author ryhoh
 * @brief Public header for MW.
 * 
 */

#ifndef _MW_PUB_H_
#define _MW_PUB_H_

// includes
#include "MW/internal/MatrixLED.h"

#ifndef SIMULATOR  // Can't use for simulation on terminal.
#include "MW/internal/LEDControllers.h"
#endif  /* SIMULATOR */

#endif  /* _MW_PUB_H_ */
