//
// Created by SomeDude on 22.07.2019.
//

#include "MqttClientPublishAction.h"
MqttClientPublishAction::MqttClientPublishAction(const std::string &topic_name,
                                                 const std::vector<uint8_t> &payload,
                                                 int32_t qos)
    : MqttClientAction(MqttClientActionType::PUBLISH), topic_name(topic_name), payload(payload), qos(qos) {}
