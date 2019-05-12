//
// Created by SomeDude on 12.05.2019.
//

#ifndef CMQTTSNFORWARDER_NETWORK_LINUX_SHARED_IP_MQTTSNIPNETWORKHELPERLOGGING_H_
#define CMQTTSNFORWARDER_NETWORK_LINUX_SHARED_IP_MQTTSNIPNETWORKHELPERLOGGING_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <logging/MqttSnLoggingInterface.h>

int log_select_error(const MqttSnLogger *logger);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_NETWORK_LINUX_SHARED_IP_MQTTSNIPNETWORKHELPERLOGGING_H_
