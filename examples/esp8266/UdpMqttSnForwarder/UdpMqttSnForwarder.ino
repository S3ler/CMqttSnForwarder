#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <SD.h>
#include <EEPROM.h>
#include <mDNSResolver.h>

#include <CMqttSnForwarderArduino.h>

#ifdef WITH_EEPROM
EEPROM_cfg* eeprom_cfg = NULL;
#else
#define STASSID "..."
#define STAPSK  "..."
const char* ssid     = STASSID;
const char* password = STAPSK;
#endif
MqttSnLogger fcfg_logger = {0};
forwarder_config fcfg = {0};

// forwarder
MqttSnForwarder mqttSnForwarder = {0};
void* clientNetworkContext = nullptr;
void* gatewayNetworkContext = nullptr;
// mqtt sn gateway address
uint16_t mqtt_sn_gateway_port = 8888;
device_address mqttSnGatewayNetworkAddress = {0};
device_address forwarderClientNetworkAddress = {0};
device_address forwarderGatewayNetworkAddress = {0};

#ifdef WITH_ARDUINO_GATEWAY_NETWORK_UDP
// udp gateway
WiFiUDP gatewayWiFiUdp;
UDP* gatewayUdp = &gatewayWiFiUdp;
MqttSnGatewayUdpNetwork udpGatewayNetworkContext = {0};
#endif

#ifdef WITH_ARDUINO_CLIENT_NETWORK_UDP
// udp client
WiFiUDP clientWiFiUdp;
UDP* clientUdp = &clientWiFiUdp;
MqttSnClientUdpNetwork udpClientNetworkContext = {0};
#endif

#ifdef WITH_ARDUINO_CLIENT_NETWORK_TCP
// tcp client
// TODO
#endif

#ifdef WITH_ARDUINO_GATEWAY_NETWORK_TCP
// tcp gateway
// TODO
#endif

char line[EEPROM_LINE_SIZE] = {0};
size_t line_pos = 0;
size_t line_max = EEPROM_LINE_SIZE;



void configureModeTimeout(int timeout) {
  if (timeout > 0) {
    Serial.print("Restarting in ");
    Serial.print(timeout, DEC);
    Serial.println(" seconds.");
  }
  int started = millis();
  digitalWrite(LED_BUILTIN, LOW);
  while (true) {
    yield();
    if (Serial.available() > 0) {
      started = millis();
      if (parse_arduino_serial_line(line, &line_pos, line_max)) {
        arduino_serial_eval_process(eeprom_cfg, &fcfg, line, line_pos);
        memset(line, 0, sizeof(line));
        line_pos = 0;

        started = millis();
        if (timeout > 0) {
          Serial.print("Restarting in ");
          Serial.print(timeout, DEC);
          Serial.println(" seconds.");
        }
      }
    }
    if (timeout > 0) {
      if (Serial.available() <= 0) {
        int current = millis();
        if ((current - started ) > (timeout * 1000)) {
          Serial.println("Restarting ...");
          eeprom_config_cleanup(&eeprom_cfg);
          log_flush(&fcfg_logger);
          delay(1000);
          ESP.restart();
        }
      }
    }
  }
}
void configureMode() {
  configureModeTimeout(0);
}
void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  Serial.println();
  yield();

  if (MqttSnLoggerInit(&fcfg_logger, LOG_LEVEL_DEBUG, arduino_serial_log_init)) {
    Serial.println("Error: Could not initialize MqttSnLogger.");
    return;
  }
  forwarder_config_init(&fcfg, &fcfg_logger);

#ifdef WITH_EEPROM
  eeprom_config_init(&eeprom_cfg);
  int eeprom_fcfg_rc = eeprom_load_forwarder_config(eeprom_cfg, &fcfg);
  if (eeprom_fcfg_rc == -2) {
    configureModeTimeout(30);
  }
  yield();

  int wifi_rc = connect_wifi(eeprom_cfg, &fcfg_logger, 20000);
  if (wifi_rc != 0) {
    configureModeTimeout(30);
  }
#else
  Serial.print("Connecting to ");
  Serial.print(ssid);
  Serial.print("  ");
  WiFi.begin(ssid, password);
