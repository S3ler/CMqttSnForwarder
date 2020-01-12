//
// Created by SomeDude on 08.07.2019.
//

#ifndef CMQTTSNFORWARDER_GATEWAY_MQTTSNGATEWAYFORWARDER_H_
#define CMQTTSNFORWARDER_GATEWAY_MQTTSNGATEWAYFORWARDER_H_

#include <platform/device_address.h>
#include <platform/MqttSnMessageData.h>
#include <logging/MqttSnLogging.h>
#ifdef __cplusplus
extern "C" {
#endif

#ifndef MQTT_SN_GATEWAY_MAX_FORWARDER_COUNT
#define MQTT_SN_GATEWAY_MAX_FORWARDER_COUNT 5
#endif

typedef struct MqttSnGatewayForwarder_ {
  device_address wireless_node_id;
  device_address forwarder_addresses[MQTT_SN_GATEWAY_MAX_FORWARDER_COUNT];
  uint8_t forwarder_radiuses[MQTT_SN_GATEWAY_MAX_FORWARDER_COUNT];
  uint16_t forwarder_len;
  uint16_t forwarder_max_len;
} MqttSnGatewayForwarder;

void MqttSnGatewayForwarderInit(MqttSnGatewayForwarder* mqtt_sn_gateway_forwarder);

uint16_t gateway_add_forwarder_encapsulation_frames(MqttSnMessageData *msg,
                                                    int32_t gen_bytes,
                                                    MqttSnGatewayForwarder *forwarders,
                                                    const MqttSnLogger *logger);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_GATEWAY_MQTTSNGATEWAYFORWARDER_H_
