//
// Created by SomeDude on 12.09.2019.
//

#include "MqttSnClientDelayAction.h"
MqttSnClientDelayAction::MqttSnClientDelayAction(const uint64_t delayDuration) : MqttSnClientAction(MqttSnClientActionType::DELAY), delayDuration(delayDuration) {}
