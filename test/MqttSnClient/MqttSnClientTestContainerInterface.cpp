

#include "MqttSnClientTestContainerInterface.h"
#include <gateway_old/abstraction/Arduino.h>
#include <gateway_old/paho/pahomqttembeddedc/MQTTClient/src/MQTTLogging.h>
#include <algorithm>
#include <numeric>
#include <utility>
#include "MqttSnClientAutoConnectAction.h"
#include "MqttSnClientDelayAction.h"
#include "MqttSnClientDirectConnectAction.h"
#include "MqttSnClientDisconnectAction.h"
#include "MqttSnClientPingAction.h"
#include "MqttSnClientPublishReceivePublishAction.h"
#include "MqttSnClientReconnectAction.h"
#include "MqttSnClientRegistrationAction.h"
#include "MqttSnClientSleepAction.h"
#include "MqttSnClientSubscribeAction.h"
#include "MqttSnClientTestContainerPublish.h"
#include "MqttSnClientTopicNamePublishAction.h"
#include "MqttSnClientUnsubscribeAction.h"
#include "MqttSnClientWakeupAction.h"

MqttSnClientTestContainerInterface::MqttSnClientTestContainerInterface(MqttSnClientTestContainerConfiguration configuration)
    : configuration(std::move(configuration)), sequence(std::make_shared<std::vector<std::unique_ptr<MqttSnClientAction>>>()) {}

