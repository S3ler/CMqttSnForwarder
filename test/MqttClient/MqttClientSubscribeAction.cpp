//
// Created by SomeDude on 22.07.2019.
//

#include "MqttClientSubscribeAction.h"
MqttClientSubscribeAction::MqttClientSubscribeAction(const std::string &topic_name, const int32_t qos)
    : MqttClientAction(MqttClientActionType::SUBSCRIBE), topic_name(topic_name), qos(qos) {}
