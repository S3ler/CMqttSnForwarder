//
// Created by SomeDude on 25.09.2019.
//

#include "MqttSnLinuxClientTcpNetworkContainer.h"
#include <string.h>
#include <stdlib.h>
#include <network/linux/shared/ip/MqttSnIpNetworkHelper.h>
#include <network/shared/ip/IpHelper.h>

int32_t MqttSnLinuxClientTcpNetworkContainerInit(MqttSnLinuxClientTcpNetworkContainer *cfg) {
    memset(cfg, 0, sizeof(*cfg));
    return 0;
}
int32_t MqttSnLinuxClientTcpNetworkContainerStart(const MqttSnLogger *logger, char *mqtt_sn_gateway_host, uint16_t mqtt_sn_gateway_port,
                                                  char *client_network_bind_address, uint16_t client_network_bind_port,
                                                  char *client_network_broadcast_address, uint16_t client_network_broadcast_bind_port,
                                                  MqttSnClientNetworkInterface *        clientNetwork,
                                                  MqttSnLinuxClientTcpNetworkContainer *clientNetworkContext,
                                                  device_address *                      mqtt_sn_gateway_address) {
    MqttSnLinuxClientTcpNetworkContainerInit(clientNetworkContext);
    if (convert_hostname_port_to_device_address(mqtt_sn_gateway_host, mqtt_sn_gateway_port,
                                                &clientNetworkContext->mqttSnGatewayNetworkAddress, "mqtt-sn gateway")) {
        return EXIT_FAILURE;
    }
    if (is_device_address_ipv4_zeroed(&clientNetworkContext->mqttSnGatewayNetworkAddress)) {
        set_device_address_ipv4_localhost(&clientNetworkContext->mqttSnGatewayNetworkAddress);
    }
    (*mqtt_sn_gateway_address) = clientNetworkContext->mqttSnGatewayNetworkAddress;

    if (convert_hostname_port_to_device_address(client_network_bind_address, client_network_bind_port,
                                                &clientNetworkContext->forwarderClientNetworkAddress, "client unicast")) {
        return EXIT_FAILURE;
    }
    if (convert_string_ip_port_to_device_address(logger, client_network_broadcast_address, client_network_broadcast_bind_port,
                                                 &clientNetworkContext->forwarderClientNetworkBroadcastAddress, "client broadcast")) {
        return EXIT_FAILURE;
    }

    if (ClientNetworkInitialize(clientNetwork, MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH, mqtt_sn_gateway_address,
                                &clientNetworkContext->forwarderClientNetworkAddress,
                                &clientNetworkContext->forwarderClientNetworkBroadcastAddress,
                                &clientNetworkContext->tcpClientNetworkContext, ClientLinuxTcpInitialize)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
