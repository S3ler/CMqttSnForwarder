//
// Created by SomeDude on 01.08.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTACTIONSEQUENCE_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTACTIONSEQUENCE_H_

#include <stdint.h>
#include <memory>
#include <vector>
#include "MqttSnClientAction.h"

class MqttSnClientActionSequence {
 public:
  std::shared_ptr<std::vector<std::unique_ptr<MqttSnClientAction>>> sequence;
  int32_t exec_count;
  int32_t exec_position;
  MqttSnClientActionSequence(const std::shared_ptr<std::vector<std::unique_ptr<MqttSnClientAction>>> &sequence,
                             int32_t exec_count,
                             int32_t exec_position);
};

#endif //CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTACTIONSEQUENCE_H_
