//
// Created by SomeDude on 22.07.2019.
//

#include "MqttClientTestContainerFactory.h"
#include "MqttClientPahoCppClientTestContainer.h"
#include "MqttClientConnectAction.h"
#include "../MqttBroker/MqttBrokerTestContainerInterface.h"

std::shared_ptr<MqttClientTestContainerInterface> MqttClientTestContainerFactory::getMqttClient(
    MqttClientTestType mqttClientTestType,
    const MqttClientConnectAction mqttClientConnectAction) {
  if (mqttClientTestType == MqttClientTestType::PAHO_CPP) {
    return std::make_shared<MqttClientPahoCppClientTestContainer>(mqttClientConnectAction);
  }
  return nullptr;
}
std::shared_ptr<MqttClientTestContainerInterface> MqttClientTestContainerFactory::getMqttClient(MqttClientTestType mqttClientTestType,
                                                                                                const MqttBrokerTestContainerConfiguration mqttBrokerConfiguration) {
  static int i = 0;
  MqttClientConnectAction connect_action(mqttBrokerConfiguration,
                                         std::string("MqttClientContainer") + std::to_string(i++),
                                         std::string(""),
                                         20,
                                         true);
  return getMqttClient(mqttClientTestType, connect_action);
}
