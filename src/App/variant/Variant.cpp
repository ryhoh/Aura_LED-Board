/**
 * @file Variant.cpp
 * @author ryhoh
 * @brief Variant source file.
 * 
 */

// インクルード
#include "Variant.h"
#include "Profile.h"

// 変数宣言
static const Variant_t gscst_variant_tbl[m_VARIANT_TABLE_SIZE] = {
  /* Machine Name        , Matrix Num, Data Pin, CS Pin, Clock Pin, Mode Pin,                     Lamp Pin, Brightness                  , RDS,              dummy    */
  { "LU-LEDB-0001A1     ",          4,       10,      5,         8, m_VARIANT_LED_MODE_PIN_NOUSE,        3, m_VARIANT_LED_BRIGHTNESS_MIN, m_VARIANT_NO_RDS, 0 },
  { "LU-LEDB-0002A1     ",          8,       10,      5,         8, m_VARIANT_LED_MODE_PIN_NOUSE,        3, m_VARIANT_LED_BRIGHTNESS_MIN, m_VARIANT_NO_RDS, 0 },
  { "LU-LEDB-0003A1     ",         12,       10,      5,         8, m_VARIANT_LED_MODE_PIN_NOUSE,        3, m_VARIANT_LED_BRIGHTNESS_MIN, m_VARIANT_RDS1,   0 },
  { "LU-LEDB-0102A1     ",          8,       10,      5,         8, m_VARIANT_LED_MODE_PIN_NOUSE,        3, m_VARIANT_LED_BRIGHTNESS_MIN, m_VARIANT_RDS1  , 0 },
  { "INVALID-VARIANT    ",          4,       10,      5,         8, m_VARIANT_LED_MODE_PIN_NOUSE,        3, m_VARIANT_LED_BRIGHTNESS_MIN, m_VARIANT_NO_RDS, 0 },
  { "INVALID-VARIANT    ",          4,       10,      5,         8, m_VARIANT_LED_MODE_PIN_NOUSE,        3, m_VARIANT_LED_BRIGHTNESS_MIN, m_VARIANT_NO_RDS, 0 },
  { "INVALID-VARIANT    ",          4,       10,      5,         8, m_VARIANT_LED_MODE_PIN_NOUSE,        3, m_VARIANT_LED_BRIGHTNESS_MIN, m_VARIANT_NO_RDS, 0 },
  { "INVALID-VARIANT    ",          4,       10,      5,         8, m_VARIANT_LED_MODE_PIN_NOUSE,        3, m_VARIANT_LED_BRIGHTNESS_MIN, m_VARIANT_NO_RDS, 0 },
  { "INVALID-VARIANT    ",          4,       10,      5,         8, m_VARIANT_LED_MODE_PIN_NOUSE,        3, m_VARIANT_LED_BRIGHTNESS_MIN, m_VARIANT_NO_RDS, 0 },
  { "INVALID-VARIANT    ",          4,       10,      5,         8, m_VARIANT_LED_MODE_PIN_NOUSE,        3, m_VARIANT_LED_BRIGHTNESS_MIN, m_VARIANT_NO_RDS, 0 },
};

static uint8_t gsu8_variant_idx = 0;
static Variant_t gsst_selected_variant;

// 関数定義
void Variant_Init(void) {
  // バリアント設定
  uint8_t u8_variant_idx = Get_NVM_Variant_idx();
  if (u8_variant_idx >= m_VARIANT_TABLE_SIZE) {
    u8_variant_idx = 0;
  }
  gsu8_variant_idx = u8_variant_idx;

  // テーブルから、選択されたバリアントのデータを展開する
  // 動作中にバリアントを変更した場合、再起動後に反映する仕様とする
  gsst_selected_variant = gscst_variant_tbl[gsu8_variant_idx];
}

const char *Get_VARIANT_MachineName(void) {
  return gsst_selected_variant.i8_machine_name;
}

uint32_t Get_VARIANT_MatrixNum(void) {
  uint32_t u32_matrix_num = gsst_selected_variant.u32_matrix_num;
  M_CLIP_MAX(u32_matrix_num, m_PROFILE_MAX_DESIGNED_PANEL_NUM);
  return u32_matrix_num;
}

uint8_t Get_VARIANT_SPIDataPin(void) {
  return gsst_selected_variant.u8_spi_data_pin;
}

uint8_t Get_VARIANT_SPICSPin(void) {
  return gsst_selected_variant.u8_spi_cs_pin;
}

uint8_t Get_VARIANT_SPIClockPin(void) {
  return gsst_selected_variant.u8_spi_clock_pin;
}

/* APモード廃止で切替がなくなるため無効化 */
#if 0
uint8_t Get_VARIANT_ModePin(void) {
  return gsst_selected_variant.u8_mode_pin;
}
#endif

uint8_t Get_VARIANT_LampPin(void) {
  return gsst_selected_variant.u8_lamp_pin;
}

uint8_t Get_VARIANT_Brightness(void) {
  uint8_t u8_brightness = gsst_selected_variant.u8_brightness;
  M_CLIP_MAX(u8_brightness, m_VARIANT_LED_BRIGHTNESS_MAX);
  M_CLIP_MIN(u8_brightness, m_VARIANT_LED_BRIGHTNESS_MIN);
  return u8_brightness;
}

uint8_t Get_VARIANT_RDS_SupportLevel(void) {
  return gsst_selected_variant.u8_rds_support_level;
}
