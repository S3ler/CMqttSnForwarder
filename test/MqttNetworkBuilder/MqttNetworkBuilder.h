//
// Created by SomeDude on 21.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTOBJECTBUILDER_MQTTOBJECTBUILDER_H_
#define CMQTTSNFORWARDER_TEST_MQTTOBJECTBUILDER_MQTTOBJECTBUILDER_H_

#include <memory>
#include <test/MqttSnNetworkBuilder/MqttSnNetworkBuilder.h>
#include <test/MqttBroker/MqttBrokerTestContainerInterface.h>
#include <test/MqttClient/MqttClientTestContainerInterface.h>
#include "MqttNetworkBuilderConfiguration.h"

class MqttSnNetworkBuilder;

class MqttNetworkBuilder {
 private:
  const MqttNetworkBuilderConfiguration configuration;
  int32_t createdMqttBrokers = 0;
  int32_t createdMqttClients = 0;

  MqttBrokerTestContainerConfiguration getMqttBrokerConfiguration() const;
  MqttClientConnectAction getMqttClientConfiguration() const;

 public:
  MqttNetworkBuilder();
  MqttNetworkBuilder(const MqttNetworkBuilderConfiguration &configuration);

  std::shared_ptr<MqttBrokerTestContainerInterface> getMqttBroker();
  std::shared_ptr<MqttClientTestContainerInterface> getMqttClient();
  std::shared_ptr<MqttSnNetworkBuilder> getMqttSnNetworkBuilder();
};

#endif //CMQTTSNFORWARDER_TEST_MQTTOBJECTBUILDER_MQTTOBJECTBUILDER_H_
