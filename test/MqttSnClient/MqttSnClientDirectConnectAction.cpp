//
// Created by SomeDude on 01.08.2019.
//

#include "MqttSnClientDirectConnectAction.h"
#include <test/shared/MqttSnURLParser/MqttSnURLParser.h>

MqttSnClientDirectConnectAction::MqttSnClientDirectConnectAction(const std::vector<uint8_t> &broker_address, const std::string &client_id, const std::string &client_password,
                                                                 const int32_t keep_alive_interval, const bool clean_session)
    : MqttSnClientConnectAction(MqttSnClientActionType::DIRECT_CONNECT, client_id, client_password, keep_alive_interval, clean_session), brokerAddress(broker_address) {}
MqttSnClientDirectConnectAction::MqttSnClientDirectConnectAction(const std::string &uri, const int32_t broker_port, const std::string &client_id, const std::string &client_password,
                                                                 const int32_t keep_alive_interval, const bool clean_session)
    : MqttSnClientConnectAction(MqttSnClientActionType::DIRECT_CONNECT, client_id, client_password, keep_alive_interval, clean_session),
      brokerAddress(MqttSnURLParser::GetBrokerAddressFromUriAndPort(uri, broker_port)) {}
MqttSnClientDirectConnectAction::MqttSnClientDirectConnectAction(const std::string &uri, const int32_t broker_port, const std::string &client_id, const std::string &client_password,
                                                                 const int32_t keep_alive_interval, const bool clean_session, const std::string &willTopic, const std::vector<uint8_t> &willMessage,
                                                                 const bool willRetain)
    : MqttSnClientConnectAction(MqttSnClientActionType::DIRECT_CONNECT, client_id, client_password, keep_alive_interval, clean_session, willTopic, willMessage, willRetain),
      brokerAddress(MqttSnURLParser::GetBrokerAddressFromUriAndPort(uri, broker_port)) {}
MqttSnClientDirectConnectAction::MqttSnClientDirectConnectAction(const std::vector<uint8_t> &broker_address, const std::string &client_id, const std::string &client_password,
                                                                 const int32_t keep_alive_interval, const bool clean_session, const std::string &willTopic, const std::vector<uint8_t> &willMessage,
                                                                 const bool willRetain)
    : MqttSnClientConnectAction(MqttSnClientActionType::DIRECT_CONNECT, client_id, client_password, keep_alive_interval, clean_session, willTopic, willMessage, willRetain),
      brokerAddress(broker_address) {}
