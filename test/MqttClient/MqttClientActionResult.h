//
// Created by SomeDude on 22.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTACTIONRESULT_H_
#define CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTACTIONRESULT_H_

#include <string>
#include <chrono>
#include <ostream>
#include "MqttClientAction.h"
#include "MqttClientActionResultType.h"

class MqttClientActionResult {
 public:
  std::string clientId;
  uint64_t action_number;
  MqttClientActionType action_type;
  MqttClientActionResultType action_result;
  const std::chrono::nanoseconds start;
  const std::chrono::nanoseconds end;

  MqttClientActionResult(std::string client_id,
                         MqttClientActionType action_type,
                         uint64_t action_number,
                         MqttClientActionResultType action_result,
                         const std::chrono::nanoseconds &start,
                         const std::chrono::nanoseconds &an_end);
  MqttClientActionResult(std::string client_id,
                         MqttClientActionType action_type,
                         uint64_t action_number,
                         MqttClientActionResultType action_result,
                         const timespec &start,
                         const timespec &an_end);
  static std::chrono::nanoseconds timespecToDuration(timespec ts);

  static std::string GetCsvColumns();
  friend std::ostream &operator<<(std::ostream &os, const MqttClientActionResult &result);
  // TODO to string
};

#endif //CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTACTIONRESULT_H_
