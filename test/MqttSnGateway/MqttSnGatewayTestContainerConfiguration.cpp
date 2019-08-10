//
// Created by SomeDude on 30.07.2019.
//

#include <test/MqttSnClient/MqttSnClientConnectAction.h>
#include <test/shared/MqttSnURLParser/MqttSnURLParser.h>
#include <config/network/client/client_network_config.h>
#include <config/gateway/gateway_advertisment_config.h>
#include "MqttSnGatewayTestContainerConfiguration.h"
const MqttSnGatewayTestContainerConfiguration MqttSnGatewayTestContainerConfiguration::GetDefaultTestContainerConfiguration() {
  auto broker_cfg = MqttBrokerTestContainerConfiguration::GetDefaultTestContainerConfiguration();
  MqttClientTestContainerConfiguration mqtt_client_configuration(broker_cfg,
                                                                 "DefaultMqttSnGateway",
                                                                 "",
                                                                 900,
                                                                 true);
  return MqttSnGatewayTestContainerConfiguration(mqtt_client_configuration,
                                                 MqttSnGatewayProtocolTestType::UDP,
                                                 std::string("localhost"),
                                                 10000,
                                                 DEFAULT_MQTT_SN_CLIENT_BROADCAST_ADDRESS,
                                                 DEFAULT_MQTT_SN_CLIENT_BROADCAST_BIND_PORT,
                                                 MQTT_SN_GATEWAY_ADVERTISEMENT_DEFAULT_GW_ID);
}

MqttSnGatewayTestContainerConfiguration::MqttSnGatewayTestContainerConfiguration(const MqttClientTestContainerConfiguration &mqtt_client_configuration,
                                                                                 const MqttSnGatewayProtocolTestType a_protocol,
                                                                                 const std::vector<uint8_t> &interface_address,
                                                                                 const std::vector<uint8_t> &interface_broadcast_address,
                                                                                 const uint8_t gateway_id)
    : mqtt_client_configuration(mqtt_client_configuration),
      protocol(a_protocol),
      interfaceAddress(interface_address),
      interfaceBroadcastAddress(interface_broadcast_address),
      gateway_id(gateway_id) {}
MqttSnGatewayTestContainerConfiguration::MqttSnGatewayTestContainerConfiguration(const MqttClientTestContainerConfiguration &mqtt_client_configuration,
                                                                                 const MqttSnGatewayProtocolTestType a_protocol,
                                                                                 const std::string &interfaceAddressURL,
                                                                                 const int32_t interfacePort,
                                                                                 const std::string &interfaceBroadcastAddressURL,
                                                                                 const int32_t interfaceBroadcastPort,
                                                                                 const uint8_t gateway_id)
    : mqtt_client_configuration(mqtt_client_configuration),
      protocol(a_protocol),
      interfaceAddress(MqttSnURLParser::GetBrokerAddressFromUriAndPort(interfaceAddressURL, interfacePort)),
      interfaceBroadcastAddress(MqttSnURLParser::GetBrokerAddressFromUriAndPort(interfaceBroadcastAddressURL,
                                                                                interfaceBroadcastPort)),
      gateway_id(gateway_id) {}
