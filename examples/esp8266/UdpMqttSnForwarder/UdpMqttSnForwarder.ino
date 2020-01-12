
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <SD.h>
#include <EEPROM.h>
#ifdef madpilot_mDNSResolver
#include <mDNSResolver.h>
#endif
#include <CMqttSnForwarderArduino.h>

#ifdef WITH_EEPROM
EEPROM_cfg *eeprom_cfg = NULL;
#else
#define STASSID "..."
#define STAPSK  "..."
const char* ssid     = STASSID;
const char* password = STAPSK;
#endif
MqttSnLogger fcfg_logger = {0};
forwarder_config fcfg = {0};

// forwarder
MqttSnForwarder mqttSnForwarder;
void *clientNetworkContext = nullptr;
void *gatewayNetworkContext = nullptr;
// mqtt sn gateway address
uint16_t mqtt_sn_gateway_port = 8888;
device_address mqttSnGatewayNetworkAddress = {0};
device_address forwarderClientNetworkAddress = {0};
device_address forwarderClientNetworkBroadcastAddress = {0};
device_address forwarderGatewayNetworkAddress = {0};
device_address forwarderGatewayNetworkBroadcastAddress = {0};

#ifdef WITH_ARDUINO_GATEWAY_NETWORK_UDP
// udp gateway
WiFiUDP gatewayWiFiUdp;
WiFiUDP gatewayBCWiFiUdp;
WiFiUDP *gatewayUdp = &gatewayWiFiUdp;
WiFiUDP *gatewayBCUdp = &gatewayBCWiFiUdp;
MqttSnGatewayUdpNetwork udpGatewayNetworkContext = {0};
#endif

#ifdef WITH_ARDUINO_CLIENT_NETWORK_UDP
// udp client
WiFiUDP clientWiFiUdp;
WiFiUDP clientBCWiFiUdp;
WiFiUDP *clientUdp = &clientWiFiUdp;
WiFiUDP *clientBCUdp = &clientBCWiFiUdp;

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
        if ((current - started) > (timeout * 1000)) {
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

  int wifi_rc = connect_wifi(eeprom_cfg->WiFi_cfg.ssid, eeprom_cfg->WiFi_cfg.password, &fcfg_logger, 20000);
  if (wifi_rc != 0) {
    configureModeTimeout(30);
  }
#else
  Serial.print("Connecting to ");
  Serial.print(ssid);
  Serial.print("  ");
  WiFi.begin(ssid, password);
#endif

  // MQTT-SN Gateway Address
  if (convert_hostname_port_to_device_address(&fcfg_logger,
                                              fcfg.mqtt_sn_gateway_host,
                                              fcfg.mqtt_sn_gateway_port,
                                              &mqttSnGatewayNetworkAddress,
                                              "mqtt-sn gateway")) {
    Serial.println(
        "Could not resolve MQTT-SN Gateway Host to device address - configure with: --mqtt_sn_gateway_host or --URL.");
    Serial.println("Use '--help' to see usage.");
    configureModeTimeout(30);
  }

  // Gateway Network Address
  add_port_to_device_address(fcfg.gateway_network_bind_port, &forwarderGatewayNetworkAddress);
#ifdef WITH_UDP_BROADCAST_GATEWAY
  if (convert_string_ip_port_to_device_address(&fcfg_logger,
                                               fcfg.gateway_network_broadcast_address,
                                               fcfg.gateway_network_broadcast_bind_port,
                                               &forwarderGatewayNetworkBroadcastAddress,
                                               "gateway broadcast")) {
    Serial.println(
        "Could not convert gateway broadcast to device address - configure with: --gateway_network_broadcast_address and --gateway_network_broadcast_bind_port or --gateway_network_broadcast_url.");
    Serial.println("Use '--help' to see usage.");
    configureModeTimeout(30);
  }
#endif

  // Client Network Address
  add_port_to_device_address(fcfg.client_network_bind_port, &forwarderClientNetworkAddress);
#ifdef WITH_UDP_BROADCAST_CLIENT
  if (convert_string_ip_port_to_device_address(&fcfg_logger,
                                               fcfg.client_network_broadcast_address,
                                               fcfg.client_network_broadcast_bind_port,
                                               &forwarderClientNetworkBroadcastAddress,
                                               "client broadcast")) {
    Serial.println(
        "Could not convert client broadcast to device address - configure with: --client_network_broadcast_address and --client_network_broadcast_bind_port or --client_network_broadcast_url.");
    Serial.println("Use '--help' to see usage.");
    configureModeTimeout(30);
  }
#endif

  if (!strcmp(fcfg.gateway_network_protocol, "udp")) {
    // Gateway Network is UDP
    gatewayNetworkContext = &udpGatewayNetworkContext;

#ifdef WITH_UDP_BROADCAST_GATEWAY
    if (GatewayNetworkInitialize(&mqttSnForwarder.gatewayNetwork,
                                 CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH,
                                 &mqttSnGatewayNetworkAddress,
                                 &forwarderGatewayNetworkAddress,
                                 &forwarderGatewayNetworkBroadcastAddress,
                                 &udpGatewayNetworkContext,
                                 GatewayArduinoUdpInit)) {
#else
    if (GatewayNetworkInit(&mqttSnForwarder.gatewayNetwork,
                           &mqttSnGatewayNetworkAddress,
                           &forwarderGatewayNetworkAddress,
                           NULL,
                           &udpGatewayNetworkContext,
                           GatewayArduinoUdpInit)) {
#endif
      Serial.println("Error init gateway network\n");
      configureModeTimeout(30);
    }
  }
  if (!strcmp(fcfg.client_network_protocol, "udp")) {
    // Gateway Network is UDP
    clientNetworkContext = &udpClientNetworkContext;
#ifdef WITH_UDP_BROADCAST_CLIENT
    if (ClientNetworkInitialize(&mqttSnForwarder.clientNetwork,
                                CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH,
                                &mqttSnGatewayNetworkAddress,
                                &forwarderClientNetworkAddress,
                                &forwarderClientNetworkBroadcastAddress,
                                &udpClientNetworkContext,
                                ClientArduinoUdpInit)) {
#else
    if (ClientNetworkInit(&mqttSnForwarder.clientNetwork,
                          &mqttSnGatewayNetworkAddress,
                          &forwarderClientNetworkAddress,
                          NULL,
                          &udpClientNetworkContext,
                          ClientArduinoUdpInit)) {
#endif
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
      arduino_serial_eval_process(eeprom_cfg, &fcfg, line, line_pos);
/*
      int fcfg_rc = arduino_serial_eval_process(eeprom_cfg, &fcfg, line, line_pos);
      memset(line, 0, sizeof(line));
      line_pos = 0;
*/
    }
  }
}
