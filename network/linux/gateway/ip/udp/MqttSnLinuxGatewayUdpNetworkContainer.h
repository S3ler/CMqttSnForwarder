//
// Created by SomeDude on 21.08.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNLINUXGATEWAYUDPNETWORKCONTAINER_H
#define CMQTTSNFORWARDER_MQTTSNLINUXGATEWAYUDPNETWORKCONTAINER_H

#include <stdint.h>
#include <platform/device_address.h>
#include <network/linux/gateway/ip/udp/MqttSnGatewayUdpNetwork.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct MqttSnLinuxGatewayUdpNetworkContainer_ {
    device_address          mqttSnGatewayNetworkAddress;
    device_address          forwarderGatewayNetworkAddress;
    device_address          forwarderGatewayNetworkBroadcastAddress;
    MqttSnGatewayUdpNetwork udpGatewayNetworkContext;
} MqttSnLinuxGatewayUdpNetworkContainer;

int32_t MqttSnLinuxGatewayUdpNetworkContainerInit(MqttSnLinuxGatewayUdpNetworkContainer *cfg);
int32_t MqttSnLinuxGatewayUdpNetworkContainerStart(const MqttSnLogger *logger, char *mqtt_sn_gateway_host, uint16_t mqtt_sn_gateway_port,
                                                   char *gateway_network_bind_address, uint16_t gateway_network_bind_port,
                                                   char *gateway_network_broadcast_address, uint16_t gateway_network_broadcast_bind_port,
                                                   MqttSnGatewayNetworkInterface *        gatewayNetwork,
                                                   MqttSnLinuxGatewayUdpNetworkContainer *gatewayNetworkContext,
                                                   device_address *                       mqtt_sn_gateway_address);

#ifdef __cplusplus
}
#endif
#endif  //CMQTTSNFORWARDER_MQTTSNLINUXGATEWAYUDPNETWORKCONTAINER_H
