//
// Created by SomeDude on 23.07.2019.
//

#include "MqttClientActionSequence.h"

MqttClientActionSequence::MqttClientActionSequence(const std::shared_ptr<std::vector<std::unique_ptr<MqttClientAction>>> &sequence,
                                                   int32_t exec_count,
                                                   int32_t exec_position)
    : sequence(sequence), exec_count(exec_count), exec_position(exec_position) {}
