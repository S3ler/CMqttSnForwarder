//
// Created by SomeDude on 12.05.2019.
//

#ifndef CMQTTSNFORWARDER_NETWORK_LINUX_SHARED_IP_MQTTSNIPNETWORKHELPERLOGGING_H_
#define CMQTTSNFORWARDER_NETWORK_LINUX_SHARED_IP_MQTTSNIPNETWORKHELPERLOGGING_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <logging/MqttSnLogging.h>

int log_select_error(const MqttSnLogger *logger);
int log_debug_select_error(const MqttSnLogger *logger, const char *file_name, const char *function_name, int line_number);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_NETWORK_LINUX_SHARED_IP_MQTTSNIPNETWORKHELPERLOGGING_H_
