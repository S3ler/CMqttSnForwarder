//
// Created by SomeDude on 13.09.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTCLIENTDELAYACTION_H
#define CMQTTSNFORWARDER_MQTTCLIENTDELAYACTION_H

#include "MqttClientAction.h"
class MqttClientDelayAction : public MqttClientAction{
 public:
  const uint64_t duration;
  MqttClientDelayAction(const uint64_t duration);
};

#endif  // CMQTTSNFORWARDER_MQTTCLIENTDELAYACTION_H
