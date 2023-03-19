#include "MatrixLED.h"

MatrixLED::MatrixLED(uint8_t width, uint8_t height)
{
  if (8 < width || 8 < height) {
    width = height = 0;
    return;
  }

  this->width  = width;
  this->height = height;
  memset(this->buffer, 0, 8);
}

void MatrixLED::flip()
{
  for (uint8_t i = 0; i < this->height; ++i)
    *(this->buffer + i) ^= 0xFF;
}

void MatrixLED::fill(bool value)
{
  memset(this->buffer, 0xFF * value, 8);
}

uint8_t MatrixLED::leftShift(uint8_t padding)
{
  uint8_t out = 0;  // 7行目, 6行目, ... 0行目とビットを詰める padding も同じ並び
  for (uint8_t bit_i = 0; bit_i < this->height; ++bit_i)
  {
    bool top_bit = (*(this->buffer + bit_i) >> 7) & 0b1;
    if (top_bit)
      out |= 0x01 << bit_i;
    else
      out &= ~(0b01 << bit_i);

    *(this->buffer + bit_i) <<= 1;
    bool bottom_bit = (padding >> bit_i) & 0b1;
    if (bottom_bit)
      *(this->buffer + bit_i) |= 0x01;
    else
      *(this->buffer + bit_i) &= 0xFE;
  }
  return out;
}
