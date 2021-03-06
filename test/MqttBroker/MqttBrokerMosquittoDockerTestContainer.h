//
// Created by SomeDude on 18.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTBROKER_MQTTBROKERMOSQUITTODOCKERTESTCONTAINER_H_
#define CMQTTSNFORWARDER_TEST_MQTTBROKER_MQTTBROKERMOSQUITTODOCKERTESTCONTAINER_H_

#include <atomic>
#include "MqttBrokerTestContainerInterface.h"
class MqttBrokerMosquittoDockerTestContainer : public MqttBrokerTestContainerInterface {
 private:
  std::atomic_bool running;
 public:
  MqttBrokerMosquittoDockerTestContainer(const MqttBrokerTestContainerConfiguration &broker_config);
  void stop_broker() override;
  bool isRunning() override ;
  bool start_broker() override;
};

#endif //CMQTTSNFORWARDER_TEST_MQTTBROKER_MQTTBROKERMOSQUITTODOCKERTESTCONTAINER_H_
