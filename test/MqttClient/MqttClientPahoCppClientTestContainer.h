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
#include "MqttClientTestContainerConfiguration.h"
#include <atomic>
#include <thread>
#include <external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/MQTTAsync.h>
#include <mutex>
#include <condition_variable>

class MqttClientPahoCppClientTestContainer : public MqttClientTestContainerInterface {
 private:
  std::string clientId;
  std::string brokerURI;
  MQTTAsync client;

  std::atomic_bool connected{false};

  std::atomic_bool running{false};
  std::atomic_bool stopped{false};

  timespec start_ts;
  timespec end_ts;
  std::atomic_bool action_completed{true};
  std::mutex m;
  std::condition_variable cv;
  std::atomic<MqttClientActionResultType> action_result;
  std::atomic<MQTTAsync_token> publish_token;
 public:
  void on_action_finished(MqttClientActionResultType action_result);
  void on_publish_action_finished(MqttClientActionResultType action_result, MQTTAsync_token publish_token);
  void on_receive_publish_action_finished(MqttClientActionResultType action_result, timespec end_ts);
  void receive_publish(char *topicName, int topicLen, MQTTAsync_message *message);
  void connection_lost(char *cause);
 public:
  MqttClientPahoCppClientTestContainer(const MqttClientTestContainerConfiguration &configuration);

  bool StartBackgroundHandler() override;
  void StopBackgroundHandler() override;
  bool IsBackgroundHandlerRunning() override;

  MqttClientActionResult connect(MqttClientConnectAction &connectAction) override;
  MqttClientActionResult disconnect(MqttClientDisconnectAction &disconnectAction) override;
  MqttClientActionResult subscribe(MqttClientSubscribeAction &subscribe_action) override;
  MqttClientActionResult unsubscribe(MqttClientUnsubscribeAction &unsubscribe_action) override;
  MqttClientActionResult publish(MqttClientPublishAction &publish_action) override;
  MqttClientActionResult publishReceivePublish(MqttClientPublishReceivePublishAction &publish_receive_publish) override;
  MqttClientActionResult delay(MqttClientDelayAction &delayAction) override;


};

#endif //CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTPAHOCPPCLIENTTESTCONTAINER_H_
