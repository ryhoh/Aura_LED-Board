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
  { { 0x00, 0x00, 0x00, 0x00, 0x00 } /* (INVALID) */ , "INVALID_ROM ", { 0, 0 }, 0 },
  { { 0x4C, 0x55, 0x2D, 0x41, 0x31 } /* "LU-A1"   */ , "LU-LEDB-01A1", { 0, 0 }, 4 },
  { { 0x4C, 0x55, 0x2D, 0x41, 0x31 } /* "LU-A1"   */ , "LU-LEDB-02A1", { 0, 0 }, 8 }
};

// 関数定義
const char *Get_VARIANT_TypeName(void) {
  const uint32_t cu32_selected_variant_idx = VARIANT_CheckVariantIndex(gscu32_variant_idx);
  return gscst_variant_tbl[cu32_selected_variant_idx].i8_type_name;
}

const char *Get_VARIANT_MachineName(void) {
  const uint32_t cu32_selected_variant_idx = VARIANT_CheckVariantIndex(gscu32_variant_idx);
  return gscst_variant_tbl[cu32_selected_variant_idx].i8_machine_name;
}

uint32_t Get_VARIANT_MatrixNum(void) {
  const uint32_t cu32_selected_variant_idx = VARIANT_CheckVariantIndex(gscu32_variant_idx);
  const uint32_t gscu32_matrix_num = gscst_variant_tbl[cu32_selected_variant_idx].u32_matrix_num;
  M_CLIP_MAX(gscu32_matrix_num, m_PROFILE_MAX_DESIGNED_PANEL_NUM);
  return gscu32_matrix_num;
}

uint32_t VARIANT_CheckVariantIndex(uint32_t u32_to_check) {
  if (u32_to_check < m_VARIANT_TABLE_SIZE) {
    return u32_to_check;
  } else {
    return 0;
  }
}
