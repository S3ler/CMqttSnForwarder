//
// Created by SomeDude on 30.07.2019.
//

#include "MqttSnClientTestContainerConfiguration.h"
#include <config/network/client/client_network_config.h>
#include <test/shared/MqttSnURLParser/MqttSnURLParser.h>
#include "MqttSnClientAdvertiseConnectAction.h"
#include "MqttSnClientDirectConnectAction.h"
#include "MqttSnClientSearchGatewayConnectAction.h"

MqttSnClientTestContainerConfiguration::MqttSnClientTestContainerConfiguration(const MqttSnClientProtocolTestType protocol_type, const std::string interfaceAddress, const uint16_t interfacePort,
                                                                               const std::string interfaceBroadcastAddress, const uint16_t interfaceBroadcastPort,
                                                                               const int32_t search_gateway_wait_timeout, const uint8_t search_gateway_radius, const std::string &client_id,
                                                                               const std::string &client_password, const int32_t keep_alive_interval, const bool clean_session)
    : protocol_type(protocol_type),
      interfaceAddress(MqttSnURLParser::GetBrokerAddressFromUriAndPort(interfaceAddress, interfacePort)),
      interfaceBroadcastAddress(MqttSnURLParser::GetBrokerAddressFromUriAndPort(interfaceBroadcastAddress, interfaceBroadcastPort)),
      find_gateway_type(MqttSnClientFindGatewayType::SEARCH),
      search_gateway_wait_timeout(search_gateway_wait_timeout),
      search_gateway_radius(search_gateway_radius),
      clientId(client_id),
      clientPassword(client_password),
      keepAliveInterval(keep_alive_interval),
      cleanSession(clean_session) {}
MqttSnClientTestContainerConfiguration::MqttSnClientTestContainerConfiguration(const MqttSnClientProtocolTestType protocol_type, const std::string interfaceAddress, const uint16_t interfacePort,
                                                                               const std::string interfaceBroadcastAddress, const uint16_t interfaceBroadcastPort, const int32_t advertise_wait_timeout,
                                                                               const std::string &client_id, const std::string &client_password, const int32_t keep_alive_interval,
                                                                               const bool clean_session)
    : protocol_type(protocol_type),
      interfaceAddress(MqttSnURLParser::GetBrokerAddressFromUriAndPort(interfaceAddress, interfacePort)),
      interfaceBroadcastAddress(MqttSnURLParser::GetBrokerAddressFromUriAndPort(interfaceBroadcastAddress, interfaceBroadcastPort)),
      find_gateway_type(MqttSnClientFindGatewayType::ADVERTISE),
      advertise_wait_timeout(advertise_wait_timeout),
      clientId(client_id),
      clientPassword(client_password),
      keepAliveInterval(keep_alive_interval),
      cleanSession(clean_session) {}
MqttSnClientTestContainerConfiguration::MqttSnClientTestContainerConfiguration(const MqttSnClientProtocolTestType protocol_type, const std::string interfaceAddress, const uint16_t interfacePort,
                                                                               const std::string interfaceBroadcastAddress, const uint16_t interfaceBroadcastPort, const std::string &broker_address,
                                                                               const uint16_t broker_port, const std::string &client_id, const std::string &client_password,
                                                                               const int32_t keep_alive_interval, const bool clean_session)
    : protocol_type(protocol_type),
      interfaceAddress(MqttSnURLParser::GetBrokerAddressFromUriAndPort(interfaceAddress, interfacePort)),
      interfaceBroadcastAddress(MqttSnURLParser::GetBrokerAddressFromUriAndPort(interfaceBroadcastAddress, interfaceBroadcastPort)),
      find_gateway_type(MqttSnClientFindGatewayType::CONNECT),
      brokerAddress(MqttSnURLParser::GetBrokerAddressFromUriAndPort(broker_address, broker_port)),
      clientId(client_id),
      clientPassword(client_password),
      keepAliveInterval(keep_alive_interval),
      cleanSession(clean_session) {}
