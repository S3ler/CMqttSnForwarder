#include <Arduino.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <EEPROM.h>
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include <SSD1306.h>
#include <MqttSnForwarderArduino.hpp>

#ifdef WITH_EEPROM
EEPROM_cfg *eeprom_cfg = NULL;
#else
#define STASSID "..."
#define STAPSK  "..."
const char* ssid     = STASSID;
const char* password = STAPSK;
#endif
MqttSnLogger fcfg_logger = {0};

SSD1306 display(0x3c, 21, 22);
ssd1306status display_status = {0};
MqttSnLogger screen_logger = {0};
forwarder_config fcfg = {0};

// forwarder
MqttSnForwarder mqttSnForwarder;

#ifndef MQTT_SN_FORWARDER_ARDUINO_IDENTIFIER
#define MQTT_SN_FORWARDER_ARDUINO_IDENTIFIER "FW"
#endif
char* fw_identifier = MQTT_SN_FORWARDER_ARDUINO_IDENTIFIER;

#ifdef WITH_ARDUINO_GATEWAY_NETWORK_UDP
WiFiUDP gatewayWiFiUdp;
WiFiUDP gatewayBCWiFiUdp;
WiFiUDP *gatewayUdp = &gatewayWiFiUdp;
WiFiUDP *gatewayBCUdp = &gatewayBCWiFiUdp;
MqttSnArduinoGatewayUdpNetworkContainer udpGatewayNetworkContainer;
#endif

#ifdef WITH_ARDUINO_CLIENT_NETWORK_UDP
WiFiUDP clientWiFiUdp;
WiFiUDP clientBCWiFiUdp;
WiFiUDP *clientUdp = &clientWiFiUdp;
WiFiUDP *clientBCUdp = &clientBCWiFiUdp;
device_address  udp_client_network_mqtt_sn_gateway_address;
MqttSnArduinoClientUdpNetworkContainer udpClientNetworkContainer;
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
        arduino_serial_eval_process(eeprom_cfg, &fcfg, fcfg.executable_name, &fcfg_logger, line, line_pos,
                                    forwarder_config_file_process_command_callback, forwarder_config_print_usage);
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

  if (MqttSnLoggerInit(&fcfg_logger, LOG_LEVEL_DEFAULT, arduino_serial_log_init)) {
    Serial.println("Error: Could not initialize MqttSnLogger.");
    return;
  }
  if (fw_identifier) {
    fcfg_logger.log_identifier = fw_identifier;
  }
  forwarder_config_init(&fcfg);
#ifdef WITH_EEPROM
  eeprom_config_init(&eeprom_cfg);
  int eeprom_fcfg_rc = eeprom_load_forwarder_config(eeprom_cfg, &fcfg, fcfg.executable_name, &fcfg_logger,
                       forwarder_config_file_process_command_callback, forwarder_config_print_usage);
  if (eeprom_fcfg_rc == -2) {
    configureModeTimeout(30);
  }
  yield();

  int wifi_rc = connect_wifi(eeprom_cfg->WiFi_cfg.ssid, eeprom_cfg->WiFi_cfg.password, 20000, &fcfg_logger);
  if (wifi_rc != 0) {
    configureModeTimeout(30);
  }
#else
  Serial.print("Connecting to ");
  Serial.print(ssid);
  Serial.print("  ");
  WiFi.begin(ssid, password);
#endif


  if (!strcmp(fcfg.gncfg.gateway_network_protocol, "udp")) {
    if (MqttSnArduinoGatewayUdpNetworkContainerStart(&fcfg_logger,
        fcfg.msgcfg.mqtt_sn_gateway_host, fcfg.msgcfg.mqtt_sn_gateway_port,
        fcfg.gncfg.gateway_network_bind_address, fcfg.gncfg.gateway_network_bind_port,
        fcfg.gncfg.gateway_network_broadcast_address,
        fcfg.gncfg.gateway_network_broadcast_bind_port, &mqttSnForwarder.gatewayNetwork,
        &udpGatewayNetworkContainer, &mqttSnForwarder.mqtt_sn_gateway_address)) {
      configureModeTimeout(30);
    }
  } else {
    log_str(&fcfg_logger, PSTR("Unsupported gateway network procotol "));
    log_str(&fcfg_logger, fcfg.gncfg.gateway_network_protocol);
    log_flush(&fcfg_logger);
    configureModeTimeout(30);
  }

  if (!strcmp(fcfg.cncfg.client_network_protocol, "udp")) {
    if (MqttSnArduinoClientUdpNetworkContainerStart(&fcfg_logger,
        fcfg.msgcfg.mqtt_sn_gateway_host, fcfg.msgcfg.mqtt_sn_gateway_port,
        fcfg.cncfg.client_network_bind_address, fcfg.cncfg.client_network_bind_port,
        fcfg.cncfg.client_network_broadcast_address,
        fcfg.cncfg.client_network_broadcast_bind_port, &mqttSnForwarder.clientNetwork,
        &udpClientNetworkContainer, &udp_client_network_mqtt_sn_gateway_address)) {
      configureModeTimeout(30);
    }
  } else {
    log_str(&fcfg_logger, PSTR("Unsupported client network procotol "));
    log_str(&fcfg_logger, fcfg.gncfg.gateway_network_protocol);
    log_flush(&fcfg_logger);
    configureModeTimeout(30);
  }

  pinMode(16, OUTPUT);
  digitalWrite(16, LOW);
  delay(50);
  digitalWrite(16, HIGH);
  if (!display.init()) {
    Serial.println("Error: Could not initialize display.");
    return;
  }

  if (MqttSnLoggerInit(&screen_logger, fcfg.mslcfg.log_lvl, ssd1306_screen_log_init)) {
    Serial.println("Error: Could not initialize MqttSnLogger.");
    configureModeTimeout(30);
  }

  if (fcfg.mslcfg.log_identifier) {
    screen_logger.log_identifier = fcfg.mslcfg.log_identifier;
  }
#ifdef WITH_LOGGING
  if (log_forwarder_started(&screen_logger, fcfg.msvcfg.version, fcfg.msvcfg.major, fcfg.msvcfg.minor, fcfg.msvcfg.tweak, fcfg.msvcfg.build_date)) {
  }
#endif

  mqttSnForwarder.clientNetworkSendTimeout = fcfg.cncfg.client_network_send_timeout;
  mqttSnForwarder.clientNetworkReceiveTimeout = fcfg.cncfg.client_network_receive_timeout;
  mqttSnForwarder.gatewayNetworkSendTimeout = fcfg.gncfg.gateway_network_send_timeout;
  mqttSnForwarder.gatewayNetworkReceiveTimeout = fcfg.gncfg.gateway_network_receive_timeout;

  MqttSnForwarderSetPinConnection(&mqttSnForwarder, 1);
  if (fcfg.pin_connection && fcfg.connection_state == DISCONNECTED_CONFIG_MQTT_SN_FORWARDER_START_CONNECTION_STATE) {
    log_str(&screen_logger, "Error: mqtt-sn gateway cannot start in disconnected state with pinned gateway connection.\n");
  }

  if (MqttSnForwarderInit(&mqttSnForwarder, &screen_logger, &mqttSnForwarder.mqtt_sn_gateway_address)) {
    MqttSnForwarderDeinit(&mqttSnForwarder);
    Serial.println("Could not initialize Forwarder");
    configureModeTimeout(30);
  }
}

int forwarder_rc = 0;
void loop() {
  if (forwarder_rc == 0) {
    forwarder_rc = MqttSnForwarderLoop(&mqttSnForwarder);
  } else if (forwarder_rc == -1) {
#ifdef WITH_LOGGING
    if (log_forwarder_terminated(&screen_logger,
                                 fcfg.msvcfg.version,
                                 fcfg.msvcfg.major,
                                 fcfg.msvcfg.minor,
                                 fcfg.msvcfg.tweak)) {
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
      arduino_serial_eval_process(eeprom_cfg, &fcfg, fcfg.executable_name, &fcfg_logger, line, line_pos,
                                  forwarder_config_file_process_command_callback, forwarder_config_print_usage);
      memset(line, 0, sizeof(line));
      line_pos = 0;
    }
  }
}