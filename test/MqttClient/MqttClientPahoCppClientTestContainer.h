//
// Created by SomeDude on 21.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTPAHOCPPCLIENTTESTCONTAINER_H_
#define CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTPAHOCPPCLIENTTESTCONTAINER_H_

#include "MqttClientTestContainerInterface.h"
#include "MqttClientPublishAction.h"
#include "MqttClientSubscribeAction.h"
#include "MqttClientDisconnectAction.h"
#include "MqttClientActionResult.h"
#include <atomic>
#include <thread>
#include <external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/MQTTAsync.h>

class MqttClientPahoCppClientTestContainer : public MqttClientTestContainerInterface {
 private:
  std::string clientId;
  std::string brokerURI;
  MQTTAsync client;
  MQTTAsync_connectOptions conn_opts;

  std::atomic_bool connected{false};

  std::atomic_bool running{false};
  std::atomic_bool stopped{false};
  std::thread thread;

  //std::atomic<MQTTClient_deliveryToken> tk;

  timespec start_ts;
  timespec end_ts;
  std::atomic_bool action_completed{true};
  std::atomic<MqttClientActionResultType> action_result;
  std::atomic<MQTTAsync_token> publish_token;
 public:
  void on_action_finished(MqttClientActionResultType action_result);
  void on_publish_action_finished(MqttClientActionResultType action_result, MQTTAsync_token publish_token);
  void on_receive_publish_action_finished(MqttClientActionResultType action_result, timespec end_ts);
 public:
  MqttClientPahoCppClientTestContainer(const MqttClientConnectAction &initial_connect_action);
  virtual ~MqttClientPahoCppClientTestContainer();

  bool StartBackgroundHandler() override;
  void StopBackgroundHandler() override;
  bool IsBackgroundHandlerRunning() override;

  MqttClientActionResult connect() override;
  MqttClientActionResult connect(MqttClientConnectAction &connect_action) override;
  MqttClientActionResult disconnect() override;
  MqttClientActionResult subscribe(MqttClientSubscribeAction &subscribe_action) override;
  MqttClientActionResult unsubscribe(MqttClientUnsubscribeAction &unsubscribe_action) override;
  MqttClientActionResult publish(MqttClientPublishAction &publish_action) override;
  MqttClientActionResult publishReceivePublish(MqttClientPublishReceivePublish &publish_receive_publish) override;

  //uint64_t rec_subscribe(MqttClientReceiveSubscribeAction receive_subscribe_action, uint64_t timeout_ns);

  void receive_publish(char *topicName, int topicLen, MQTTAsync_message *message);
  void connection_lost(char *cause);
};

#endif //CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTPAHOCPPCLIENTTESTCONTAINER_H_
