#include "Scroll.h"

void Scroller::reset() {
    this->offset = -1;
    this->isEnd = false;
    for (uint8_t i = 0; i < ledlen; ++i)
      offset += matrixLEDs[i].width;
  }

  void Scroller::step() {
    if (this->isEnd)
      return;

    for (uint8_t i = 0; i < ledlen; ++i)
      matrixLEDs[i].fill(false);

    if(writeJISsToMatrixLEDs(matrixLEDs, 8, string, offset) == 0) {
      this->isEnd = true;
      return;
    }
    --offset;
  }
