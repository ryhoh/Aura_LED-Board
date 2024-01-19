#include "Max7219Mock.h"

static char screen[8][MOCK_MAX7219_MAX_NUM * 8];  // 画面表示用バッファ


void Mock_Max7219_init(void) {
  memset(screen, MOCK_MAX7219_OFF_CHAR, sizeof(screen));
}

void Mock_Max7219_flushMatrixLED(MatrixLED &matrixLED) {
  // 画面表示用バッファを前に8つずらす
  for (uint8_t row_i = 0; row_i < 8; ++row_i) {
    for (uint8_t col_i = 8; col_i < MOCK_MAX7219_MAX_NUM * 8; ++col_i) {
      screen[row_i][col_i - 8] = screen[row_i][col_i];
    }
  }

  // 画面表示用バッファの後ろ8つに新しいデータを入れる
  for (uint8_t row_i = 0; row_i < 8; ++row_i) {
    for (uint8_t col_i = 0; col_i < 8; ++col_i) {
      screen[row_i][MOCK_MAX7219_MAX_NUM * 8 - 8 + col_i] = *(matrixLED.buffer + row_i) & (1 << (7 - col_i)) ? MOCK_MAX7219_ON_CHAR : MOCK_MAX7219_OFF_CHAR;
    }
  }
}

void Mock_Max7219_flushMatrixLEDs(MatrixLED *matrixLEDs, uint8_t length) {
  for (uint8_t matrix_i = 0; matrix_i < length; ++matrix_i) {
    Mock_Max7219_flushMatrixLED(*(matrixLEDs + matrix_i));
  }
}

void Mock_Max7219_flushMatrixLEDs(MatrixLED *matrixLEDs, uint8_t length, uint8_t vertical_invert) {
  if (vertical_invert) {
    for (uint8_t matrix_i = 0; matrix_i < length; ++matrix_i) {
      Mock_Max7219_flushMatrixLED(*(matrixLEDs + length - matrix_i - 1));  // @@@反転未対応
    }
  } else {
    Mock_Max7219_flushMatrixLEDs(matrixLEDs, length);
  }
}

void Mock_Max7219_PrintToStdout(void) {
  // 画面表示用バッファを標準出力に出力

  // 枠を表示
  printf("+");
  for (uint8_t col_i = 0; col_i < MOCK_MAX7219_ACTUAL_NUM * 8; ++col_i) {
    printf("-");
  }
  printf("+\n");

  // バッファの後ろ MOCK_MAX7219_ACTUAL_NUM 個分を出力する
  for (uint8_t row_i = 0; row_i < 8; ++row_i) {
    printf("|");
    for (uint8_t col_i = MOCK_MAX7219_MAX_NUM * 8 - MOCK_MAX7219_ACTUAL_NUM * 8; col_i < MOCK_MAX7219_MAX_NUM * 8; ++col_i) {
      printf("%c", screen[row_i][col_i]);
    }
    printf("|\n");
  }

  // 枠を表示
  printf("+");
  for (uint8_t col_i = 0; col_i < MOCK_MAX7219_ACTUAL_NUM * 8; ++col_i) {
    printf("-");
  }
  printf("+\n");

  // カーソル位置を左上に戻す
  printf("\033[10A");
}
