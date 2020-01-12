//
// Created by SomeDude on 12.09.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNCLIENTSLEEPACTION_H
#define CMQTTSNFORWARDER_MQTTSNCLIENTSLEEPACTION_H

#include "MqttSnClientAction.h"
class MqttSnClientSleepAction : public MqttSnClientAction {
 public:
  const uint16_t sleepDuration;
  MqttSnClientSleepAction(const uint16_t sleepDuration);
};

#endif  // CMQTTSNFORWARDER_MQTTSNCLIENTSLEEPACTION_H
