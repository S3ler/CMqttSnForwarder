//
// Created by SomeDude on 08.07.2019.
//

#include "MqttSnGatewayForwarder.h"
#include <string.h>

void MqttSnGatewayForwarderInit(MqttSnGatewayForwarder *mqtt_sn_gateway_forwarder) {
  memset(mqtt_sn_gateway_forwarder, 0, sizeof(MqttSnGatewayForwarder));
  mqtt_sn_gateway_forwarder->forwarder_max_len = MQTT_SN_GATEWAY_MAX_FORWARDER_COUNT;
}
