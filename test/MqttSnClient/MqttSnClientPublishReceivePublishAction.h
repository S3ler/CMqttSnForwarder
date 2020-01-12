//
// Created by SomeDude on 12.09.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNCLIENTPUBLISHRECEIVEPUBLISHACTION_H
#define CMQTTSNFORWARDER_MQTTSNCLIENTPUBLISHRECEIVEPUBLISHACTION_H

#include <vector>
#include "MqttSnClientAction.h"
#include "MqttSnClientPublishTopicIdType.h"
class MqttSnClientPublishReceivePublishAction : public MqttSnClientAction {
 public:
  const MqttSnClientPublishTopicIdType topicIdType;
  const std::string topicName;
  const uint16_t topicId;
  const int32_t qos;
  const bool retain;
  const std::vector<uint8_t> payload;
  MqttSnClientPublishReceivePublishAction(std::string& topicName, const int32_t qos, const bool retain, const std::vector<uint8_t>& payload);
  MqttSnClientPublishReceivePublishAction(const MqttSnClientPublishTopicIdType topicIdType, const uint16_t topicId, const int32_t qos, const bool retain, const std::vector<uint8_t>& payload);
};

#endif  // CMQTTSNFORWARDER_MQTTSNCLIENTPUBLISHRECEIVEPUBLISHACTION_H
