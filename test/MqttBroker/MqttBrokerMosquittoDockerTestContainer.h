//
// Created by SomeDude on 18.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTBROKER_MQTTBROKERMOSQUITTODOCKERTESTCONTAINER_H_
#define CMQTTSNFORWARDER_TEST_MQTTBROKER_MQTTBROKERMOSQUITTODOCKERTESTCONTAINER_H_

#include "MqttBrokerTestContainerInterface.h"
class MqttBrokerMosquittoDockerTestContainer : public MqttBrokerTestContainerInterface {
 public:
  MqttBrokerMosquittoDockerTestContainer();
  void stop_broker() override;
  bool isRunning() override ;
  bool start_broker() override;
};

#endif //CMQTTSNFORWARDER_TEST_MQTTBROKER_MQTTBROKERMOSQUITTODOCKERTESTCONTAINER_H_
