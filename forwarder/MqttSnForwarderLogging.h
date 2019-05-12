//
// Created by SomeDude on 11.05.2019.
//

#ifndef CMQTTSNFORWARDER_FORWARDER_MQTTSNFORWARDERLOGGING_H_
#define CMQTTSNFORWARDER_FORWARDER_MQTTSNFORWARDERLOGGING_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <logging/MqttSnLoggingInterface.h>

int log_forwarder_started(const MqttSnLogger *logger,
                          const char *version,
                          int major,
                          int minor,
                          int tweak,
                          const char *build_date);

int log_forwarder_terminated(const MqttSnLogger *logger,
                             const char *version,
                             uint32_t major,
                             uint32_t minor,
                             uintmax_t tweak);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_FORWARDER_MQTTSNFORWARDERLOGGING_H_
