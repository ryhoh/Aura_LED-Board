/**
 * @file AccelSensor.h
 * @author ryhoh
 * @brief AccelSensor in App layer.
 * 
 */

#ifndef _ACCELSENSOR_H_
#define _ACCELSENSOR_H_

#include "Driver/DeviceInterface.h"
#include "App/Type.h"
#include "App/accelsensor/Direction.h"

/* 構造体 */
typedef struct {
  int16_t s_X;  /* [Q14, g] X方向加速度 */  /* ±2g 4/(2^16) = Q14 */
  int16_t s_Y;  /* [Q14, g] Y方向加速度 */  /* ±2g 4/(2^16) = Q14 */
  int16_t s_Z;  /* [Q14, g] Z方向加速度 */  /* ±2g 4/(2^16) = Q14 */
} MPU6050_t;  /* ANG_MPU6050 */

/* 関数 */
void ACL_Init(void);
void ACL_Priority_Task_Main(void);
void ACL_Background_Task_Main(void);
MPU6050_t get_ACL_Output(void);

#endif  /* _ACCELSENSOR_H_ */
