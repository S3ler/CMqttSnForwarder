//
// Created by SomeDude on 21.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTBROKER_MQTTBROKERTESTCONTAINERFACTORY_H_
#define CMQTTSNFORWARDER_TEST_MQTTBROKER_MQTTBROKERTESTCONTAINERFACTORY_H_

#include <memory>
#include "MqttBrokerTestContainerInterface.h"
#include "MqttBrokerTestType.h"

class MqttBrokerTestContainerFactory {
 public:
  static std::shared_ptr<MqttBrokerTestContainerInterface> getMqttBroker();
  static std::shared_ptr<MqttBrokerTestContainerInterface> getMqttBroker(MqttBrokerTestType mqttBrokerTestType);
  static std::shared_ptr<MqttBrokerTestContainerInterface> getMqttBroker(MqttBrokerTestType mqttBrokerTestType, MqttBrokerTestContainerConfiguration mqttBrokerConfiguration);
};

#endif  // CMQTTSNFORWARDER_TEST_MQTTBROKER_MQTTBROKERTESTCONTAINERFACTORY_H_
