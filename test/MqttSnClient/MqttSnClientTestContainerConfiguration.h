//
// Created by SomeDude on 30.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTTESTCONTAINERCONFIGURATION_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTTESTCONTAINERCONFIGURATION_H_

#include <memory>
#include <string>
#include <vector>
#include "MqttSnClientConnectAction.h"
#include "MqttSnClientFindGatewayType.h"
#include "MqttSnClientProtocolTestType.h"
#include <test/MqttSnGateway/MqttSnGatewayProtocolTestType.h>

#ifndef DEFAULT_MQTT_SN_FORWADER_BROADCAST_ADDRESS
#define DEFAULT_MQTT_SN_FORWADER_BROADCAST_ADDRESS "225.1.1.2"
#endif

#ifndef DEFAULT_MQTT_SN_FORWADER_BROADCAST_PORT
#define DEFAULT_MQTT_SN_FORWADER_BROADCAST_PORT 1883
#endif

class MqttSnClientConnectAction;

class MqttSnClientTestContainerConfiguration {
  // Mqtt-Sn Client
  // Mqtt-Sn Client implementation
  // Mqtt-SN Client's find gateway strategy (Default=Direct, SearchGw, Advertisement)
  // On Direct: Mqtt-SN Client's Gateway Address + Mqtt-SN Client's Multicast Network Address (same as Gateway's)
  // On Other: Mqtt-SN Client's Multicast Network Address (same as Gateway)
  // Mqtt-Sn Client Network Address Pool - depend on implementation (Min, Max, List)
 public:
  const MqttSnClientProtocolTestType protocol_type;
  const std::vector<uint8_t> interfaceAddress;
  const std::vector<uint8_t> interfaceBroadcastAddress;

  const MqttSnClientFindGatewayType find_gateway_type;

  // for search
  const int32_t search_gateway_wait_timeout = 5000;
  const uint8_t search_gateway_radius = 1;

  // for advertise
  const int32_t advertise_wait_timeout = 900;

  // direct connect
  const std::vector<uint8_t> brokerAddress;

  const std::string clientId;        // not optional
  const std::string clientPassword;  // optional

  const int32_t keepAliveInterval;
  const bool cleanSession;

  // TODO will
  // const std::string willTopic;
  // const std::vector<uint8_t> willMessage;
  // const int32_t willQoS;
  // for any possible connect option

  const std::vector<std::tuple<uint16_t, std::string>> predefined_topics;

  MqttSnClientTestContainerConfiguration(const MqttSnClientProtocolTestType protocol_type, const std::vector<uint8_t> interfaceAddress, const std::vector<uint8_t> interfaceBroadcastAddress,
                                         const int32_t advertise_wait_timeout, const int32_t search_gateway_wait_timeout, const uint8_t search_gateway_radius,
                                         const std::vector<uint8_t> broker_address, const std::string &client_id, const std::string &client_password, const int32_t keep_alive_interval,
                                         const bool clean_session);

  // search gateway constructor
  MqttSnClientTestContainerConfiguration(const MqttSnClientProtocolTestType protocol_type, const std::string interfaceAddress, const uint16_t interfacePort,
                                         const std::string interfaceBroadcastAddress, const uint16_t interfaceBroadcastPort, const int32_t search_gateway_wait_timeout,
                                         const uint8_t search_gateway_radius, const std::string &client_id, const std::string &client_password, const int32_t keep_alive_interval,
                                         const bool clean_session);
  MqttSnClientTestContainerConfiguration(const MqttSnClientProtocolTestType protocol_type, const std::vector<uint8_t> interfaceAddress, const std::vector<uint8_t> interfaceBroadcastAddress,
                                         const int32_t search_gateway_wait_timeout, const uint8_t search_gateway_radius, const std::string &client_id, const std::string &client_password,
                                         const int32_t keep_alive_interval, const bool clean_session);
  // wait advertisement constructor
  MqttSnClientTestContainerConfiguration(const MqttSnClientProtocolTestType protocol_type, const std::string interfaceAddress, const uint16_t interfacePort,
                                         const std::string interfaceBroadcastAddress, const uint16_t interfaceBroadcastPort, const int32_t advertise_wait_timeout, const std::string &client_id,
                                         const std::string &client_password, const int32_t keep_alive_interval, const bool clean_session);
  MqttSnClientTestContainerConfiguration(const MqttSnClientProtocolTestType protocol_type, const std::vector<uint8_t> interfaceAddress, const std::vector<uint8_t> interfaceBroadcastAddress,
                                         const int32_t advertise_wait_timeout, const std::string &client_id, const std::string &client_password, const int32_t keep_alive_interval,
                                         const bool clean_session);
  // direct connect
  MqttSnClientTestContainerConfiguration(const MqttSnClientProtocolTestType protocol_type, const std::string interfaceAddress, const uint16_t interfacePort,
                                         const std::string interfaceBroadcastAddress, const uint16_t interfaceBroadcastPort, const std::string &broker_address, const uint16_t broker_port,
                                         const std::string &client_id, const std::string &client_password, const int32_t keep_alive_interval, const bool clean_session);
  MqttSnClientTestContainerConfiguration(const MqttSnClientProtocolTestType protocol_type, const std::vector<uint8_t> interfaceAddress, const std::vector<uint8_t> interfaceBroadcastAddress,
                                         const std::vector<uint8_t> broker_address, const std::string &client_id, const std::string &client_password, const int32_t keep_alive_interval,
                                         const bool clean_session);

  static MqttSnClientTestContainerConfiguration GetDefaultConfiguration();
  static MqttSnClientTestContainerConfiguration GetDefaultForwarderConfiguration();

  static MqttSnClientProtocolTestType GetProtocolFromGatewayProtocol(const MqttSnGatewayProtocolTestType &t);
};

#endif  // CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTTESTCONTAINERCONFIGURATION_H_
