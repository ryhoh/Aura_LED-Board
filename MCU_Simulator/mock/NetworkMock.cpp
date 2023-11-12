#include "NetworkMock.h"

/* functions */

// WiFi
WebServer::WebServer(int32_t port) {
}

void WebServer::begin() {
  return;
}

void WebServer::on(std::string path, void (*handler)()) {
  return;
}

void WebServer::on(std::string path, int32_t method, void (*handler)()) {
  return;
}

void WebServer::onNotFound(void (*handler)()) {
  return;
}

void WebServer::send(int32_t code, std::string content) {
  return;
}

void WebServer::send(int32_t code, std::string type, std::string content) {
  return;
}

bool WebServer::hasArg (std::string arg) {
    return false;
}

std::string WebServer::arg (std::string arg) {
    return "";
}

void WebServer::handleClient() {
  return;
}

// IPAddress
IPAddress::IPAddress(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
  this->octet[0] = a;
  this->octet[1] = b;
  this->octet[2] = c;
  this->octet[3] = d;
}

uint8_t IPAddress::operator[] (uint8_t index) const {
  return this->octet[index];
}

// WiFiClass
void WiFiClass::mode(int32_t mode) {
  return;
}

void WiFiClass::begin(std::string ssid, std::string password) {
  return;
}

void WiFiClass::softAP(std::string ssid, std::string password) {
  return;
}

IPAddress WiFiClass::softAPIP() {
  return IPAddress(127, 0, 0, 1);
}

void WiFiClass::softAPConfig(IPAddress ip, IPAddress gateway, IPAddress subnet) {
  return;
}

IPAddress WiFiClass::localIP() {
  return IPAddress(192, 168, 1, 60);  // 適当
}

int32_t WiFiClass::status() {
  return WL_CONNECTED;
}

void WiFiClass::disconnect() {
  return;
}

void configTime(int32_t timezone, int32_t daylightOffset_sec, std::string server1) {
  return;
}

/* variables */
WiFiClass WiFi;
