/* Include */
#include "AccelSensor.h"

/* Define */
#define Y_ACL_BUFFER_SIZE (8)  // ACL_バッファサイズ
#define Y_ACL_PWR_MGMT_1 (0x6B)  // PWR_MGMT_1
#define Y_ACL_ACCEL_XOUT_H (0x3B)  // ACCEL_XOUT_H

/* 変数宣言 */
const static uint8_t Xuc_ACL_I2C_ADDR = 0x68;

static MPU6050_t zst_ACL_AccelSensor_Raw_Input;  /* ACL_MPU6050_読み取り値 */
static MPU6050_t zst_ACL_AccelSensor_RingBuff[Y_ACL_BUFFER_SIZE];  /* ACL_MPU6050_リングバッファ */
static uint8_t zuc_ACL_RingBuff_Index = 0;  /* ACL_MPU6050_リングバッファインデックス */
static MPU6050_t zst_ACL_AccelSensor_MovingAverage;  /* ACL_MPU6050_移動平均値 */
static MPU6050_t zst_ACL_AccelSensor_Output;  /* ACL_MPU6050_出力値 */

/* 関数宣言 */
static void ACL_Input(void);
static void ACL_Store_RingBuff(void);
static void ACL_Moving_Average(void);
static void ACL_Output(void);

/* 関数定義 */
/**
 * @brief AccelSensorの初期化
 * 
 */
void ACL_Init(void) {
  call_i2c_init();
  call_i2c_beginTransmission(Xuc_ACL_I2C_ADDR);
  call_i2c_write(Y_ACL_PWR_MGMT_1);  // PWR_MGMT_1
  call_i2c_write(0x00);  // 動作開始
  call_i2c_endTransmission();
}

/**
 * @brief AccelSensorの割り込みタスク
 * 
 */
void ACL_Priority_Task_Main(void) {
  ACL_Input();
  ACL_Store_RingBuff();
}

/**
 * @brief AccelSensorのバックグラウンドタスク
 * 
 */
void ACL_Background_Task_Main(void) {
  ACL_Moving_Average();
  ACL_Output();

  ACL_DIR_Main();
}

/**
 * @brief AccelSensorの入力処理
 * 
 */
static void ACL_Input(void) {
  int16_t s_accel_x = 0;
  int16_t s_accel_y = 0;
  int16_t s_accel_z = 0;

  call_i2c_beginTransmission(Xuc_ACL_I2C_ADDR);
  call_i2c_write(Y_ACL_ACCEL_XOUT_H);
  call_i2c_endTransmission();
  call_i2c_requestFrom(Xuc_ACL_I2C_ADDR, 6);

  s_accel_x = (uint16_t)call_i2c_read() << 8 | call_i2c_read();
  s_accel_y = (uint16_t)call_i2c_read() << 8 | call_i2c_read();
  s_accel_z = (uint16_t)call_i2c_read() << 8 | call_i2c_read();

  /* 出力 */
  zst_ACL_AccelSensor_Raw_Input.s_X = s_accel_x;
  zst_ACL_AccelSensor_Raw_Input.s_Y = s_accel_y;
  zst_ACL_AccelSensor_Raw_Input.s_Z = s_accel_z;
}

/**
 * @brief AccelSensorのリングバッファ保存処理
 * 
 */
static void ACL_Store_RingBuff(void) {
  const MPU6050_t st_AccelSensor_input = zst_ACL_AccelSensor_Raw_Input;
  const uint8_t uc_index = zuc_ACL_RingBuff_Index;
  const uint8_t uc_index_next = (zuc_ACL_RingBuff_Index + 1) & 0x07;  /* % 8 */

  zst_ACL_AccelSensor_RingBuff[uc_index] = st_AccelSensor_input;
  zuc_ACL_RingBuff_Index = uc_index_next;
}

/**
 * @brief AccelSensorの移動平均処理
 * 
 */
static void ACL_Moving_Average(void) {
  int32_t l_sum_x = 0;
  int32_t l_sum_y = 0;
  int32_t l_sum_z = 0;

  for (uint8_t uc_index = 0; uc_index < Y_ACL_BUFFER_SIZE; uc_index++) {
    l_sum_x += zst_ACL_AccelSensor_RingBuff[uc_index].s_X;
    l_sum_y += zst_ACL_AccelSensor_RingBuff[uc_index].s_Y;
    l_sum_z += zst_ACL_AccelSensor_RingBuff[uc_index].s_Z;
  }

  zst_ACL_AccelSensor_MovingAverage.s_X = (int16_t)(l_sum_x >> 3);  /* /8 */
  zst_ACL_AccelSensor_MovingAverage.s_Y = (int16_t)(l_sum_y >> 3);  /* /8 */
  zst_ACL_AccelSensor_MovingAverage.s_Z = (int16_t)(l_sum_z >> 3);  /* /8 */
}

/**
 * @brief AccelSensorの出力処理
 * 
 */
static void ACL_Output(void) {
  zst_ACL_AccelSensor_Output = zst_ACL_AccelSensor_MovingAverage;
}

/* IF */
/**
 * @brief AccelSensorの出力値取得
 * 
 * @return MPU6050_t 
 */
MPU6050_t get_ACL_Output(void) {
  return zst_ACL_AccelSensor_Output;
}
