//
// Created by SomeDude on 21.07.2019.
//

#include "MqttClientTestContainerInterface.h"
#include "../MqttClientMessageTester/MqttTestMessageReceiverInterface.h"
#include <algorithm>

MqttClientTestContainerInterface::MqttClientTestContainerInterface(const MqttClientConnectAction &initial_connect_action)
    : initial_connect_action(initial_connect_action),
      sequence(std::make_shared<std::vector<std::unique_ptr<MqttClientAction>>>()) {
}
void MqttClientTestContainerInterface::SetReceiverInterface(MqttClientTestContainerReceiverInterface *receiver_interface) {
  MqttClientTestContainerInterface::receiver_interface = receiver_interface;
}
void MqttClientTestContainerInterface::ReceivePublish(MqttClientTestContainerPublish &mqtt_publish) {
  if (receiver_interface) {
    receiver_interface->receive(mqtt_publish);
  }
}

bool MqttClientTestContainerInterface::execActionSequence() {
  running_action_sequence = true;

  std::sort(actionSequences.begin(), actionSequences.end(),
            [](const std::shared_ptr<MqttClientActionSequence> &a,
               const std::shared_ptr<MqttClientActionSequence> &b) -> bool {
              return a->exec_position < b->exec_position;
            });
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
      result.push_back(connect());
    } else if (item->action_type == MqttClientActionType::DISCONNECT) {
      result.push_back(disconnect());
    } else if (item->action_type == MqttClientActionType::PUBLISH) {
      result.push_back(publish(static_cast<MqttClientPublishAction &>((*item.get()))));
    } else if (item->action_type == MqttClientActionType::PUBLISHRECEIVEPUBLISH) {
      result.push_back(publishReceivePublish(static_cast<MqttClientPublishReceivePublish &>((*item.get()))));
    } else if (item->action_type == MqttClientActionType::UNSUBSCRIBE) {
      result.push_back(unsubscribe(static_cast<MqttClientUnsubscribeAction &>((*item.get()))));
    } else if (item->action_type == MqttClientActionType::SUBSCRIBE) {
      result.push_back(subscribe(static_cast<MqttClientSubscribeAction &>((*item.get()))));
    } else {
      timespec start_end_ts;
      clock_gettime(CLOCK_REALTIME, &start_end_ts);
      MqttClientActionResult unknown_action(initial_connect_action.clientId,
                                            item->action_type,
                                            MqttClientTestContainerInterface::action_number++,
                                            MqttClientActionResultType::UNKNOWNACTION,
                                            start_end_ts,
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
void MqttClientTestContainerInterface::addAction(std::unique_ptr<MqttClientAction> &action) {
  sequence->push_back(std::move(action));
}
const std::vector<MqttClientActionResult> &MqttClientTestContainerInterface::GetResult() const {
  return result;
}
const bool MqttClientTestContainerInterface::IsActionSequenceExecuted() const {
  return running_action_sequence;
}
bool MqttClientTestContainerInterface::execActionSequence_async() {
  thread = std::thread(&MqttClientTestContainerInterface::execActionSequence, this);
  thread.detach();
  return true;
}

