//
// Created by SomeDude on 25.09.2019.
//

#include "MqttSnArduinoGatewayUdpNetworkContainer.hpp"
#include <string.h>
#include <config/common/arduino/system/system.h>
#include <network/shared/ip/IpHelper.h>

int32_t MqttSnArduinoGatewayUdpNetworkContainerInit(MqttSnArduinoGatewayUdpNetworkContainer *gatewayNetworkContext) {
    memset(gatewayNetworkContext, 0, sizeof(*gatewayNetworkContext));
    return 0;
}
int32_t MqttSnArduinoGatewayUdpNetworkContainerStart(const MqttSnLogger *logger, char *mqtt_sn_gateway_host, uint16_t mqtt_sn_gateway_port,
                                                     char *gateway_network_bind_address, uint16_t gateway_network_bind_port,
                                                     char *gateway_network_broadcast_address, uint16_t gateway_network_broadcast_bind_port,
                                                     MqttSnGatewayNetworkInterface *        gatewayNetwork,
                                                     MqttSnArduinoGatewayUdpNetworkContainer *gatewayNetworkContext,
                                                     device_address *                       mqtt_sn_gateway_address) {
    MqttSnArduinoGatewayUdpNetworkContainerInit(gatewayNetworkContext);
    if (convert_hostname_port_to_device_address(logger, mqtt_sn_gateway_host, mqtt_sn_gateway_port, mqtt_sn_gateway_address,
                                                "mqtt-sn gateway")) {
        Serial.println("Could not resolve MQTT-SN Gateway Host to device address - configure with: --mqtt_sn_gateway_host or --URL.");
        Serial.println("Use '--help' to see usage.");
        return EXIT_FAILURE;
    }
    add_port_to_device_address(gateway_network_bind_port, &gatewayNetworkContext->forwarderGatewayNetworkAddress);
    if (convert_string_ip_port_to_device_address(logger, gateway_network_broadcast_address, gateway_network_broadcast_bind_port,
                                                 &gatewayNetworkContext->forwarderGatewayNetworkBroadcastAddress, "gateway broadcast")) {
        Serial.println("Could not convert gateway broadcast to device address - configure with: --gateway_network_broadcast_address and "
                       "--gateway_network_broadcast_bind_port or --gateway_network_broadcast_url.");
        Serial.println("Use '--help' to see usage.");
        return EXIT_FAILURE;
    }

    if (GatewayNetworkInitialize(gatewayNetwork, CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN, NULL,
                                 &gatewayNetworkContext->forwarderGatewayNetworkAddress,
                                 &gatewayNetworkContext->forwarderGatewayNetworkBroadcastAddress,
                                 &gatewayNetworkContext->udpGatewayNetworkContext, GatewayArduinoUdpInitialize)) {
        Serial.println("Error init gateway network\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}