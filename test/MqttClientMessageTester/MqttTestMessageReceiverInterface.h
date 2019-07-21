//
// Created by SomeDude on 21.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTCLIENTMESSAGETESTER_MQTTTESTMESSAGERECEIVERINTERFACE_H_
#define CMQTTSNFORWARDER_TEST_MQTTCLIENTMESSAGETESTER_MQTTTESTMESSAGERECEIVERINTERFACE_H_

#include "MqttPublish.h"
class MqttTestMessageReceiverInterface {
 public:
  virtual ~MqttTestMessageReceiverInterface() {};
  virtual bool receive(MqttPublish mqtt_publish)=0;
};

#endif //CMQTTSNFORWARDER_TEST_MQTTCLIENTMESSAGETESTER_MQTTTESTMESSAGERECEIVERINTERFACE_H_
