//
// Created by SomeDude on 24.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNTESTCONTAINERFACTORY_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNTESTCONTAINERFACTORY_H_

#include <memory>
#include "MqttSnClientTestContainerInterface.h"
#include "MqttSnClientTestType.h"
#include "MqttSnClientConnectAction.h"

class MqttSnTestContainerFactory {
 public:
  static std::shared_ptr<MqttSnClientTestContainerInterface> getMqttSnClient(
      MqttSnClientTestType mqttSnClientTestType,
      const MqttSnClientConnectAction mqttSnClientConnectAction);
};

#endif //CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNTESTCONTAINERFACTORY_H_
