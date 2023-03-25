#ifndef _DEVICEINTERFACE_H_
#define _DEVICEINTERFACE_H_

#include <stdint.h>

#ifdef ARDUINO
#include <Arduino.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#endif

/* デバイスの差異を吸収する共通の関数群 */

/* -- Definition -- */
void call_pinMode(uint8_t pin, uint8_t INPUT_or_OUTPUT);
int32_t call_digitalRead(uint8_t pin);
void call_digitalWrite(uint8_t pin, uint8_t HIGH_or_LOW);
void call_sleep(uint32_t ms);
void call_usleep(uint32_t us);
void call_shiftOut(uint8_t data, uint8_t dat_pin, uint8_t clk_pin, uint8_t lat_pin);
void call_nvm_init(uint32_t size);
void call_nvm_read(int32_t begin_addr, char buf[], uint32_t size);
void call_nvm_write(int32_t begin_addr, const char buf[], uint32_t size);
void call_nvm_commit();
int32_t call_randint(int32_t max);
/* -------------------- */

#endif  /* _DEVICEINTERFACE_H_ */
