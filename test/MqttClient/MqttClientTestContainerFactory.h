//
// Created by SomeDude on 22.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTTESTCONTAINERFACTORY_H_
#define CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTTESTCONTAINERFACTORY_H_

#include <memory>
#include "MqttClientTestType.h"
#include "MqttClientTestContainerInterface.h"
#include "MqttClientTestContainerConfiguration.h"

class MqttClientTestContainerFactory {
 public:
  static std::shared_ptr<MqttClientTestContainerInterface> getMqttClient(
      MqttClientTestType mqttClientTestType,
      const MqttClientTestContainerConfiguration mqttClientConfiguration);
  static std::shared_ptr<MqttClientTestContainerInterface> getMqttClient
      (MqttClientTestType mqttClientTestType,
       const MqttBrokerTestContainerConfiguration &mqttBrokerConfiguration);
};

#endif //CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTTESTCONTAINERFACTORY_H_
