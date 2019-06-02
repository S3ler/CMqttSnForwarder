//
// Created by SomeDude on 12.05.2019.
//

#include "system_logging.h"
#include <platform/platform_compatibility.h>

int print_system_config_usage(const MqttSnLogger *logger) {
  log_str(logger, PSTR("EEPROM Usage:     [-sr system_restart]\n"));
  log_str(logger, PSTR("                  --system_help\n"));
  log_flush(logger);
  log_str(logger, PSTR(" -sr : restarts the system.\n"));
#ifdef WITH_EEPROM
  log_str(logger, PSTR("       EEPROM is not automatically saved.\n"));
#endif
  log_str(logger, PSTR(" --system_help : display this message.\n"));
  log_flush(logger);
  return log_status(logger);
}

int print_system_restarting(const MqttSnLogger *logger) {
  log_str(logger, PSTR("Restarting system ..."));
  log_flush(logger);
  return log_status(logger);
}
int print_system_unknown_option(const MqttSnLogger *logger, const char *unknown_option) {
  log_str(logger, PSTR("Error: Unknown system option "));
  log_str(logger, unknown_option);
  log_str(logger, PSTR("."));
  log_flush(logger);
  return log_status(logger);
}
int print_cannot_convert_or_resolve_network_address(const MqttSnLogger *logger,
                                                    const char *hostname,
                                                    const char *address_name) {
  log_str(logger, PSTR("Cannot convert or resolve "));
  log_str(logger, hostname);
  log_str(logger, PSTR(" to "));
  log_str(logger, address_name);
  log_str(logger, PSTR(" network address."));
  log_flush(logger);
  return log_status(logger);
}
int print_resolved_hostname_to(const MqttSnLogger *logger, const char *hostname, const device_address *address) {
  log_str(logger, PSTR("Resolved "));
  log_str(logger, hostname);
  log_str(logger, PSTR(" to "));
  log_device_address(logger, address);
  log_str(logger, PSTR("."));
  log_flush(logger);
  return log_status(logger);
}
int print_wifi_could_not_connect(const MqttSnLogger *logger) {
  log_str(logger, PSTR("Could not connect to Wifi - configure with: --eeprom_wifi_name and --eeprom_wifi_password."));
  log_flush(logger);
  return log_status(logger);
}
int print_wifi_connected(const MqttSnLogger *logger) {
  log_str(logger, PSTR(" Connected as: "));
  IPAddress ownIp = WiFi.localIP();
  print_arduino_IPAddress(logger, &ownIp);
  log_flush(logger);
  return log_status(logger);
}
int print_arduino_IPAddress(const MqttSnLogger *logger, const IPAddress *ipAddress) {
  for (uint8_t i = 0; i < 4; i++) {
    log_uint8(logger, (*ipAddress)[i]);
    if (i + 1 < 4) {
      log_str(logger, PSTR("."));
    }
  }
  return log_status(logger);
}
int print_wifi_connecting_to(const MqttSnLogger *logger, const char *ssid, const char *password) {
  log_str(logger, PSTR("Connecting to WiFi: "));
  log_str(logger, ssid);
  log_str(logger, PSTR(" , password: "));
#ifdef WITH_DEBUG_LOGGING
  if (password) {
    log_str(logger, password);
  }
#else
  log_str(logger, PSTR("****"));
#endif
  log_str(logger, PSTR(" "));
  return log_status(logger);
}
int print_wifi_not_configured(const MqttSnLogger *logger) {
  log_str(logger, PSTR("No WiFi Configuration found - configure with: --eeprom_wifi_name and --eeprom_wifi_password."));
  log_flush(logger);
  return log_status(logger);
}
int log_init_gateway_network_unknown_protocol(const MqttSnLogger *logger, const char *protocol) {
  return log_init_network_unknown_protocol(logger, PSTR("gateway"), protocol);
}
int log_init_client_network_unknown_protocol(const MqttSnLogger *logger, const char *protocol) {
  return log_init_network_unknown_protocol(logger, PSTR("client"), protocol);
}
int log_init_network_unknown_protocol(const MqttSnLogger *logger, const char *network_name, const char *protocol) {
  log_str(logger, PSTR("Error init "));
  log_str(logger, network_name);
  log_str(logger, PSTR(" network unknown protocol: "));
  log_str(logger, protocol);
  log_flush(logger);
  return log_status(logger);
}

