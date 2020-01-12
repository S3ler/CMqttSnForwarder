#include <utility>

#include <utility>

//
// Created by SomeDude on 09.09.2019.
//

#include "MqttSnClientPublishAction.h"
MqttSnClientPublishAction::MqttSnClientPublishAction(const MqttSnClientPublishTopicIdType topicIdType, const uint16_t topicId, const int32_t qos, const bool retain, std::vector<uint8_t> payload)
    : MqttSnClientAction(MqttSnClientActionType::PUBLISH), topicIdType(topicIdType), topicId(topicId), qos(qos), retain(retain), payload(std::move(payload)) {}
MqttSnClientPublishAction::MqttSnClientPublishAction(std::string topicName, const int32_t qos, const bool retain, std::vector<uint8_t> payload)
    : MqttSnClientAction(MqttSnClientActionType::PUBLISH),
      topicIdType(MqttSnClientPublishTopicIdType::SHORT_NAME),
      topicName(std::move(topicName)),
      topicId(0),
      qos(qos),
      retain(retain),
      payload(std::move(payload)) {}
