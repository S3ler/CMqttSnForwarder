//
// Created by SomeDude on 12.09.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNCLIENTDELAYACTION_H
#define CMQTTSNFORWARDER_MQTTSNCLIENTDELAYACTION_H

#include "MqttSnClientAction.h"
class MqttSnClientDelayAction  : public MqttSnClientAction {
 public:
  const uint64_t delayDuration;
  MqttSnClientDelayAction(const uint64_t delayDuration);
};

#endif  // CMQTTSNFORWARDER_MQTTSNCLIENTDELAYACTION_H
