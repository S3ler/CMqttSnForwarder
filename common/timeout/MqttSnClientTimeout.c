//
// Created by SomeDude on 03.09.2019.
//

#include "MqttSnClientTimeout.h"

uint64_t get_tolerance_timeout(uint16_t timeout, uint16_t low_threshold_s, int32_t low_threshold_tolerance_percentage,
                               int32_t high_threshold_tolerance_percentage) {
    uint64_t result = timeout;
    if (timeout <= low_threshold_s) {
        result += (uint64_t)((double)timeout * (double)low_threshold_tolerance_percentage / (double)100);
    } else {
        result += (uint64_t)((double)timeout * (double)high_threshold_tolerance_percentage / (double)100);
    }
    return result;
}