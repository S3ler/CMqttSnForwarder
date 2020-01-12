#include <utility>

//
// Created by SomeDude on 21.07.2019.
//

#include <algorithm>
#include <numeric>
#include "../MqttClientMessageTester/MqttTestMessageReceiverInterface.h"
#include "MqttClientDelayAction.h"
#include "MqttClientTestContainerInterface.h"

MqttClientTestContainerInterface::MqttClientTestContainerInterface(MqttClientTestContainerConfiguration configuration)
    : configuration(std::move(configuration)), sequence(std::make_shared<std::vector<std::unique_ptr<MqttClientAction>>>()) {}
void MqttClientTestContainerInterface::SetReceiverInterface(MqttClientTestContainerReceiverInterface *receiver_interface) { MqttClientTestContainerInterface::receiver_interface = receiver_interface; }
void MqttClientTestContainerInterface::ReceivePublish(MqttClientTestContainerPublish &mqtt_publish) {
  if (receiver_interface) {
    receiver_interface->receive(mqtt_publish);
  }
}
bool MqttClientTestContainerInterface::execActionSequence() {
  running_action_sequence = true;

  std::sort(actionSequences.begin(), actionSequences.end(),
            [](const std::shared_ptr<MqttClientActionSequence> &a, const std::shared_ptr<MqttClientActionSequence> &b) -> bool { return a->exec_position < b->exec_position; });
  if (!executeSequence(sequence)) {
    running_action_sequence = false;
    return false;
  }
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
bool MqttClientTestContainerInterface::executeSequence(std::shared_ptr<std::vector<std::unique_ptr<MqttClientAction>>> &sequence) {
  for (auto &item : (*sequence)) {
    if (item->action_type == MqttClientActionType::CONNECT) {
      result.push_back(connect(static_cast<MqttClientConnectAction &>((*item.get()))));
    } else if (item->action_type == MqttClientActionType::DISCONNECT) {
      result.push_back(disconnect(static_cast<MqttClientDisconnectAction &>((*item.get()))));
    } else if (item->action_type == MqttClientActionType::PUBLISH) {
      result.push_back(publish(static_cast<MqttClientPublishAction &>((*item.get()))));
    } else if (item->action_type == MqttClientActionType::PUBLISH_RECEIVE_PUBLISH) {
      result.push_back(publishReceivePublish(static_cast<MqttClientPublishReceivePublishAction &>((*item.get()))));
    } else if (item->action_type == MqttClientActionType::UNSUBSCRIBE) {
      result.push_back(unsubscribe(static_cast<MqttClientUnsubscribeAction &>((*item.get()))));
    } else if (item->action_type == MqttClientActionType::SUBSCRIBE) {
      result.push_back(subscribe(static_cast<MqttClientSubscribeAction &>((*item.get()))));
    } else if (item->action_type == MqttClientActionType::DELAY) {
      result.push_back(delay(static_cast<MqttClientDelayAction &>((*item.get()))));
    } else {
      timespec start_end_ts;
      clock_gettime(CLOCK_REALTIME, &start_end_ts);
      MqttClientActionResult unknown_action(configuration.clientId, item->action_type, MqttClientTestContainerInterface::action_number++, MqttClientActionResultType::UNKNOWNACTION, start_end_ts,
                                            start_end_ts);
      result.push_back(unknown_action);
      return false;
    }
  }
  return true;
}

std::shared_ptr<MqttClientActionSequence> MqttClientTestContainerInterface::generateActionSequence(int32_t exec_count) {
  auto result = std::make_shared<MqttClientActionSequence>(std::move(sequence), exec_count, actionSequences.size());
  sequence = std::make_shared<std::vector<std::unique_ptr<MqttClientAction>>>();
  actionSequences.push_back(result);
  return result;
}
void MqttClientTestContainerInterface::addAction(std::unique_ptr<MqttClientAction> &action) { sequence->push_back(std::move(action)); }
const std::vector<MqttClientActionResult> &MqttClientTestContainerInterface::GetResult() const { return result; }
bool MqttClientTestContainerInterface::IsActionSequenceExecuted() const { return running_action_sequence; }
bool MqttClientTestContainerInterface::execActionSequence_async() {
  thread = std::thread(&MqttClientTestContainerInterface::execActionSequence, this);
  thread.detach();
  return true;
}
std::vector<uint8_t> MqttClientTestContainerInterface::getPayload(uint16_t payloadLength) {
  std::vector<uint8_t> payload(payloadLength);
  for (int64_t i = 0; i < payloadLength; i++) {
    payload[i] = ((i + '1') % 'z');
  }
  return payload;
}
std::unique_ptr<MqttClientAction> MqttClientTestContainerInterface::getConnectAction() { return std::unique_ptr<MqttClientAction>(new MqttClientConnectAction(configuration)); }
std::unique_ptr<MqttClientAction> MqttClientTestContainerInterface::getDisconnectAction() { return std::unique_ptr<MqttClientAction>(new MqttClientDisconnectAction()); }
std::unique_ptr<MqttClientAction> MqttClientTestContainerInterface::getSubscribeAction(const std::string &topicName, int32_t qos) {
  return std::unique_ptr<MqttClientAction>(new MqttClientSubscribeAction(topicName, qos));
}
std::unique_ptr<MqttClientAction> MqttClientTestContainerInterface::getUnsubscribeAction(const std::string &topicName) {
  return std::unique_ptr<MqttClientAction>(new MqttClientUnsubscribeAction(topicName));
}
std::unique_ptr<MqttClientAction> MqttClientTestContainerInterface::getPublishAction(const std::string &topicName, const std::vector<uint8_t> &payload, int32_t qos, bool retain) {
  return std::unique_ptr<MqttClientAction>(new MqttClientPublishAction(topicName, payload, qos, retain));
}
std::unique_ptr<MqttClientAction> MqttClientTestContainerInterface::getPublishReceivePublishAction(const std::string &topicName, const std::vector<uint8_t> &payload, int32_t qos, bool retain) {
  return std::unique_ptr<MqttClientAction>(new MqttClientPublishReceivePublishAction(topicName, payload, qos, retain));
}
std::unique_ptr<MqttClientAction> MqttClientTestContainerInterface::getDelayAction(uint64_t delayDuration) { return std::unique_ptr<MqttClientAction>(new MqttClientDelayAction(delayDuration)); }

MqttClientActionResult MqttClientTestContainerInterface::disconnect() {
  auto disconnectAction = getDisconnectAction();
  return disconnect(static_cast<MqttClientDisconnectAction &>((*disconnectAction)));
}
MqttClientActionResult MqttClientTestContainerInterface::connect() {
  auto connectAction = getConnectAction();
  return connect(static_cast<MqttClientConnectAction &>((*connectAction)));
}
