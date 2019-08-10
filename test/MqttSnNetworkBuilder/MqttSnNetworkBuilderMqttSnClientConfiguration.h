//
// Created by SomeDude on 30.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNNETWORKBUILDER_MQTTSNNETWORKBUILDERMQTTSNCLIENTCONFIGURATION_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNNETWORKBUILDER_MQTTSNNETWORKBUILDERMQTTSNCLIENTCONFIGURATION_H_

#include <stdint.h>
#include <string>
#include <test/MqttSnClient/MqttSnClientTestType.h>
#include <test/MqttSnClient/MqttSnClientProtocolTestType.h>
#include <test/MqttSnClient/MqttSnClientFindGatewayType.h>

class MqttSnNetworkBuilderMqttSnClientConfiguration {
 public:
  const MqttSnClientTestType type;
  const int32_t maxClientCount;

  const MqttSnClientFindGatewayType find_gateway_type;

  // for searching
  const int32_t advertise_wait_timeout = -1;
  const int32_t search_gateway_wait_timeout = -1;
  const uint8_t search_gateway_radius = 0;

  // direct connect
  // const std::string brokerAddress; // example localhost
  //const uint16_t brokerPort = 0; // example 1883

  const std::string baseClientId;
  const std::string baseClientPassword;

  const int32_t keepAliveInterval;
  const bool cleanSession;

  MqttSnNetworkBuilderMqttSnClientConfiguration(const MqttSnClientTestType type,
                                                const int32_t max_client_count,
                                                const MqttSnClientFindGatewayType find_gateway_type,
                                                const int32_t advertise_wait_timeout,
                                                const int32_t search_gateway_wait_timeout,
                                                const uint8_t search_gateway_radius,
                                                const std::string &base_client_id,
                                                const std::string &base_client_password,
                                                const int32_t keep_alive_interval,
                                                const bool clean_session);
  static const MqttSnNetworkBuilderMqttSnClientConfiguration GetDefaultConfiguration();
  static const MqttSnNetworkBuilderMqttSnClientConfiguration GetDefaultConfiguration(const MqttSnClientFindGatewayType &find_gateway_type);
};

#endif //CMQTTSNFORWARDER_TEST_MQTTSNNETWORKBUILDER_MQTTSNNETWORKBUILDERMQTTSNCLIENTCONFIGURATION_H_
