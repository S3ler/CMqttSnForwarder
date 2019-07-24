//
// Created by SomeDude on 22.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTACTIONRESULT_H_
#define CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTACTIONRESULT_H_

#include <stdint.h>
#include <string>
#include <chrono>
#include <ostream>
#include "MqttClientAction.h"
#include "MqttClientActionResultType.h"

class MqttClientActionResult {
 public:
  const std::string clientId;
  const uint64_t action_number;
  const MqttClientActionType action_type;
  const MqttClientActionResultType action_result;
  const std::chrono::nanoseconds start;
  const std::chrono::nanoseconds end;

  MqttClientActionResult(const std::string &client_id,
                         const uint64_t action_number,
                         const MqttClientActionType action_type,
                         const MqttClientActionResultType action_result,
                         const std::chrono::nanoseconds &start,
                         const std::chrono::nanoseconds &an_end);
  MqttClientActionResult(const std::string client_id,
                         const MqttClientActionType action_type,
                         const uint64_t action_number,
                         const MqttClientActionResultType action_result,
                         const timespec &start,
                         const timespec &an_end);
  static std::chrono::nanoseconds timespecToDuration(timespec ts);

  static std::string GetCsvColumns();
  friend std::ostream &operator<<(std::ostream &os, const MqttClientActionResult &result);
};

#endif //CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTACTIONRESULT_H_
