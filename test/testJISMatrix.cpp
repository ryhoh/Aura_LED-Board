#include <gtest/gtest.h>
#include <iostream>
#include <cstdio>

#include <JISMatrix.h>
#include <MatrixLED.h>

TEST(UTF8andJISTest, writeJISToMatrixLED_ascii) {
  MatrixLED matLED(8, 8);

  writeJISToMatrixLED(matLED, "H", 0);

  EXPECT_EQ(0b00000000, matLED.buffer[0]);
  EXPECT_EQ(0b10001000, matLED.buffer[1]);
  EXPECT_EQ(0b10001000, matLED.buffer[2]);
  EXPECT_EQ(0b10001000, matLED.buffer[3]);
  EXPECT_EQ(0b11111000, matLED.buffer[4]);
  EXPECT_EQ(0b10001000, matLED.buffer[5]);
  EXPECT_EQ(0b10001000, matLED.buffer[6]);
  EXPECT_EQ(0b10001000, matLED.buffer[7]);

  matLED.fill(false);
  writeJISToMatrixLED(matLED, "H", 3);

  EXPECT_EQ(0b00000000, matLED.buffer[0]);
  EXPECT_EQ(0b00010001, matLED.buffer[1]);
  EXPECT_EQ(0b00010001, matLED.buffer[2]);
  EXPECT_EQ(0b00010001, matLED.buffer[3]);
  EXPECT_EQ(0b00011111, matLED.buffer[4]);
  EXPECT_EQ(0b00010001, matLED.buffer[5]);
  EXPECT_EQ(0b00010001, matLED.buffer[6]);
  EXPECT_EQ(0b00010001, matLED.buffer[7]);

  // try composite ("Hi")
  matLED.fill(false);
  writeJISToMatrixLED(matLED, "H", 0);
  writeJISToMatrixLED(matLED, "i", 6);

  EXPECT_EQ(0b00000000, matLED.buffer[0]);
  EXPECT_EQ(0b10001000, matLED.buffer[1]);
  EXPECT_EQ(0b10001000, matLED.buffer[2]);
  EXPECT_EQ(0b10001000, matLED.buffer[3]);
  EXPECT_EQ(0b11111001, matLED.buffer[4]);
  EXPECT_EQ(0b10001000, matLED.buffer[5]);
  EXPECT_EQ(0b10001000, matLED.buffer[6]);
  EXPECT_EQ(0b10001001, matLED.buffer[7]);

  // try minus offset and composite
  matLED.fill(false);
  writeJISToMatrixLED(matLED, "H", -2);
  writeJISToMatrixLED(matLED, "i", 4);
  EXPECT_EQ(0b00000000, matLED.buffer[0]);
  EXPECT_EQ(0b00100000, matLED.buffer[1]);
  EXPECT_EQ(0b00100010, matLED.buffer[2]);
  EXPECT_EQ(0b00100000, matLED.buffer[3]);
  EXPECT_EQ(0b11100110, matLED.buffer[4]);
  EXPECT_EQ(0b00100010, matLED.buffer[5]);
  EXPECT_EQ(0b00100010, matLED.buffer[6]);
  EXPECT_EQ(0b00100111, matLED.buffer[7]);
}

// TEST(UTF8andJISTest, 1byte) {
//   MatrixLED matLED;
//   initMatrixLED(&matLED, 8, 8);

//   const char s[] = "＼";
//   printf("%d %d %d %d\n", (uint8_t)*s, (uint8_t)*(s+1), (uint8_t)*(s+2), (uint8_t)*(s+3));
  
//   size_t val = writeJISToMatrixLED(matLED, "＼", 0);
//   // {0x80, 0x40, 0x20, 0x10, 0x8, 0x4, 0x2, 0x0},

//   EXPECT_EQ(0b00000000, matLED.buffer[0]);
//   EXPECT_EQ(0b10000000, matLED.buffer[1]);
//   EXPECT_EQ(0b01000000, matLED.buffer[2]);
//   EXPECT_EQ(0b00100000, matLED.buffer[3]);
//   EXPECT_EQ(0b00010000, matLED.buffer[4]);
//   EXPECT_EQ(0b00001000, matLED.buffer[5]);
//   EXPECT_EQ(0b00000100, matLED.buffer[6]);
//   EXPECT_EQ(0b00000010, matLED.buffer[7]);
//   EXPECT_EQ(val, 1);
// }

