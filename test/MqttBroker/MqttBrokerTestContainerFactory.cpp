//
// Created by SomeDude on 21.07.2019.
//

#include "MqttBrokerTestContainerFactory.h"
#include "MqttBrokerMosquittoDockerTestContainer.h"
#include "MqttBrokerExternalTestContainer.h"
std::shared_ptr<MqttBrokerTestContainerInterface> MqttBrokerTestContainerFactory::getMqttBroker() { return getMqttBroker(MqttBrokerTestType::MOSQUITTO_DOCKER); }
std::shared_ptr<MqttBrokerTestContainerInterface> MqttBrokerTestContainerFactory::getMqttBroker(MqttBrokerTestType mqttBrokerTestType) {
  if (mqttBrokerTestType == MqttBrokerTestType::MOSQUITTO_DOCKER) {
    return std::make_shared<MqttBrokerMosquittoDockerTestContainer>(MqttBrokerTestContainerConfiguration::GetDefaultTestContainerConfiguration());
  }
  if (mqttBrokerTestType == MqttBrokerTestType::EXTERNAL) {
    return std::make_shared<MqttBrokerExternalTestContainer>(MqttBrokerTestContainerConfiguration::GetDefaultTestContainerConfiguration());
  }
  return nullptr;
}
std::shared_ptr<MqttBrokerTestContainerInterface> MqttBrokerTestContainerFactory::getMqttBroker(MqttBrokerTestType mqttBrokerTestType,
                                                                                                MqttBrokerTestContainerConfiguration mqttBrokerConfiguration) {
  if (mqttBrokerTestType == MqttBrokerTestType::MOSQUITTO_DOCKER) {
    return std::make_shared<MqttBrokerMosquittoDockerTestContainer>(mqttBrokerConfiguration);
  }
  if (mqttBrokerTestType == MqttBrokerTestType::EXTERNAL) {
    return std::make_shared<MqttBrokerExternalTestContainer>(mqttBrokerConfiguration);
  }
  return nullptr;
}
