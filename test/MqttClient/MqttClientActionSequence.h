//
// Created by SomeDude on 23.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTACTIONSEQUENCE_H_
#define CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTACTIONSEQUENCE_H_

#include <memory>
#include <vector>
#include "MqttClientAction.h"

class MqttClientActionSequence {
 public:
  std::shared_ptr<std::vector<std::unique_ptr<MqttClientAction>>> sequence;
  int32_t exec_count;
  int32_t exec_position;
  MqttClientActionSequence(const std::shared_ptr<std::vector<std::unique_ptr<MqttClientAction>>> &sequence,
                           int32_t exec_count,
                           int32_t exec_position);
};

#endif //CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTACTIONSEQUENCE_H_
