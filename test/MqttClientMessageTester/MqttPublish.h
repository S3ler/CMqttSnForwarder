//
// Created by SomeDude on 21.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTCLIENTMESSAGETESTER_MQTTPUBLISH_H_
#define CMQTTSNFORWARDER_TEST_MQTTCLIENTMESSAGETESTER_MQTTPUBLISH_H_

#include <string>
#include <vector>

class MqttPublish {
 public:
  std::string topic;
  std::vector<uint8_t> data;

  MqttPublish(const char* topic, const uint8_t * payload, uint16_t payload_length):topic(std::string(topic)){
    for (uint16_t i = 0; i < payload_length; i++) {
      data.push_back(*(payload + i));
    }
  }
};

#endif //CMQTTSNFORWARDER_TEST_MQTTCLIENTMESSAGETESTER_MQTTPUBLISH_H_
