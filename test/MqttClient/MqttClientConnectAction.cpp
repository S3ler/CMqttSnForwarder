//
// Created by SomeDude on 22.07.2019.
//

#include <cstdint>
#include "MqttClientConnectAction.h"
#include "../MqttBroker/MqttBrokerTestContainerConfiguration.h"
MqttClientConnectAction::MqttClientConnectAction(const std::string &a_protocol,
                                                 const std::string &broker_address,
                                                 const uint16_t broker_port,
                                                 const std::string &client_id,
                                                 const std::string &client_password,
                                                 const int32_t keep_alive_interval,
                                                 const bool clean_session)
    : MqttClientAction(MqttClientActionType::CONNECT),
      protocol(a_protocol),
      brokerAddress(broker_address),
      brokerPort(broker_port),
      clientId(client_id),
      clientPassword(client_password),
      keepAliveInterval(keep_alive_interval),
      cleanSession(clean_session) {}

MqttClientConnectAction::MqttClientConnectAction(const MqttBrokerTestContainerConfiguration &broker_config,
                                                 const std::string &client_id,
                                                 const std::string &client_password,
                                                 const int32_t keep_alive_interval,
                                                 const bool clean_session)
    : MqttClientAction(MqttClientActionType::CONNECT),
      protocol(broker_config.protocol),
      brokerAddress(broker_config.brokerAddress),
      brokerPort(broker_config.brokerPort),
      clientId(client_id),
      clientPassword(client_password),
      keepAliveInterval(keep_alive_interval),
      cleanSession(clean_session) {}

MqttClientConnectAction::MqttClientConnectAction()
    : MqttClientAction(MqttClientActionType::CONNECT),
      protocol("tcp"),
      brokerAddress("localhost"),
      brokerPort(1883),
      clientId(""),
      clientPassword(""),
      keepAliveInterval(60),
      cleanSession(true) {}