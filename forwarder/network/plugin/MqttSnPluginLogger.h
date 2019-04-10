//
// Created by SomeDude on 10.04.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNPLUGINLOGGER_H
#define CMQTTSNFORWARDER_MQTTSNPLUGINLOGGER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <MqttSnForwarderLogging.h>

int log_dlerror(const MqttSnLogger *logger, int level, const char *error);
int log_protocol_mismatch(const MqttSnLogger *logger, int level, const char *expected, const char *actual);
int log_max_msg_length_mismatch(const MqttSnLogger *logger, int level, uint16_t gw_max, uint16_t plugin_max);
int log_too_long_message(const MqttSnLogger *logger,
                         int level,
                         const device_address* address,
                         const uint8_t *data,
                         uint16_t data_len);
int log_incomplete_message(const MqttSnLogger *logger,
                         int level,
                         const device_address* address,
                         const uint8_t *data,
                         uint16_t data_len);
#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_MQTTSNPLUGINLOGGER_H