bool MqttSnClientTestContainerInterface::executeSequence(std::shared_ptr<std::vector<std::unique_ptr<MqttSnClientAction>>> &sequence) {
  for (auto &item : (*sequence)) {
    if (item->action_type == MqttSnClientActionType::CONNECT) {
      result.push_back(connect(static_cast<MqttSnClientConnectAction &>((*item.get()))));
    } else if (item->action_type == MqttSnClientActionType::AUTO_CONNECT) {
      result.push_back(connect(static_cast<MqttSnClientConnectAction &>((*item.get()))));
    } else if (item->action_type == MqttSnClientActionType::DIRECT_CONNECT) {
      result.push_back(connect(static_cast<MqttSnClientDirectConnectAction &>((*item.get()))));
    } else if (item->action_type == MqttSnClientActionType::SEARCHGW_CONNECT) {
      result.push_back(connect(static_cast<MqttSnClientSearchGatewayConnectAction &>((*item.get()))));
    } else if (item->action_type == MqttSnClientActionType::ADVERTISE_CONNECT) {
      result.push_back(connect(static_cast<MqttSnClientAdvertiseConnectAction &>((*item.get()))));
    } else if (item->action_type == MqttSnClientActionType::RECONNECT) {
      result.push_back(reconnect(static_cast<MqttSnClientReconnectAction &>((*item.get()))));
    } else if (item->action_type == MqttSnClientActionType::DISCONNECT) {
      result.push_back(disconnect());
    } else if (item->action_type == MqttSnClientActionType::REGISTER) {
      result.push_back(registration(static_cast<MqttSnClientRegistrationAction &>((*item.get()))));
    } else if (item->action_type == MqttSnClientActionType::SUBSCRIBE) {
      result.push_back(subscribe(static_cast<MqttSnClientSubscribeAction &>((*item.get()))));
    } else if (item->action_type == MqttSnClientActionType::UNSUBSCRIBE) {
      result.push_back(unsubscribe(static_cast<MqttSnClientUnsubscribeAction &>((*item.get()))));
    } else if (item->action_type == MqttSnClientActionType::PUBLISH) {
      result.push_back(publish(static_cast<MqttSnClientPublishAction &>((*item.get()))));
    } else if (item->action_type == MqttSnClientActionType::PUBLISH_RECEIVE_PUBLISH) {
      result.push_back(publishReceivePublish(static_cast<MqttSnClientPublishReceivePublishAction &>((*item.get()))));
    } else if (item->action_type == MqttSnClientActionType::SLEEP) {
      result.push_back(sleep(static_cast<MqttSnClientSleepAction &>((*item.get()))));
    } else if (item->action_type == MqttSnClientActionType::WAKEUP) {
      result.push_back(wakeup(static_cast<MqttSnClientWakeupAction &>((*item.get()))));
    } else if (item->action_type == MqttSnClientActionType::PING) {
      result.push_back(ping(static_cast<MqttSnClientPingAction &>((*item.get()))));
    } else if (item->action_type == MqttSnClientActionType::DELAY) {
      result.push_back(this->delay(static_cast<MqttSnClientDelayAction &>((*item.get()))));
    } else if (item->action_type == MqttSnClientActionType::UNKNOWNACTION) {
      timespec start_end_ts;
      clock_gettime(CLOCK_REALTIME, &start_end_ts);
      MqttSnClientActionResult unknown_action(configuration.clientId, MqttSnClientTestContainerInterface::action_number++, item->action_type, MqttSnClientActionResultType::UNKNOWNACTION, start_end_ts,
                                              start_end_ts);
      result.push_back(unknown_action);
      // return false;
    } else {
      timespec start_end_ts;
      clock_gettime(CLOCK_REALTIME, &start_end_ts);
      MqttSnClientActionResult unknown_action(configuration.clientId, MqttSnClientTestContainerInterface::action_number++, item->action_type, MqttSnClientActionResultType::UNKNOWNACTION, start_end_ts,
                                              start_end_ts);
      result.push_back(unknown_action);
      return false;
    }
  }
  return true;
}
bool MqttSnClientTestContainerInterface::execActionSequence() {
  running_action_sequence = true;

  std::sort(actionSequences.begin(), actionSequences.end(),
            [](const std::shared_ptr<MqttSnClientActionSequence> &a, const std::shared_ptr<MqttSnClientActionSequence> &b) -> bool { return a->exec_position < b->exec_position; });
  for (auto &action_sequence : actionSequences) {
    for (int32_t i = 0; i < action_sequence->exec_count; i++) {
      if (!executeSequence(action_sequence->sequence)) {
        running_action_sequence = false;
        return false;
      }
    }
  }
  running_action_sequence = false;
  return true;
}
bool MqttSnClientTestContainerInterface::execActionSequence_async() {
  thread = std::thread(&MqttSnClientTestContainerInterface::execActionSequence, this);
  thread.detach();
  return true;
}
void MqttSnClientTestContainerInterface::addAction(std::unique_ptr<MqttSnClientAction> &action) { sequence->push_back(std::move(action)); }
bool MqttSnClientTestContainerInterface::IsActionSequenceExecuted() const { return running_action_sequence; }
const std::vector<MqttSnClientActionResult> &MqttSnClientTestContainerInterface::GetResult() const { return result; }
std::shared_ptr<MqttSnClientActionSequence> MqttSnClientTestContainerInterface::generateActionSequence(int32_t exec_count) {
  auto result = std::make_shared<MqttSnClientActionSequence>(std::move(sequence), exec_count, actionSequences.size());
  sequence = std::make_shared<std::vector<std::unique_ptr<MqttSnClientAction>>>();
  actionSequences.push_back(result);
  return result;
}
std::unique_ptr<MqttSnClientAction> MqttSnClientTestContainerInterface::getDisconnectAction() { return std::unique_ptr<MqttSnClientAction>(new MqttSnClientDisconnectAction()); }

