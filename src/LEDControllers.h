#ifndef _LED_CONTROLLERS_H_
#define _LED_CONTROLLERS_H_

#include <cstdint>

#include "DeviceInterface.h"
#include "MatrixLED.h"

/*
 *  マトリクスLEDが複数接続（MAX7219のカスケード接続）されている場合、
 *  MAX7219に2Byteのデータを送る度に、古いデータが後ろのMAX7219へ押し出される
 *
 *  表示内容の更新を行うには、全体の表示内容を作った上で、
 *  それを1行(1digit)ずつMAX7219の数だけ繰り返して送信し、
 *  8行(8digit)分それを繰り返す
 *
 *  データ送信は (レジスタアドレス 1Byte, データ 1Byte) で行う
 *  レジスタアドレスは 0x01 ~ 0x08 を使う（0スタートでない）ことに注意
 */

/**
 * @brief Controller for Max7219
*/
struct Max7219 {
  uint8_t dat;  ///< Pin number connecting dat
  uint8_t lat;  ///< Pin number connecting lat (or cs)
  uint8_t clk;  ///< Pin number connecting clk

  /**
   * @brief Initialize Max7219.
  */
  Max7219(): dat(0), lat(0), clk(0) {}

  /**
   * @brief Initialize Max7219.
   * @param dat [in] dat pin.
   * @param lat [in] lat pin.
   * @param clk [in] clk pin.
   * @param brightness [in] Integer between [0, 9].
  */
  Max7219(uint8_t dat, uint8_t lat, uint8_t clk, uint8_t brightness);

  /**
   * @brief Destroy Max7219.
  */
  ~Max7219() {}

  /**
   * @brief Do test run.
  */
  void testRun();

  /**
   * @brief Flush data from single MatrixLED to Max7219.
   * @param matrixLED [in] Reference of MatrixLED to read.
  */
  void flushMatrixLED(MatrixLED &matrixLED);

  /**
   * @brief Flush data from multiple MatrixLEDs to Max7219.
   * @param matrixLEDs [in] Pointer of MatrixLED-Array to read.
   * @param length [in] Length of MatrixLED-Array.
   * @note When length is 1, same to flushMatrixLEDByMax7219.
  */
  void flushMatrixLEDs(MatrixLED *matrixLEDs, uint8_t length);

  // params: レジスタアドレス, データ
  // ラッチ操作を行わないバージョン
  inline void shiftOut(uint8_t addr, uint8_t data)
  {
    call_shiftOut(this->dat, this->clk, MSBFIRST, addr);
    call_shiftOut(this->dat, this->clk, MSBFIRST, data);
  }

  // params: レジスタアドレス, データ
  // ラッチ操作を行う（1件だけ送る）バージョン
  inline void send(uint8_t addr, uint8_t data)
  {
    call_digitalWrite(this->lat, LOW);
    this->shiftOut(addr, data);
    call_digitalWrite(this->lat, HIGH);
    call_digitalWrite(this->lat, LOW);
  }
};

#endif  /* _LED_CONTROLLERS_H_ */
