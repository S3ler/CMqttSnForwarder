//
// Created by SomeDude on 12.09.2019.
//

#include "MqttSnClientSubscribeAction.h"
#include "MqttSnClientPublishTopicIdType.h"
MqttSnClientSubscribeAction::MqttSnClientSubscribeAction(std::string topicName, int32_t qos)
    : MqttSnClientAction(MqttSnClientActionType::SUBSCRIBE), topicIdType(MqttSnClientPublishTopicIdType::SHORT_NAME), topicName(topicName), topicId(0), qos(qos) {}
MqttSnClientSubscribeAction::MqttSnClientSubscribeAction(MqttSnClientPublishTopicIdType topicIdType, uint16_t topicId, int32_t qos)
    : MqttSnClientAction(MqttSnClientActionType::SUBSCRIBE), topicIdType(topicIdType), topicId(topicId), qos(qos) {}
