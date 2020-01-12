//
// Created by SomeDude on 11.09.2019.
//

#include "MqttSnClientRegistrationAction.h"
MqttSnClientRegistrationAction::MqttSnClientRegistrationAction(const std::string& topicName) : MqttSnClientAction(MqttSnClientActionType::REGISTER), topicName(topicName) {}
