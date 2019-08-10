//
// Created by SomeDude on 22.07.2019.
//

#include "MqttClientTestContainerFactory.h"
#include "MqttClientPahoCppClientTestContainer.h"
#include "MqttClientConnectAction.h"
#include "../MqttBroker/MqttBrokerTestContainerInterface.h"

std::shared_ptr<MqttClientTestContainerInterface> MqttClientTestContainerFactory::getMqttClient(
    MqttClientTestType mqttClientTestType,
    const MqttClientTestContainerConfiguration mqttClientConfiguration) {
  if (mqttClientTestType == MqttClientTestType::PAHO_CPP) {
    return std::make_shared<MqttClientPahoCppClientTestContainer>(mqttClientConfiguration);
  }
  return nullptr;
}
std::shared_ptr<MqttClientTestContainerInterface> MqttClientTestContainerFactory::getMqttClient(
    MqttClientTestType mqttClientTestType,
    const MqttBrokerTestContainerConfiguration &mqttBrokerConfiguration) {
  static int i = 0;
  MqttClientTestContainerConfiguration mqttClientConfiguration(mqttBrokerConfiguration,
                                                               std::string("MqttClientContainer") + std::to_string(i++),
                                                               std::string(""),
                                                               20,
                                                               true);
  return getMqttClient(mqttClientTestType, mqttClientConfiguration);
}
