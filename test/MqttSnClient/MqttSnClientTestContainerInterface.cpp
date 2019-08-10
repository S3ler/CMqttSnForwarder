

#include <utility>
#include <algorithm>
#include "MqttSnClientTestContainerInterface.h"
#include "MqttSnClientDirectConnectAction.h"
#include "MqttSnClientTestContainerPublish.h"

MqttSnClientTestContainerInterface::MqttSnClientTestContainerInterface(MqttSnClientTestContainerConfiguration configuration)
    : configuration(std::move(configuration)),
      sequence(std::make_shared<std::vector<std::unique_ptr<MqttSnClientAction>>>()) {}
void MqttSnClientTestContainerInterface::SetReceiverInterface(MqttSnClientTestContainerReceiverInterface *receiver_interface) {
  MqttSnClientTestContainerInterface::receiver_interface = receiver_interface;
}
void MqttSnClientTestContainerInterface::ReceivePublish(MqttSnClientTestContainerPublish &mqtt_sn_publish) {
  if (receiver_interface) {
    receiver_interface->receive(mqtt_sn_publish);
  }
}

bool MqttSnClientTestContainerInterface::executeSequence(std::shared_ptr<std::vector<std::unique_ptr<MqttSnClientAction>>> &sequence) {
  for (auto &item : (*sequence)) {
    if (item->action_type == MqttSnClientActionType::DIRECTCONNECT) {
      result.push_back(connect(static_cast<MqttSnClientDirectConnectAction &>((*item.get()))));
    } else if (item->action_type == MqttSnClientActionType::SEARCHGWCONNECT) {
      result.push_back(connect(static_cast<MqttSnClientSearchGwConnectAction &>((*item.get()))));
    } else if (item->action_type == MqttSnClientActionType::ADVERTISECONNECT) {
      result.push_back(connect(static_cast<MqttSnClientAdvertiseConnectAction &>((*item.get()))));
    } else if (item->action_type == MqttSnClientActionType::DISCONNECT) {
      result.push_back(disconnect());
      /*
      } else if (item->action_type == MqttClientActionType::PUBLISH) {
        result.push_back(publish(static_cast<MqttClientPublishAction &>((*item.get()))));
      } else if (item->action_type == MqttClientActionType::PUBLISHRECEIVEPUBLISH) {
        result.push_back(publishReceivePublish(static_cast<MqttClientPublishReceivePublish &>((*item.get()))));
      } else if (item->action_type == MqttClientActionType::UNSUBSCRIBE) {
        result.push_back(unsubscribe(static_cast<MqttClientUnsubscribeAction &>((*item.get()))));
      } else if (item->action_type == MqttClientActionType::SUBSCRIBE) {
        result.push_back(subscribe(static_cast<MqttClientSubscribeAction &>((*item.get()))));
      */
    } else {
      timespec start_end_ts;
      clock_gettime(CLOCK_REALTIME, &start_end_ts);
      MqttSnClientActionResult unknown_action(configuration.clientId,
                                              MqttSnClientTestContainerInterface::action_number++,
                                              item->action_type,
                                              MqttSnClientActionResultType::UNKNOWNACTION,
                                              start_end_ts,
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
            [](const std::shared_ptr<MqttSnClientActionSequence> &a,
               const std::shared_ptr<MqttSnClientActionSequence> &b) -> bool {
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
bool MqttSnClientTestContainerInterface::execActionSequence_async() {
  thread = std::thread(&MqttSnClientTestContainerInterface::execActionSequence, this);
  thread.detach();
  return true;
}
void MqttSnClientTestContainerInterface::addAction(std::unique_ptr<MqttSnClientAction> &action) {
  sequence->push_back(std::move(action));
}
const bool MqttSnClientTestContainerInterface::IsActionSequenceExecuted() const {
  return running_action_sequence;
}
const std::vector<MqttSnClientActionResult> &MqttSnClientTestContainerInterface::GetResult() const {
  return result;
}
std::shared_ptr<MqttSnClientActionSequence> MqttSnClientTestContainerInterface::generateActionSequence(int32_t exec_count) {
  auto result = std::make_shared<MqttSnClientActionSequence>(std::move(sequence), exec_count, actionSequences.size());
  sequence = std::make_shared<std::vector<std::unique_ptr<MqttSnClientAction>>>();
  actionSequences.push_back(result);
  return result;
}
