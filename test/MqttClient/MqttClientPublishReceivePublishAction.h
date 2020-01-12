//
// Created by SomeDude on 23.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTPUBLISHRECEIVEPUBLISH_H_
#define CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTPUBLISHRECEIVEPUBLISH_H_

#include "MqttClientAction.h"
#include "MqttClientPublishAction.h"

class MqttClientPublishReceivePublishAction : public MqttClientPublishAction {
 public:
  MqttClientPublishReceivePublishAction(const std::string& topicName, const std::vector<uint8_t>& payload, const int32_t qos, const bool retain);
};

#endif  // CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTPUBLISHRECEIVEPUBLISH_H_
