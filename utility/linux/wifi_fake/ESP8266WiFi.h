//
// Created by SomeDude on 12.05.2019.
//

#ifndef CMQTTSNFORWARDER_UTILITY_LINUX_WIFI_FAKE_ESP8266WIFI_H_
#define CMQTTSNFORWARDER_UTILITY_LINUX_WIFI_FAKE_ESP8266WIFI_H_

#include <IPAddress.h>

#define WL_CONNECTED 1
#define WIFI_STA 2

class ESP8266WiFiClass {
 public:
  int begin(const char *ssid, const char *password);
  int status();
  IPAddress localIP();
  void disconnect();
  void mode(int m);
  int hostByName(const char *hostname, IPAddress remote_addr, int timeout_ms);
};

extern ESP8266WiFiClass WiFi;

#endif //CMQTTSNFORWARDER_UTILITY_LINUX_WIFI_FAKE_ESP8266WIFI_H_
