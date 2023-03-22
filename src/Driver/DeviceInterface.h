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
static inline void call_digitalWrite(uint8_t pin, uint8_t HIGH_or_LOW);
static inline void call_sleep(uint32_t ms);
static inline void call_usleep(uint32_t us);
static inline void call_shiftOut(uint8_t data, uint8_t dat_pin, uint8_t clk_pin, uint8_t lat_pin);
static inline char *call_eeprom_read(int32_t addr, char buf[]);
/* -------------------- */

#ifdef ARDUINO

static void call_pinMode(uint8_t pin, uint8_t INPUT_or_OUTPUT) {
  pinMode(pin, INPUT_or_OUTPUT);
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

static inline char *call_eeprom_read(int32_t addr, char buf[]) {
  return EEPROM.get(addr, buf);
}

#endif  /* ARDUINO */

#ifdef SIMULATOR
#include <unistd.h>

static inline void call_sleep(uint32_t ms) {
  usleep(ms * 1000);
}
#endif  /* SIMULATOR */

#endif  /* _DEVICEINTERFACE_H_ */
