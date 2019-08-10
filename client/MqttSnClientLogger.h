//
// Created by SomeDude on 27.06.2019.
//

#ifndef CMQTTSNFORWARDER_CLIENT_MQTTSNCLIENTLOGGER_H_
#define CMQTTSNFORWARDER_CLIENT_MQTTSNCLIENTLOGGER_H_

#include <stdint.h>
#include <logging/MqttSnLoggingInterface.h>
#include "MqttSnClient.h"
#ifdef __cplusplus
extern "C" {
#endif

int32_t log_mqtt_sn_client(const MqttSnLogger* logger, MQTT_SN_CLIENT_RETURN_CODE mqtt_sn_client_return_code);
int32_t log_mqtt_sn_client_connection_status(const MqttSnLogger *logger,
                                             uint8_t connection_status,
                                             const device_address *gateway_address);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_CLIENT_MQTTSNCLIENTLOGGER_H_
