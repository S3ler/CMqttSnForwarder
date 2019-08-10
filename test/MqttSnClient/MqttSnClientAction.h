//
// Created by SomeDude on 24.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTACTION_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTACTION_H_

#include "MqttSnClientActionType.h"
class MqttSnClientAction {
 public:
  const MqttSnClientActionType action_type;
  MqttSnClientAction(const MqttSnClientActionType type);
};

#endif //CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTACTION_H_
