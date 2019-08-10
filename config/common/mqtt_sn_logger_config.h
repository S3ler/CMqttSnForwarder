//
// Created by SomeDude on 28.05.2019.
//

#ifndef CMQTTSNFORWARDER_CONFIG_MQTT_SN_LOGGER_CONFIG_H_
#define CMQTTSNFORWARDER_CONFIG_MQTT_SN_LOGGER_CONFIG_H_
//TODO implement me: log level

#include <stdint.h>
#include <logging/MqttSnLoggingInterface.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MQTT_SN_LOGGER_DEFAULT_LOGGING_TARGET
#define MQTT_SN_LOGGER_DEFAULT_LOGGING_TARGET "console"
#endif

#ifndef MQTT_SN_LOGGER_DEFAULT_LOG_LVL
#define MQTT_SN_LOGGER_DEFAULT_LOG_LVL LOG_LEVEL_DEFAULT
#endif

typedef struct mqtt_sn_logger_config_ {
  char default_logging_target[sizeof(MQTT_SN_LOGGER_DEFAULT_LOGGING_TARGET)];
  char* log_target;
  char* log_file_path;
  char* log_identifier;
  log_level_t log_lvl;
} mqtt_sn_logger_config;

#ifndef MQTT_SN_LOGGER_CONFIG_BUFFER_LOG_TARGET_LENGTH
#define MQTT_SN_LOGGER_CONFIG_BUFFER_LOG_TARGET_LENGTH 20
#endif
#ifndef MQTT_SN_LOGGER_CONFIG_BUFFER_LOG_log_file_path_LENGTH
#define MQTT_SN_LOGGER_CONFIG_BUFFER_LOG_log_file_path_LENGTH 255
#endif
#ifndef MQTT_SN_LOGGER_CONFIG_BUFFER_LOG_log_identifier_LENGTH
#define MQTT_SN_LOGGER_CONFIG_BUFFER_LOG_log_identifier_LENGTH 64
#endif
typedef struct mqtt_sn_logger_config_buffer_ {
  char log_target[MQTT_SN_LOGGER_CONFIG_BUFFER_LOG_TARGET_LENGTH];
  char log_file_path[MQTT_SN_LOGGER_CONFIG_BUFFER_LOG_log_file_path_LENGTH];
  char log_identifier[MQTT_SN_LOGGER_CONFIG_BUFFER_LOG_log_identifier_LENGTH];
} mqtt_sn_logger_config_buffer;

int32_t mqtt_sn_logger_config_buffer_copy_to_buffer(mqtt_sn_logger_config* cfg, mqtt_sn_logger_config_buffer* cfg_buffer);

int32_t mqtt_sn_logger_config_init(mqtt_sn_logger_config *cfg);
void mqtt_sn_logger_config_cleanup(mqtt_sn_logger_config *cfg);
int32_t is_mqtt_sn_logger_config_command(const char *arg, int *i);
int32_t mqtt_sn_logger_config_process_args(mqtt_sn_logger_config *cfg,
                                           const MqttSnLogger *logger,
                                           int argc,
                                           char *argv[]);
void mqtt_sn_logger_config_print_usage_short(const MqttSnLogger *logger, const char *indent);
void mqtt_sn_logger_config_print_usage_long(const MqttSnLogger *logger);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_CONFIG_MQTT_SN_LOGGER_CONFIG_H_
