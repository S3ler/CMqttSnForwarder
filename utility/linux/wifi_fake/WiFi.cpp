//
// Created by SomeDude on 12.05.2019.
//

#include "WiFi.h"

int ESP8266WiFiClass::status() {
  return WL_CONNECTED;
}
IPAddress ESP8266WiFiClass::localIP() {
  return IPAddress();
}
void ESP8266WiFiClass::disconnect() {

}
void ESP8266WiFiClass::mode(int m) {

}
int ESP8266WiFiClass::begin(const char *ssid, const char *password) {
  return WL_CONNECTED;
}
int ESP8266WiFiClass::hostByName(const char *hostname, IPAddress remote_addr, int timeout_ms) {
  return 1;
}

ESP8266WiFiClass WiFi;
