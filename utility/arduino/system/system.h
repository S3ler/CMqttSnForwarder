//
// Created by SomeDude on 01.05.2019.
//

#ifndef CMQTTSNFORWARDER_UTILITY_ARDUINO_SYSTEM_H_
#define CMQTTSNFORWARDER_UTILITY_ARDUINO_SYSTEM_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef MDNS_RESOLVER_h
#include <WiFiUdp.h>
#include <mDNSResolver.h>
#endif
#include <utility/arduino/eeprom/eeprom_config.h>

int connect_wifi(EEPROM_cfg *ecfg, const MqttSnLogger *logger, uint32_t timeout_ms);

int parse_arduino_serial_line(char *line, size_t *line_pos, size_t line_max);

int arduino_serial_eval_process(EEPROM_cfg *ecfg, forwarder_config *fcfg, char *line, size_t line_pos);

int convert_hostname_port_to_device_address(const MqttSnLogger *logger,
                                            const char *hostname,
                                            int port,
                                            device_address *address,
                                            const char *address_name);

int print_system_config_usage(const MqttSnLogger *logger);
int process_system_config_str(const MqttSnLogger *logger, char *ecfg_line, size_t ecfg_line_len);
int validate_system_config_token(const MqttSnLogger *logger, int argc, char *argv[]);
int process_system_config_token(const MqttSnLogger *logger, int argc, char *argv[]);

int get_device_address_from_hostname(const char *hostname, device_address *dst);
int EndsWith(const char *str, const char *suffix);
int EndsWithLocal(const char *str);
int StartsWith(const char *pre, const char *str);

#ifdef WITH_LOGGING
int log_system_restarting(const MqttSnLogger* logger);
int print_system_unknown_option(const MqttSnLogger *logger, const char *unknown_option);
int print_resolved_hostname_to(const MqttSnLogger *logger, const char *hostname, const device_address *address);
int print_wifi_not_configured(const MqttSnLogger *logger);
int print_wifi_connecting_to(const MqttSnLogger *logger, const char *ssid, const char *password);
int print_wifi_could_not_connect(const MqttSnLogger *logger);
int connect_wifi(EEPROM_cfg *ecfg, const MqttSnLogger *logger, uint32_t timeout_ms);
int print_cannot_convert_or_resolve_network_address(const MqttSnLogger *logger,
                                                    const char *hostname,
                                                    const char *address_name);
int log_init_gateway_network_unknown_protocol(const MqttSnLogger *logger, const char *protocol);
int log_init_client_network_unknown_protocol(const MqttSnLogger *logger, const char *protocol);
int log_init_network_unknown_protocol(const MqttSnLogger *logger, const char *network_name, const char *protocol);
#endif

#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_UTILITY_ARDUINO_SYSTEM_H_
