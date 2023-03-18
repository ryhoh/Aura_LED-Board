#include <gtest/gtest.h>

#include <AsciiMatrix.h>

TEST(AsciiMatrixTest, writeAsciiToMatrixLED) {
  MatrixLED matLED(8, 8);

  writeAsciiToMatrixLED(matLED, 'H', 0);

  EXPECT_EQ(0b00000000, matLED.buffer[0]);
  EXPECT_EQ(0b10001000, matLED.buffer[1]);
  EXPECT_EQ(0b10001000, matLED.buffer[2]);
  EXPECT_EQ(0b10001000, matLED.buffer[3]);
  EXPECT_EQ(0b11111000, matLED.buffer[4]);
  EXPECT_EQ(0b10001000, matLED.buffer[5]);
  EXPECT_EQ(0b10001000, matLED.buffer[6]);
  EXPECT_EQ(0b10001000, matLED.buffer[7]);

  matLED.fill(false);
  writeAsciiToMatrixLED(matLED, 'H', 3);

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
  writeAsciiToMatrixLED(matLED, 'H', 0);
  writeAsciiToMatrixLED(matLED, 'i', 6);

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
  writeAsciiToMatrixLED(matLED, 'H', -2);
  writeAsciiToMatrixLED(matLED, 'i', 4);
  EXPECT_EQ(0b00000000, matLED.buffer[0]);
  EXPECT_EQ(0b00100000, matLED.buffer[1]);
  EXPECT_EQ(0b00100010, matLED.buffer[2]);
  EXPECT_EQ(0b00100000, matLED.buffer[3]);
  EXPECT_EQ(0b11100110, matLED.buffer[4]);
  EXPECT_EQ(0b00100010, matLED.buffer[5]);
  EXPECT_EQ(0b00100010, matLED.buffer[6]);
  EXPECT_EQ(0b00100111, matLED.buffer[7]);

  // try with illegal char
  matLED.fill(false);
  writeAsciiToMatrixLED(matLED, -1, 3);

  EXPECT_EQ(0b00000000, matLED.buffer[0]);
  EXPECT_EQ(0b00011001, matLED.buffer[1]);
  EXPECT_EQ(0b00010101, matLED.buffer[2]);
  EXPECT_EQ(0b00010011, matLED.buffer[3]);
  EXPECT_EQ(0b00000000, matLED.buffer[4]);
  EXPECT_EQ(0b00000100, matLED.buffer[5]);
  EXPECT_EQ(0b00001110, matLED.buffer[6]);
  EXPECT_EQ(0b00010001, matLED.buffer[7]);
}

TEST(AsciiMatrixTest, writeAsciisToMatrixLEDs) {
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
