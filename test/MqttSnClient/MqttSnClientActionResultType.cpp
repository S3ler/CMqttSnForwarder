//
// Created by SomeDude on 24.07.2019.
//

#include "MqttSnClientActionResultType.h"
std::ostream& operator<<(std::ostream& os, const MqttSnClientActionResultType& obj) {
  switch (obj) {
    case MqttSnClientActionResultType::SUCCESS:
      os << "SUCCESS";
      break;
    case MqttSnClientActionResultType::ERROR:
      os << "ERROR";
      break;
    case MqttSnClientActionResultType::TIMEOUT:
      os << "TIMEOUT";
      break;
    case MqttSnClientActionResultType::UNKNOWNACTION:
      os << "UNKNOWNACTION";
      break;
    default:
      os << "UNKNOWN";
      break;
  }
  return os;
}
