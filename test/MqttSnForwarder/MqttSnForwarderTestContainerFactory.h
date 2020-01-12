//
// Created by SomeDude on 17.09.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNFORWARDERTESTCONTAINERFACTORY_H
#define CMQTTSNFORWARDER_MQTTSNFORWARDERTESTCONTAINERFACTORY_H

#include <memory>
#include <string>
#include "MqttSnForwarderTestContainerConfiguration.h"
#include "MqttSnForwarderTestContainerInterface.h"
#include "MqttSnForwarderTestType.h"

class MqttSnForwarderTestContainerFactory {
 public:
  static std::shared_ptr<MqttSnForwarderTestContainerInterface> getMqttSnForwarder();
  static std::shared_ptr<MqttSnForwarderTestContainerInterface> getMqttSnForwarder(MqttSnForwarderTestType mqttSnForwarderTestType);
  static std::shared_ptr<MqttSnForwarderTestContainerInterface> getMqttSnForwarder(MqttSnForwarderTestType mqttSnForwarderTestType, const MqttSnForwarderTestContainerConfiguration& configuration);
};

#endif  // CMQTTSNFORWARDER_MQTTSNFORWARDERTESTCONTAINERFACTORY_H
