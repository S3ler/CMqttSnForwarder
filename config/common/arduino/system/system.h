//
// Created by SomeDude on 01.05.2019.
//

#ifndef CMQTTSNFORWARDER_UTILITY_ARDUINO_SYSTEM_H_
#define CMQTTSNFORWARDER_UTILITY_ARDUINO_SYSTEM_H_

#include <config/common/arduino/eeprom/eeprom_config.h>
#include <forwarder/config/forwarder_config.h>

#ifdef MDNS_RESOLVER_h
#include <WiFiUdp.h>
#include <mDNSResolver.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

int connect_wifi(const char *ssid, const char *password, uint32_t timeout_ms, const MqttSnLogger *logger);

int parse_arduino_serial_line(char *line, size_t *line_pos, size_t line_max);

int arduino_serial_eval_process(EEPROM_cfg *ecfg,
                                void *fcfg,
                                const char* executable_name,
                                const MqttSnLogger *logger,
                                char *line,
                                size_t line_pos,
                                int32_t (*forwarder_config_file_process_command_callback)(void *cfg, const MqttSnLogger *logger, int argc, char **argv),
                                void (*config_print_usage)(const MqttSnLogger* logger));
int convert_hostname_port_to_device_address(const MqttSnLogger *logger,
                                            const char *hostname,
                                            int port,
                                            device_address *address,
                                            const char *address_name);

int process_system_config_str(const MqttSnLogger *logger, char *ecfg_line, size_t ecfg_line_len);
int validate_system_config_token(const MqttSnLogger *logger, int argc, char *argv[]);
int process_system_config_token(const MqttSnLogger *logger, int argc, char *argv[]);

int get_device_address_from_hostname(const char *hostname, device_address *dst);
int EndsWith(const char *str, const char *suffix);
int EndsWithLocal(const char *str);
int StartsWith(const char *pre, const char *str);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_UTILITY_ARDUINO_SYSTEM_H_
