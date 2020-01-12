//
// Created by SomeDude on 17.09.2019.
//

#include "MqttSnForwarderTestContainerConfiguration.h"
#include <config/network/client/client_network_config.h>
#include <test/shared/MqttSnURLParser/MqttSnURLParser.h>
MqttSnForwarderTestContainerConfiguration::MqttSnForwarderTestContainerConfiguration(const MqttSnClientTestContainerConfiguration& mqttSnClientConfiguration,
                                                                                     const MqttSnGatewayProtocolTestType protocol, const std::vector<uint8_t>& interfaceAddress,
                                                                                     const std::vector<uint8_t>& interfaceBroadcastAddress)
    : mqttSnClientConfiguration(mqttSnClientConfiguration), protocol(protocol), interfaceAddress(interfaceAddress), interfaceBroadcastAddress(interfaceBroadcastAddress) {}
MqttSnForwarderTestContainerConfiguration::MqttSnForwarderTestContainerConfiguration(const MqttSnClientTestContainerConfiguration& mqttSnClientConfiguration,
                                                                                     const MqttSnGatewayProtocolTestType protocol, const std::string interfaceAddress, const uint16_t interfacePort,
                                                                                     const std::string interfaceBroadcastAddress, const uint16_t interfaceBroadcastPort)
    : mqttSnClientConfiguration(mqttSnClientConfiguration),
      protocol(protocol),
      interfaceAddress(MqttSnURLParser::GetBrokerAddressFromUriAndPort(interfaceAddress, interfacePort)),
      interfaceBroadcastAddress(MqttSnURLParser::GetBrokerAddressFromUriAndPort(interfaceBroadcastAddress, interfaceBroadcastPort)) {}

MqttSnForwarderTestContainerConfiguration MqttSnForwarderTestContainerConfiguration::GetDefaultTestContainerConfiguration() {
  auto mqttSnClientTestContainerConfiguration = MqttSnClientTestContainerConfiguration(MqttSnClientProtocolTestType::UDP, "localhost", 30000, DEFAULT_MQTT_SN_CLIENT_BROADCAST_ADDRESS,
                                                                                       DEFAULT_MQTT_SN_CLIENT_BROADCAST_BIND_PORT, "localhost", 10000, "MqttSnForwarder", "", 60, true);
  return MqttSnForwarderTestContainerConfiguration(mqttSnClientTestContainerConfiguration, MqttSnGatewayProtocolTestType::UDP, "localhost", 30001, DEFAULT_MQTT_SN_FORWADER_BROADCAST_ADDRESS,
                                                   DEFAULT_MQTT_SN_FORWADER_BROADCAST_PORT);
}
