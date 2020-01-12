//
// Created by SomeDude on 21.08.2019.
//

#include "MqttSnLinuxGatewayUdpNetworkContainer.h"
#include <string.h>
#include <stdlib.h>
#include <network/linux/shared/ip/MqttSnIpNetworkHelper.h>
#include <network/shared/ip/IpHelper.h>

int32_t MqttSnLinuxGatewayUdpNetworkContainerInit(MqttSnLinuxGatewayUdpNetworkContainer *cfg) {
    memset(cfg, 0, sizeof(*cfg));
    return 0;
}

int32_t MqttSnLinuxGatewayUdpNetworkContainerStart(const MqttSnLogger *logger, char *mqtt_sn_gateway_host, uint16_t mqtt_sn_gateway_port,
                                                   char *gateway_network_bind_address, uint16_t gateway_network_bind_port,
                                                   char *gateway_network_broadcast_address, uint16_t gateway_network_broadcast_bind_port,
                                                   MqttSnGatewayNetworkInterface *        gatewayNetwork,
                                                   MqttSnLinuxGatewayUdpNetworkContainer *gatewayNetworkContext,
                                                   device_address *                       mqtt_sn_gateway_address) {
    MqttSnLinuxGatewayUdpNetworkContainerInit(gatewayNetworkContext);
    if (convert_hostname_port_to_device_address(mqtt_sn_gateway_host, mqtt_sn_gateway_port,
                                                &gatewayNetworkContext->mqttSnGatewayNetworkAddress, "mqtt-sn gateway")) {
        return EXIT_FAILURE;
    }
    if (is_device_address_ipv4_zeroed(&gatewayNetworkContext->mqttSnGatewayNetworkAddress)) {
        set_device_address_ipv4_localhost(&gatewayNetworkContext->mqttSnGatewayNetworkAddress);
    }
    (*mqtt_sn_gateway_address) = gatewayNetworkContext->mqttSnGatewayNetworkAddress;

    if (convert_hostname_port_to_device_address(gateway_network_bind_address, gateway_network_bind_port,
                                                &gatewayNetworkContext->forwarderGatewayNetworkAddress, "gateway unicast")) {
        return EXIT_FAILURE;
    }
    if (convert_string_ip_port_to_device_address(logger, gateway_network_broadcast_address, gateway_network_broadcast_bind_port,
                                                 &gatewayNetworkContext->forwarderGatewayNetworkBroadcastAddress, "gateway broadcast")) {
        return EXIT_FAILURE;
    }

    if (GatewayNetworkInitialize(gatewayNetwork, MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH, mqtt_sn_gateway_address,
                                 &gatewayNetworkContext->forwarderGatewayNetworkAddress,
                                 &gatewayNetworkContext->forwarderGatewayNetworkBroadcastAddress,
                                 &gatewayNetworkContext->udpGatewayNetworkContext, GatewayLinuxUdpInitialize)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