TEST(UTF8andJISTest, writeJISToMatrixLED_2byte) {
  MatrixLED matLED(8, 8);

  // const char s[] = "Д";
  // printf("%d %d %d\n", (uint8_t)*s, (uint8_t)*(s+1), (uint8_t)*(s+2));
  // uint32_t target = ((uint8_t)*s << 8) + (uint8_t)*(s + 1);
  // printf("%d\n", target);

  size_t val = writeJISToMatrixLED(matLED, "Д", 0);

  EXPECT_EQ(0b00000000, matLED.buffer[0]);
  EXPECT_EQ(0b00111100, matLED.buffer[1]);
  EXPECT_EQ(0b00100100, matLED.buffer[2]);
  EXPECT_EQ(0b00100100, matLED.buffer[3]);
  EXPECT_EQ(0b00100100, matLED.buffer[4]);
  EXPECT_EQ(0b01000100, matLED.buffer[5]);
  EXPECT_EQ(0b11111110, matLED.buffer[6]);
  EXPECT_EQ(0b10000010, matLED.buffer[7]);
  EXPECT_EQ(val, 2);
}

TEST(UTF8andJISTest, writeJISToMatrixLED_3byte) {
  MatrixLED matLED(8, 8);

  size_t val = writeJISToMatrixLED(matLED, "〆", 0);

  EXPECT_EQ(0b00000000, matLED.buffer[0]);
  EXPECT_EQ(0b00000010, matLED.buffer[1]);
  EXPECT_EQ(0b00000100, matLED.buffer[2]);
  EXPECT_EQ(0b01101000, matLED.buffer[3]);
  EXPECT_EQ(0b10010000, matLED.buffer[4]);
  EXPECT_EQ(0b10101000, matLED.buffer[5]);
  EXPECT_EQ(0b11000100, matLED.buffer[6]);
  EXPECT_EQ(0b10000000, matLED.buffer[7]);
  EXPECT_EQ(val, 3);

  matLED.fill(false);
  val = writeJISToMatrixLED(matLED, "あ", 0);

  EXPECT_EQ(0b00000000, matLED.buffer[0]);
  EXPECT_EQ(0b00100000, matLED.buffer[1]);
  EXPECT_EQ(0b01111100, matLED.buffer[2]);
  EXPECT_EQ(0b00100000, matLED.buffer[3]);
  EXPECT_EQ(0b01111100, matLED.buffer[4]);
  EXPECT_EQ(0b10101010, matLED.buffer[5]);
  EXPECT_EQ(0b10110010, matLED.buffer[6]);
  EXPECT_EQ(0b01100100, matLED.buffer[7]);
  EXPECT_EQ(val, 3);

  matLED.fill(false);
  val = writeJISToMatrixLED(matLED, "改", 0);

  EXPECT_EQ(0b00000000, matLED.buffer[0]);
  EXPECT_EQ(0b11101000, matLED.buffer[1]);
  EXPECT_EQ(0b00101110, matLED.buffer[2]);
  EXPECT_EQ(0b11111010, matLED.buffer[3]);
  EXPECT_EQ(0b10001010, matLED.buffer[4]);
  EXPECT_EQ(0b10101010, matLED.buffer[5]);
  EXPECT_EQ(0b11100100, matLED.buffer[6]);
  EXPECT_EQ(0b00011010, matLED.buffer[7]);
  EXPECT_EQ(val, 3);

  // corner case (last char)
  matLED.fill(false);
  val = writeJISToMatrixLED(matLED, "￥", 0);

  EXPECT_EQ(0b00000000, matLED.buffer[0]);
  EXPECT_EQ(0b01000100, matLED.buffer[1]);
  EXPECT_EQ(0b01000100, matLED.buffer[2]);
  EXPECT_EQ(0b00101000, matLED.buffer[3]);
  EXPECT_EQ(0b01111100, matLED.buffer[4]);
  EXPECT_EQ(0b00010000, matLED.buffer[5]);
  EXPECT_EQ(0b01111100, matLED.buffer[6]);
  EXPECT_EQ(0b00010000, matLED.buffer[7]);
  EXPECT_EQ(val, 3);

  // "＼" : use 3byte code
  matLED.fill(false);
  val = writeJISToMatrixLED(matLED, "＼", 0);

  EXPECT_EQ(0b00000000, matLED.buffer[0]);
  EXPECT_EQ(0b10000000, matLED.buffer[1]);
  EXPECT_EQ(0b01000000, matLED.buffer[2]);
  EXPECT_EQ(0b00100000, matLED.buffer[3]);
  EXPECT_EQ(0b00010000, matLED.buffer[4]);
  EXPECT_EQ(0b00001000, matLED.buffer[5]);
  EXPECT_EQ(0b00000100, matLED.buffer[6]);
  EXPECT_EQ(0b00000010, matLED.buffer[7]);
  EXPECT_EQ(val, 3);
}

