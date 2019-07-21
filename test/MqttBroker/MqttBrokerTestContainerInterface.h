//
// Created by SomeDude on 18.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTBROKER_MQTTBROKERTESTCONTAINERINTERFACE_H_
#define CMQTTSNFORWARDER_TEST_MQTTBROKER_MQTTBROKERTESTCONTAINERINTERFACE_H_

class MqttBrokerTestContainerInterface {
 public:
  virtual ~MqttBrokerTestContainerInterface() {  }
  virtual void stop_broker() = 0;
  virtual bool isRunning() = 0;
  virtual bool start_broker() = 0;
};

#endif //CMQTTSNFORWARDER_TEST_MQTTBROKER_MQTTBROKERTESTCONTAINERINTERFACE_H_
