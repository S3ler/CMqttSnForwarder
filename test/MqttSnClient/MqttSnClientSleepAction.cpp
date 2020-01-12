//
// Created by SomeDude on 12.09.2019.
//

#include "MqttSnClientSleepAction.h"
MqttSnClientSleepAction::MqttSnClientSleepAction(const uint16_t sleepDuration) : MqttSnClientAction(MqttSnClientActionType::SLEEP), sleepDuration(sleepDuration) {}
