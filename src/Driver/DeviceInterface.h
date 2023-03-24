#ifndef _DEVICEINTERFACE_H_
#define _DEVICEINTERFACE_H_

#include <stdint.h>

#ifdef ARDUINO
#include <Arduino.h>
#include <EEPROM.h>
#endif

/* デバイスの差異を吸収する共通の関数群 */

/* -- Definition -- */
static inline void call_pinMode(uint8_t pin, uint8_t INPUT_or_OUTPUT);
static inline int32_t call_digitalRead(uint8_t pin);
static inline void call_digitalWrite(uint8_t pin, uint8_t HIGH_or_LOW);
static inline void call_sleep(uint32_t ms);
static inline void call_usleep(uint32_t us);
static inline void call_shiftOut(uint8_t data, uint8_t dat_pin, uint8_t clk_pin, uint8_t lat_pin);
static inline void call_nvm_init(uint32_t size);
static inline void call_nvm_read(int32_t begin_addr, char buf[], uint32_t size);
static inline void call_nvm_write(int32_t begin_addr, const char buf[], uint32_t size);
static inline void call_nvm_commit();
static inline int32_t call_randint(int32_t max);
/* -------------------- */

#ifdef ARDUINO
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

static void call_pinMode(uint8_t pin, uint8_t INPUT_or_OUTPUT) {
  pinMode(pin, INPUT_or_OUTPUT);
}

static inline int32_t call_digitalRead(uint8_t pin) {
  return digitalRead(pin);
}

static inline void call_digitalWrite(uint8_t pin, uint8_t HIGH_or_LOW) {
  digitalWrite(pin, HIGH_or_LOW);
}

static inline void call_sleep(uint32_t ms) {
  delay(ms);
}

static inline void call_usleep(uint32_t us) {
  delayMicroseconds(us);
}

static inline void call_shiftOut(uint8_t data, uint8_t dat_pin, uint8_t clk_pin, uint8_t lat_pin) {
  shiftOut(data, dat_pin, clk_pin, lat_pin);
}

static inline void call_nvm_init(uint32_t size) {
  EEPROM.begin(size);
}

#define EEPROM_RW_MAX_SIZE 32
static inline void call_nvm_read(int32_t begin_addr, char buf[], uint32_t size) {
  // Clip by EEPROM_RW_MAX_SIZE
  int32_t end_addr = begin_addr + size;
  if (end_addr > EEPROM_RW_MAX_SIZE + begin_addr) {
    end_addr = EEPROM_RW_MAX_SIZE + begin_addr;
  }

  for (int32_t addr = begin_addr; addr < end_addr; addr++) {
    buf[addr - begin_addr] = EEPROM.read(addr);
  }
}

static inline void call_nvm_write(int32_t begin_addr, const char buf[], uint32_t size) {
  // Clip by EEPROM_RW_MAX_SIZE
  int32_t end_addr = begin_addr + size;
  if (end_addr > EEPROM_RW_MAX_SIZE + begin_addr) {
    end_addr = EEPROM_RW_MAX_SIZE + begin_addr;
  }

  for (int32_t addr = begin_addr; addr < end_addr; addr++) {
    EEPROM.write(addr, buf[addr - begin_addr]);
  }
}

static inline void call_nvm_commit() {
  EEPROM.commit();
}

static inline int32_t call_randint(int32_t max) {
  return random(0, max);
}

#endif  /* ARDUINO */

#ifdef SIMULATOR
#include <unistd.h>

static inline void call_sleep(uint32_t ms) {
  usleep(ms * 1000);
}
#endif  /* SIMULATOR */

#endif  /* _DEVICEINTERFACE_H_ */
