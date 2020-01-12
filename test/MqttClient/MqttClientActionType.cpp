//
// Created by SomeDude on 22.07.2019.
//

#include "MqttClientActionType.h"
std::ostream &operator<<(std::ostream &os, const MqttClientActionType &obj) {
  if (obj == MqttClientActionType::CONNECT) {
    os << "CONNECT";
  } else if (obj == MqttClientActionType::CONNECT_WITH_WILL) {
    os << "CONNECT_WITH_WILL";
  } else if (obj == MqttClientActionType::DISCONNECT) {
    os << "DISCONNECT";
  } else if (obj == MqttClientActionType::SUBSCRIBE) {
    os << "SUBSCRIBE";
  } else if (obj == MqttClientActionType::UNSUBSCRIBE) {
    os << "UNSUBSCRIBE";
  } else if (obj == MqttClientActionType::PUBLISH) {
    os << "PUBLISH";
  } else if (obj == MqttClientActionType::PUBLISH_QOS0) {
    os << "PUBLISH_QOS0";
  } else if (obj == MqttClientActionType::PUBLISH_QOS1) {
    os << "PUBLISH_QOS1";
  } else if (obj == MqttClientActionType::PUBLISH_QOS2) {
    os << "PUBLISH_QOS2";
  } else if (obj == MqttClientActionType::PUBLISH_RECEIVE_PUBLISH) {
    os << "PUBLISH_RECEIVE_PUBLISH";
  } else if (obj == MqttClientActionType::PUBLISH_RECEIVE_PUBLISH_QOS0) {
    os << "PUBLISH_RECEIVE_PUBLISH_QOS0";
  } else if (obj == MqttClientActionType::PUBLISH_RECEIVE_PUBLISH_QOS1) {
    os << "PUBLISH_RECEIVE_PUBLISH_QOS1";
  } else if (obj == MqttClientActionType::PUBLISH_RECEIVE_PUBLISH_QOS2) {
    os << "PUBLISH_RECEIVE_PUBLISH_QOS2";
  } else if (obj == MqttClientActionType::DELAY) {
    os << "DELAY";
  } else if (obj == MqttClientActionType::UNKNOWNACTION) {
    os << "UNKNOWNACTION";
  } else {
    os << "UNKNOWNACTION";
  }
  return os;
}
