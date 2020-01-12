//
// Created by SomeDude on 24.09.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNLINUXCLIENTUDPNETWORKCONTAINER_H
#define CMQTTSNFORWARDER_MQTTSNLINUXCLIENTUDPNETWORKCONTAINER_H

#include <stdint.h>
#include <platform/device_address.h>
#include "MqttSnClientUdpNetwork.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct MqttSnLinuxClientUdpNetworkContainer_ {
    device_address         mqttSnGatewayNetworkAddress;
    device_address         forwarderClientNetworkAddress;
    device_address         forwarderClientNetworkBroadcastAddress;
    MqttSnClientUdpNetwork udpClientNetworkContext;
} MqttSnLinuxClientUdpNetworkContainer;

int32_t MqttSnLinuxClientUdpNetworkContainerInit(MqttSnLinuxClientUdpNetworkContainer *cfg);
int32_t MqttSnLinuxClientUdpNetworkContainerStart(const MqttSnLogger *logger,  char *mqtt_sn_gateway_host, uint16_t mqtt_sn_gateway_port,
                                                  char *client_network_bind_address, uint16_t client_network_bind_port,
                                                  char *client_network_broadcast_address, uint16_t client_network_broadcast_bind_port,
                                                  MqttSnClientNetworkInterface *        clientNetwork,
                                                  MqttSnLinuxClientUdpNetworkContainer *clientNetworkContext,
                                                  device_address *                      mqtt_sn_gateway_address);

#ifdef __cplusplus
}
#endif
#endif  //CMQTTSNFORWARDER_MQTTSNLINUXCLIENTUDPNETWORKCONTAINER_H
