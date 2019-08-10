//
// Created by SomeDude on 30.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNNETWORKBUILDER_MQTTSNNETWORKBUILDERMQTTSNGATEWAYCONFIGURATION_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNNETWORKBUILDER_MQTTSNNETWORKBUILDERMQTTSNGATEWAYCONFIGURATION_H_

#include <stdint.h>
#include <string>
#include <test/MqttSnGateway/MqttSnGatewayTestType.h>
#include "test/MqttSnGateway/MqttSnGatewayProtocolTestType.h"
class MqttSnNetworkBuilderMqttSnGatewayConfiguration {
 public:
  const MqttNetworkBuilderConfiguration mqtt_network_builder_configuration;

  const std::string baseClientId;
  const std::string baseClientPassword;
  const int32_t keepAliveInterval;
  const bool cleanSession;

  const MqttSnGatewayTestType type;
  const int32_t maxGatewayCount;

  const MqttSnGatewayProtocolTestType protocol_type;
  const std::string gatewayAddressURL;
  const uint16_t minGatewayPort;
  const std::string gatewayBroadcastAddressURL;
  const uint16_t gatewayBroadcastPort;

  const uint16_t minGatewayId;

  MqttSnNetworkBuilderMqttSnGatewayConfiguration(const MqttNetworkBuilderConfiguration &mqtt_network_builder_configuration,
                                                 const std::string &base_client_id,
                                                 const std::string &base_client_password,
                                                 const int32_t keep_alive_interval,
                                                 const bool clean_session,
                                                 const MqttSnGatewayTestType type,
                                                 const int32_t max_gateway_count,
                                                 const MqttSnGatewayProtocolTestType protocol_type,
                                                 const std::string &gateway_address_url,
                                                 const uint16_t min_gateway_port,
                                                 const std::string &gatewayBroadcastAddressURL,
                                                 const uint16_t gatewayBroadcastPort,
                                                 const uint8_t mingGatewayId);

  static const MqttSnNetworkBuilderMqttSnGatewayConfiguration GetDefaultConfiguration();
  static const MqttSnNetworkBuilderMqttSnGatewayConfiguration GetDefaultConfiguration(const MqttNetworkBuilderConfiguration &mqtt_network_builder_configuration);

};

#endif //CMQTTSNFORWARDER_TEST_MQTTSNNETWORKBUILDER_MQTTSNNETWORKBUILDERMQTTSNGATEWAYCONFIGURATION_H_
