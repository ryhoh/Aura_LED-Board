/**
 * @file Variant.h
 * @author ryhoh
 * @brief Variant header file.
 * 
 */

#ifndef _VARIANT_H_
#define _VARIANT_H_

// インクルード
#include "App/Common.h"

// マクロ定義
#define m_VARIANT_TABLE_SIZE (3)

// 構造体定義
typedef struct _Variant {
    char i8_type_name[5];
    char i8_machine_name[13];
    uint8_t u8_dummy[2];
    uint32_t u32_matrix_num;
} Variant_t;

// プロトタイプ宣言
const char *Get_VARIANT_TypeName(void);
const char *Get_VARIANT_MachineName(void);
uint32_t Get_VARIANT_MatrixNum(void);

#endif  /* _VARIANT_H_ */
