//
// Created by SomeDude on 24.09.2019.
//

#include <stdint.h>
#include <logging/MqttSnLogging.h>
#ifndef CMQTTSNFORWARDER_MQTT_SN_LOGGER_CONFIG_START_H
#define CMQTTSNFORWARDER_MQTT_SN_LOGGER_CONFIG_START_H

#ifdef __cplusplus
extern "C" {
#endif

int32_t mqtt_sn_logger_config_start(MqttSnLogger *dst_logger, log_level_t log_lvl, const char *log_target, char *log_filepath,
                                    char *log_identifier);

#ifdef __cplusplus
}
#endif
#endif  //CMQTTSNFORWARDER_MQTT_SN_LOGGER_CONFIG_START_H
