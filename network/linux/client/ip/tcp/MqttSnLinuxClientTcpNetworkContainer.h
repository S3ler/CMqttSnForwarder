//
// Created by SomeDude on 25.09.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNLINUXCLIENTTCPNETWORKCONTAINER_H
#define CMQTTSNFORWARDER_MQTTSNLINUXCLIENTTCPNETWORKCONTAINER_H
#include <stdint.h>
#include <platform/device_address.h>
#include "MqttSnClientTcpNetwork.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct MqttSnLinuxClientTcpNetworkContainer_ {
    device_address         mqttSnGatewayNetworkAddress;
    device_address         forwarderClientNetworkAddress;
    device_address         forwarderClientNetworkBroadcastAddress;
    MqttSnClientTcpNetwork tcpClientNetworkContext;
} MqttSnLinuxClientTcpNetworkContainer;

int32_t MqttSnLinuxClientTcpNetworkContainerInit(MqttSnLinuxClientTcpNetworkContainer *cfg);
int32_t MqttSnLinuxClientTcpNetworkContainerStart(const MqttSnLogger *logger, char *mqtt_sn_gateway_host, uint16_t mqtt_sn_gateway_port,
                                                  char *client_network_bind_address, uint16_t client_network_bind_port,
                                                  char *client_network_broadcast_address, uint16_t client_network_broadcast_bind_port,
                                                  MqttSnClientNetworkInterface *        clientNetwork,
                                                  MqttSnLinuxClientTcpNetworkContainer *clientNetworkContext,
                                                  device_address *                      mqtt_sn_gateway_address);

#ifdef __cplusplus
}
#endif
#endif  //CMQTTSNFORWARDER_MQTTSNLINUXCLIENTTCPNETWORKCONTAINER_H