TEST(UTF8andJISTest, writeJISsToMatrixLEDs_ascii) {
  MatrixLED matLEDs[5];
  for (size_t i = 0; i < 5; ++i) matLEDs[i] = MatrixLED(8, 8);

  // "Hi"
  writeAsciisToMatrixLEDs(matLEDs, 2, "Hi", 0);
  EXPECT_EQ(0b00000000, matLEDs[0].buffer[0]);
  EXPECT_EQ(0b10001000, matLEDs[0].buffer[1]);
  EXPECT_EQ(0b10001000, matLEDs[0].buffer[2]);
  EXPECT_EQ(0b10001000, matLEDs[0].buffer[3]);
  EXPECT_EQ(0b11111001, matLEDs[0].buffer[4]);
  EXPECT_EQ(0b10001000, matLEDs[0].buffer[5]);
  EXPECT_EQ(0b10001000, matLEDs[0].buffer[6]);
  EXPECT_EQ(0b10001001, matLEDs[0].buffer[7]);

  EXPECT_EQ(0b00000000, matLEDs[1].buffer[0]);
  EXPECT_EQ(0b00000000, matLEDs[1].buffer[1]);
  EXPECT_EQ(0b10000000, matLEDs[1].buffer[2]);
  EXPECT_EQ(0b00000000, matLEDs[1].buffer[3]);
  EXPECT_EQ(0b10000000, matLEDs[1].buffer[4]);
  EXPECT_EQ(0b10000000, matLEDs[1].buffer[5]);
  EXPECT_EQ(0b10000000, matLEDs[1].buffer[6]);
  EXPECT_EQ(0b11000000, matLEDs[1].buffer[7]);

  // "Hi" with 5 MatrixLEDs
  for (int i = 0; i < 5; ++i)
    (matLEDs + i)->fill(false);
  
  writeAsciisToMatrixLEDs(matLEDs, 5, "Hi", 0);
  EXPECT_EQ(0b00000000, matLEDs[0].buffer[0]);
  EXPECT_EQ(0b10001000, matLEDs[0].buffer[1]);
  EXPECT_EQ(0b10001000, matLEDs[0].buffer[2]);
  EXPECT_EQ(0b10001000, matLEDs[0].buffer[3]);
  EXPECT_EQ(0b11111001, matLEDs[0].buffer[4]);
  EXPECT_EQ(0b10001000, matLEDs[0].buffer[5]);
  EXPECT_EQ(0b10001000, matLEDs[0].buffer[6]);
  EXPECT_EQ(0b10001001, matLEDs[0].buffer[7]);

  EXPECT_EQ(0b00000000, matLEDs[1].buffer[0]);
  EXPECT_EQ(0b00000000, matLEDs[1].buffer[1]);
  EXPECT_EQ(0b10000000, matLEDs[1].buffer[2]);
  EXPECT_EQ(0b00000000, matLEDs[1].buffer[3]);
  EXPECT_EQ(0b10000000, matLEDs[1].buffer[4]);
  EXPECT_EQ(0b10000000, matLEDs[1].buffer[5]);
  EXPECT_EQ(0b10000000, matLEDs[1].buffer[6]);
  EXPECT_EQ(0b11000000, matLEDs[1].buffer[7]);

  EXPECT_EQ(0b00000000, matLEDs[2].buffer[0]);
  EXPECT_EQ(0b00000000, matLEDs[2].buffer[1]);
  EXPECT_EQ(0b00000000, matLEDs[2].buffer[2]);
  EXPECT_EQ(0b00000000, matLEDs[2].buffer[3]);
  EXPECT_EQ(0b00000000, matLEDs[2].buffer[4]);
  EXPECT_EQ(0b00000000, matLEDs[2].buffer[5]);
  EXPECT_EQ(0b00000000, matLEDs[2].buffer[6]);
  EXPECT_EQ(0b00000000, matLEDs[2].buffer[7]);

  EXPECT_EQ(0b00000000, matLEDs[3].buffer[0]);
  EXPECT_EQ(0b00000000, matLEDs[3].buffer[1]);
  EXPECT_EQ(0b00000000, matLEDs[3].buffer[2]);
  EXPECT_EQ(0b00000000, matLEDs[3].buffer[3]);
  EXPECT_EQ(0b00000000, matLEDs[3].buffer[4]);
  EXPECT_EQ(0b00000000, matLEDs[3].buffer[5]);
  EXPECT_EQ(0b00000000, matLEDs[3].buffer[6]);
  EXPECT_EQ(0b00000000, matLEDs[3].buffer[7]);

  EXPECT_EQ(0b00000000, matLEDs[4].buffer[0]);
  EXPECT_EQ(0b00000000, matLEDs[4].buffer[1]);
  EXPECT_EQ(0b00000000, matLEDs[4].buffer[2]);
  EXPECT_EQ(0b00000000, matLEDs[4].buffer[3]);
  EXPECT_EQ(0b00000000, matLEDs[4].buffer[4]);
  EXPECT_EQ(0b00000000, matLEDs[4].buffer[5]);
  EXPECT_EQ(0b00000000, matLEDs[4].buffer[6]);
  EXPECT_EQ(0b00000000, matLEDs[4].buffer[7]);

  // "Hello!"
  for (int i = 0; i < 5; ++i)
    (matLEDs + i)->fill(false);
  
  writeAsciisToMatrixLEDs(matLEDs, 5, "Hello!", 0);
  EXPECT_EQ(0b00000000, matLEDs[0].buffer[0]);
  EXPECT_EQ(0b10001000, matLEDs[0].buffer[1]);
  EXPECT_EQ(0b10001000, matLEDs[0].buffer[2]);
  EXPECT_EQ(0b10001001, matLEDs[0].buffer[3]);
  EXPECT_EQ(0b11111010, matLEDs[0].buffer[4]);
  EXPECT_EQ(0b10001011, matLEDs[0].buffer[5]);
  EXPECT_EQ(0b10001010, matLEDs[0].buffer[6]);
  EXPECT_EQ(0b10001001, matLEDs[0].buffer[7]);

  EXPECT_EQ(0b00000000, matLEDs[1].buffer[0]);
  EXPECT_EQ(0b00000110, matLEDs[1].buffer[1]);
  EXPECT_EQ(0b00000010, matLEDs[1].buffer[2]);
  EXPECT_EQ(0b11000010, matLEDs[1].buffer[3]);
  EXPECT_EQ(0b00100010, matLEDs[1].buffer[4]);
  EXPECT_EQ(0b11100010, matLEDs[1].buffer[5]);
  EXPECT_EQ(0b00000010, matLEDs[1].buffer[6]);
  EXPECT_EQ(0b11100010, matLEDs[1].buffer[7]);

  EXPECT_EQ(0b00000000, matLEDs[2].buffer[0]);
  EXPECT_EQ(0b00011000, matLEDs[2].buffer[1]);
  EXPECT_EQ(0b00001000, matLEDs[2].buffer[2]);
  EXPECT_EQ(0b00001000, matLEDs[2].buffer[3]);
  EXPECT_EQ(0b00001000, matLEDs[2].buffer[4]);
  EXPECT_EQ(0b00001000, matLEDs[2].buffer[5]);
  EXPECT_EQ(0b00001000, matLEDs[2].buffer[6]);
  EXPECT_EQ(0b00001000, matLEDs[2].buffer[7]);

  EXPECT_EQ(0b00000000, matLEDs[3].buffer[0]);
  EXPECT_EQ(0b00000000, matLEDs[3].buffer[1]);
  EXPECT_EQ(0b00000000, matLEDs[3].buffer[2]);
  EXPECT_EQ(0b01110000, matLEDs[3].buffer[3]);
  EXPECT_EQ(0b10001000, matLEDs[3].buffer[4]);
  EXPECT_EQ(0b10001000, matLEDs[3].buffer[5]);
  EXPECT_EQ(0b10001000, matLEDs[3].buffer[6]);
  EXPECT_EQ(0b01110000, matLEDs[3].buffer[7]);

  EXPECT_EQ(0b00000000, matLEDs[4].buffer[0]);
  EXPECT_EQ(0b10000000, matLEDs[4].buffer[1]);
  EXPECT_EQ(0b10000000, matLEDs[4].buffer[2]);
  EXPECT_EQ(0b10000000, matLEDs[4].buffer[3]);
  EXPECT_EQ(0b10000000, matLEDs[4].buffer[4]);
  EXPECT_EQ(0b10000000, matLEDs[4].buffer[5]);
  EXPECT_EQ(0b00000000, matLEDs[4].buffer[6]);
  EXPECT_EQ(0b10000000, matLEDs[4].buffer[7]);

  // "Hello!" with only 2 MatrixLEDs
  for (int i = 0; i < 5; ++i)
    (matLEDs + i)->fill(false);
  
  writeAsciisToMatrixLEDs(matLEDs, 2, "Hello!", 0);
  EXPECT_EQ(0b00000000, matLEDs[0].buffer[0]);
  EXPECT_EQ(0b10001000, matLEDs[0].buffer[1]);
  EXPECT_EQ(0b10001000, matLEDs[0].buffer[2]);
  EXPECT_EQ(0b10001001, matLEDs[0].buffer[3]);
  EXPECT_EQ(0b11111010, matLEDs[0].buffer[4]);
  EXPECT_EQ(0b10001011, matLEDs[0].buffer[5]);
  EXPECT_EQ(0b10001010, matLEDs[0].buffer[6]);
  EXPECT_EQ(0b10001001, matLEDs[0].buffer[7]);

  EXPECT_EQ(0b00000000, matLEDs[1].buffer[0]);
  EXPECT_EQ(0b00000110, matLEDs[1].buffer[1]);
  EXPECT_EQ(0b00000010, matLEDs[1].buffer[2]);
  EXPECT_EQ(0b11000010, matLEDs[1].buffer[3]);
  EXPECT_EQ(0b00100010, matLEDs[1].buffer[4]);
  EXPECT_EQ(0b11100010, matLEDs[1].buffer[5]);
  EXPECT_EQ(0b00000010, matLEDs[1].buffer[6]);
  EXPECT_EQ(0b11100010, matLEDs[1].buffer[7]);

  EXPECT_EQ(0b00000000, matLEDs[2].buffer[0]);
  EXPECT_EQ(0b00000000, matLEDs[2].buffer[1]);
  EXPECT_EQ(0b00000000, matLEDs[2].buffer[2]);
  EXPECT_EQ(0b00000000, matLEDs[2].buffer[3]);
  EXPECT_EQ(0b00000000, matLEDs[2].buffer[4]);
  EXPECT_EQ(0b00000000, matLEDs[2].buffer[5]);
  EXPECT_EQ(0b00000000, matLEDs[2].buffer[6]);
  EXPECT_EQ(0b00000000, matLEDs[2].buffer[7]);

  EXPECT_EQ(0b00000000, matLEDs[3].buffer[0]);
  EXPECT_EQ(0b00000000, matLEDs[3].buffer[1]);
  EXPECT_EQ(0b00000000, matLEDs[3].buffer[2]);
  EXPECT_EQ(0b00000000, matLEDs[3].buffer[3]);
  EXPECT_EQ(0b00000000, matLEDs[3].buffer[4]);
  EXPECT_EQ(0b00000000, matLEDs[3].buffer[5]);
  EXPECT_EQ(0b00000000, matLEDs[3].buffer[6]);
  EXPECT_EQ(0b00000000, matLEDs[3].buffer[7]);

  EXPECT_EQ(0b00000000, matLEDs[4].buffer[0]);
  EXPECT_EQ(0b00000000, matLEDs[4].buffer[1]);
  EXPECT_EQ(0b00000000, matLEDs[4].buffer[2]);
  EXPECT_EQ(0b00000000, matLEDs[4].buffer[3]);
  EXPECT_EQ(0b00000000, matLEDs[4].buffer[4]);
  EXPECT_EQ(0b00000000, matLEDs[4].buffer[5]);
  EXPECT_EQ(0b00000000, matLEDs[4].buffer[6]);
  EXPECT_EQ(0b00000000, matLEDs[4].buffer[7]);
}

