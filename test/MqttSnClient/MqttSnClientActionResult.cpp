//
// Created by SomeDude on 24.07.2019.
//

#include "MqttSnClientActionResult.h"

std::chrono::nanoseconds MqttSnClientActionResult::timespecToDuration(timespec ts) {
  auto duration = std::chrono::seconds{ts.tv_sec}
      + std::chrono::nanoseconds{ts.tv_nsec};
  return std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
}
  std::chrono::nanoseconds MqttSnClientActionResult::uin64msToDuration(uint64_t ms) {
    auto duration = std::chrono::milliseconds{ms};
    return std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
}
MqttSnClientActionResult::MqttSnClientActionResult(const std::string &identifier,
                                                   const uint64_t action_number,
                                                   const MqttSnClientActionType action_type,
                                                   const MqttSnClientActionResultType action_result,
                                                   const std::chrono::nanoseconds &start,
                                                   const std::chrono::nanoseconds &an_end)
    : identifier(identifier),
      action_number(action_number),
      action_type(action_type),
      action_result(action_result),
      start(start),
      end(an_end) {}
MqttSnClientActionResult::MqttSnClientActionResult(const std::string &identifier,
                                                   const uint64_t action_number,
                                                   const MqttSnClientActionType action_type,
                                                   const MqttSnClientActionResultType action_result,
                                                   const timespec &start,
                                                   const timespec &an_end)
    : identifier(identifier),
      action_number(action_number),
      action_type(action_type),
      action_result(action_result),
      start(MqttSnClientActionResult::timespecToDuration(start)),
      end(MqttSnClientActionResult::timespecToDuration(an_end)) {}
MqttSnClientActionResult::MqttSnClientActionResult(const std::string &identifier,
                                                   const uint64_t action_number,
                                                   const MqttSnClientActionType action_type,
                                                   const MqttSnClientActionResultType action_result,
                                                   const uint64_t &start_ms,
                                                   const uint64_t &end_ms)
    : identifier(identifier),
      action_number(action_number),
      action_type(action_type),
      action_result(action_result),
      start(MqttSnClientActionResult::uin64msToDuration(start_ms)),
      end(MqttSnClientActionResult::uin64msToDuration(end_ms)) {}

std::string MqttSnClientActionResult::GetCsvColums() {
  return "identifier,action,action_number,action_result,start,end,duration";
}

/*
std::ostream &operator<<(std::ostream &os, const MqttSnClientActionResult &result) {
  os << result.identifier << "," << result.action_number << "," << result.action_type << ","
     << result.action_result << "," << result.start.count() / 1000000L << "," << result.end.count() / 1000000L
     << "," << (result.end.count() - result.start.count());
  return os;
}
*/
