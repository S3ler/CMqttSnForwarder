//
// Created by SomeDude on 25.08.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNCLIENTREGISTRATION_H
#define CMQTTSNFORWARDER_MQTTSNCLIENTREGISTRATION_H

#include <stdint.h>
#include "MqttSnClientConfig.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct MqttSnClientRegistration_ {
    char     topic_name[MQTT_SN_CLIENT_MAX_REGISTRATION_TOPIC_NAME_LENGTH];
    uint16_t topic_id;
} MqttSnClientRegistration;

#ifdef __cplusplus
}
#endif
#endif  //CMQTTSNFORWARDER_MQTTSNCLIENTREGISTRATION_H
