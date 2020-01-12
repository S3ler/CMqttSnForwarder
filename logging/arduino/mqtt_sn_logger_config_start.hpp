//
// Created by SomeDude on 25.09.2019.
//

#ifndef CMQTTSNFORWARDER_MQTT_SN_LOGGER_CONFIG_START_H
#define CMQTTSNFORWARDER_MQTT_SN_LOGGER_CONFIG_START_HPP

int32_t mqtt_sn_logger_config_start(MqttSnLogger *dst_logger, log_level_t log_lvl, const char *log_target, char *log_filepath,
                                    char *log_identifier);

#endif  //CMQTTSNFORWARDER_MQTT_SN_LOGGER_CONFIG_START_H
