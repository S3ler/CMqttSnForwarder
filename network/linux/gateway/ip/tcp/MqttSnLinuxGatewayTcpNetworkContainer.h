//
// Created by SomeDude on 25.09.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNLINUXGATEWAYTCPNETWORKCONTAINER_H
#define CMQTTSNFORWARDER_MQTTSNLINUXGATEWAYTCPNETWORKCONTAINER_H

#include <stdint.h>
#include <platform/device_address.h>
#include "MqttSnGatewayTcpNetwork.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct MqttSnLinuxGatewayTcpNetworkContainer_ {
    device_address          mqttSnGatewayNetworkAddress;
    device_address          forwarderGatewayNetworkAddress;
    device_address          forwarderGatewayNetworkBroadcastAddress;
    MqttSnGatewayTcpNetwork tcpGatewayNetworkContext;
} MqttSnLinuxGatewayTcpNetworkContainer;

int32_t MqttSnLinuxGatewayTcpNetworkContainerInit(MqttSnLinuxGatewayTcpNetworkContainer *cfg);
int32_t MqttSnLinuxGatewayTcpNetworkContainerStart(const MqttSnLogger *logger, char *mqtt_sn_gateway_host, uint16_t mqtt_sn_gateway_port,
                                                   char *gateway_network_bind_address, uint16_t gateway_network_bind_port,
                                                   char *gateway_network_broadcast_address, uint16_t gateway_network_broadcast_bind_port,
                                                   MqttSnGatewayNetworkInterface *        gatewayNetwork,
                                                   MqttSnLinuxGatewayTcpNetworkContainer *gatewayNetworkContext,
                                                   device_address *                       mqtt_sn_gateway_address);
#ifdef __cplusplus
}
#endif
#endif  //CMQTTSNFORWARDER_MQTTSNLINUXGATEWAYTCPNETWORKCONTAINER_H