MqttSnClientTestContainerConfiguration MqttSnClientTestContainerConfiguration::GetDefaultConfiguration() {
  return MqttSnClientTestContainerConfiguration(MqttSnClientProtocolTestType::UDP, "localhost", 20000, DEFAULT_MQTT_SN_CLIENT_BROADCAST_ADDRESS, DEFAULT_MQTT_SN_CLIENT_BROADCAST_BIND_PORT,
                                                "localhost", 10000, "MqttSnDefaultClient", "", 60, true);
}
MqttSnClientTestContainerConfiguration MqttSnClientTestContainerConfiguration::GetDefaultForwarderConfiguration() {
  return MqttSnClientTestContainerConfiguration(MqttSnClientProtocolTestType::UDP, "localhost", 20001, DEFAULT_MQTT_SN_FORWADER_BROADCAST_ADDRESS, DEFAULT_MQTT_SN_FORWADER_BROADCAST_PORT, "localhost",
                                                30001, "MqttSnDefaultForwarderClient", "", 60, true);
}
MqttSnClientProtocolTestType MqttSnClientTestContainerConfiguration::GetProtocolFromGatewayProtocol(const MqttSnGatewayProtocolTestType &t) {
  if (t == MqttSnGatewayProtocolTestType::UDP) {
    return MqttSnClientProtocolTestType::UDP;
  }
  if (t == MqttSnGatewayProtocolTestType::TCP) {
    return MqttSnClientProtocolTestType::TCP;
  }
  throw std::exception();
}
MqttSnClientTestContainerConfiguration::MqttSnClientTestContainerConfiguration(const MqttSnClientProtocolTestType protocol_type, const std::vector<uint8_t> interfaceAddress,
                                                                               const std::vector<uint8_t> interfaceBroadcastAddress, const std::vector<uint8_t> broker_address,
                                                                               const std::string &client_id, const std::string &client_password, const int32_t keep_alive_interval,
                                                                               const bool clean_session)
    : protocol_type(protocol_type),
      interfaceAddress(interfaceAddress),
      interfaceBroadcastAddress(interfaceBroadcastAddress),
      find_gateway_type(MqttSnClientFindGatewayType::CONNECT),
      brokerAddress(broker_address),
      clientId(client_id),
      clientPassword(client_password),
      keepAliveInterval(keep_alive_interval),
      cleanSession(clean_session) {}
MqttSnClientTestContainerConfiguration::MqttSnClientTestContainerConfiguration(const MqttSnClientProtocolTestType protocol_type, const std::vector<uint8_t> interfaceAddress,
                                                                               const std::vector<uint8_t> interfaceBroadcastAddress, const int32_t advertise_wait_timeout, const std::string &client_id,
                                                                               const std::string &client_password, const int32_t keep_alive_interval, const bool clean_session)
    : protocol_type(protocol_type),
      interfaceAddress(interfaceAddress),
      interfaceBroadcastAddress(interfaceBroadcastAddress),
      find_gateway_type(MqttSnClientFindGatewayType::ADVERTISE),
      advertise_wait_timeout(advertise_wait_timeout),
      clientId(client_id),
      clientPassword(client_password),
      keepAliveInterval(keep_alive_interval),
      cleanSession(clean_session) {}
MqttSnClientTestContainerConfiguration::MqttSnClientTestContainerConfiguration(const MqttSnClientProtocolTestType protocol_type, const std::vector<uint8_t> interfaceAddress,
                                                                               const std::vector<uint8_t> interfaceBroadcastAddress, const int32_t search_gateway_wait_timeout,
                                                                               const uint8_t search_gateway_radius, const std::string &client_id, const std::string &client_password,
                                                                               const int32_t keep_alive_interval, const bool clean_session)
    : protocol_type(protocol_type),
      interfaceAddress(interfaceAddress),
      interfaceBroadcastAddress(interfaceBroadcastAddress),
      find_gateway_type(MqttSnClientFindGatewayType::SEARCH),
      search_gateway_wait_timeout(search_gateway_wait_timeout),
      search_gateway_radius(search_gateway_radius),
      clientId(client_id),
      clientPassword(client_password),
      keepAliveInterval(keep_alive_interval),
      cleanSession(clean_session) {}
MqttSnClientTestContainerConfiguration::MqttSnClientTestContainerConfiguration(const MqttSnClientProtocolTestType protocol_type, const std::vector<uint8_t> interfaceAddress,
                                                                               const std::vector<uint8_t> interfaceBroadcastAddress, const int32_t advertise_wait_timeout,
                                                                               const int32_t search_gateway_wait_timeout, const uint8_t search_gateway_radius,
                                                                               const std::vector<uint8_t> broker_address, const std::string &client_id, const std::string &client_password,
                                                                               const int32_t keep_alive_interval, const bool clean_session)
    : protocol_type(protocol_type),
      interfaceAddress(interfaceAddress),
      interfaceBroadcastAddress(interfaceBroadcastAddress),
      find_gateway_type(MqttSnClientFindGatewayType::ANY),
      search_gateway_wait_timeout(search_gateway_wait_timeout),
      search_gateway_radius(search_gateway_radius),
      advertise_wait_timeout(advertise_wait_timeout),
      brokerAddress(broker_address),
      clientId(client_id),
      clientPassword(client_password),
      keepAliveInterval(keep_alive_interval),
      cleanSession(clean_session) {}
