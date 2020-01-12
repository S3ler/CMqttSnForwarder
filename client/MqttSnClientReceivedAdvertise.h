//
// Created by SomeDude on 18.08.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNCLIENTRECEIVEDADVERTISE_H
#define CMQTTSNFORWARDER_MQTTSNCLIENTRECEIVEDADVERTISE_H

#include <stdint.h>
#include <platform/device_address.h>
#include <parser/MqttSnAdvertiseMessage.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct MqttSnClientReceivedAdvertise_ {
    MqttSnAdvertise received_advertise;
    device_address  from;
    uint8_t         signal_strength;
} MqttSnClientReceivedAdvertise;

#ifdef __cplusplus
}
#endif
#endif  //CMQTTSNFORWARDER_MQTTSNCLIENTRECEIVEDADVERTISE_H
