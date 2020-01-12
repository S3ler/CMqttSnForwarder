//
// Created by SomeDude on 24.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNNETWORKBUILDER_MQTTSNNETWORKBUILDER_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNNETWORKBUILDER_MQTTSNNETWORKBUILDER_H_

#include <test/MqttNetworkBuilder/MqttNetworkBuilder.h>
#include <test/MqttSnClient/MqttSnClientTestContainerInterface.h>
#include <test/MqttSnForwarder/MqttSnForwarderTestContainerConfiguration.h>
#include <test/MqttSnForwarder/MqttSnForwarderTestContainerInterface.h>
#include <test/MqttSnGateway/MqttSnGatewayTestContainerInterface.h>
#include <test/MqttSnNetworkBuilder/MqttSnAddressGenerator/MqttSnNetworkBuilderMqttSnAddressGeneratorFactory.h>
#include <memory>
#include <vector>
#include "MqttSnNetworkBuilderConfiguration.h"
#include "test/MqttSnGateway/MqttSnGatewayTestContainerConfiguration.h"

class MqttNetworkBuilder;

class MqttSnNetworkBuilder {
 private:
  const MqttNetworkBuilder* mqttNetworkBuilder;
  const MqttSnNetworkBuilderConfiguration configuration;
  MqttSnNetworkBuilderMqttSnAddressGeneratorFactory address_generator_factory;

  std::vector<MqttSnGatewayTestContainerConfiguration> mqtt_sn_gateway_configurations;
  std::vector<MqttSnForwarderTestContainerConfiguration> mqtt_sn_forwarder_configurations;
  std::vector<MqttSnClientTestContainerConfiguration> mqtt_sn_client_configurations;

  std::vector<std::shared_ptr<MqttSnGatewayTestContainerInterface>> mqttSnGateways;
  std::vector<std::shared_ptr<MqttSnForwarderTestContainerInterface>> mqttSnForwarders;
  std::vector<std::shared_ptr<MqttSnClientTestContainerInterface>> mqttSnClients;

 public:
  MqttSnNetworkBuilder(const MqttNetworkBuilder* mqtt_network_builder);

  std::shared_ptr<MqttSnGatewayTestContainerInterface> getMqttSnGateway();
  std::shared_ptr<MqttSnGatewayTestContainerInterface> getMqttSnGateway(const MqttSnGatewayTestContainerConfiguration& gatewayConfiguration);
  std::shared_ptr<MqttSnForwarderTestContainerInterface> getMqttSnForwarder();
  std::shared_ptr<MqttSnClientTestContainerInterface> getMqttSnClient();
  std::shared_ptr<MqttSnClientTestContainerInterface> getForwardedMqttSnClient();

  MqttSnGatewayTestContainerConfiguration getMqttSnGatewayConfiguration();
  MqttSnGatewayTestContainerConfiguration getMqttSnGatewayConfiguration(const std::vector<uint8_t>& interface_broadcast_address);
  MqttSnClientTestContainerConfiguration getForwardedMqttSnClientConfiguration();
  MqttSnClientTestContainerConfiguration getMqttSnClientConfiguration();
  MqttSnForwarderTestContainerConfiguration getMqttSnForwarderTestContainerConfiguration();

  MqttSnForwarderTestContainerConfiguration getMqttSnForwarderTestContainerConfiguration(const std::vector<uint8_t>& interface_broadcast_address);

  const std::vector<std::shared_ptr<MqttSnGatewayTestContainerInterface>>& getMqttSnGateways() const;
  const std::vector<std::shared_ptr<MqttSnForwarderTestContainerInterface>>& getMqttSnForwarders() const;
  const std::vector<std::shared_ptr<MqttSnClientTestContainerInterface>>& getMqttSnClients() const;

  bool startMqttSnGateways();
  bool startMqttSnForwarders();
  bool startMqttSnClients();

  void execMqttSnClientsActionSequenceAsync();
  std::tuple<int, int> getMqttSnClientsActionsSequenceExecutingFinished();

  void stopMqttSnGateways();
  void stopMqttSnForwarders();
  void stopMqttSnClients();



 private:
  bool createPahoClientsConfFile(const vector<MqttSnClientTestContainerConfiguration>& mqtt_sn_client_configurations,
                                 const vector<MqttSnForwarderTestContainerConfiguration>& mqtt_sn_forwarder_configurations);
  bool createPahoPredefinedTopicConfFile(const vector<MqttSnGatewayTestContainerConfiguration> mqtt_sn_gateway_configurations,
                                         const vector<MqttSnClientTestContainerConfiguration>& mqtt_sn_client_configurations);
};

#endif  // CMQTTSNFORWARDER_TEST_MQTTSNNETWORKBUILDER_MQTTSNNETWORKBUILDER_H_
