#include "StatusClient.h"

StatusClientOption postStatusToBoard(String name)
{
  const char *host = "aurastatusboard.herokuapp.com";
  // const char *host = "machine-hub.rycery.tk";
  const char *endpoint = "/api/heartbeat";
  const char *api_password = /* "api_password_here" */;

  // static time_t last_time = INT16_MIN;
  // const time_t current_time = time(NULL);
  // if (current_time < INT16_MIN)  // Using local time
  //   return StatusClientOption {
  //     "NTP not synchronized",
  //     StatusClientResult::FAILURE_NTP_NOT_SYNC
  //   };

  //const struct tm *current_tm = localtime(&current_time);
  // Suppress from 0 AM to 6 AM.
  //if (current_tm->tm_hour < 6 || current_time - last_time < INTERVAL_S)
    // return StatusClientOption {
    //   String("Skipped (next is ") + String(INTERVAL_S - current_time + last_time) + "s later)",
    //   StatusClientResult::SKIP
    // };
  // last_time = current_time;

  WiFiClient client;
  if (!client.connect(String(host), 80))
    return StatusClientOption {
      "Connection for StatusBoard cannot be established",
      StatusClientResult::FAILURE_CONNECTION
    };

  const String payload =
    String("name=") + name + "&password=" + api_password;

  client.print(
    String("POST ") + endpoint + " HTTP/1.1\r\n" +
    "Host: " + host + "\r\n" +
    "User-Agent: Rycery Machine-Butterfly (ESP8266)\r\n" +
    "Connection: close\r\n" +
    "Content-Type: application/x-www-form-urlencoded; charset=utf-8\r\n" +
    "Content-Length: " + payload.length() + "\r\n" +
    "\r\n" +
    payload + "\r\n"
  );
  client.flush();

  // receive and skip header
  const String buf = client.readString();
  const size_t msg_start = buf.indexOf("\r\n\r\n") + 4;

  return StatusClientOption {
    buf.substring(msg_start),
    StatusClientResult::SUCCESS
  };
}
