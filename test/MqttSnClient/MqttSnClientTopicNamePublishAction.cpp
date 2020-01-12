//
// Created by SomeDude on 12.09.2019.
//

#include "MqttSnClientTopicNamePublishAction.h"
MqttSnClientTopicNamePublishAction::MqttSnClientTopicNamePublishAction(const MqttSnClientPublishTopicIdType topicIdType, const int32_t qos, const std::string& topicName, const bool retain,
                                                                       const std::vector<uint8_t>& payload)
    : MqttSnClientAction(MqttSnClientActionType::PUBLISH), topicIdType(topicIdType), qos(qos), topicName(topicName), retain(retain), payload(payload) {}
