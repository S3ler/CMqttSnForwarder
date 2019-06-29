//
// Created by SomeDude on 27.05.2019.
//

#ifndef CMQTTSNFORWARDER_CONFIG_CONFIG_COMMAND_HELPER_H_
#define CMQTTSNFORWARDER_CONFIG_CONFIG_COMMAND_HELPER_H_

#include <stdint.h>
#include <logging/MqttSnLoggingInterface.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

int32_t contains_config_argument_help(int argc, char **argv);

int32_t get_config_argument_file_index(int argc, char **argv, const MqttSnLogger *logger);

int32_t process_config_file(const char *config_file_path,
                            const MqttSnLogger *logger,
                            void *cfg,
                            int32_t (*callback)(void *, const MqttSnLogger *, int, char **),
                            const char *argv_0);
int32_t process_config_file_line(const MqttSnLogger *logger,
                                 char *line,
                                 size_t len,
                                 const char *argv_0,
                                 void *cfg,
                                 int32_t (*callback)(void *, const MqttSnLogger *, int, char **));

int32_t parse_port(const MqttSnLogger *logger, char *port_str, int32_t *dst);

int32_t parse_duration(const MqttSnLogger *logger, char *duration_str, uint16_t *duration_dst);

int32_t parse_radius(const MqttSnLogger *logger, char *radius_str, uint8_t *radius_dst);

int32_t parse_gateway_id(const MqttSnLogger *logger, char *gateway_id_str, uint8_t *gateway_id_dst);

int32_t parse_timeout(const MqttSnLogger *logger, char *timeout_str, int32_t *dst);

int32_t parse_timeout_offset(const MqttSnLogger *logger, char *timeout_offset_str, int32_t *dst);

int32_t parse_topic_id(const MqttSnLogger *logger, char *topic_id_str, uint16_t *dst);

int32_t parse_retain(const MqttSnLogger *logger, char *retain_str, uint8_t *dst);

int32_t parse_clean_session(const MqttSnLogger *logger, char *clean_session_str, uint8_t *dst);

int32_t print_invalid_retain_given(const MqttSnLogger *logger, const char *given_str);

int32_t print_invalid_clean_session_given(const MqttSnLogger *logger, const char *given_str);

int32_t print_invalid_topic_id_given(const MqttSnLogger *logger, const char *given_str);

int32_t print_invalid_duration_given(const MqttSnLogger *logger, const char *given_str);

int32_t print_invalid_radius_given(const MqttSnLogger *logger, const char *given_str);

int32_t print_invalid_gateway_id_given(const MqttSnLogger *logger, const char *given_str);

int32_t print_config_parser_invalid_port_given(const MqttSnLogger *logger, long invalid_port);

int32_t print_config_parser_invalid_qos_given(const MqttSnLogger *logger, uint64_t given_qos);

int32_t print_argument_value_not_specified(const MqttSnLogger *logger, const char *argument, const char *argument_name);

int32_t print_unsupported_url_scheme(const MqttSnLogger *logger);

int32_t print_invalid_timeout_given(const MqttSnLogger *logger, long timeout);

int32_t print_invalid_timeout_offset_given(const MqttSnLogger *logger, long timeout);

int32_t print_invalid_protocol_version_given(const MqttSnLogger *logger);

int32_t print_argc_argv(const MqttSnLogger *logger, int argc, char *argv[]);

int32_t print_show_help(const MqttSnLogger *logger);

int32_t print_unknown_option(const MqttSnLogger *logger, const char *unknown_option);

int32_t print_could_not_read_config_file(const MqttSnLogger *logger, char *strerror);

int32_t print_config_list_full(const MqttSnLogger *logger, char *arg_list, uint16_t max_arg_list_len);

int32_t parse_client_subscription_config_qos(const MqttSnLogger *logger, char *qos_str, int8_t *qos_dst);

int32_t parse_client_publish_config_qos(const MqttSnLogger *logger, char *qos_str, int8_t *qos_dst);

int32_t print_see_usage(const MqttSnLogger *logger, const char *executable_name);


// TODO parse protocol version
// TODO parse connect/gateway-/client-network url
// TODO parse subscribe url
// TODO parse publish url

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_CONFIG_CONFIG_COMMAND_HELPER_H_
