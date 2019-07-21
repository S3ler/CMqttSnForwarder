//
// Created by SomeDude on 21.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTCLIENTMESSAGETESTER_MQTTTESTMESSAGERECEIVERMOCK_H_
#define CMQTTSNFORWARDER_TEST_MQTTCLIENTMESSAGETESTER_MQTTTESTMESSAGERECEIVERMOCK_H_

#include "MqttTestMessageReceiverInterface.h"
#include "MqttPublish.h"
class MqttTestMessageReceiverMock : public MqttTestMessageReceiverInterface{
 public:
  MOCK_METHOD1(receive, bool(MqttPublish source));
};

#endif //CMQTTSNFORWARDER_TEST_MQTTCLIENTMESSAGETESTER_MQTTTESTMESSAGERECEIVERMOCK_H_
