//
// Created by SomeDude on 20.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNGATEWAY_MQTTSNGATEWAYPAHOTESTCONTAINER_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNGATEWAY_MQTTSNGATEWAYPAHOTESTCONTAINER_H_

#include "MqttSnGatewayTestContainerInterface.h"
#include <MQTTSNGateway.h>
#include <MQTTSNGWBrokerRecvTask.h>
#include <MQTTSNGWBrokerSendTask.h>
#include <MQTTSNGWClientRecvTask.h>
#include <MQTTSNGWClientSendTask.h>
#include <MQTTSNGWPacketHandleTask.h>

using namespace MQTTSNGW;

class MqttSnGatewayPahoTestContainer : public MqttSnGatewayTestContainerInterface {
 private:
  Gateway gateway;
  PacketHandleTask  task1;
  ClientRecvTask    task2;
  ClientSendTask    task3;
  BrokerRecvTask    task4;
  BrokerSendTask    task5;

  thread runner;
  atomic_bool running{false};
  atomic_bool stopped{false};

 public:
  MqttSnGatewayPahoTestContainer(const std::string &identifier, const std::string &cmd);
  int32_t initialize() override;
  int32_t start() override;
  void stop() override;
  bool isStopped() override;
  bool isRunning() override;
  void loop();
  bool createGatewayConfFile();
  bool removeGatewayConfFile();
  bool createPredefinedTopicConfFile();
  bool removePredefinedTopicConfFile();

};

#endif //CMQTTSNFORWARDER_TEST_MQTTSNGATEWAY_MQTTSNGATEWAYPAHOTESTCONTAINER_H_
