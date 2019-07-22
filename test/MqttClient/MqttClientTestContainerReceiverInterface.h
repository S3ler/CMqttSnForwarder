//
// Created by SomeDude on 22.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTTESTCONTAINERRECEIVERINTERFACE_H_
#define CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTTESTCONTAINERRECEIVERINTERFACE_H_

#include "MqttClientTestContainerPublish.h"

class MqttClientTestContainerReceiverInterface {
 public:
  virtual ~MqttClientTestContainerReceiverInterface() {};
  virtual bool receive(MqttClientTestContainerPublish mqtt_publish)=0;
};

#endif //CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTTESTCONTAINERRECEIVERINTERFACE_H_
