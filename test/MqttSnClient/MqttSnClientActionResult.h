//
// Created by SomeDude on 24.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTACTIONRESULT_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTACTIONRESULT_H_

#include <stdint.h>
#include <string>
#include <chrono>
#include <ostream>
#include "MqttSnClientActionResultType.h"
#include "MqttSnClientActionType.h"

class MqttSnClientActionResult {
 public:
  const std::string identifier;
  const uint64_t action_number;
  const MqttSnClientActionType action_type;
  const MqttSnClientActionResultType action_result;
  const std::chrono::nanoseconds start;
  const std::chrono::nanoseconds end;

  MqttSnClientActionResult(const std::string &identifier,
                           const uint64_t action_number,
                           const MqttSnClientActionType action_type,
                           const MqttSnClientActionResultType action_result,
                           const std::chrono::nanoseconds &start,
                           const std::chrono::nanoseconds &an_end);
  MqttSnClientActionResult(const std::string &identifier,
                           const uint64_t action_number,
                           const MqttSnClientActionType action_type,
                           const MqttSnClientActionResultType action_result,
                           const timespec &start,
                           const timespec &an_end);
  static std::chrono::nanoseconds timespecToDuration(timespec ts);

  static std::string GetCsvColums();
  //friend std::ostream &operator<<(std::ostream &os, const MqttSnClientActionResult &result);
};

#endif //CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTACTIONRESULT_H_
