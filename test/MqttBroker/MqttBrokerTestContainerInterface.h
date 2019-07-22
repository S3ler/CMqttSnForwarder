//
// Created by SomeDude on 18.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTBROKER_MQTTBROKERTESTCONTAINERINTERFACE_H_
#define CMQTTSNFORWARDER_TEST_MQTTBROKER_MQTTBROKERTESTCONTAINERINTERFACE_H_

#include "MqttBrokerTestContainerConfiguration.h"
class MqttBrokerTestContainerInterface {
 public:
  const MqttBrokerTestContainerConfiguration broker_config;
  MqttBrokerTestContainerInterface(const MqttBrokerTestContainerConfiguration &broker_config);
  virtual ~MqttBrokerTestContainerInterface() {  }
  virtual void stop_broker() = 0;
  virtual bool isRunning() = 0;
  virtual bool start_broker() = 0;
};

#endif //CMQTTSNFORWARDER_TEST_MQTTBROKER_MQTTBROKERTESTCONTAINERINTERFACE_H_
