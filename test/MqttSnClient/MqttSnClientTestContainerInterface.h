//
// Created by SomeDude on 24.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTTESTCONTAINERINTERFACE_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTTESTCONTAINERINTERFACE_H_

#include <stdint.h>
#include <atomic>
#include <thread>
#include "MqttSnClientActionResult.h"
#include "MqttSnClientActionSequence.h"
#include "MqttSnClientAdvertiseConnectAction.h"
#include "MqttSnClientDelayAction.h"
#include "MqttSnClientDirectConnectAction.h"
#include "MqttSnClientPingAction.h"
#include "MqttSnClientPublishAction.h"
#include "MqttSnClientPublishReceivePublishAction.h"
#include "MqttSnClientPublishTopicIdType.h"
#include "MqttSnClientReconnectAction.h"
#include "MqttSnClientRegistrationAction.h"
#include "MqttSnClientSearchGatewayConnectAction.h"
#include "MqttSnClientSleepAction.h"
#include "MqttSnClientSubscribeAction.h"
#include "MqttSnClientTestContainerConfiguration.h"
#include "MqttSnClientTestContainerPublish.h"
#include "MqttSnClientTestContainerReceiverInterface.h"
#include "MqttSnClientUnsubscribeAction.h"
#include "MqttSnClientWakeupAction.h"

class MqttSnClientTestContainerInterface {
 private:
  MqttSnClientTestContainerReceiverInterface *receiver_interface = nullptr;

 protected:
  uint64_t action_number = 0;

 public:
  const MqttSnClientTestContainerConfiguration configuration;
  MqttSnClientTestContainerInterface(MqttSnClientTestContainerConfiguration configuration);
  void SetReceiverInterface(MqttSnClientTestContainerReceiverInterface *receiver_interface);
  void ReceivePublish(MqttSnClientTestContainerPublish &mqtt_sn_publish);

  virtual bool StartBackgroundHandler() = 0;
  virtual void StopBackgroundHandler() = 0;
  virtual bool IsBackgroundHandlerRunning() = 0;

  virtual MqttSnClientActionResult connect(MqttSnClientConnectAction &connect_action) = 0;
  virtual MqttSnClientActionResult connect(MqttSnClientDirectConnectAction &connect_action) = 0;
  virtual MqttSnClientActionResult connect(MqttSnClientAdvertiseConnectAction &connect_action) = 0;
  virtual MqttSnClientActionResult connect(MqttSnClientSearchGatewayConnectAction &connect_action) = 0;
  virtual MqttSnClientActionResult disconnect() = 0;
  virtual MqttSnClientActionResult publish(MqttSnClientPublishAction &publish_action) = 0;

  void addAction(std::unique_ptr<MqttSnClientAction> &action);
  bool execActionSequence();
  bool execActionSequence_async();
  bool IsActionSequenceExecuted() const;
  const std::vector<MqttSnClientActionResult> &GetResult() const;
  std::shared_ptr<MqttSnClientActionSequence> generateActionSequence(int32_t exec_count);

 private:
  std::shared_ptr<std::vector<std::unique_ptr<MqttSnClientAction>>> sequence;
  std::vector<std::shared_ptr<MqttSnClientActionSequence>> actionSequences;
  std::vector<MqttSnClientActionResult> result;
  bool executeSequence(std::shared_ptr<std::vector<std::unique_ptr<MqttSnClientAction>>> &sequence);
  std::atomic_bool running_action_sequence{false};
  std::thread thread;

