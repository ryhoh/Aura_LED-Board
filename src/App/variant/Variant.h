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
#define m_VARIANT_TYPE_NAME_SIZE (5)
#define m_VARIANT_MACHINE_NAME_SIZE (13)

#define m_VARIANT_LED_BRIGHTNESS_MAX (9)
#define m_VARIANT_LED_BRIGHTNESS_MIN (1)

// 構造体定義
typedef struct _Variant {
    char i8_type_name[m_VARIANT_TYPE_NAME_SIZE];
    char i8_machine_name[m_VARIANT_MACHINE_NAME_SIZE];
    uint8_t u8_dummy[2];
    uint32_t u32_matrix_num;
    uint8_t u8_spi_data_pin;
    uint8_t u8_spi_cs_pin;
    uint8_t u8_spi_clock_pin;
    uint8_t u8_mode_pin;
    uint8_t u8_lamp_pin;
    uint8_t u8_brightness;
} Variant_t;

// プロトタイプ宣言
// const char *Get_VARIANT_TypeName(void);
// const char *Get_VARIANT_MachineName(void);
uint8_t Get_VARIANT_SPIDataPin(void);
uint8_t Get_VARIANT_SPICSPin(void);
uint8_t Get_VARIANT_SPIClockPin(void);
uint8_t Get_VARIANT_ModePin(void);
uint8_t Get_VARIANT_LampPin(void);
uint8_t Get_VARIANT_Brightness(void);
uint32_t Get_VARIANT_MatrixNum(void);

#endif  /* _VARIANT_H_ */
