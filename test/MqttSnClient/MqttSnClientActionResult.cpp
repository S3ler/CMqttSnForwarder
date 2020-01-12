//
// Created by SomeDude on 24.07.2019.
//

#include "MqttSnClientActionResult.h"

std::chrono::nanoseconds MqttSnClientActionResult::timespecToDuration(timespec ts) {
  auto duration = std::chrono::seconds{ts.tv_sec} + std::chrono::nanoseconds{ts.tv_nsec};
  return std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
}
std::chrono::nanoseconds MqttSnClientActionResult::uin64msToDuration(uint64_t ms) {
  auto duration = std::chrono::milliseconds{ms};
  return std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
}
std::chrono::nanoseconds MqttSnClientActionResult::timespecDifferenceToDuration(timespec start_ts, timespec end_ts) {
  auto duration = std::chrono::seconds{end_ts.tv_sec - start_ts.tv_sec} + std::chrono::nanoseconds{end_ts.tv_nsec - start_ts.tv_nsec};
  return std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
}
MqttSnClientActionResult::MqttSnClientActionResult(const std::string &identifier, const uint64_t action_number, const MqttSnClientActionType action_type,
                                                   const MqttSnClientActionResultType action_result, const std::chrono::nanoseconds &start, const std::chrono::nanoseconds &an_end,
                                                   const std::chrono::nanoseconds duration_ns)
    : identifier(identifier), action_number(action_number), action_type(action_type), action_result(action_result), start(start), end(an_end), duration(duration_ns) {}
MqttSnClientActionResult::MqttSnClientActionResult(const std::string &identifier, const uint64_t action_number, const MqttSnClientActionType action_type,
                                                   const MqttSnClientActionResultType action_result, const timespec &start, const timespec &an_end)
    : identifier(identifier),
      action_number(action_number),
      action_type(action_type),
      action_result(action_result),
      start(MqttSnClientActionResult::timespecToDuration(start)),
      end(MqttSnClientActionResult::timespecToDuration(an_end)),
      duration(MqttSnClientActionResult::timespecDifferenceToDuration(start, an_end)) {}
MqttSnClientActionResult::MqttSnClientActionResult(const std::string &identifier, const uint64_t action_number, const MqttSnClientActionType action_type,
                                                   const MqttSnClientActionResultType action_result, const uint64_t &start_ms, const uint64_t &end_ms)
    : identifier(identifier),
      action_number(action_number),
      action_type(action_type),
      action_result(action_result),
      start(MqttSnClientActionResult::uin64msToDuration(start_ms)),
      end(MqttSnClientActionResult::uin64msToDuration(end_ms)),
      duration(MqttSnClientActionResult::uin64msToDuration(end_ms - start_ms)) {}
MqttSnClientActionResult::MqttSnClientActionResult(const std::string &identifier, const uint64_t action_number, const MqttSnClientActionType action_type,
                                                   const MqttSnClientActionResultType action_result, const uint64_t &start_ms, const uint64_t &end_ms, const uint64_t &duration_ns)
    : identifier(identifier),
      action_number(action_number),
      action_type(action_type),
      action_result(action_result),
      start(MqttSnClientActionResult::uin64msToDuration(start_ms)),
      end(MqttSnClientActionResult::uin64msToDuration(end_ms)),
      duration(duration_ns) {}

std::string MqttSnClientActionResult::GetCsvColumns() { return "identifier,action_number,action_type,action_result,start_ms,end_ms,duration_ns"; }
std::ostream &operator<<(std::ostream &os, const MqttSnClientActionResult &result) {
  // os << result.identifier << "," << result.action_number << "," << result.action_type << "," << result.action_result << "," << result.start.count() << "," << result.end.count() << ","
  //   << result.duration.count();
  os << result.identifier << "," << result.action_number << "," << result.action_type << "," << result.action_result << ","
     << std::chrono::duration_cast<std::chrono::milliseconds>(result.start).count() << "," << std::chrono::duration_cast<std::chrono::milliseconds>(result.end).count() << "," << result.duration.count();
  return os;
}
