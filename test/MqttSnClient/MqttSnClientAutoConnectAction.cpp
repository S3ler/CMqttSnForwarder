//
// Created by SomeDude on 12.09.2019.
//

#include "MqttSnClientAutoConnectAction.h"
MqttSnClientAutoConnectAction::MqttSnClientAutoConnectAction(const std::string clientId, const std::string clientPassword, const int32_t connectDuration, const bool cleanSession)
    : MqttSnClientConnectAction(MqttSnClientActionType::AUTO_CONNECT, clientId, clientPassword, connectDuration, cleanSession) {}
