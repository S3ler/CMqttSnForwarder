//
// Created by SomeDude on 22.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTPUBLISHACTION_H_
#define CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTPUBLISHACTION_H_

#include <stdint.h>
#include <string>
#include <vector>
#include "MqttClientAction.h"

class MqttClientPublishAction : public MqttClientAction {
 public:
  const std::string topic_name;
  const std::vector<uint8_t> payload;
  const int32_t qos;
  const bool retain;
  MqttClientPublishAction(const std::string &topic_name,
                          const std::vector<uint8_t> &payload,
                          int32_t qos,
                          bool retain);
};

#endif //CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTPUBLISHACTION_H_
