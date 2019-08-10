//
// Created by SomeDude on 31.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTTESTCONTAINERFACTORY_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTTESTCONTAINERFACTORY_H_

#include <memory>
#include "MqttSnClientTestContainerInterface.h"
#include "MqttSnClientTestContainerConfiguration.h"
#include "MqttSnClientTestType.h"

class MqttSnClientTestContainerFactory {
 public:
  static std::shared_ptr<MqttSnClientTestContainerInterface> getMqttSnClient(MqttSnClientTestType type);
  static std::shared_ptr<MqttSnClientTestContainerInterface> getMqttSnClient(MqttSnClientTestType type,
                                                                             const MqttSnClientTestContainerConfiguration configuration);
};

#endif //CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTTESTCONTAINERFACTORY_H_
