//
// Created by SomeDude on 21.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTCLIENTMESSAGETESTER_MQTTMESSAGETESTERFACTORY_H_
#define CMQTTSNFORWARDER_TEST_MQTTCLIENTMESSAGETESTER_MQTTMESSAGETESTERFACTORY_H_

#include <memory>
#include "MqttMessageTestType.h"
#include "MqttMessageTester.h"

class MqttMessageTesterFactory {
 public:
  static std::shared_ptr<MqttMessageTester> getMqttMessageTester(MqttMessageTestType mqttMessageTestType);
};

#endif //CMQTTSNFORWARDER_TEST_MQTTCLIENTMESSAGETESTER_MQTTMESSAGETESTERFACTORY_H_
