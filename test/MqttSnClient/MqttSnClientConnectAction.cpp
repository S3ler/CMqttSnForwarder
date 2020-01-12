//
// Created by SomeDude on 24.07.2019.
//

#include "MqttSnClientConnectAction.h"
#include <arpa/inet.h>
#include <memory.h>
#include <netdb.h>
#include <test/shared/MqttSnURLParser/MqttSnURLParser.h>
MqttSnClientConnectAction::MqttSnClientConnectAction(const std::string& client_id, const std::string& client_password, const int32_t connect_duration, const bool clean_session,
                                                     const std::string& willTopic, const std::vector<uint8_t>& willMessage, const bool willRetain)
    : MqttSnClientAction(MqttSnClientActionType::CONNECT),
      clientId(client_id),
      clientPassword(client_password),
      keepAliveInterval(connect_duration),
      cleanSession(clean_session),
      willTopic(willTopic),
      willMessage(willMessage),
      willRetain(willRetain) {}
MqttSnClientConnectAction::MqttSnClientConnectAction(const std::string& client_id, const std::string& client_password, const int32_t connect_duration, const bool clean_session)
    : MqttSnClientAction(MqttSnClientActionType::CONNECT), clientId(client_id), clientPassword(client_password), keepAliveInterval(connect_duration), cleanSession(clean_session),willRetain(false) {}
MqttSnClientConnectAction::MqttSnClientConnectAction(const std::string& clientId, const int32_t keepAliveInterval, const bool clean_session)
    : MqttSnClientAction(MqttSnClientActionType::CONNECT), clientId(clientId), keepAliveInterval(keepAliveInterval), cleanSession(clean_session),willRetain(false) {}

MqttSnClientConnectAction::MqttSnClientConnectAction(MqttSnClientActionType actionType, const std::string& client_id, const std::string& client_password, const int32_t connect_duration,
                                                     const bool clean_session)
    : MqttSnClientAction(actionType), clientId(client_id), clientPassword(client_password), keepAliveInterval(connect_duration), cleanSession(clean_session),willRetain(false) {}
MqttSnClientConnectAction::MqttSnClientConnectAction(MqttSnClientActionType actionType, const std::string& clientId, const int32_t keepAliveInterval, const bool clean_session)
    : MqttSnClientAction(actionType), clientId(clientId), keepAliveInterval(keepAliveInterval), cleanSession(clean_session),willRetain(false) {}
MqttSnClientConnectAction::MqttSnClientConnectAction(MqttSnClientActionType actionType, const std::string& client_id, const std::string& client_password, const int32_t connect_duration,
                                                     const bool clean_session, const std::string& willTopic, const std::vector<uint8_t>& willMessage, const bool willRetain)
    : MqttSnClientAction(actionType),
      clientId(client_id),
      clientPassword(client_password),
      keepAliveInterval(connect_duration),
      cleanSession(clean_session),
      willTopic(willTopic),
      willMessage(willMessage),
      willRetain(willRetain) {}

