//
// Created by SomeDude on 12.07.2019.
//

#ifndef CMQTTSNFORWARDER_LOGGING_LINUX_STARTER_LINUX_LOGGING_STARTER_H_
#define CMQTTSNFORWARDER_LOGGING_LINUX_STARTER_LINUX_LOGGING_STARTER_H_
#include <logging/MqttSnLoggingInterface.h>
#ifdef __cplusplus
extern "C" {
#endif

int32_t start_logger(const mqtt_sn_logger_config *cfg, MqttSnLogger *logger);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_LOGGING_LINUX_STARTER_LINUX_LOGGING_STARTER_H_
