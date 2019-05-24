//
// Created by SomeDude on 24.05.2019.
//

#ifndef CMQTTSNFORWARDER_FORWARDER_MQTTSNFORWARDERPARSER_H_
#define CMQTTSNFORWARDER_FORWARDER_MQTTSNFORWARDERPARSER_H_

#include <stdint.h>
#include <platform/device_address.h>
#include <ringbuffer/MqttSnFixedSizeRingBuffer.h>
#include <parser/MqttSnForwarderEncapsulationMessage.h>
#include <logging/MqttSnLoggingInterface.h>

#ifdef __cplusplus
extern "C" {
#endif

int32_t remove_mqtt_sn_forwarder_encapsulation_frame(MqttSnMessageData *gatewayMessageData,
                                                     MqttSnMessageData *clientMessageData,
                                                     const device_address *client_network_broadcast_address,
                                                     const MqttSnLogger *logger);

int32_t add_mqtt_sn_forwarder_encapsulation_frame(MqttSnMessageData *clientMessageData,
                                                  MqttSnMessageData *gatewayMessageData,
                                                  const device_address *mqtt_sn_gateway_network_address,
                                                  const device_address *client_network_broadcast_address,
                                                  const MqttSnLogger *logger);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_FORWARDER_MQTTSNFORWARDERPARSER_H_
