//
// Created by SomeDude on 22.07.2019.
//

#include "MqttClientActionResultType.h"
std::ostream &operator<<(std::ostream &os, const MqttClientActionResultType &obj) {
  if (obj == MqttClientActionResultType::SUCCESS) {
    os << "SUCCESS";
  } else if (obj == MqttClientActionResultType::ERROR) {
    os << "ERROR";
  } else if (obj == MqttClientActionResultType::CONNECTIONLOST) {
    os << "CONNECTIONLOST";
  }
  return os;
}
