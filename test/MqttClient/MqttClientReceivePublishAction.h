//
// Created by SomeDude on 22.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTRECEIVEPUBLISHACTION_H_
#define CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTRECEIVEPUBLISHACTION_H_

#include <stdint.h>
#include <string>
#include <vector>

class MqttClientReceivePublishAction {
 public:
  const std::string subscription_topic_name;
  const std::vector<uint8_t> payload;
  const int32_t subscription_qos;
  // TODO rec timestamp?
  // TODO callback function?
  MqttClientReceivePublishAction(const std::string &subscription_topic_name,
                                 const std::vector<uint8_t> &payload,
                                 const int32_t subscription_qos);
};

#endif //CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTRECEIVEPUBLISHACTION_H_
