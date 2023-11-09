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
#include "App/nvm/NVM.h"

// マクロ定義
#define m_VARIANT_TABLE_SIZE (10)
#define m_VARIANT_TYPE_NAME_SIZE (5)
#define m_VARIANT_MACHINE_NAME_SIZE (20)

#define m_VARIANT_LED_BRIGHTNESS_MAX (0x0F)
#define m_VARIANT_LED_BRIGHTNESS_MIN (0x00)

#define m_VARIANT_LED_MODE_PIN_NOUSE (255)

// 構造体定義
typedef struct _Variant {
  /*  0-19 */  char i8_machine_name[m_VARIANT_MACHINE_NAME_SIZE];
  /* 20-23 */  uint32_t u32_matrix_num;
  /* 24-24 */  uint8_t u8_spi_data_pin;
  /* 25-25 */  uint8_t u8_spi_cs_pin;
  /* 26-26 */  uint8_t u8_spi_clock_pin;
  /* 27-27 */  uint8_t u8_mode_pin;
  /* 28-28 */  uint8_t u8_lamp_pin;
  /* 29-29 */  uint8_t u8_brightness;
  /* 30-31 */  uint8_t u8_dummy[2];
} Variant_t;

// プロトタイプ宣言
void Variant_Init(void);
const char *Get_VARIANT_MachineName(void);
uint8_t Get_VARIANT_SPIDataPin(void);
uint8_t Get_VARIANT_SPICSPin(void);
uint8_t Get_VARIANT_SPIClockPin(void);
uint8_t Get_VARIANT_ModePin(void);
uint8_t Get_VARIANT_LampPin(void);
uint8_t Get_VARIANT_Brightness(void);
uint32_t Get_VARIANT_MatrixNum(void);

#endif  /* _VARIANT_H_ */
