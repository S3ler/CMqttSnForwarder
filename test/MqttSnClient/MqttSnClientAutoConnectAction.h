//
// Created by SomeDude on 12.09.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNCLIENTAUTOCONNECTACTION_H
#define CMQTTSNFORWARDER_MQTTSNCLIENTAUTOCONNECTACTION_H

#include "MqttSnClientConnectAction.h"
class MqttSnClientAutoConnectAction : public MqttSnClientConnectAction{
 public:
  MqttSnClientAutoConnectAction(const std::string clientId, const std::string clientPassword, const int32_t connectDuration, const bool cleanSession);
};

#endif  // CMQTTSNFORWARDER_MQTTSNCLIENTAUTOCONNECTACTION_H
