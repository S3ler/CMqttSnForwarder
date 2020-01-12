//
// Created by SomeDude on 24.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTACTIONRESULT_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTACTIONRESULT_H_

#include <stdint.h>
#include <chrono>
#include <ostream>
#include <string>
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
  const std::chrono::nanoseconds duration;

  MqttSnClientActionResult(const std::string &identifier, const uint64_t action_number, const MqttSnClientActionType action_type, const MqttSnClientActionResultType action_result,
                           const std::chrono::nanoseconds &start, const std::chrono::nanoseconds &an_end, const std::chrono::nanoseconds duration_ns);
  MqttSnClientActionResult(const std::string &identifier, const uint64_t action_number, const MqttSnClientActionType action_type, const MqttSnClientActionResultType action_result,
                           const timespec &start, const timespec &an_end);
  MqttSnClientActionResult(const std::string &identifier, const uint64_t action_number, const MqttSnClientActionType action_type, const MqttSnClientActionResultType action_result,
                           const uint64_t &start_ms, const uint64_t &end_ms);
  MqttSnClientActionResult(const std::string &identifier, const uint64_t action_number, const MqttSnClientActionType action_type, const MqttSnClientActionResultType action_result,
                           const uint64_t &start_ms, const uint64_t &end_ms, const uint64_t &duration_ns);

  static std::chrono::nanoseconds timespecToDuration(timespec ts);
  static std::chrono::nanoseconds uin64msToDuration(uint64_t ms);
  static std::chrono::nanoseconds timespecDifferenceToDuration(timespec start_ts, timespec end_ts);

  static std::string GetCsvColumns();
  friend std::ostream &operator<<(std::ostream &os, const MqttSnClientActionResult &result);
};

#endif  // CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTACTIONRESULT_H_
