/* インクルード */
#include "Direction.h"

/* 定義 */
#define Y_ACL_DIR_G_THRESHOLD (13926)  /* [Q14, g] ACL_重力加速度閾値 13926 / (2^14) = 0.85g */
#define Y_ACL_DIR_TRANSITION_THRESHOLD (5)  /* [16, ms] ACL_方向転換閾値 */
static uint8_t zuc_ACL_XY_BottomDirection_Raw        = Y_ACL_DIR_BOTTOM_NUTRAL;   /* ACL_XY面_下向き方向(実測値) */
static uint8_t zuc_ACL_X_BottomDirection_PLUSorMINUS = Y_ACL_DIR_BOTTOM_X_MINUS;  /* ACL_X軸_下向き方向(正負値) */

/* 関数 */
static void ACL_DIR_Estimate_XY_BottomDirection();
static void ACL_DIR_X_BottomDirection_Transition(void);

/**
 * @brief 下向き方向の推定メイン
 * 
 */
void ACL_DIR_Main(void) {
  /* 下向き方向の推定(XY面) */
  ACL_DIR_Estimate_XY_BottomDirection();

  /* 制御用 下向き方向の決定(XY面) */
  ACL_DIR_X_BottomDirection_Transition();
}

/**
 * @brief 下向き方向の推定(XZ面)
 * 
 */
static void ACL_DIR_Estimate_XY_BottomDirection() {
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
  zuc_ACL_XY_BottomDirection_Raw = uc_dir;
}

static void ACL_DIR_X_BottomDirection_Transition(void) {
  uint8_t uc_dir = zuc_ACL_XY_BottomDirection_Raw;
  uint8_t uc_dir_unneutral = zuc_ACL_X_BottomDirection_PLUSorMINUS;

  if (uc_dir != Y_ACL_DIR_BOTTOM_X_MINUS
   && uc_dir != Y_ACL_DIR_BOTTOM_X_PLUS) {
    /* X軸ニュートラル時は制御値を更新しない */
  } else {
    /* X軸ニュートラル以外は制御値を更新 */
    uc_dir_unneutral = uc_dir;
  }

  /* 出力 */
  zuc_ACL_X_BottomDirection_PLUSorMINUS = uc_dir_unneutral;
}

/**
 * @brief X軸 下向き方向の取得
 * 
 * @return uint8_t 下向き方向
 */
uint8_t get_ACL_DIR_X_BottomDirection(void) {
  return zuc_ACL_X_BottomDirection_PLUSorMINUS;
}

/**
 * @brief XY面 下向き方向の取得
 * 
 * @return uint8_t 下向き方向
 */
// uint8_t get_ACL_DIR_XY_BottomDirection(void) {
//   return zuc_ACL_XY_BottomDirection_UnNeutral;
// }
