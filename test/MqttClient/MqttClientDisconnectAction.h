//
// Created by SomeDude on 22.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTDISCONNECTACTION_H_
#define CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTDISCONNECTACTION_H_

#include "MqttClientAction.h"

class MqttClientDisconnectAction : public MqttClientAction {
 public:
  MqttClientDisconnectAction(MqttClientActionType action_type);
};

#endif //CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTDISCONNECTACTION_H_
