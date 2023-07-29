#include <iostream>
#include <cstdint>
#include <unistd.h>

#include "MW/MW_pub.h"
#include "App/App_pub.h"

const uint8_t matrix_length = 8;
const uint8_t height = 8;
const uint8_t width = 8;
const char on[]  = "●";
const char off[] = "○";

void printMatrix(MatrixLED *);

int main(int argc, char **argv) {
  const char *target = (argv[1] == nullptr) ? "qwertyuiop\r\nasdfghjkl\r\nzxcvbnm\r\n" : argv[1];

  MatrixLED matrixLEDs[matrix_length];
  for (uint8_t i = 0; i < matrix_length; ++i) {
    matrixLEDs[i] = MatrixLED(height, width);
  }

  // writeJISsToMatrixLEDs(matrixLEDs, matrix_length, target, 0);
  //
  // for (uint8_t row_i = 0; row_i < height; ++row_i) {
  //   for (uint8_t matrix_i = 0; matrix_i < matrix_length; ++matrix_i) {
  //     uint8_t row = *((matrixLEDs + matrix_i)->buffer + row_i);
  //     for (uint8_t bit_i = 0; bit_i < 8; ++bit_i)
  //       std::cout << ((row >> (7 - bit_i) & 0b1) ? on : off);
  //   }
  //   std::cout << std::endl;
  // }

  // for (int16_t i = 0; i < INT16_MAX - 1; ++i) {
  //   for (uint8_t j = 0; j < 8; ++j) {
  //     matrixLEDs[j].fill(false);
  //   }
  //   writeJISsToMatrixLEDs(matrixLEDs, 8, target, -i);
  //
  //   std::system("clear");
  //   for (uint8_t row_i = 0; row_i < height; ++row_i) {
  //     for (uint8_t matrix_i = 0; matrix_i < matrix_length; ++matrix_i) {
  //       uint8_t row = *((matrixLEDs + matrix_i)->buffer + row_i);
  //       for (uint8_t bit_i = 0; bit_i < 8; ++bit_i)
  //         std::cout << ((row >> (7 - bit_i) & 0b1) ? on : off);
  //     }
  //     std::cout << std::endl;
  //   }
  //
  //   usleep(50000);
  // }

  int16_t offset = -1;
  for (uint8_t i = 0; i < matrix_length; ++i)
    offset += matrixLEDs[i].width;

  while (true) {
    for (uint8_t i = 0; i < matrix_length; ++i)
      matrixLEDs[i].fill(false);

    if (writeJISsToMatrixLEDs(matrixLEDs, 8, target, offset) == 0)
      return 0;
    std::system("clear");
    printMatrix(matrixLEDs);
    usleep(50000);
    --offset;
  }

  // scrollJIS(nullptr, 50, matrixLEDs, 8, target);

}


void printMatrix(MatrixLED *matrixLEDs) {
  for (uint8_t row_i = 0; row_i < height; ++row_i) {
    for (uint8_t matrix_i = 0; matrix_i < matrix_length; ++matrix_i) {
      uint8_t row = *((matrixLEDs + matrix_i)->buffer + row_i);
      for (uint8_t bit_i = 0; bit_i < 8; ++bit_i)
        std::cout << ((row >> (7 - bit_i) & 0b1) ? on : off);
    }
    std::cout << std::endl;
  }
}
