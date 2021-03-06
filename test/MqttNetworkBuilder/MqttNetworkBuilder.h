//
// Created by SomeDude on 21.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTOBJECTBUILDER_MQTTOBJECTBUILDER_H_
#define CMQTTSNFORWARDER_TEST_MQTTOBJECTBUILDER_MQTTOBJECTBUILDER_H_

#include <test/MqttBroker/MqttBrokerTestContainerInterface.h>
#include <test/MqttClient/MqttClientTestContainerInterface.h>
//#include <test/MqttSnNetworkBuilder/MqttSnNetworkBuilder.h>
#include <memory>
#include <vector>
#include "MqttNetworkBuilderConfiguration.h"


class MqttNetworkBuilder {
 private:
  const MqttNetworkBuilderConfiguration configuration;
  std::vector<MqttBrokerTestContainerConfiguration> mqtt_broker_configurations;
  std::vector<MqttClientTestContainerConfiguration> mqtt_client_configurations;

  MqttBrokerTestContainerConfiguration getMqttBrokerConfiguration();
  MqttClientTestContainerConfiguration getMqttClientConfiguration();

  std::vector<std::shared_ptr<MqttBrokerTestContainerInterface>> mqttBrokers;
  std::vector<std::shared_ptr<MqttClientTestContainerInterface>> mqttClients;

  //std::vector<std::shared_ptr<MqttSnNetworkBuilder>> mqttSnNetworkBuilders;

 public:
  MqttNetworkBuilder();
  MqttNetworkBuilder(const MqttNetworkBuilderConfiguration &configuration);

  std::shared_ptr<MqttBrokerTestContainerInterface> getMqttBroker();
  std::shared_ptr<MqttClientTestContainerInterface> getMqttClient();
  //std::shared_ptr<MqttSnNetworkBuilder> getMqttSnNetworkBuilder();

  const std::vector<MqttBrokerTestContainerConfiguration> &GetMqttBrokerConfigurations() const;
  const std::vector<MqttClientTestContainerConfiguration> &GetMqttClientConfigurations() const;
  const std::vector<std::shared_ptr<MqttBrokerTestContainerInterface>> &getMqttBrokers() const;
  const  std::vector<std::shared_ptr<MqttClientTestContainerInterface>> &getMqttClients() const;

  bool startMqttBrokers();
  bool startMqttClients();

  void execMqttClientsActionSequenceAsync();
  std::tuple<int, int> getMqttClientsActionsSequenceExecutingFinished();

  void stopMqttBrokers();
  void stopMqttClients();

  virtual ~MqttNetworkBuilder();
};

#endif  // CMQTTSNFORWARDER_TEST_MQTTOBJECTBUILDER_MQTTOBJECTBUILDER_H_
