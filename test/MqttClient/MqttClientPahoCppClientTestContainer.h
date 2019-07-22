//
// Created by SomeDude on 21.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTPAHOCPPCLIENTTESTCONTAINER_H_
#define CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTPAHOCPPCLIENTTESTCONTAINER_H_

#include "MqttClientTestContainerInterface.h"
#include "../../external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/MQTTClient.h"
#include "MqttClientPublishAction.h"
#include "MqttClientSubscribeAction.h"
#include "MqttClientDisconnectAction.h"
#include "MqttClientActionResult.h"
#include <atomic>
#include <thread>

class MqttClientPahoCppClientTestContainer : public MqttClientTestContainerInterface {
 private:
  std::string brokerURI;
  MQTTClient client;
  MQTTClient_connectOptions conn_opts;

  std::atomic_bool running{false};
  std::atomic_bool stopped{false};
  std::thread thread;

  std::atomic<MQTTClient_deliveryToken> tk;

 public:
  MqttClientPahoCppClientTestContainer(const MqttClientConnectAction &initial_connect_action);

  bool StartBackgroundHandler() override;
  void StopBackgroundHandler() override;
  bool IsBackgroundHandlerRunning() override;

  MqttClientActionResult connect() override;
  uint64_t connect(MqttClientConnectAction &connect_action) override;
  MqttClientActionResult disconnect() override;
  uint64_t publish(MqttClientPublishAction &publish_action) override;
  MqttClientActionResult subscribe(MqttClientSubscribeAction &subscribe_action) override;
  virtual ~MqttClientPahoCppClientTestContainer();
  // synchronous methods - return time in ns?
  //uint64_t connect();
  //uint64_t disconnect();
  //uint64_t publish(MqttClientPublishAction publish_action);
  //uint64_t subscribe(MqttClientSubscribeAction subscribe_action);
  //uint64_t rec_subscribe(MqttClientReceiveSubscribeAction receive_subscribe_action, uint64_t timeout_ns);

  void message_delivered(MQTTClient_deliveryToken token);
  void receive_publish(char *topicName, int topicLen, MQTTClient_message *message);
  void connection_lost(char *cause);
};

#endif //CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTPAHOCPPCLIENTTESTCONTAINER_H_
