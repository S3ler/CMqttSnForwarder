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
  std::string gatewayIdentifierString;

  thread runner;
  atomic_bool running{false};
  atomic_bool stopped{false};
 protected:
  std::string identifier;
  std::string cmd;
  vector<char> line_str;
  uint16_t argc = 0;
  const static uint16_t argv_max_len = 1024;
  char *argv[argv_max_len];

 public:
  MqttSnGatewayPahoTestContainer(const MqttSnGatewayTestContainerConfiguration &configuration);
  int32_t initialize() override;
  int32_t start() override;
  void stop() override;
  bool isStopped() override;
  bool isRunning() override;
  void loop();
  bool createGatewayConfFile(const MqttSnGatewayTestContainerConfiguration &cfg);
  bool removeGatewayConfFile();
  bool createPredefinedTopicConfFile(const MqttSnGatewayTestContainerConfiguration &cfg);
  bool removePredefinedTopicConfFile();

  bool createClientConfFile(const MqttSnGatewayTestContainerConfiguration& configuration);
};

#endif //CMQTTSNFORWARDER_TEST_MQTTSNGATEWAY_MQTTSNGATEWAYPAHOTESTCONTAINER_H_
