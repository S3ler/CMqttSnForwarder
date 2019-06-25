//
// Created by SomeDude on 12.05.2019.
//

#ifndef CMQTTSNFORWARDER_UTILITY_ARDUINO_SYSTEM_SYSTEM_LOGGING_H_
#define CMQTTSNFORWARDER_UTILITY_ARDUINO_SYSTEM_SYSTEM_LOGGING_H_

#include <logging/MqttSnLoggingInterface.h>
#include <IPAddress.h>
#include <WiFi.h>

#ifdef __cplusplus
extern "C" {
#endif

int print_system_config_usage(const MqttSnLogger *logger);

int print_system_restarting(const MqttSnLogger *logger);
int print_system_unknown_option(const MqttSnLogger *logger, const char *unknown_option);
int print_resolved_hostname_to(const MqttSnLogger *logger, const char *hostname, const device_address *address);
int print_wifi_not_configured(const MqttSnLogger *logger);
int print_wifi_connecting_to(const MqttSnLogger *logger, const char *ssid, const char *password);
int print_wifi_could_not_connect(const MqttSnLogger *logger);
int print_wifi_connected(const MqttSnLogger *logger);
int print_arduino_IPAddress(const MqttSnLogger *logger, const IPAddress *ipAddress);
int print_cannot_convert_or_resolve_network_address(const MqttSnLogger *logger,
                                                    const char *hostname,
                                                    const char *address_name);
int log_init_gateway_network_unknown_protocol(const MqttSnLogger *logger, const char *protocol);
int log_init_client_network_unknown_protocol(const MqttSnLogger *logger, const char *protocol);
int log_init_network_unknown_protocol(const MqttSnLogger *logger, const char *network_name, const char *protocol);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_UTILITY_ARDUINO_SYSTEM_SYSTEM_LOGGING_H_