void MqttSnClientTestContainerInterface::SetReceiverInterface(MqttSnClientTestContainerReceiverInterface *receiver_interface) {
  MqttSnClientTestContainerInterface::receiver_interface = receiver_interface;
}
void MqttSnClientTestContainerInterface::ReceivePublish(MqttSnClientTestContainerPublish &mqtt_sn_publish) {
  if (receiver_interface) {
    receiver_interface->receive(mqtt_sn_publish);
  }
}
std::unique_ptr<MqttSnClientAction> MqttSnClientTestContainerInterface::getPublishAction(MqttSnClientPublishTopicIdType topicIdType, int32_t qos, const uint16_t topicId, const bool retain,
                                                                                         const std::vector<uint8_t> &payload) {
  if (qos < -1 || qos > 2) {
    throw std::exception();
  }
  return std::unique_ptr<MqttSnClientAction>(new MqttSnClientPublishAction(topicIdType, topicId, qos, retain, payload));
}
std::unique_ptr<MqttSnClientAction> MqttSnClientTestContainerInterface::getRegistrationAction(const std::string &topicName) {
  return std::unique_ptr<MqttSnClientAction>(new MqttSnClientRegistrationAction(topicName));
}
std::unique_ptr<MqttSnClientAction> MqttSnClientTestContainerInterface::getPublishAction(const std::string topicName, int32_t qos, const bool retain, const std::vector<uint8_t> &payload) {
  if (qos < -1 || qos > 2) {
    throw std::exception();
  }
  return std::unique_ptr<MqttSnClientAction>(new MqttSnClientPublishAction(topicName, qos, retain, payload));
}
std::unique_ptr<MqttSnClientAction> MqttSnClientTestContainerInterface::getConnectAction() {
  return std::unique_ptr<MqttSnClientAction>(new MqttSnClientConnectAction(configuration.clientId, configuration.keepAliveInterval, configuration.cleanSession));
}
std::unique_ptr<MqttSnClientAction> MqttSnClientTestContainerInterface::getDirectConnectAction() {
  return std::unique_ptr<MqttSnClientAction>(
      new MqttSnClientDirectConnectAction(configuration.brokerAddress, configuration.clientId, configuration.clientPassword, configuration.keepAliveInterval, configuration.cleanSession));
}
std::unique_ptr<MqttSnClientAction> MqttSnClientTestContainerInterface::getDirectConnectAction(const std::vector<uint8_t> &brokerAddress, const std::string &clientId, const uint16_t keepAliveInterval,
                                                                                               const bool cleanSession) {
  return std::unique_ptr<MqttSnClientAction>(new MqttSnClientDirectConnectAction(brokerAddress, clientId, configuration.clientPassword, keepAliveInterval, cleanSession));
}
std::unique_ptr<MqttSnClientAction> MqttSnClientTestContainerInterface::getDirectConnectAction(const std::vector<uint8_t> &brokerAddress, const std::string &clientId, const uint16_t keepAliveInterval,
                                                                                               const bool cleanSession, const std::string &willTopic, const std::vector<uint8_t> willMessage,
                                                                                               const bool willRetain) {
  return std::unique_ptr<MqttSnClientAction>(
      new MqttSnClientDirectConnectAction(brokerAddress, clientId, configuration.clientPassword, keepAliveInterval, cleanSession, willTopic, willMessage, willRetain));
}
std::unique_ptr<MqttSnClientAction> MqttSnClientTestContainerInterface::getAdvertiseConnectAction(int64_t advertiseWaitTimeout) {
  return std::unique_ptr<MqttSnClientAction>(
      new MqttSnClientAdvertiseConnectAction(advertiseWaitTimeout, configuration.clientId, configuration.clientPassword, configuration.keepAliveInterval, configuration.cleanSession));
}
std::unique_ptr<MqttSnClientAction> MqttSnClientTestContainerInterface::getAdvertiseConnectAction() {
  return std::unique_ptr<MqttSnClientAction>(
      new MqttSnClientAdvertiseConnectAction(configuration.advertise_wait_timeout, configuration.clientId, configuration.clientPassword, configuration.keepAliveInterval, configuration.cleanSession));
}
std::unique_ptr<MqttSnClientAction> MqttSnClientTestContainerInterface::getSearchGatewayConnectAction(int32_t searchGatewayWaitTimeout, uint8_t searchGatewayRadius) {
  return std::unique_ptr<MqttSnClientAction>(new MqttSnClientSearchGatewayConnectAction(searchGatewayWaitTimeout, searchGatewayRadius, configuration.clientId, configuration.clientPassword,
                                                                                        configuration.keepAliveInterval, configuration.cleanSession));
}
std::unique_ptr<MqttSnClientAction> MqttSnClientTestContainerInterface::getSearchGatewayConnectAction() {
  return std::unique_ptr<MqttSnClientAction>(new MqttSnClientSearchGatewayConnectAction(configuration.search_gateway_wait_timeout, configuration.search_gateway_radius, configuration.clientId,
                                                                                        configuration.clientPassword, configuration.keepAliveInterval, configuration.cleanSession));
}
std::unique_ptr<MqttSnClientAction> MqttSnClientTestContainerInterface::getAutoConnectAction() {
  return std::unique_ptr<MqttSnClientAction>(new MqttSnClientAutoConnectAction(configuration.clientId, configuration.clientPassword, configuration.keepAliveInterval, configuration.cleanSession));
}
std::unique_ptr<MqttSnClientAction> MqttSnClientTestContainerInterface::getReconnectAction() { return std::unique_ptr<MqttSnClientAction>(new MqttSnClientReconnectAction()); }
std::unique_ptr<MqttSnClientAction> MqttSnClientTestContainerInterface::getPingAction() { return std::unique_ptr<MqttSnClientAction>(new MqttSnClientPingAction()); }
std::unique_ptr<MqttSnClientAction> MqttSnClientTestContainerInterface::getSubscriptionAction(const std::string &topicName, int32_t qos) {
  return std::unique_ptr<MqttSnClientAction>(new MqttSnClientSubscribeAction(topicName, qos));
}
std::unique_ptr<MqttSnClientAction> MqttSnClientTestContainerInterface::getSubscriptionAction(MqttSnClientPublishTopicIdType topicIdType, uint16_t topicId, int32_t qos) {
  return std::unique_ptr<MqttSnClientAction>(new MqttSnClientSubscribeAction(topicIdType, topicId, qos));
}
std::unique_ptr<MqttSnClientAction> MqttSnClientTestContainerInterface::getUnsubscriptionAction(std::string &topicName) {
  return std::unique_ptr<MqttSnClientAction>(new MqttSnClientUnsubscribeAction(topicName));
}
std::unique_ptr<MqttSnClientAction> MqttSnClientTestContainerInterface::getUnsubscriptionAction(MqttSnClientPublishTopicIdType topicIdType, uint16_t topicId) {
  return std::unique_ptr<MqttSnClientAction>(new MqttSnClientUnsubscribeAction(topicIdType, topicId));
}
std::unique_ptr<MqttSnClientAction> MqttSnClientTestContainerInterface::getSleepAction(uint16_t sleepDuration) {
  return std::unique_ptr<MqttSnClientAction>(new MqttSnClientSleepAction(sleepDuration));
}
std::unique_ptr<MqttSnClientAction> MqttSnClientTestContainerInterface::getWakeUpAction() { return std::unique_ptr<MqttSnClientAction>(new MqttSnClientWakeupAction()); }
std::unique_ptr<MqttSnClientAction> MqttSnClientTestContainerInterface::getDelayAction(uint64_t delayDuration) {
  return std::unique_ptr<MqttSnClientAction>(new MqttSnClientDelayAction(delayDuration));
}
std::unique_ptr<MqttSnClientAction> MqttSnClientTestContainerInterface::getPublishReceivePublishAction(std::string topicName, int32_t qos, bool retain, const std::vector<uint8_t> &payload) {
  return std::unique_ptr<MqttSnClientAction>(new MqttSnClientPublishReceivePublishAction(topicName, qos, retain, payload));
}
std::unique_ptr<MqttSnClientAction> MqttSnClientTestContainerInterface::getPublishReceivePublishAction(MqttSnClientPublishTopicIdType topicIdType, uint16_t topicId, int32_t qos, const bool retain,
                                                                                                       const std::vector<uint8_t> &payload) {
  return std::unique_ptr<MqttSnClientAction>(new MqttSnClientPublishReceivePublishAction(topicIdType, topicId, qos, retain, payload));
}
std::vector<uint8_t> MqttSnClientTestContainerInterface::getPayload(uint16_t payloadLength) {
  std::vector<uint8_t> payload(payloadLength);
  for (int64_t i = 0; i < payloadLength; i++) {
    payload[i] = ((i + '1') % 'z');
  }
  return payload;
}
std::vector<uint8_t> MqttSnClientTestContainerInterface::getStringPayload(uint16_t payloadLength) {
  std::vector<uint8_t> payload(payloadLength);
  if (payloadLength > 0) {
    for (int64_t i = 0; i < payloadLength; i++) {
      payload[i] = ((i + '1') % 'z');
    }
  }
  payload[payloadLength - 1] = '\0';
  return payload;
}
