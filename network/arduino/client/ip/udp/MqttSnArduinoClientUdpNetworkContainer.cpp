//
// Created by SomeDude on 25.09.2019.
//

#include "MqttSnArduinoClientUdpNetworkContainer.hpp"
#include <string.h>

int32_t MqttSnArduinoClientUdpNetworkContainerInit(MqttSnArduinoClientUdpNetworkContainer *clientNetworkContext) {
    memset(clientNetworkContext, 0, sizeof(*clientNetworkContext));
    return 0;
}
int32_t MqttSnArduinoClientUdpNetworkContainerStart(const MqttSnLogger *logger, char *mqtt_sn_gateway_host, uint16_t mqtt_sn_gateway_port,
                                                    char *client_network_bind_address, uint16_t client_network_bind_port,
                                                    char *client_network_broadcast_address, uint16_t client_network_broadcast_bind_port,
                                                    MqttSnClientNetworkInterface *          clientNetwork,
                                                    MqttSnArduinoClientUdpNetworkContainer *clientNetworkContext,
                                                    device_address *                        mqtt_sn_gateway_address) {
    MqttSnArduinoClientUdpNetworkContainerInit(clientNetworkContext);
    if (convert_hostname_port_to_device_address(logger, mqtt_sn_gateway_host, mqtt_sn_gateway_port, mqtt_sn_gateway_address,
                                                "mqtt-sn gateway")) {
        Serial.println("Could not resolve MQTT-SN Gateway Host to device address - configure with: --mqtt_sn_gateway_host or --URL.");
        Serial.println("Use '--help' to see usage.");
        return EXIT_FAILURE;
    }
    add_port_to_device_address(client_network_bind_port, &clientNetworkContext->forwarderClientNetworkAddress);
    if (convert_string_ip_port_to_device_address(logger, client_network_broadcast_address, client_network_broadcast_bind_port,
                                                 &clientNetworkContext->forwarderClientNetworkBroadcastAddress, "gateway broadcast")) {
        Serial.println("Could not convert client broadcast to device address - configure with: --client_network_broadcast_address and "
                       "--client_network_broadcast_bind_port or --client_network_broadcast_URL.");
        Serial.println("Use '--help' to see usage.");
        return EXIT_FAILURE;
    }
    if (ClientNetworkInitialize(clientNetwork, MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH, mqtt_sn_gateway_address,
                                &clientNetworkContext->forwarderClientNetworkAddress,
                                &clientNetworkContext->forwarderClientNetworkBroadcastAddress,
                                &clientNetworkContext->udpClientNetworkContext, ClientArduinoUdpInitialize)) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}