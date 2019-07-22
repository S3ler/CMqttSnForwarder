//
// Created by SomeDude on 22.07.2019.
//

#include "MqttClientActionType.h"
std::ostream &operator<<(std::ostream &os, const MqttClientActionType &obj) {
  if (obj == MqttClientActionType::CONNECT) {
    os << "CONNECT";
  } else if (obj == MqttClientActionType::DISCONNECT) {
    os << "DISCONNECT";
  } else if (obj == MqttClientActionType::PUBLISH) {
    os << "PUBLISH";
  } else if (obj == MqttClientActionType::SUBSCRIBE) {
    os << "SUBSCRIBE";
  } else if (obj == MqttClientActionType::PUBLISHRECEIVEPUBLISH) {
    os << "PUBLISHRECEIVEPUBLISH";
  } else{
    os << "UNKNOWN";
  }
  return os;
}
