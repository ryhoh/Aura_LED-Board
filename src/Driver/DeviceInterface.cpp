#include "Driver/DeviceInterface.h"

/* デバイスの差異を吸収する共通の関数群 */

uint32_t drv_eeprom_max_size = 0;

void call_pinMode(uint8_t pin, uint8_t INPUT_or_OUTPUT) {
#ifdef ARDUINO
  pinMode(pin, INPUT_or_OUTPUT);
#endif
}

int32_t call_digitalRead(uint8_t pin) {
#ifdef ARDUINO
  return digitalRead(pin);
#else
  return HIGH;  // WiFi をクライアントモードとする設定
#endif
}

void call_digitalWrite(uint8_t pin, uint8_t HIGH_or_LOW) {
#ifdef ARDUINO
  digitalWrite(pin, HIGH_or_LOW);
#endif
}

void call_sleep(uint32_t ms) {
#ifdef ARDUINO
  delay(ms);
#else
  usleep(ms * 1000);
#endif
}

void call_usleep(uint32_t us) {
#ifdef ARDUINO
  delayMicroseconds(us);
#else
  usleep(us);
#endif
}

void call_shiftOut(uint8_t data, uint8_t dat_pin, uint8_t clk_pin, uint8_t lat_pin) {
#ifdef ARDUINO
  shiftOut(data, dat_pin, clk_pin, lat_pin);
#endif
}

void call_nvm_init(uint32_t size) {
#ifdef ARDUINO
  EEPROM.begin(size);
  drv_eeprom_max_size = size;
#endif
}

#define EEPROM_RW_MAX_SIZE 31
void call_nvm_read(int32_t begin_addr, char buf[], uint32_t size) {
#ifdef ARDUINO
  // Clip by EEPROM_RW_MAX_SIZE
  int32_t end_addr = begin_addr + size;
  if (end_addr > EEPROM_RW_MAX_SIZE + begin_addr) {
    end_addr = EEPROM_RW_MAX_SIZE + begin_addr;
  }

  // Clip by EEPROM size
  if (end_addr > (int32_t)drv_eeprom_max_size + 1) {
      end_addr = drv_eeprom_max_size + 1;
  }

  for (int32_t addr = begin_addr; addr < end_addr; addr++) {
    buf[addr - begin_addr] = EEPROM.read(addr);
  }
#else
  NVM_Mock_Read(begin_addr, buf, size);
#endif
}

void call_nvm_write(int32_t begin_addr, const char buf[], uint32_t size) {
#ifdef ARDUINO
  // Clip by EEPROM_RW_MAX_SIZE
  int32_t end_addr = begin_addr + size;
  if (end_addr > EEPROM_RW_MAX_SIZE + begin_addr) {
    end_addr = EEPROM_RW_MAX_SIZE + begin_addr;
  }

  // Clip by EEPROM size
  if (end_addr > (int32_t)drv_eeprom_max_size + 1) {
      end_addr = drv_eeprom_max_size + 1;
  }
  for (int32_t addr = begin_addr; addr < end_addr; addr++) {
    EEPROM.write(addr, buf[addr - begin_addr]);
  }
#else
  NVM_Mock_Write(begin_addr, buf, size);
#endif
}

void call_nvm_commit() {
#ifdef ARDUINO
  EEPROM.commit();
#endif
}

int32_t call_randint(int32_t max) {
#ifdef ARDUINO
  return random(0, max);
#else
  return std::rand() % max;
#endif
}

/* -------------------- */
#ifdef SIMULATOR
void noInterrupts() {}
void interrupts() {}
#endif
