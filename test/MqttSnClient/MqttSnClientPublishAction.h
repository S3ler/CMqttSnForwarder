//
// Created by SomeDude on 09.09.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNCLIENTPUBLISHACTION_H
#define CMQTTSNFORWARDER_MQTTSNCLIENTPUBLISHACTION_H
#include <stdint.h>
#include <vector>
#include "MqttSnClientAction.h"
#include "MqttSnClientPublishTopicIdType.h"

class MqttSnClientPublishAction : public MqttSnClientAction {
 public:
  const MqttSnClientPublishTopicIdType  topicIdType;
  const std::string topicName;
  const uint16_t topicId;
  const int32_t qos;
  const bool retain;
  const std::vector<uint8_t> payload;
  MqttSnClientPublishAction(const MqttSnClientPublishTopicIdType topicIdType, const uint16_t topicId, const int32_t qos, const bool retain, std::vector<uint8_t>  payload);
  MqttSnClientPublishAction(std::string topicName, const int32_t qos, const bool retain, std::vector<uint8_t> payload);

};
#endif  // CMQTTSNFORWARDER_MQTTSNCLIENTPUBLISHACTION_H
