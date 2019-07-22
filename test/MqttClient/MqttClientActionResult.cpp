//
// Created by SomeDude on 22.07.2019.
//

#include "MqttClientActionResult.h"
std::chrono::nanoseconds MqttClientActionResult::timespecToDuration(timespec ts) {
  auto duration = std::chrono::seconds{ts.tv_sec}
      + std::chrono::nanoseconds{ts.tv_nsec};
  return std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
}

MqttClientActionResult::MqttClientActionResult(std::string client_id,
                                               MqttClientActionType action_type,
                                               uint64_t action_number,
                                               MqttClientActionResultType action_result,
                                               const std::chrono::nanoseconds &start,
                                               const std::chrono::nanoseconds &an_end)
    : clientId(client_id),
      action_type(action_type),
      action_number(action_number),
      action_result(action_result),
      start(start),
      end(an_end) {}
MqttClientActionResult::MqttClientActionResult(std::string client_id,
                                               MqttClientActionType action_type,
                                               uint64_t action_number,
                                               MqttClientActionResultType action_result,
                                               const timespec &start,
                                               const timespec &an_end)
    : clientId(client_id),
      action_type(action_type),
      action_number(action_number),
      action_result(action_result),
      start(MqttClientActionResult::timespecToDuration(start)),
      end(MqttClientActionResult::timespecToDuration(an_end)) {}

std::string MqttClientActionResult::GetCsvColumns() {
  return "clientId,action,action_number,action_result,start,end,duration";
}
std::ostream &operator<<(std::ostream &os, const MqttClientActionResult &result) {
  os << result.clientId << "," << result.action_number << "," << result.action_type << ","
     << result.action_result << "," << result.start.count() / 1000000L << "," << result.end.count() / 1000000L
     << "," << (result.end.count() - result.start.count());
  return os;
}

