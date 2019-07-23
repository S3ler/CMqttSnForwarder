//
// Created by SomeDude on 23.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTUNSUBSCRIBEACTION_H_
#define CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTUNSUBSCRIBEACTION_H_

#include <string>
#include "MqttClientAction.h"

class MqttClientUnsubscribeAction : public MqttClientAction {
 public:
  const std::string topic_name;
  MqttClientUnsubscribeAction(const std::string &topic_name);
};

#endif //CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTUNSUBSCRIBEACTION_H_
