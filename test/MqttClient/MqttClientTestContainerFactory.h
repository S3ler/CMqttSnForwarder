//
// Created by SomeDude on 22.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTTESTCONTAINERFACTORY_H_
#define CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTTESTCONTAINERFACTORY_H_

#include <memory>
#include <test/MqttBroker/MqttBrokerTestContainerInterface.h>
#include "MqttClientTestType.h"
#include "MqttClientTestContainerInterface.h"

class MqttClientTestContainerFactory {
 public:
  static std::shared_ptr<MqttClientTestContainerInterface> getMqttClient(
      MqttClientTestType mqttClientTestType,
      MqttClientConnectAction &mqttClientConnectAction);
  static std::shared_ptr<MqttClientTestContainerInterface> getMqttClient(MqttClientTestType mqttClientTestType,
                                                                  std::shared_ptr<MqttBrokerTestContainerInterface> mqttBroker);
};

#endif //CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTTESTCONTAINERFACTORY_H_
