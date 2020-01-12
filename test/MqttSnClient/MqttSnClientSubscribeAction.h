//
// Created by SomeDude on 12.09.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNCLIENTSUBSCRIBEACTION_H
#define CMQTTSNFORWARDER_MQTTSNCLIENTSUBSCRIBEACTION_H

#include "MqttSnClientAction.h"
#include "MqttSnClientPublishTopicIdType.h"

class MqttSnClientSubscribeAction : public MqttSnClientAction {
 public:
  const MqttSnClientPublishTopicIdType topicIdType;
  const std::string topicName;
  const uint16_t topicId;
  const int32_t qos;
  MqttSnClientSubscribeAction(std::string topicName, int32_t qos);
  MqttSnClientSubscribeAction(MqttSnClientPublishTopicIdType topicIdType, uint16_t topicId, int32_t qos);
};

#endif  // CMQTTSNFORWARDER_MQTTSNCLIENTSUBSCRIBEACTION_H
