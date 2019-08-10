//
// Created by SomeDude on 30.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNNETWORKBUILDER_MQTTSNNETWORKBUILDERCONFIGURATION_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNNETWORKBUILDER_MQTTSNNETWORKBUILDERCONFIGURATION_H_

#include <test/MqttNetworkBuilder/MqttNetworkBuilderConfiguration.h>
#include "MqttSnNetworkBuilderMqttSnGatewayConfiguration.h"
#include "MqttSnNetworkBuilderMqttSnClientConfiguration.h"
class MqttSnNetworkBuilderConfiguration {
 public:
  const MqttNetworkBuilderConfiguration mqtt_network_builder_configuration;
  const MqttSnNetworkBuilderMqttSnGatewayConfiguration mqtt_sn_gateway_config;
  const MqttSnNetworkBuilderMqttSnClientConfiguration mqtt_sn_client_config;
  // Mqtt-Sn Gateway
  // Mqtt-SN Gateway implementation
  // Mqtt-Sn Gateway's Broker Address+Port with Distribution (single, equal)
  // Mqtt-Sn Gateway's Network Address Pool - depend on implementation (Min, Max, List)
  // Mqtt-sn Gateway's Multicast Network Address

  // Mqtt-Sn Client
  // Mqtt-Sn Client implementation
  // Mqtt-SN Client's find gateway strategy (Default=Direct, SearchGw, Advertisement)
  // On Direct: Mqtt-SN Client's Gateway Address + Mqtt-SN Client's Multicast Network Address (same as Gateway's)
  // On Other: Mqtt-SN Client's Multicast Network Address (same as Gateway)
  // Mqtt-Sn Client Network Address Pool - depend on implementation (Min, Max, List)

  // Mqtt-Sn Forwarder
  // Mqtt-Sn Forwarder implementation
  // Mqtt-Sn Forwarder's find gateway strategy (Default=Direct, SearchGw, Advertisement)
  // On Direct: Mqtt-Sn Forwarder's Gateway Address + Mqtt-Sn Forwarder's Multicast Network Address (same as Gateway's)
  // On Other: Mqtt-Sn Forwarder's Multicast Network Address (same as Gateway's)
  MqttSnNetworkBuilderConfiguration(const MqttNetworkBuilderConfiguration &mqtt_network_builder_configuration,
                                    const MqttSnNetworkBuilderMqttSnGatewayConfiguration &mqtt_sn_gateway_config,
                                    const MqttSnNetworkBuilderMqttSnClientConfiguration &mqtt_sn_client_config);
  static const MqttSnNetworkBuilderConfiguration DefaultConfiguration(const MqttNetworkBuilderConfiguration &mqttNetworkBuilderConfiguration);
  static const MqttSnNetworkBuilderConfiguration DefaultConfiguration();
};

#endif //CMQTTSNFORWARDER_TEST_MQTTSNNETWORKBUILDER_MQTTSNNETWORKBUILDERCONFIGURATION_H_
