//
// Created by SomeDude on 25.09.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNARDUINOGATEWAYUDPNETWORKCONTAINER_HPP
#define CMQTTSNFORWARDER_MQTTSNARDUINOGATEWAYUDPNETWORKCONTAINER_HPP

#include <stdint.h>
#include <platform/device_address.h>
#include <WiFiUdp.h>
#include "MqttSnGatewayUdpNetwork.hpp"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct MqttSnArduinoGatewayUdpNetworkContainer_ {
    device_address          mqttSnGatewayNetworkAddress;
    device_address          forwarderGatewayNetworkAddress;
    device_address          forwarderGatewayNetworkBroadcastAddress;
    MqttSnGatewayUdpNetwork udpGatewayNetworkContext;
} MqttSnArduinoGatewayUdpNetworkContainer;

int32_t MqttSnArduinoGatewayUdpNetworkContainerInit(MqttSnArduinoGatewayUdpNetworkContainer *gatewayNetworkContext);
int32_t MqttSnArduinoGatewayUdpNetworkContainerStart(const MqttSnLogger *logger, char *mqtt_sn_gateway_host, uint16_t mqtt_sn_gateway_port,
                                                     char *gateway_network_bind_address, uint16_t gateway_network_bind_port,
                                                     char *gateway_network_broadcast_address, uint16_t gateway_network_broadcast_bind_port,
                                                     MqttSnGatewayNetworkInterface *          gatewayNetwork,
                                                     MqttSnArduinoGatewayUdpNetworkContainer *gatewayNetworkContext,
                                                     device_address *                         mqtt_sn_gateway_address);

#ifdef __cplusplus
}
#endif
#endif  //CMQTTSNFORWARDER_MQTTSNARDUINOGATEWAYUDPNETWORKCONTAINER_H
