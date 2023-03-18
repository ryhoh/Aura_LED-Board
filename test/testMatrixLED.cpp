#include <gtest/gtest.h>
#include <iostream>

#include <MatrixLED.h>

TEST(MatrixLEDTest, init) {
  MatrixLED matLED(8, 8);
  EXPECT_EQ(8, matLED.width);
  EXPECT_EQ(8, matLED.height);
}

TEST(MatrixLEDTest, flip) {
  MatrixLED matLED(8, 8);

  matLED.buffer[0] = 0b00100100;
  matLED.buffer[1] = 0b01001001;
  matLED.buffer[2] = 0b10010010;
  matLED.buffer[3] = 0b00100100;
  matLED.buffer[4] = 0b01001001;
  matLED.buffer[5] = 0b10010010;
  matLED.buffer[6] = 0b00100100;
  matLED.buffer[7] = 0b01001001;

  matLED.flip();

  EXPECT_EQ(0b11011011, matLED.buffer[0]);
  EXPECT_EQ(0b10110110, matLED.buffer[1]);
  EXPECT_EQ(0b01101101, matLED.buffer[2]);
  EXPECT_EQ(0b11011011, matLED.buffer[3]);
  EXPECT_EQ(0b10110110, matLED.buffer[4]);
  EXPECT_EQ(0b01101101, matLED.buffer[5]);
  EXPECT_EQ(0b11011011, matLED.buffer[6]);
  EXPECT_EQ(0b10110110, matLED.buffer[7]);

  matLED.flip();

  EXPECT_EQ(0b00100100, matLED.buffer[0]);
  EXPECT_EQ(0b01001001, matLED.buffer[1]);
  EXPECT_EQ(0b10010010, matLED.buffer[2]);
  EXPECT_EQ(0b00100100, matLED.buffer[3]);
  EXPECT_EQ(0b01001001, matLED.buffer[4]);
  EXPECT_EQ(0b10010010, matLED.buffer[5]);
  EXPECT_EQ(0b00100100, matLED.buffer[6]);
  EXPECT_EQ(0b01001001, matLED.buffer[7]);
}

TEST(MatrixLEDTest, fill) {
  MatrixLED matLED(8, 8);

  matLED.buffer[0] = 0b00100100;
  matLED.buffer[1] = 0b01001001;
  matLED.buffer[2] = 0b10010010;
  matLED.buffer[3] = 0b00100100;
  matLED.buffer[4] = 0b01001001;
  matLED.buffer[5] = 0b10010010;
  matLED.buffer[6] = 0b00100100;
  matLED.buffer[7] = 0b01001001;

  matLED.fill(false);

  EXPECT_EQ(0x00, matLED.buffer[0]);
  EXPECT_EQ(0x00, matLED.buffer[1]);
  EXPECT_EQ(0x00, matLED.buffer[2]);
  EXPECT_EQ(0x00, matLED.buffer[3]);
  EXPECT_EQ(0x00, matLED.buffer[4]);
  EXPECT_EQ(0x00, matLED.buffer[5]);
  EXPECT_EQ(0x00, matLED.buffer[6]);
  EXPECT_EQ(0x00, matLED.buffer[7]);

  matLED.fill(true);

  EXPECT_EQ(0xFF, matLED.buffer[0]);
  EXPECT_EQ(0xFF, matLED.buffer[1]);
  EXPECT_EQ(0xFF, matLED.buffer[2]);
  EXPECT_EQ(0xFF, matLED.buffer[3]);
  EXPECT_EQ(0xFF, matLED.buffer[4]);
  EXPECT_EQ(0xFF, matLED.buffer[5]);
  EXPECT_EQ(0xFF, matLED.buffer[6]);
  EXPECT_EQ(0xFF, matLED.buffer[7]);
}

TEST(MatrixLEDTest, leftShift) {
  MatrixLED matLED(8, 8);

  uint8_t carry_bits;

  // 0 padding
  matLED.buffer[0] = 0b00100100;
  matLED.buffer[1] = 0b01001001;
  matLED.buffer[2] = 0b10010010;
  matLED.buffer[3] = 0b00100100;
  matLED.buffer[4] = 0b01001001;
  matLED.buffer[5] = 0b10010010;
  matLED.buffer[6] = 0b00100100;
  matLED.buffer[7] = 0b01001001;

  carry_bits = matLED.leftShift(0x00);

  EXPECT_EQ(0b01001000, matLED.buffer[0]);
  EXPECT_EQ(0b10010010, matLED.buffer[1]);
  EXPECT_EQ(0b00100100, matLED.buffer[2]);
  EXPECT_EQ(0b01001000, matLED.buffer[3]);
  EXPECT_EQ(0b10010010, matLED.buffer[4]);
  EXPECT_EQ(0b00100100, matLED.buffer[5]);
  EXPECT_EQ(0b01001000, matLED.buffer[6]);
  EXPECT_EQ(0b10010010, matLED.buffer[7]);
  EXPECT_EQ(0b00100100, carry_bits);

  carry_bits = matLED.leftShift(0x00);

  EXPECT_EQ(0b10010000, matLED.buffer[0]);
  EXPECT_EQ(0b00100100, matLED.buffer[1]);
  EXPECT_EQ(0b01001000, matLED.buffer[2]);
  EXPECT_EQ(0b10010000, matLED.buffer[3]);
  EXPECT_EQ(0b00100100, matLED.buffer[4]);
  EXPECT_EQ(0b01001000, matLED.buffer[5]);
  EXPECT_EQ(0b10010000, matLED.buffer[6]);
  EXPECT_EQ(0b00100100, matLED.buffer[7]);
  EXPECT_EQ(0b10010010, carry_bits);

  // 1 padding
  matLED.buffer[0] = 0b00100100;
  matLED.buffer[1] = 0b01001001;
  matLED.buffer[2] = 0b10010010;
  matLED.buffer[3] = 0b00100100;
  matLED.buffer[4] = 0b01001001;
  matLED.buffer[5] = 0b10010010;
  matLED.buffer[6] = 0b00100100;
  matLED.buffer[7] = 0b01001001;

  carry_bits = matLED.leftShift(0xFF);

  EXPECT_EQ(0b01001001, matLED.buffer[0]);
  EXPECT_EQ(0b10010011, matLED.buffer[1]);
  EXPECT_EQ(0b00100101, matLED.buffer[2]);
  EXPECT_EQ(0b01001001, matLED.buffer[3]);
  EXPECT_EQ(0b10010011, matLED.buffer[4]);
  EXPECT_EQ(0b00100101, matLED.buffer[5]);
  EXPECT_EQ(0b01001001, matLED.buffer[6]);
  EXPECT_EQ(0b10010011, matLED.buffer[7]);
  EXPECT_EQ(0b00100100, carry_bits);

  carry_bits = matLED.leftShift(0xFF);

  EXPECT_EQ(0b10010011, matLED.buffer[0]);
  EXPECT_EQ(0b00100111, matLED.buffer[1]);
  EXPECT_EQ(0b01001011, matLED.buffer[2]);
  EXPECT_EQ(0b10010011, matLED.buffer[3]);
  EXPECT_EQ(0b00100111, matLED.buffer[4]);
  EXPECT_EQ(0b01001011, matLED.buffer[5]);
  EXPECT_EQ(0b10010011, matLED.buffer[6]);
  EXPECT_EQ(0b00100111, matLED.buffer[7]);
  EXPECT_EQ(0b10010010, carry_bits);
}
