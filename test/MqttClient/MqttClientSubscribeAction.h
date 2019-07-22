//
// Created by SomeDude on 22.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTSUBSCRIBEACTION_H_
#define CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTSUBSCRIBEACTION_H_

#include <stdint.h>
#include <string>
#include "MqttClientAction.h"

class MqttClientSubscribeAction : public MqttClientAction {
 public:
  const std::string topic_name;
  const int32_t qos;
  MqttClientSubscribeAction(const std::string &topic_name, const int32_t qos);

};

#endif //CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTSUBSCRIBEACTION_H_
