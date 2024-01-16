#include "LEDControllers.h"

Max7219::Max7219(uint8_t dat, uint8_t lat, uint8_t clk, uint8_t brightness, uint32_t cascading_num)
{
#ifdef ARDUINO

  this->dat = dat;
  this->lat = lat;
  this->clk = clk;
  this->cascading_num = cascading_num;

  uint8_t brightness_tmp = brightness;

  call_pinMode(dat, OUTPUT);
  call_pinMode(lat, OUTPUT);
  call_pinMode(clk, OUTPUT);
  call_digitalWrite(this->lat, HIGH);

  // 輝度設定
  if (9 < brightness_tmp) {
    brightness_tmp = 9;
  }

  /* 項目ごとに全チップへ送信する */
  for (uint32_t ul_i = 0; ul_i < cascading_num; ++ul_i) {
    this->send(0x09, 0x00);  // デコードモード -> No decodeモード
  }

  for (uint32_t ul_i = 0; ul_i < cascading_num; ++ul_i) {
    this->send(0x0A, brightness_tmp);  // 輝度設定
  }

  for (uint32_t ul_i = 0; ul_i < cascading_num; ++ul_i) {
    this->send(0x0B, 0x07);  // スキャンリミット設定 -> 8桁
  }

  for (uint32_t ul_i = 0; ul_i < cascading_num; ++ul_i) {
    this->send(0x0F, 0x00);  // ディスプレイテストモード -> テストモード終了
  }
  
  for (uint32_t ul_i = 0; ul_i < cascading_num; ++ul_i) {
    this->send(0x0C, 0x01);  // シャットダウン -> オペレート
  }

#endif  /* SIMULATOR */
}

void Max7219::testRun(uint8_t step)
{
#ifndef SIMULATOR  // シミュレータでは設定不要

  switch (step) {
  case 0:
    // ディスプレイテストモード（全点灯）
    this->send(0x0f, 0x01);
    break;
  case 1:
    // テストモード終了
    this->send(0x0f, 0x00);
    break;
  case 2:
    // リセット
    for (uint8_t addr_i = 1; addr_i < 9; ++addr_i) {
      this->send(addr_i, 0x00);
    }
    break;
  default:
    break;
  }

#endif  /* SIMULATOR */
}

void Max7219::flushMatrixLED(MatrixLED &matrixLED)
{
#ifdef SIMULATOR
  Mock_Max7219_flushMatrixLED(matrixLED);
#else  /* SIMULATOR */

  call_digitalWrite(this->lat, LOW);
  for (uint8_t row_i = 0; row_i < 8; ++row_i) {
    this->shiftOut(row_i + 1, *(matrixLED.buffer + row_i));
  }
  call_digitalWrite(this->lat, HIGH);

#endif  /* SIMULATOR */
}

void Max7219::flushMatrixLEDs(MatrixLED *matrixLEDs, uint8_t length)
{
#ifdef SIMULATOR
  Mock_Max7219_flushMatrixLEDs(matrixLEDs, length);
#else  /* SIMULATOR */

  for (uint8_t row_i = 0; row_i < 8; ++row_i) {
    call_digitalWrite(this->lat, LOW);
    for (uint8_t matrix_i = 0; matrix_i < length; ++matrix_i) {
        this->shiftOut(row_i + 1, *((matrixLEDs + matrix_i)->buffer + row_i));
    }
    call_digitalWrite(this->lat, HIGH);
  }

#endif  /* SIMULATOR */
}

void Max7219::flushMatrixLEDs(MatrixLED *matrixLEDs, uint8_t length, uint8_t vertical_invert)
{
#ifdef SIMULATOR
  Mock_Max7219_flushMatrixLEDs(matrixLEDs, length, vertical_invert);  // wip
#else  /* SIMULATOR */
  uint8_t temp = 0;
  
  if (vertical_invert) {
    for (uint8_t row_i = 0; row_i < 8; ++row_i) {
      call_digitalWrite(this->lat, LOW);
      for (int32_t matrix_i = length-1; matrix_i > -1; --matrix_i) {  // Matrix invert
          temp = *((matrixLEDs + matrix_i)->buffer + (7 - row_i));
          
          // MSB-LSB invert
          temp = ((temp & 0x01) << 7) | ((temp & 0x02) << 5) | ((temp & 0x04) << 3) | ((temp & 0x08) << 1) |
                 ((temp & 0x10) >> 1) | ((temp & 0x20) >> 3) | ((temp & 0x40) >> 5) | ((temp & 0x80) >> 7);

          this->shiftOut(row_i + 1, temp);
      }
      call_digitalWrite(this->lat, HIGH);
    }
  } else {
    for (uint8_t row_i = 0; row_i < 8; ++row_i) {
      call_digitalWrite(this->lat, LOW);
      for (uint8_t matrix_i = 0; matrix_i < length; ++matrix_i) {
          this->shiftOut(row_i + 1, *((matrixLEDs + matrix_i)->buffer + row_i));
      }
      call_digitalWrite(this->lat, HIGH);
    }
  }
#endif  /* SIMULATOR */
}

inline void Max7219::shiftOut(uint8_t addr, uint8_t data)
{
  call_shiftOut(this->dat, this->clk, MSBFIRST, addr);
  call_shiftOut(this->dat, this->clk, MSBFIRST, data);
}

inline void Max7219::send(uint8_t addr, uint8_t data)
{
  call_digitalWrite(this->lat, LOW);
  this->shiftOut(addr, data);
  call_digitalWrite(this->lat, HIGH);
}