 public:
  std::unique_ptr<MqttSnClientAction> getConnectAction();
  std::unique_ptr<MqttSnClientAction> getDirectConnectAction();
  std::unique_ptr<MqttSnClientAction> getDirectConnectAction(const std::vector<uint8_t> &brokerAddress, const std::string &clientId, const uint16_t keepAliveInterval, const bool cleanSession);
  std::unique_ptr<MqttSnClientAction> getDirectConnectAction(const std::vector<uint8_t> &brokerAddress, const std::string &clientId, const uint16_t keepAliveInterval, const bool cleanSession,
                                                             const std::string &willTopic, const std::vector<uint8_t> willMessage, const bool willRetain);
  std::unique_ptr<MqttSnClientAction> getAdvertiseConnectAction();
  std::unique_ptr<MqttSnClientAction> getAdvertiseConnectAction(int64_t advertiseWaitTimeout);
  std::unique_ptr<MqttSnClientAction> getSearchGatewayConnectAction();
  std::unique_ptr<MqttSnClientAction> getSearchGatewayConnectAction(int32_t advertiseWaitTimeout, uint8_t searchGatewayRadius);
  std::unique_ptr<MqttSnClientAction> getAutoConnectAction();
  std::unique_ptr<MqttSnClientAction> getReconnectAction();
  std::unique_ptr<MqttSnClientAction> getPingAction();

  std::unique_ptr<MqttSnClientAction> getPublishAction(MqttSnClientPublishTopicIdType topicIdType, int32_t qos, const uint16_t topicId, const bool retain, const std::vector<uint8_t> &payload);
  std::unique_ptr<MqttSnClientAction> getPublishAction(const std::string topicName, int32_t qos, const bool retain, const std::vector<uint8_t> &payload);
  std::unique_ptr<MqttSnClientAction> getPublishReceivePublishAction(std::string topicName, int32_t qos, bool retain, const std::vector<uint8_t> &payload);
  std::unique_ptr<MqttSnClientAction> getPublishReceivePublishAction(MqttSnClientPublishTopicIdType topicIdType, uint16_t topicId, int32_t qos, const bool retain, const std::vector<uint8_t> &payload);

  std::unique_ptr<MqttSnClientAction> getDisconnectAction();

  std::unique_ptr<MqttSnClientAction> getRegistrationAction(const std::string &topicName);

  std::unique_ptr<MqttSnClientAction> getSubscriptionAction(const std::string &topicName, int32_t qos);
  std::unique_ptr<MqttSnClientAction> getSubscriptionAction(MqttSnClientPublishTopicIdType topicIdType, uint16_t topicId, int32_t qos);

  std::unique_ptr<MqttSnClientAction> getUnsubscriptionAction(std::string &topicName);
  std::unique_ptr<MqttSnClientAction> getUnsubscriptionAction(MqttSnClientPublishTopicIdType topicIdType, uint16_t topicId);

  std::unique_ptr<MqttSnClientAction> getSleepAction(uint16_t sleepDuration);
  std::unique_ptr<MqttSnClientAction> getWakeUpAction();

  std::unique_ptr<MqttSnClientAction> getDelayAction(uint64_t delayDuration);

  std::vector<uint8_t> getPayload(uint16_t payloadLength);

  virtual MqttSnClientActionResult reconnect(MqttSnClientReconnectAction &reconnectAction) = 0;
  virtual MqttSnClientActionResult registration(MqttSnClientRegistrationAction &registrationAction) = 0;
  virtual MqttSnClientActionResult subscribe(MqttSnClientSubscribeAction &subscribeAction) = 0;
  virtual MqttSnClientActionResult unsubscribe(MqttSnClientUnsubscribeAction &action) = 0;
  virtual MqttSnClientActionResult publishReceivePublish(MqttSnClientPublishReceivePublishAction &publishReceivePublishAction) = 0;
  virtual MqttSnClientActionResult sleep(MqttSnClientSleepAction &sleepAction) = 0;
  virtual MqttSnClientActionResult wakeup(MqttSnClientWakeupAction &wakeupAction) = 0;
  virtual MqttSnClientActionResult ping(MqttSnClientPingAction &pingAction) = 0;
  virtual MqttSnClientActionResult delay(MqttSnClientDelayAction &delayAction) = 0;
  std::vector<uint8_t> getStringPayload(uint16_t payloadLength);
};

#endif  // CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTTESTCONTAINERINTERFACE_H_
