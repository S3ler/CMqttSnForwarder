//
// Created by SomeDude on 21.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTCLIENTMESSAGETESTER_MQTTMESSAGETESTER_H_
#define CMQTTSNFORWARDER_TEST_MQTTCLIENTMESSAGETESTER_MQTTMESSAGETESTER_H_

#include <atomic>

class MqttMessageTester {
 private:
  std::atomic_bool stopped{false};
  std::atomic_bool running{false};
 public:
  bool start();
  void stop();
  bool isRunning();
};

#endif //CMQTTSNFORWARDER_TEST_MQTTCLIENTMESSAGETESTER_MQTTMESSAGETESTER_H_
