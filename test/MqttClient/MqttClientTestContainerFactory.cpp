//
// Created by SomeDude on 22.07.2019.
//

#include "MqttClientTestContainerFactory.h"
#include "../MqttBroker/MqttBrokerTestContainerInterface.h"
#include "MqttClientConnectAction.h"
#include "MqttClientPahoCppClientTestContainer.h"
std::shared_ptr<MqttClientTestContainerInterface> MqttClientTestContainerFactory::getMqttClient() {
  return getMqttClient(MqttClientTestType::PAHO_CPP, MqttBrokerTestContainerConfiguration::GetDefaultTestContainerConfiguration());
}

std::shared_ptr<MqttClientTestContainerInterface> MqttClientTestContainerFactory::getMqttClient(MqttClientTestType mqttClientTestType,
                                                                                                const MqttClientTestContainerConfiguration mqttClientConfiguration) {
  if (mqttClientTestType == MqttClientTestType::PAHO_CPP) {
    return std::make_shared<MqttClientPahoCppClientTestContainer>(mqttClientConfiguration);
  }
  return nullptr;
}
std::shared_ptr<MqttClientTestContainerInterface> MqttClientTestContainerFactory::getMqttClient(MqttClientTestType mqttClientTestType,
                                                                                                const MqttBrokerTestContainerConfiguration &mqttBrokerConfiguration) {
  static int i = 0;
  MqttClientTestContainerConfiguration mqttClientConfiguration(mqttBrokerConfiguration, std::string("MqttClientContainer") + std::to_string(i++), std::string(""), 20, true, 10);
  return getMqttClient(mqttClientTestType, mqttClientConfiguration);
}
