#ifndef _NETWORK_MOCK_H_
#define _NETWORK_MOCK_H_

#include <stdint.h>
#include <string>

#define WIFI_STA (0)
#define WIFI_AP (1)
#define HTTP_GET (0)
#define HTTP_POST (1)
#define WL_CONNECTED (1)

struct WebServer {
    WebServer (int32_t port);

    void on (std::string path, void (*handler)());
    void on (std::string path, int32_t method, void (*handler)());
    void onNotFound (void (*handler)());
    void begin ();
    void send (int32_t code, std::string content);
    void send (int32_t code, std::string type, std::string content);
    bool hasArg (std::string arg);
    std::string arg (std::string arg);
    void handleClient ();
};

struct IPAddress {
    int32_t octet[4];

    IPAddress (int32_t a, int32_t b, int32_t c, int32_t d);

    int32_t operator[] (int32_t index) const;
};

struct WiFiClass {
    void mode (int32_t mode);
    void begin(std::string ssid, std::string password);
    void softAP(std::string ssid, std::string password);
    IPAddress softAPIP();
    void softAPConfig(IPAddress ip, IPAddress gateway, IPAddress subnet);
    int32_t status();
    void disconnect();
};

void configTime (int32_t timezone, int32_t daylightOffset_sec, std::string server1);

extern WiFiClass WiFi;

#endif  /* _NETWORK_MOCK_H_ */
