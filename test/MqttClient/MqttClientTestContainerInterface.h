//
// Created by SomeDude on 21.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTTESTCONTAINERINTERFACE_H_
#define CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTTESTCONTAINERINTERFACE_H_

#include <atomic>
#include <memory>
#include <string>
#include <thread>
#include "MqttClientActionResult.h"
#include "MqttClientActionSequence.h"
#include "MqttClientConnectAction.h"
#include "MqttClientDelayAction.h"
#include "MqttClientDisconnectAction.h"
#include "MqttClientPublishAction.h"
#include "MqttClientPublishReceivePublishAction.h"
#include "MqttClientSubscribeAction.h"
#include "MqttClientTestContainerReceiverInterface.h"
#include "MqttClientUnsubscribeAction.h"

class MqttClientTestContainerInterface {
 protected:
  uint64_t action_number = 0;
  MqttClientTestContainerReceiverInterface *receiver_interface = nullptr;

  // FEATURE stop/abort client by function, all not processed MqttClientActionResult show ABORTED as MqttClientActionResultType
 public:
  const MqttClientTestContainerConfiguration configuration;
  MqttClientTestContainerInterface(MqttClientTestContainerConfiguration configuration);
  void SetReceiverInterface(MqttClientTestContainerReceiverInterface *receiver_interface);
  void ReceivePublish(MqttClientTestContainerPublish &mqtt_publish);

  virtual bool StartBackgroundHandler() = 0;
  virtual void StopBackgroundHandler() = 0;
  virtual bool IsBackgroundHandlerRunning() = 0;

  virtual MqttClientActionResult connect(MqttClientConnectAction &connectAction) = 0;
  virtual MqttClientActionResult disconnect(MqttClientDisconnectAction &disconnectAction) = 0;
  virtual MqttClientActionResult subscribe(MqttClientSubscribeAction &subscribe_action) = 0;
  virtual MqttClientActionResult unsubscribe(MqttClientUnsubscribeAction &unsubscribe_action) = 0;
  virtual MqttClientActionResult publish(MqttClientPublishAction &publish_action) = 0;
  virtual MqttClientActionResult publishReceivePublish(MqttClientPublishReceivePublishAction &publish_receive_publish) = 0;
  virtual MqttClientActionResult delay(MqttClientDelayAction &delayAction) = 0;

  MqttClientActionResult disconnect();
  MqttClientActionResult connect();

  void addAction(std::unique_ptr<MqttClientAction> &action);
  bool execActionSequence();
  bool execActionSequence_async();
  bool IsActionSequenceExecuted() const;
  const std::vector<MqttClientActionResult> &GetResult() const;
  std::shared_ptr<MqttClientActionSequence> generateActionSequence(int32_t exec_count);

  std::unique_ptr<MqttClientAction> getConnectAction();
  std::unique_ptr<MqttClientAction> getDisconnectAction();
  std::unique_ptr<MqttClientAction> getSubscribeAction(const std::string &topicName, int32_t qos);
  std::unique_ptr<MqttClientAction> getUnsubscribeAction(const std::string &topicName);
  std::unique_ptr<MqttClientAction> getPublishAction(const std::string &topicName, const std::vector<uint8_t> &payload, int32_t qos, bool retain);
  std::unique_ptr<MqttClientAction> getPublishReceivePublishAction(const std::string &topicName, const std::vector<uint8_t> &payload, int32_t qos, bool retain);
  std::unique_ptr<MqttClientAction> getDelayAction(uint64_t delayDuration);

 public:
  std::vector<uint8_t> getPayload(uint16_t payloadLength);

 private:
  std::shared_ptr<std::vector<std::unique_ptr<MqttClientAction>>> sequence;
  std::vector<std::shared_ptr<MqttClientActionSequence>> actionSequences;
  std::vector<MqttClientActionResult> result;
  bool executeSequence(std::shared_ptr<std::vector<std::unique_ptr<MqttClientAction>>> &sequence);
  std::atomic_bool running_action_sequence{false};
  std::thread thread;
};

#endif  // CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTTESTCONTAINERINTERFACE_H_
