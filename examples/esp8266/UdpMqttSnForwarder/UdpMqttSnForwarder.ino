#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <SD.h>

#include <CMqttSnForwarderArduino.h>

#ifndef STASSID
#define STASSID "BlackMesa"
#define STAPSK  "GordonGetYourOwnWifi"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;

int argc = 3;
char* arg0 = "arg0";
char* arg1 = "-cp";
char* arg2 = "98765";
char *argv[3];

forwarder_config fcfg = {0};
MqttSnLogger logger = {0};

void setup() {
  argv[0] = arg0;
  argv[1] = arg1;
  argv[1] = arg2;

  Serial.begin(115200);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  forwarder_config fcfg = {0};
  if (forwarder_config_init(&fcfg)) {
    Serial.println("forwarder_config_init");
  }

  int rc = process_forwarder_config_line(&fcfg, argc, argv);
  if (rc) {
    if (rc == 2) {
      print_usage(fcfg.logger);
    } else {
      log_flush(fcfg.logger);
      log_str(fcfg.logger, "Use 'cmqttsnforwarder --help' to see usage.");
      log_flush(fcfg.logger);
      delay(5000);
      return;
    }
  }
}


void loop() {

}