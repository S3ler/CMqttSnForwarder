//
// Created by SomeDude on 11.05.2019.
//

#ifndef CMQTTSNFORWARDER_FORWARDER_MQTTSNFORWARDERLOGGING_H_
#define CMQTTSNFORWARDER_FORWARDER_MQTTSNFORWARDERLOGGING_H_

#include <logging/MqttSnLogging.h>

#ifdef __cplusplus
extern "C" {
#endif


int32_t log_mqtt_sn_forwarder_connected(const MqttSnLogger *logger, const device_address* mqtt_sn_gateway_address);
int32_t log_mqtt_sn_forwarder_disconnect(const MqttSnLogger *logger, const device_address* mqtt_sn_gateway_address);

int32_t log_mqtt_sn_forwarder_client_and_gateway_network_connection_lost(const MqttSnLogger *logger);
int32_t log_mqtt_sn_forwarder_client_network_connection_lost(const MqttSnLogger *logger);
int32_t log_mqtt_sn_forwarder_gateway_network_connection_lost(const MqttSnLogger *logger);

int32_t log_mqtt_sn_forwarder_sending_buffer_gateway_messages(const MqttSnLogger *logger);
int32_t log_mqtt_sn_forwarder_sending_buffer_client_messages(const MqttSnLogger *logger);

int32_t log_mqtt_sn_forwarder_buffer_gateway_messages_send(const MqttSnLogger *logger);
int32_t log_mqtt_sn_forwarder_buffer_client_messages_send(const MqttSnLogger *logger);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_FORWARDER_MQTTSNFORWARDERLOGGING_H_
