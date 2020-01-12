//
// Created by SomeDude on 11.09.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNCLIENTREGISTRATIONACTION_H
#define CMQTTSNFORWARDER_MQTTSNCLIENTREGISTRATIONACTION_H

#include "MqttSnClientAction.h"
class MqttSnClientRegistrationAction : public  MqttSnClientAction {
 public:
  const std::string topicName;

  MqttSnClientRegistrationAction(const std::string& topicName);
};

#endif  // CMQTTSNFORWARDER_MQTTSNCLIENTREGISTRATIONACTION_H
