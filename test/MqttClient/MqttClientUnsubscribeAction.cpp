//
// Created by SomeDude on 23.07.2019.
//

#include "MqttClientUnsubscribeAction.h"
MqttClientUnsubscribeAction::MqttClientUnsubscribeAction(const std::string &topic_name)
    : MqttClientAction(MqttClientActionType::UNSUBSCRIBE), topic_name(topic_name) {}
