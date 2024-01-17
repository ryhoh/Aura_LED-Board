/**
 * @file Direction.h
 * @author ryhoh
 * @brief Direction estimator in App layer
 * 
 */

#ifndef _DIRECTION_H_
#define _DIRECTION_H_

#include "App/Type.h"
#include "App/accelsensor/AccelSensor.h"

/* defines */
#define Y_ACL_DIR_BOTTOM_NUTRAL  (0)  /* ニュートラル */
#define Y_ACL_DIR_BOTTOM_X_PLUS  (1)  /* X+方向 */
#define Y_ACL_DIR_BOTTOM_X_MINUS (2)  /* X-方向 */
#define Y_ACL_DIR_BOTTOM_Y_PLUS  (3)  /* Y+方向 */
#define Y_ACL_DIR_BOTTOM_Y_MINUS (4)  /* Y-方向 */
#define Y_ACL_DIR_BOTTOM_Z_PLUS  (5)  /* Z+方向 */
#define Y_ACL_DIR_BOTTOM_Z_MINUS (6)  /* Z-方向 */

/* functions */
void ACL_DIR_Main(void);
uint8_t get_ACL_DIR_X_BottomDirection(void);
uint8_t get_ACL_DIR_XY_BottomDirection(void);

#endif  /* _DIRECTION_H_ */
