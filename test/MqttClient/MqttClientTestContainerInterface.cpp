//
// Created by SomeDude on 21.07.2019.
//

#include "MqttClientTestContainerInterface.h"
#include "../MqttClientMessageTester/MqttTestMessageReceiverInterface.h"

MqttClientTestContainerInterface::MqttClientTestContainerInterface(const MqttClientConnectAction &initial_connect_action)
    : initial_connect_action(initial_connect_action) {}
void MqttClientTestContainerInterface::addAction(MqttClientAction action) {
  return sequence.push_back(action);
}
bool MqttClientTestContainerInterface::executeSequence() {
  for (auto &item : sequence) {
    if (item.action_type == MqttClientActionType::CONNECT) {
      connect(static_cast<MqttClientConnectAction&>(item));
    }else if (item.action_type == MqttClientActionType::DISCONNECT) {
      disconnect();
    }else if (item.action_type == MqttClientActionType::PUBLISH) {
      publish(static_cast<MqttClientPublishAction&>(item));
    }else if (item.action_type == MqttClientActionType::SUBSCRIBE) {
      subscribe(static_cast<MqttClientSubscribeAction&>(item));
    }else if( item.action_type == MqttClientActionType::PUBLISHRECEIVEPUBLISH){
      // TODO write into result: unknown action type
      return false;
    }
  }
  return true;
}
void MqttClientTestContainerInterface::addConnectAction(MqttClientConnectAction connect_action) {
  addAction(connect_action);
}
void MqttClientTestContainerInterface::addDisconnectAction(MqttClientDisconnectAction disconnect_action) {
  addAction(disconnect_action);
}
void MqttClientTestContainerInterface::SetReceiverInterface(MqttClientTestContainerReceiverInterface *receiver_interface) {
  MqttClientTestContainerInterface::receiver_interface = receiver_interface;
}
void MqttClientTestContainerInterface::ReceivePublish(MqttClientTestContainerPublish &mqtt_publish) {
  if (receiver_interface) {
    receiver_interface->receive(mqtt_publish);
  }
}

