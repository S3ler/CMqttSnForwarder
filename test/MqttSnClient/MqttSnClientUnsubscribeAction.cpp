//
// Created by SomeDude on 12.09.2019.
//

#include "MqttSnClientUnsubscribeAction.h"
MqttSnClientUnsubscribeAction::MqttSnClientUnsubscribeAction(const MqttSnClientPublishTopicIdType topicIdType, const uint16_t topicId)
    : MqttSnClientAction(MqttSnClientActionType::UNSUBSCRIBE), topicIdType(topicIdType), topicId(topicId) {}
MqttSnClientUnsubscribeAction::MqttSnClientUnsubscribeAction(const std::string& topicName)
    : MqttSnClientAction(MqttSnClientActionType::UNSUBSCRIBE), topicIdType(MqttSnClientPublishTopicIdType::SHORT_NAME), topicName(topicName),topicId(0) {}
