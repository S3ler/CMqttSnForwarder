//
// Created by SomeDude on 22.07.2019.
//

#include "MqttClientActionResultType.h"
std::ostream &operator<<(std::ostream &os, const MqttClientActionResultType &obj) {
  if (obj == MqttClientActionResultType::SUCCESS) {
    os << "SUCCESS";
  } else {
    os << "ERROR";
  }
  return os;
}
