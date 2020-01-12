//
// Created by SomeDude on 16.09.2019.
//

#include "mqtt_sn_client_interactive_tcp_buffer.h"
#include <string.h>
#include <stdlib.h>
#include <network/linux/shared/ip/MqttSnIpNetworkHelper.h>
#include <network/shared/ip/IpHelper.h>

void interactive_client_tcp_buffer_init(interactive_client_tcp_buffer *cfg) {
    memset(cfg, 0, sizeof(*cfg));
}
int32_t interactive_client_tcp_buffer_start(const MqttSnLogger *logger, const mqtt_sn_gateway_config *msngcfg,
                                            const gateway_network_config *gncfg, MqttSnGatewayNetworkInterface *gatewayNetwork,
                                            interactive_client_tcp_buffer *gatewayNetworkContext, device_address *mqtt_sn_gateway_address) {
    interactive_client_tcp_buffer_init(gatewayNetworkContext);

    if (convert_hostname_port_to_device_address(msngcfg->mqtt_sn_gateway_host, msngcfg->mqtt_sn_gateway_port,
                                                &gatewayNetworkContext->mqttSnGatewayNetworkAddress, "mqtt-sn gateway")) {
        return EXIT_FAILURE;
    }
    if (is_device_address_ipv4_zeroed(&gatewayNetworkContext->mqttSnGatewayNetworkAddress)) {
        set_device_address_ipv4_localhost(&gatewayNetworkContext->mqttSnGatewayNetworkAddress);
    }

    (*mqtt_sn_gateway_address) = gatewayNetworkContext->mqttSnGatewayNetworkAddress;

    if (convert_hostname_port_to_device_address(gncfg->gateway_network_bind_address, gncfg->gateway_network_bind_port,
                                                &gatewayNetworkContext->forwarderGatewayNetworkAddress, "gateway unicast")) {
        return EXIT_FAILURE;
    }
    if (convert_string_ip_port_to_device_address(logger, gncfg->gateway_network_broadcast_address,
                                                 gncfg->gateway_network_broadcast_bind_port,
                                                 &gatewayNetworkContext->forwarderGatewayNetworkBroadcastAddress, "gateway broadcast")) {
        return EXIT_FAILURE;
    }

    if (GatewayNetworkInitialize(gatewayNetwork, MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH, mqtt_sn_gateway_address,
                                 &gatewayNetworkContext->forwarderGatewayNetworkAddress,
                                 &gatewayNetworkContext->forwarderGatewayNetworkBroadcastAddress,
                                 &gatewayNetworkContext->tcpGatewayNetworkContext, GatewayLinuxTcpInitialize)) {
        log_str(logger, "Error init tcp gateway network\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
