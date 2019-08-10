//
// Created by SomeDude on 01.08.2019.
//

#include "MqttSnClientActionSequence.h"
MqttSnClientActionSequence::MqttSnClientActionSequence(const std::shared_ptr<std::vector<std::unique_ptr<
    MqttSnClientAction>>> &sequence, int32_t exec_count, int32_t exec_position)
    : sequence(sequence), exec_count(exec_count), exec_position(exec_position) {}
