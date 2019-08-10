//
// Created by SomeDude on 21.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTOBJECTBUILDER_MQTTOBJECTBUILDER_H_
#define CMQTTSNFORWARDER_TEST_MQTTOBJECTBUILDER_MQTTOBJECTBUILDER_H_

#include <memory>
#include <vector>
#include <test/MqttSnNetworkBuilder/MqttSnNetworkBuilder.h>
#include <test/MqttBroker/MqttBrokerTestContainerInterface.h>
#include <test/MqttClient/MqttClientTestContainerInterface.h>
#include "MqttNetworkBuilderConfiguration.h"

class MqttSnNetworkBuilder;

class MqttNetworkBuilder {
 private:
  const MqttNetworkBuilderConfiguration configuration;
  std::vector<MqttBrokerTestContainerConfiguration> mqtt_broker_configurations;
  std::vector<MqttClientTestContainerConfiguration> mqtt_client_configurations;

  MqttBrokerTestContainerConfiguration getMqttBrokerConfiguration() ;
  MqttClientTestContainerConfiguration getMqttClientConfiguration() ;

 public:
  MqttNetworkBuilder();
  MqttNetworkBuilder(const MqttNetworkBuilderConfiguration &configuration);

  std::shared_ptr<MqttBrokerTestContainerInterface> getMqttBroker();
  std::shared_ptr<MqttClientTestContainerInterface> getMqttClient();
  std::shared_ptr<MqttSnNetworkBuilder> getMqttSnNetworkBuilder();

  const std::vector<MqttBrokerTestContainerConfiguration> &GetMqttBrokerConfigurations() const;
  const std::vector<MqttClientTestContainerConfiguration> &GetMqttClientConfigurations() const;
};

#endif //CMQTTSNFORWARDER_TEST_MQTTOBJECTBUILDER_MQTTOBJECTBUILDER_H_