#endif

  if (convert_hostname_port_to_device_address(&fcfg_logger, fcfg.mqtt_sn_gateway_host, fcfg.mqtt_sn_gateway_port, &mqttSnGatewayNetworkAddress, "mqtt-sn gateway")) {
    Serial.println("Could not resolve MQTT-SN Gateway Host to device address - configure with: --mqtt_sn_gateway_host or --URL.");
    Serial.println("Use '--help' to see usage.");
    configureModeTimeout(30);
  }

  add_port_to_device_address(fcfg.gateway_network_bind_port, &forwarderGatewayNetworkAddress);
  add_port_to_device_address(fcfg.client_network_bind_port, &forwarderClientNetworkAddress);

  if (!strcmp(fcfg.gateway_network_protocol, "udp")) {
    // Gateway Network is UDP
    gatewayNetworkContext = &udpGatewayNetworkContext;
    if (GatewayNetworkInit(&mqttSnForwarder.gatewayNetwork,
                           &mqttSnGatewayNetworkAddress,
                           &forwarderGatewayNetworkAddress,
                           NULL,
                           &udpGatewayNetworkContext,
                           GatewayArduinoUdpInit)) {
      Serial.println("Error init gateway network\n");
      configureModeTimeout(30);
    }
  }
  if (!strcmp(fcfg.client_network_protocol, "udp")) {
    // Gateway Network is UDP
    clientNetworkContext = &udpClientNetworkContext;
    if (ClientNetworkInit(&mqttSnForwarder.clientNetwork,
                          &mqttSnGatewayNetworkAddress,
                          &forwarderClientNetworkAddress,
                          NULL,
                          &udpClientNetworkContext,
                          ClientArduinoUdpInit)) {
      Serial.println("Error init client network\n");
      configureModeTimeout(30);
    }
  }
  if (!strcmp(fcfg.gateway_network_protocol, "udp")) {
    // Gateway Network is TCP
    // TODO
  }
  if (!strcmp(fcfg.client_network_protocol, "tcp")) {
    // Gateway Network is TCP
    // TODO
  }
  if (gatewayNetworkContext == nullptr || clientNetworkContext == nullptr) {
    if (gatewayNetworkContext == nullptr) {
      log_init_gateway_network_unknown_protocol(&fcfg_logger, fcfg.gateway_network_protocol);
    }
    if (clientNetworkContext == nullptr) {
      log_init_client_network_unknown_protocol(&fcfg_logger, fcfg.client_network_protocol);
    }
  }

  mqttSnForwarder.clientNetworkSendTimeout = fcfg.client_network_send_timeout;
  mqttSnForwarder.clientNetworkReceiveTimeout = fcfg.client_network_receive_timeout;
  mqttSnForwarder.gatewayNetworkSendTimeout = fcfg.gateway_network_send_timeout;
  mqttSnForwarder.gatewayNetworkReceiveTimeout = fcfg.gateway_network_receive_timeout;

  if (MqttSnForwarderInit(&mqttSnForwarder, fcfg.log_lvl, clientNetworkContext, gatewayNetworkContext)) {
    MqttSnForwarderDeinit(&mqttSnForwarder);
    Serial.println("Could not initialize Forwarder");
    configureModeTimeout(30);
  }

#ifdef WITH_LOGGING
  if (log_forwarder_started(&mqttSnForwarder.logger,
                            fcfg.version,
                            fcfg.major,
                            fcfg.minor,
                            fcfg.tweak,
                            fcfg.build_date)) {
  }
#endif
}

int forwarder_rc = 0;
void loop() {
  if (forwarder_rc == 0) {
    forwarder_rc = MqttSnForwarderLoop(&mqttSnForwarder);
  } else if (forwarder_rc == -1) {
#ifdef WITH_LOGGING
    if (log_forwarder_terminated(&mqttSnForwarder.logger,
                                 fcfg.version,
                                 fcfg.major,
                                 fcfg.minor,
                                 fcfg.tweak)) {
    }
#endif
    forwarder_rc = 2;
  } else if (forwarder_rc == 2) {
    delay(2000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(2000);
    digitalWrite(LED_BUILTIN, HIGH);
  }
  if (Serial.available() > 0) {
    if (parse_arduino_serial_line(line, &line_pos, line_max)) {
      int fcfg_rc = arduino_serial_eval_process(eeprom_cfg, &fcfg, line, line_pos);
      memset(line, 0, sizeof(line));
      line_pos = 0;
    }
  }
}
