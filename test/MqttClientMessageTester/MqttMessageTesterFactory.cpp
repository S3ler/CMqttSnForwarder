//
// Created by SomeDude on 21.07.2019.
//

#include "MqttMessageTesterFactory.h"

std::shared_ptr<MqttMessageTester> MqttMessageTesterFactory::getMqttMessageTester(MqttMessageTestType mqttMessageTestType) {
  if (mqttMessageTestType == MqttMessageTestType::PAHOC) {
    return std::make_shared<MqttMessageTester>();
  }
  return nullptr;
}