TEST(UTF8andJISTest, writeJISsToMatrixLEDs_mix) {
  MatrixLED matLEDs[5];
  for (size_t i = 0; i < 5; ++i) matLEDs[i] = MatrixLED(8, 8);

  writeJISsToMatrixLEDs(matLEDs, 5, "1あ5￥", 0);

  EXPECT_EQ(0b00000000, matLEDs[0].buffer[0]);
  EXPECT_EQ(0b00100000, matLEDs[0].buffer[1]);
  EXPECT_EQ(0b01100001, matLEDs[0].buffer[2]);
  EXPECT_EQ(0b00100000, matLEDs[0].buffer[3]);
  EXPECT_EQ(0b00100001, matLEDs[0].buffer[4]);
  EXPECT_EQ(0b00100010, matLEDs[0].buffer[5]);
  EXPECT_EQ(0b00100010, matLEDs[0].buffer[6]);
  EXPECT_EQ(0b01110001, matLEDs[0].buffer[7]);

  EXPECT_EQ(0b00000000, matLEDs[1].buffer[0]);
  EXPECT_EQ(0b10000011, matLEDs[1].buffer[1]);
  EXPECT_EQ(0b11110010, matLEDs[1].buffer[2]);
  EXPECT_EQ(0b10000011, matLEDs[1].buffer[3]);
  EXPECT_EQ(0b11110000, matLEDs[1].buffer[4]);
  EXPECT_EQ(0b10101000, matLEDs[1].buffer[5]);
  EXPECT_EQ(0b11001010, matLEDs[1].buffer[6]);
  EXPECT_EQ(0b10010001, matLEDs[1].buffer[7]);

  EXPECT_EQ(0b00000000, matLEDs[2].buffer[0]);
  EXPECT_EQ(0b11100100, matLEDs[2].buffer[1]);
  EXPECT_EQ(0b00000100, matLEDs[2].buffer[2]);
  EXPECT_EQ(0b11000010, matLEDs[2].buffer[3]);
  EXPECT_EQ(0b00100111, matLEDs[2].buffer[4]);
  EXPECT_EQ(0b00100001, matLEDs[2].buffer[5]);
  EXPECT_EQ(0b00100111, matLEDs[2].buffer[6]);
  EXPECT_EQ(0b11000001, matLEDs[2].buffer[7]);

  EXPECT_EQ(0b00000000, matLEDs[3].buffer[0]);
  EXPECT_EQ(0b01000000, matLEDs[3].buffer[1]);
  EXPECT_EQ(0b01000000, matLEDs[3].buffer[2]);
  EXPECT_EQ(0b10000000, matLEDs[3].buffer[3]);
  EXPECT_EQ(0b11000000, matLEDs[3].buffer[4]);
  EXPECT_EQ(0b00000000, matLEDs[3].buffer[5]);
  EXPECT_EQ(0b11000000, matLEDs[3].buffer[6]);
  EXPECT_EQ(0b00000000, matLEDs[3].buffer[7]);
}
