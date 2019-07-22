//
// Created by SomeDude on 22.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTTESTCONTAINERRECEIVERMOCK_H_
#define CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTTESTCONTAINERRECEIVERMOCK_H_

#include "MqttClientTestContainerReceiverInterface.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

class MqttClientTestContainerReceiverMock : public MqttClientTestContainerReceiverInterface {
 public:
  MOCK_METHOD1(receive, bool(MqttClientTestContainerPublish source));
};

#endif //CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTTESTCONTAINERRECEIVERMOCK_H_
