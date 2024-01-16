/* インクルード */
#include "Direction.h"

/* 定義 */
#define Y_ACL_DIR_G_THRESHOLD (13926)  /* [Q14, g] ACL_重力加速度閾値 13926 / (2^14) = 0.85g */
static uint8_t zuc_ACL_XY_BottomDirection = Y_ACL_DIR_BOTTOM_NUTRAL;  /* ACL_XY面_下向き方向 */

/* 関数 */
void ACL_DIR_Estimate_XY_BottomDirection();

/**
 * @brief 下向き方向の推定メイン
 * 
 */
void ACL_DIR_Main(void) {
  ACL_DIR_Estimate_XY_BottomDirection();
}

/**
 * @brief 下向き方向の推定(XZ面)
 * 
 */
void ACL_DIR_Estimate_XY_BottomDirection() {
  MPU6050_t st_ACL_AccelSensor_Output = get_ACL_Output();
  int16_t s_accel_x = st_ACL_AccelSensor_Output.s_X;
  int16_t s_accel_y = st_ACL_AccelSensor_Output.s_Y;
  uint8_t uc_dir = Y_ACL_DIR_BOTTOM_NUTRAL;
  uint8_t uc_x_over_plus_th;
  uint8_t uc_x_over_minus_th;
  uint8_t uc_x_neutral;
  uint8_t uc_y_over_plus_th;
  uint8_t uc_y_over_minus_th;
  uint8_t uc_y_neutral;

  /* X軸方向 */
  uc_x_over_plus_th = (s_accel_x > Y_ACL_DIR_G_THRESHOLD) ? m_ON : m_OFF;
  uc_x_over_minus_th = (s_accel_x < -Y_ACL_DIR_G_THRESHOLD) ? m_ON : m_OFF;
  uc_x_neutral = (uc_x_over_plus_th == m_OFF && uc_x_over_minus_th == m_OFF) ? m_ON : m_OFF;

  /* Y軸方向 */
  uc_y_over_plus_th = (s_accel_y > Y_ACL_DIR_G_THRESHOLD) ? m_ON : m_OFF;
  uc_y_over_minus_th = (s_accel_y < -Y_ACL_DIR_G_THRESHOLD) ? m_ON : m_OFF;
  uc_y_neutral = (uc_y_over_plus_th == m_OFF && uc_y_over_minus_th == m_OFF) ? m_ON : m_OFF;

  /* 下向き方向判定 */
  if ( uc_x_over_plus_th == m_ON  /* X軸プラスが下向き */
    && uc_y_neutral == m_ON ) {   /* かつ Y軸ニュートラル */
    uc_dir = Y_ACL_DIR_BOTTOM_X_PLUS;
  } else if ( uc_x_over_minus_th == m_ON   /* X軸マイナスが下向き */
           && uc_y_neutral == m_ON ) {     /* かつ Y軸ニュートラル */
    uc_dir = Y_ACL_DIR_BOTTOM_X_MINUS;
  } else if ( uc_x_neutral == m_ON           /* X軸ニュートラル */
           && uc_y_over_plus_th == m_ON ) {  /* かつ Y軸プラスが下向き */
    uc_dir = Y_ACL_DIR_BOTTOM_Y_PLUS;
  } else if ( uc_x_neutral == m_ON            /* X軸ニュートラル */
           && uc_y_over_minus_th == m_ON ) {  /* かつ Y軸マイナスが下向き */
    uc_dir = Y_ACL_DIR_BOTTOM_Y_MINUS;
  } else {
    uc_dir = Y_ACL_DIR_BOTTOM_NUTRAL;  /* ニュートラル：中間状態またはZ軸が下向き */
  }

  /* 下向き方向の保存 */
  zuc_ACL_XY_BottomDirection = uc_dir;
}

/**
 * @brief XY面 下向き方向の取得
 * 
 * @return uint8_t 下向き方向
 */
uint8_t get_ACL_DIR_XY_BottomDirection(void) {
  return zuc_ACL_XY_BottomDirection;
}
