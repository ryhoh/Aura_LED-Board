#ifndef _DEVICEINTERFACE_H_
#define _DEVICEINTERFACE_H_

#include <stdint.h>

#ifdef ARDUINO
#include <Arduino.h>
#include <EEPROM.h>
#endif

#ifdef EPS8266
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#endif

#ifdef ESP32
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#endif

#ifdef SIMULATOR
#include <string>
#include <unistd.h>
#include <random>
#include "../../MCU_Simulator/mock/NetworkMock.h"
#include "../../MCU_Simulator/mock/NVMMock.h"
#include "../../MCU_Simulator/mock/Max7219Mock.h"

#define INPUT (0)
#define OUTPUT (1)
#define INPUT_PULLUP (2)
#define INPUT_PULLDOWN (3)
#define HIGH (1)
#define LOW (0)
#define delay(ms) (void)0
#define delayMicroseconds(us) (void)0
#define MSBFIRST (0)
#define LSBFIRST (1)

#define substring(begin, end) substr(begin, end)

using String = std::string;

void noInterrupts();
void interrupts();

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
