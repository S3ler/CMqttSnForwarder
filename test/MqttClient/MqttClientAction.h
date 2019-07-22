//
// Created by SomeDude on 22.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTACTION_H_
#define CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTACTION_H_

#include "MqttClientActionType.h"
class MqttClientAction {
 public:
  const MqttClientActionType action_type;
  MqttClientAction(MqttClientActionType action_type);
};

#endif //CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTACTION_H_
