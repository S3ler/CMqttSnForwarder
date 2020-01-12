//
// Created by SomeDude on 03.09.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNCLIENTTIMEOUT_H
#define CMQTTSNFORWARDER_MQTTSNCLIENTTIMEOUT_H
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
uint64_t get_tolerance_timeout(uint16_t timeout, uint16_t low_threshold_s, int32_t low_threshold_tolerance_percentage,
                               int32_t high_threshold_tolerance_percentage);
#ifdef __cplusplus
}
#endif
#endif  //CMQTTSNFORWARDER_MQTTSNCLIENTTIMEOUT_H
