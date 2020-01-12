//
// Created by SomeDude on 22.07.2019.
//

#include "MqttClientConnectAction.h"
#include <cstdint>

MqttClientConnectAction::MqttClientConnectAction(const MqttBrokerTestContainerConfiguration& broker_config, const std::string& client_id, const std::string& client_password,
                                                 const int32_t keep_alive_interval, const bool clean_session, const std::string& willTopic, const std::vector<uint8_t>& willMessage,
                                                 const int32_t willQoS, const bool willRetain)
    : MqttClientAction(MqttClientActionType::CONNECT),
      protocol(GetProtocolStringFromMqttClientProtocolType(GetProtocolStringFromMqttBrokerProtocolType(broker_config.protocol_type))),
      brokerAddress(broker_config.brokerAddress),
      brokerPort(broker_config.brokerPort),
      clientId(client_id),
      clientPassword(client_password),
      keepAliveInterval(keep_alive_interval),
      cleanSession(clean_session),
      willTopic(willTopic),
      willMessage(willMessage),
      willQoS(willQoS),
      willRetain(willRetain) {}

MqttClientConnectAction::MqttClientConnectAction()
    : MqttClientAction(MqttClientActionType::CONNECT),
      protocol(GetProtocolStringFromMqttClientProtocolType(MqttClientProtocolTestType::TCP)),
      brokerAddress("localhost"),
      brokerPort(1883),
      clientId(""),
      clientPassword(""),
      keepAliveInterval(60),
      cleanSession(true),
      willQoS(0),
      willRetain(false) {}
MqttClientProtocolTestType MqttClientConnectAction::GetProtocolStringFromMqttBrokerProtocolType(MqttBrokerProtocolTestType t) {
  if (t == MqttBrokerProtocolTestType::TCP) {
    return MqttClientProtocolTestType::TCP;
  }
  // FEATURE WEBSOCKET
  throw std::exception();
}
std::string MqttClientConnectAction::GetProtocolStringFromMqttClientProtocolType(const MqttClientProtocolTestType t) {
  if (t == MqttClientProtocolTestType::TCP) {
    return std::string("tcp");
  }
  // FEATURE WEBSOCKET
  throw std::exception();
}
MqttClientConnectAction::MqttClientConnectAction(const MqttClientTestContainerConfiguration& configuration)
    : MqttClientAction(MqttClientActionType::CONNECT),
      protocol(GetProtocolStringFromMqttClientProtocolType(configuration.protocol_type)),
      brokerAddress(configuration.brokerAddress),
      brokerPort(configuration.brokerPort),
      clientId(configuration.clientId),
      clientPassword(configuration.clientPassword),
      keepAliveInterval(configuration.keepAliveInterval),
      cleanSession(configuration.cleanSession),
      willTopic(configuration.willTopic),
      willMessage(configuration.willMessage),
      willQoS(configuration.willQoS),
      willRetain(configuration.willRetain) {}
