//
// Created by SomeDude on 22.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTTESTCONTAINERPUBLISH_H_
#define CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTTESTCONTAINERPUBLISH_H_

#include <stdint.h>
#include <string>
#include <vector>

class MqttClientTestContainerPublish {
 public:
  std::vector<uint8_t> data;
  std::string topic;
  int32_t qos;
  MqttClientTestContainerPublish(const std::vector<uint8_t> &data, const std::string &topic, int32_t qos);
};

#endif //CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTTESTCONTAINERPUBLISH_H_
