//
// Created by SomeDude on 22.07.2019.
//

#include "MqttClientPublishAction.h"
MqttClientPublishAction::MqttClientPublishAction(const std::string &topicName,
                                                 const std::vector<uint8_t> &payload,
                                                 int32_t qos,
                                                 bool retain)
    : MqttClientAction(MqttClientActionType::PUBLISH),
      topicName(topicName),
      payload(payload),
      qos(qos),
      retain(retain) {}
MqttClientPublishAction::MqttClientPublishAction(MqttClientActionType actionType, const std::string &topicName, const std::vector<uint8_t> &payload, const int32_t qos, const bool retain)
    : MqttClientAction(actionType), topicName(topicName), payload(payload), qos(qos), retain(retain) {}
