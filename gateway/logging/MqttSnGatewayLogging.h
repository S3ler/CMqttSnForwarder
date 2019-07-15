//
// Created by SomeDude on 20.06.2019.
//

#ifndef CMQTTSNFORWARDER_GATEWAY_LOGGING_MQTTSNGATEWAYLOGGING_H_
#define CMQTTSNFORWARDER_GATEWAY_LOGGING_MQTTSNGATEWAYLOGGING_H_

#include <stdint.h>
#include <logging/MqttSnLoggingInterface.h>
#include <gateway/MqttSnGatewayStatus.h>
#include <gateway/database/db_entry_mqtt_sn_client.h>
#include <gateway/database/db_handler_result.h>
#ifdef __cplusplus
extern "C" {
#endif

int32_t log_unhandled_message(const MqttSnLogger *logger,
                              MQTT_SN_GATEWAY_STATUS status,
                              device_address *from,
                              uint8_t signal_strength,
                              uint8_t *data,
                              int32_t data_len);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_GATEWAY_LOGGING_MQTTSNGATEWAYLOGGING_H_
