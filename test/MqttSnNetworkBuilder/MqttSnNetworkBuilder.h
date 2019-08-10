//
// Created by SomeDude on 24.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNNETWORKBUILDER_MQTTSNNETWORKBUILDER_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNNETWORKBUILDER_MQTTSNNETWORKBUILDER_H_

#include <memory>
#include <vector>
#include <test/MqttNetworkBuilder/MqttNetworkBuilder.h>
#include <test/MqttSnGateway/MqttSnGatewayTestContainerInterface.h>
#include <test/MqttSnClient/MqttSnClientTestContainerInterface.h>
#include <test/MqttSnForwarder/MqttSnForwarderTestContainerInterface.h>
#include <test/MqttSnNetworkBuilder/MqttSnAddressGenerator/MqttSnNetworkBuilderMqttSnAddressGeneratorFactory.h>
#include "MqttSnNetworkBuilderConfiguration.h"
#include "test/MqttSnGateway/MqttSnGatewayTestContainerConfiguration.h"

class MqttNetworkBuilder;

class MqttSnNetworkBuilder {
 private:
  const MqttNetworkBuilder *mqttNetworkBuilder;
  const MqttSnNetworkBuilderConfiguration configuration;
  MqttSnNetworkBuilderMqttSnAddressGeneratorFactory address_generator_factory;

  std::vector<MqttSnGatewayTestContainerConfiguration> mqtt_sn_gateway_configurations;
  std::vector<MqttSnClientTestContainerConfiguration> mqtt_sn_client_configurations;

 public:
  MqttSnNetworkBuilder(const MqttNetworkBuilder *mqtt_network_builder);

  std::shared_ptr<MqttSnGatewayTestContainerInterface> getMqttSnGateway();
  std::shared_ptr<MqttSnClientTestContainerInterface> getMqttSnClient();
  std::shared_ptr<MqttSnForwarderTestContainerInterface> getMqttSnForwarder();
 private:
  MqttSnGatewayTestContainerConfiguration getMqttSnGatewayConfiguration();
  MqttSnClientTestContainerConfiguration getMqttSnClientConfiguration();
};

#endif //CMQTTSNFORWARDER_TEST_MQTTSNNETWORKBUILDER_MQTTSNNETWORKBUILDER_H_
