//
// Created by SomeDude on 13.09.2019.
//

#include "MqttClientDelayAction.h"
MqttClientDelayAction::MqttClientDelayAction(const uint64_t duration) : MqttClientAction(MqttClientActionType::DELAY), duration(duration) {}
