//
// Created by SomeDude on 28.05.2019.
//

#ifndef CMQTTSNFORWARDER_CONFIG_MQTT_SN_LOGGER_CONFIG_H_
#define CMQTTSNFORWARDER_CONFIG_MQTT_SN_LOGGER_CONFIG_H_
//TODO implement me: log level

#include <logging/MqttSnLoggingInterface.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MQTT_SN_LOGGER_DEFAULT_LOG_LVL
#define MQTT_SN_LOGGER_DEFAULT_LOG_LVL LOG_LEVEL_DEFAULT
#endif

typedef struct mqtt_sn_logger_config_ {
  log_level_t log_lvl;
} mqtt_sn_logger_config;

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
