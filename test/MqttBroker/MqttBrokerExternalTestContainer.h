//
// Created by SomeDude on 21.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTBROKER_MQTTBROKEREXTERNALTESTCONTAINER_H_
#define CMQTTSNFORWARDER_TEST_MQTTBROKER_MQTTBROKEREXTERNALTESTCONTAINER_H_

#include <atomic>
#include "MqttBrokerTestContainerInterface.h"
class MqttBrokerExternalTestContainer : public MqttBrokerTestContainerInterface {
 private:
  std::atomic_bool started{false};
 public:
  MqttBrokerExternalTestContainer();
  void stop_broker() override;
  bool isRunning() override;
  bool start_broker() override;
};

#endif //CMQTTSNFORWARDER_TEST_MQTTBROKER_MQTTBROKEREXTERNALTESTCONTAINER_H_
