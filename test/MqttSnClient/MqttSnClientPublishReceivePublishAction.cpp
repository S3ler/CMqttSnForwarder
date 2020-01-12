//
// Created by SomeDude on 12.09.2019.
//

#include "MqttSnClientPublishReceivePublishAction.h"
MqttSnClientPublishReceivePublishAction::MqttSnClientPublishReceivePublishAction(std::string& topicName, const int32_t qos, const bool retain, const std::vector<uint8_t>& payload)
    : MqttSnClientAction(MqttSnClientActionType::PUBLISH_RECEIVE_PUBLISH), topicIdType(MqttSnClientPublishTopicIdType::SHORT_NAME), topicName(topicName), topicId(0), qos(qos), retain(retain), payload(payload) {}
MqttSnClientPublishReceivePublishAction::MqttSnClientPublishReceivePublishAction(const MqttSnClientPublishTopicIdType topicIdType, const uint16_t topicId, const int32_t qos, const bool retain,
                                                                                 const std::vector<uint8_t>& payload)
    : MqttSnClientAction(MqttSnClientActionType::PUBLISH_RECEIVE_PUBLISH), topicIdType(topicIdType), topicId(topicId), qos(qos), retain(retain), payload(payload) {}
