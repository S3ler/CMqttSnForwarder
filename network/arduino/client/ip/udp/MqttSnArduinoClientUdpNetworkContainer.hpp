//
// Created by SomeDude on 25.09.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNARDUINOCLIENTUDPNETWORKCONTAINER_HPP
#define CMQTTSNFORWARDER_MQTTSNARDUINOCLIENTUDPNETWORKCONTAINER_HPP

#include <stdint.h>
#include <platform/device_address.h>
#include <WiFiUdp.h>
#include "MqttSnClientUdpNetwork.hpp"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct MqttSnArduinoClientUdpNetworkContainer_ {
    device_address         mqttSnGatewayNetworkAddress;
    device_address         forwarderClientNetworkAddress;
    device_address         forwarderClientNetworkBroadcastAddress;
    MqttSnClientUdpNetwork udpClientNetworkContext;
} MqttSnArduinoClientUdpNetworkContainer;

int32_t MqttSnArduinoClientUdpNetworkContainerInit(MqttSnArduinoClientUdpNetworkContainer *clientNetworkContext);
int32_t MqttSnArduinoClientUdpNetworkContainerStart(const MqttSnLogger *logger, char *mqtt_sn_gateway_host, uint16_t mqtt_sn_gateway_port,
                                                    char *client_network_bind_address, uint16_t client_network_bind_port,
                                                    char *client_network_broadcast_address, uint16_t client_network_broadcast_bind_port,
                                                    MqttSnClientNetworkInterface *          clientNetwork,
                                                    MqttSnArduinoClientUdpNetworkContainer *clientNetworkContext,
                                                    device_address *                        mqtt_sn_gateway_address);

#ifdef __cplusplus
}
#endif
#endif  //CMQTTSNFORWARDER_MQTTSNARDUINOCLIENTUDPNETWORKCONTAINER_HPP
