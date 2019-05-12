//
// Created by SomeDude on 12.05.2019.
//

#ifndef CMQTTSNFORWARDER_FORWARDER_CONFIG_FORWARDER_CONFIG_LOGGER_H_
#define CMQTTSNFORWARDER_FORWARDER_CONFIG_FORWARDER_CONFIG_LOGGER_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <logging/MqttSnLoggingInterface.h>
int print_argc_argv(const MqttSnLogger *logger, int argc, char *argv[]);

int print_usage(const MqttSnLogger *logger);
int print_fcfg_invalid_port_given(const MqttSnLogger *logger, long invalid_port);
int print_invalid_timeout_given(const MqttSnLogger *logger, long timeout);
int log_could_not_read_config_file(const MqttSnLogger *logger, char *strerror);
int log_argument_value_not_specified(const MqttSnLogger *logger, const char *argument, const char *argument_name);
int log_unsupported_url_scheme(const MqttSnLogger *logger);
int log_invalid_protocol_version_given(const MqttSnLogger *logger);
int log_unknown_option(const MqttSnLogger *logger, const char *unknown_option);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_FORWARDER_CONFIG_FORWARDER_CONFIG_LOGGER_H_
