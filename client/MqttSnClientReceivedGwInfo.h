//
// Created by SomeDude on 18.08.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNCLIENTRECEIVEDGWINFO_H
#define CMQTTSNFORWARDER_MQTTSNCLIENTRECEIVEDGWINFO_H

#include <stdint.h>
#include <platform/device_address.h>
#include <parser/MqttSnGwInfoMessage.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct MqttSnClientReceivedGwInfo_ {
    MqttSnGwInfo   received_gw_info;
    device_address from;
    uint8_t        signal_strength;
} MqttSnClientReceivedGwInfo;

#ifdef __cplusplus
}
#endif
#endif  //CMQTTSNFORWARDER_MQTTSNCLIENTRECEIVEDGWINFO_H
