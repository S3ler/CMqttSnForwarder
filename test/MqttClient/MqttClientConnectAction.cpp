//
// Created by SomeDude on 22.07.2019.
//

#include <cstdint>
#include "MqttClientConnectAction.h"
#include "../MqttBroker/MqttBrokerTestContainerConfiguration.h"

MqttClientConnectAction::MqttClientConnectAction(const MqttBrokerTestContainerConfiguration &broker_config,
                                                 const std::string &client_id,
                                                 const std::string &client_password,
                                                 const int32_t keep_alive_interval,
                                                 const bool clean_session)
    : MqttClientAction(MqttClientActionType::CONNECT),
      protocol(GetProtocolStringFromMqttClientProtocolType(GetProtocolStringFromMqttBrokerProtocolType(broker_config.protocol_type))),
      brokerAddress(broker_config.brokerAddress),
      brokerPort(broker_config.brokerPort),
      clientId(client_id),
      clientPassword(client_password),
      keepAliveInterval(keep_alive_interval),
      cleanSession(clean_session) {}

MqttClientConnectAction::MqttClientConnectAction()
    : MqttClientAction(MqttClientActionType::CONNECT),
      protocol(GetProtocolStringFromMqttClientProtocolType(MqttClientProtocolTestType::TCP)),
      brokerAddress("localhost"),
      brokerPort(1883),
      clientId(""),
      clientPassword(""),
      keepAliveInterval(60),
      cleanSession(true) {}
MqttClientConnectAction::MqttClientConnectAction(MqttClientActionType action_type,
                                                 const MqttClientProtocolTestType protocol_type,
                                                 const std::string &broker_address,
                                                 const uint16_t broker_port,
                                                 const std::string &client_id,
                                                 const std::string &client_password,
                                                 const int32_t keep_alive_interval,
                                                 const bool clean_session)
    : MqttClientAction(action_type),
      protocol(GetProtocolStringFromMqttClientProtocolType(protocol_type)),
      brokerAddress(broker_address),
      brokerPort(broker_port),
      clientId(client_id),
      clientPassword(client_password),
      keepAliveInterval(keep_alive_interval),
      cleanSession(clean_session) {}
const MqttClientProtocolTestType MqttClientConnectAction::GetProtocolStringFromMqttBrokerProtocolType(
    MqttBrokerProtocolTestType t) {
  if (t == MqttBrokerProtocolTestType::TCP) {
    return MqttClientProtocolTestType::TCP;
  }
  /*
   * TODO WEBSOCKET
   */
  throw std::exception();
}
const std::string MqttClientConnectAction::GetProtocolStringFromMqttClientProtocolType(const MqttClientProtocolTestType t) {
  if (t == MqttClientProtocolTestType::TCP) {
    return std::string("tcp");
  }
  /*
   * TODO WEBSOCKET
   */
  throw std::exception();
}
MqttClientConnectAction::MqttClientConnectAction(const MqttClientTestContainerConfiguration &configuration)
    : MqttClientAction(MqttClientActionType::CONNECT),
      protocol(GetProtocolStringFromMqttClientProtocolType(configuration.protocol_type)),
      brokerAddress(configuration.brokerAddress),
      brokerPort(configuration.brokerPort),
      clientId(configuration.clientId),
      clientPassword(configuration.clientPassword),
      keepAliveInterval(configuration.keepAliveInterval),
      cleanSession(configuration.cleanSession) {}
