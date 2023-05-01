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
  /* Type Name                                       , Machine Name    , Matrix Num, Data Pin, CS Pin, Clock Pin, Mode Pin, Lamp Pin, Brightness                  , dummy    */
  { { 0x4C, 0x55, 0x2D, 0x41, 0x31 } /* "LU-A1"   */ , "LU-LEDB-001A1 ",          4,       10,      5,         8,        9,        3, m_VARIANT_LED_BRIGHTNESS_MIN, { 0, 0 } },
  { { 0x4C, 0x55, 0x2D, 0x41, 0x31 } /* "LU-A1"   */ , "LU-LEDB-002A1 ",          8,       10,      5,         8,        9,        3, m_VARIANT_LED_BRIGHTNESS_MIN, { 0, 0 } },
  { { 0x4C, 0x55, 0x2D, 0x41, 0x31 } /* "LU-A1"   */ , "LU-LEDB-003A1 ",         12,       10,      5,         8,        9,        3, m_VARIANT_LED_BRIGHTNESS_MIN, { 0, 0 } },
};

// プロトタイプ宣言
static uint32_t VARIANT_GetVariantIndex();

// 関数定義
// const char *Get_VARIANT_TypeName(void) {
//   const uint32_t cu32_selected_variant_idx = VARIANT_GetVariantIndex();
//   return gscst_variant_tbl[cu32_selected_variant_idx].i8_type_name;
// }

const char *Get_VARIANT_MachineName(void) {
  const uint32_t cu32_selected_variant_idx = VARIANT_GetVariantIndex();
  return gscst_variant_tbl[cu32_selected_variant_idx].i8_machine_name;
}

uint32_t Get_VARIANT_MatrixNum(void) {
  const uint32_t cu32_selected_variant_idx = VARIANT_GetVariantIndex();
  uint32_t u32_matrix_num = gscst_variant_tbl[cu32_selected_variant_idx].u32_matrix_num;
  M_CLIP_MAX(u32_matrix_num, m_PROFILE_MAX_DESIGNED_PANEL_NUM);
  return u32_matrix_num;
}

uint8_t Get_VARIANT_SPIDataPin(void) {
  const uint32_t cu32_selected_variant_idx = VARIANT_GetVariantIndex();
  return gscst_variant_tbl[cu32_selected_variant_idx].u8_spi_data_pin;
}

uint8_t Get_VARIANT_SPICSPin(void) {
  const uint32_t cu32_selected_variant_idx = VARIANT_GetVariantIndex();
  return gscst_variant_tbl[cu32_selected_variant_idx].u8_spi_cs_pin;
}

uint8_t Get_VARIANT_SPIClockPin(void) {
  const uint32_t cu32_selected_variant_idx = VARIANT_GetVariantIndex();
  return gscst_variant_tbl[cu32_selected_variant_idx].u8_spi_clock_pin;
}

uint8_t Get_VARIANT_ModePin(void) {
  const uint32_t cu32_selected_variant_idx = VARIANT_GetVariantIndex();
  return gscst_variant_tbl[cu32_selected_variant_idx].u8_mode_pin;
}

uint8_t Get_VARIANT_LampPin(void) {
  const uint32_t cu32_selected_variant_idx = VARIANT_GetVariantIndex();
  return gscst_variant_tbl[cu32_selected_variant_idx].u8_lamp_pin;
}

uint8_t Get_VARIANT_Brightness(void) {
  const uint32_t cu32_selected_variant_idx = VARIANT_GetVariantIndex();
  uint8_t u8_brightness = gscst_variant_tbl[cu32_selected_variant_idx].u8_brightness;
  M_CLIP_MAX(u8_brightness, m_VARIANT_LED_BRIGHTNESS_MAX);
  M_CLIP_MIN(u8_brightness, m_VARIANT_LED_BRIGHTNESS_MIN);
  return u8_brightness;
}

static uint32_t VARIANT_GetVariantIndex() {
  uint8_t u8_variant_idx = Get_NVM_Variant_idx();
  if (u8_variant_idx < m_VARIANT_TABLE_SIZE) {
    return u8_variant_idx;
  } else {
    return 0;
  }
}
