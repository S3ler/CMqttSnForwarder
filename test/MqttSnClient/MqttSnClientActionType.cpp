//
// Created by SomeDude on 24.07.2019.
//

#include "MqttSnClientActionType.h"
std::ostream& operator<<(std::ostream& os, const MqttSnClientActionType& obj) {
  switch (obj) {
    case MqttSnClientActionType::CONNECT:
      os << "CONNECT";
      break;
    case MqttSnClientActionType::AUTO_CONNECT:
      os << "AUTO_CONNECT";
      break;
    case MqttSnClientActionType::ADVERTISE_CONNECT:
      os << "ADVERTISE_CONNECT";
      break;
    case MqttSnClientActionType::SEARCHGW_CONNECT:
      os << "SEARCHGW_CONNECT";
      break;
    case MqttSnClientActionType::DIRECT_CONNECT:
      os << "DIRECT_CONNECT";
      break;
    case MqttSnClientActionType::DISCONNECT:
      os << "DISCONNECT";
      break;
    case MqttSnClientActionType::REGISTER:
      os << "REGISTER";
      break;

    case MqttSnClientActionType::SUBSCRIBE:
      os << "SUBSCRIBE";
      break;
    case MqttSnClientActionType::PREDEFINED_SUBSCRIBE:
      os << "PREDEFINED_SUBSCRIBE";
      break;
    case MqttSnClientActionType::TOPICNAME_SUBSCRIBE:
      os << "TOPICNAME_SUBSCRIBE";
      break;
    case MqttSnClientActionType::TOPICID_SUBSCRIBE:
      os << "TOPICID_SUBSCRIBE";
      break;

    case MqttSnClientActionType::UNSUBSCRIBE:
      os << "UNSUBSCRIBE";
      break;

    case MqttSnClientActionType::PUBLISH:
      os << "PUBLISH";
      break;
    case MqttSnClientActionType::UNCONNECTED_PREDEFINED_PUBLISH_QOSM1:
      os << "UNCONNECTED_PREDEFINED_PUBLISH_QOSM1";
      break;

    case MqttSnClientActionType::PREDEFINED_PUBLISH_QOSM1:
      os << "PREDEFINED_PUBLISH_QOSM1";
      break;
    case MqttSnClientActionType::PREDEFINED_PUBLISH_QOS0:
      os << "PREDEFINED_PUBLISH_QOS0";
      break;
    case MqttSnClientActionType::PREDEFINED_PUBLISH_QOS1:
      os << "PREDEFINED_PUBLISH_QOS1";
      break;
    case MqttSnClientActionType::PREDEFINED_PUBLISH_QOS2:
      os << "PREDEFINED_PUBLISH_QOS2";
      break;

    case MqttSnClientActionType::TOPICNAME_PUBLISH_QOSM1:
      os << "TOPICNAME_PUBLISH_QOSM1";
      break;
    case MqttSnClientActionType::TOPICNAME_PUBLISH_QOS0:
      os << "TOPICNAME_PUBLISH_QOS0";
      break;
    case MqttSnClientActionType::TOPICNAME_PUBLISH_QOS1:
      os << "TOPICNAME_PUBLISH_QOS1";
      break;
    case MqttSnClientActionType::TOPICNAME_PUBLISH_QOS2:
      os << "TOPICNAME_PUBLISH_QOS2";
      break;

    case MqttSnClientActionType::TOPICID_PUBLISH_QOSM1:
      os << "TOPICID_PUBLISH_QOSM1";
      break;
    case MqttSnClientActionType::TOPICID_PUBLISH_QOS0:
      os << "TOPICID_PUBLISH_QOS0";
      break;
    case MqttSnClientActionType::TOPICID_PUBLISH_QOS1:
      os << "TOPICID_PUBLISH_QOS1";
      break;
    case MqttSnClientActionType::TOPICID_PUBLISH_QOS2:
      os << "TOPICID_PUBLISH_QOS2";
      break;

    case MqttSnClientActionType::PUBLISH_RECEIVE_PUBLISH:
      os << "PUBLISH_RECEIVE_PUBLISH";
      break;

    case MqttSnClientActionType::PUBLISH_RECEIVE_PUBLISH_PREDEFINED_PUBLISH_QOSM1:
      os << "PUBLISH_RECEIVE_PUBLISH_PREDEFINED_PUBLISH_QOSM1";
      break;
    case MqttSnClientActionType::PUBLISH_RECEIVE_PUBLISH_PREDEFINED_PUBLISH_QOS0:
      os << "PUBLISH_RECEIVE_PUBLISH_PREDEFINED_PUBLISH_QOS0";
      break;
    case MqttSnClientActionType::PUBLISH_RECEIVE_PUBLISH_PREDEFINED_PUBLISH_QOS1:
      os << "PUBLISH_RECEIVE_PUBLISH_PREDEFINED_PUBLISH_QOS1";
      break;
    case MqttSnClientActionType::PUBLISH_RECEIVE_PUBLISH_PREDEFINED_PUBLISH_QOS2:
      os << "PUBLISH_RECEIVE_PUBLISH_PREDEFINED_PUBLISH_QOS2";
      break;

    case MqttSnClientActionType::PUBLISH_RECEIVE_PUBLISH_TOPICNAME_PUBLISH_QOSM1:
      os << "PUBLISH_RECEIVE_PUBLISH_TOPICNAME_PUBLISH_QOSM1";
      break;
    case MqttSnClientActionType::PUBLISH_RECEIVE_PUBLISH_TOPICNAME_PUBLISH_QOS0:
      os << "PUBLISH_RECEIVE_PUBLISH_TOPICNAME_PUBLISH_QOS0";
      break;
    case MqttSnClientActionType::PUBLISH_RECEIVE_PUBLISH_TOPICNAME_PUBLISH_QOS1:
      os << "PUBLISH_RECEIVE_PUBLISH_TOPICNAME_PUBLISH_QOS1";
      break;
    case MqttSnClientActionType::PUBLISH_RECEIVE_PUBLISH_TOPICNAME_PUBLISH_QOS2:
      os << "PUBLISH_RECEIVE_PUBLISH_TOPICNAME_PUBLISH_QOS2";
      break;

    case MqttSnClientActionType::PUBLISH_RECEIVE_PUBLISH_TOPICID_PUBLISH_QOSM1:
      os << "PUBLISH_RECEIVE_PUBLISH_TOPICID_PUBLISH_QOSM1";
      break;
    case MqttSnClientActionType::PUBLISH_RECEIVE_PUBLISH_TOPICID_PUBLISH_QOS0:
      os << "PUBLISH_RECEIVE_PUBLISH_TOPICID_PUBLISH_QOS0";
      break;
    case MqttSnClientActionType::PUBLISH_RECEIVE_PUBLISH_TOPICID_PUBLISH_QOS1:
      os << "PUBLISH_RECEIVE_PUBLISH_TOPICID_PUBLISH_QOS1";
      break;
    case MqttSnClientActionType::PUBLISH_RECEIVE_PUBLISH_TOPICID_PUBLISH_QOS2:
      os << "PUBLISH_RECEIVE_PUBLISH_TOPICID_PUBLISH_QOS2";
      break;

    case MqttSnClientActionType::SLEEP:
      os << "SLEEP";
      break;
    case MqttSnClientActionType::WAKEUP:
      os << "WAKEUP";
      break;
    case MqttSnClientActionType::PING:
      os << "PING";
      break;
    case MqttSnClientActionType::RECONNECT:
      os << "RECONNECT";
      break;
    case MqttSnClientActionType::DELAY:
      os << "DELAY";
      break;
    case MqttSnClientActionType::UNKNOWNACTION:
      os << "UNKNOWNACTION";
      break;
    default:
      os << "UNKNOWNACTION";
      break;
  }
  return os;
}
