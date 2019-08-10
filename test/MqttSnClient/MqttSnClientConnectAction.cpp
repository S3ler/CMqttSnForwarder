//
// Created by SomeDude on 24.07.2019.
//

#include <arpa/inet.h>
#include <memory.h>
#include <netdb.h>
#include <test/shared/MqttSnURLParser/MqttSnURLParser.h>
#include "MqttSnClientConnectAction.h"

MqttSnClientConnectAction::MqttSnClientConnectAction(const MqttSnClientActionType action_type,
                                                     const std::string &client_id,
                                                     const std::string &client_password,
                                                     const int32_t keep_alive_interval,
                                                     const bool clean_session)
    : MqttSnClientAction(action_type),
      clientId(client_id),
      clientPassword(client_password),
      keepAliveInterval(keep_alive_interval),
      cleanSession(clean_session) {}
