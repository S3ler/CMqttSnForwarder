//
// Created by SomeDude on 23.07.2019.
//

#include "MqttClientPublishReceivePublishAction.h"

MqttClientPublishReceivePublishAction::MqttClientPublishReceivePublishAction(const std::string& topicName, const std::vector<uint8_t>& payload, const int32_t qos, const bool retain)
    : MqttClientPublishAction(MqttClientActionType::PUBLISH_RECEIVE_PUBLISH, topicName, payload, qos, retain) {}
