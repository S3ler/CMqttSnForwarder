//
// Created by SomeDude on 12.09.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNCLIENTUNSUBSCRIBEACTION_H
#define CMQTTSNFORWARDER_MQTTSNCLIENTUNSUBSCRIBEACTION_H

#include "MqttSnClientAction.h"
#include "MqttSnClientPublishTopicIdType.h"
class MqttSnClientUnsubscribeAction : public MqttSnClientAction {
 public:
  const MqttSnClientPublishTopicIdType topicIdType;
  const std::string topicName;
  const uint16_t topicId;
  MqttSnClientUnsubscribeAction(const MqttSnClientPublishTopicIdType topicIdType, const uint16_t topicId);
  MqttSnClientUnsubscribeAction(const std::string& topicName);
};

#endif  // CMQTTSNFORWARDER_MQTTSNCLIENTUNSUBSCRIBEACTION_H
