//
// Created by SomeDude on 10.04.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNPLUGINLOGGER_H
#define CMQTTSNFORWARDER_MQTTSNPLUGINLOGGER_H

#include <forwarder/MqttSnForwarderLogging.h>
#ifdef __cplusplus
extern "C" {
#endif

int log_dlerror(const MqttSnLogger *logger, int level, const char *error);
int log_max_msg_length_mismatch(const MqttSnLogger *logger, int level, uint16_t gw_max, uint16_t plugin_max);

#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_MQTTSNPLUGINLOGGER_H
