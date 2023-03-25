#include "Driver/DeviceInterface.h"

/* デバイスの差異を吸収する共通の関数群 */

#ifdef ARDUINO
uint32_t drv_eeprom_max_size = 0;

void call_pinMode(uint8_t pin, uint8_t INPUT_or_OUTPUT) {
  pinMode(pin, INPUT_or_OUTPUT);
}

int32_t call_digitalRead(uint8_t pin) {
  return digitalRead(pin);
}

void call_digitalWrite(uint8_t pin, uint8_t HIGH_or_LOW) {
  digitalWrite(pin, HIGH_or_LOW);
}

void call_sleep(uint32_t ms) {
  delay(ms);
}

void call_usleep(uint32_t us) {
  delayMicroseconds(us);
}

void call_shiftOut(uint8_t data, uint8_t dat_pin, uint8_t clk_pin, uint8_t lat_pin) {
  shiftOut(data, dat_pin, clk_pin, lat_pin);
}

void call_nvm_init(uint32_t size) {
  EEPROM.begin(size);
  drv_eeprom_max_size = size;
}

#define EEPROM_RW_MAX_SIZE 31
void call_nvm_read(int32_t begin_addr, char buf[], uint32_t size) {
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
}

void call_nvm_write(int32_t begin_addr, const char buf[], uint32_t size) {
  // Clip by EEPROM_RW_MAX_SIZE
  int32_t end_addr = begin_addr + size;
  if (end_addr > EEPROM_RW_MAX_SIZE + begin_addr) {
    end_addr = EEPROM_RW_MAX_SIZE + begin_addr;
  }

  // Clip by EEPROM size
  if (end_addr > (int32_t)drv_eeprom_max_size + 1) {
      end_addr = drv_eeprom_max_size + 1;
  }
  Serial.begin(115200);
  Serial.println(String(begin_addr) + String(" ") + String(end_addr));
  for (int32_t addr = begin_addr; addr < end_addr; addr++) {
    Serial.println(String(addr) + String(" ") + String(buf[addr - begin_addr]));
    Serial.flush();
    EEPROM.write(addr, buf[addr - begin_addr]);
  }
}

void call_nvm_commit() {
  EEPROM.commit();
}

int32_t call_randint(int32_t max) {
  return random(0, max);
}

#endif  /* ARDUINO */

#ifdef SIMULATOR
#include <unistd.h>

void call_sleep(uint32_t ms) {
  usleep(ms * 1000);
}
#endif  /* SIMULATOR */
