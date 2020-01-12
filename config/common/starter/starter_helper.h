//
// Created by SomeDude on 31.05.2019.
//

#ifndef CMQTTSNFORWARDER_CONFIG_STARTER_STARTER_HELPER_H_
#define CMQTTSNFORWARDER_CONFIG_STARTER_STARTER_HELPER_H_

#include <stdint.h>
#include <logging/MqttSnLogging.h>
#include <config/common/mqtt_sn_version_config.h>

#ifdef __cplusplus
extern "C" {
#endif

int32_t print_program_started(const MqttSnLogger *logger, const mqtt_sn_version_config *msvcfg, const char *program_name);
int32_t print_program_terminated(const MqttSnLogger *logger,
                                 const mqtt_sn_version_config *msvcfg,
                                 const char *program_name);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_CONFIG_STARTER_STARTER_HELPER_H_
