//
// Created by SomeDude on 30.07.2019.
//

#include "MqttClientTestContainerConfiguration.h"
MqttClientTestContainerConfiguration::MqttClientTestContainerConfiguration()
    : protocol_type(MqttClientProtocolTestType::TCP),
      brokerAddress("localhost"),
      brokerPort(1883),
      clientId(""),
      clientPassword(""),
      keepAliveInterval(60),
      cleanSession(true) {}
MqttClientTestContainerConfiguration::MqttClientTestContainerConfiguration(const MqttClientProtocolTestType protocol_type,
                                                                           const std::string &broker_address,
                                                                           const uint16_t broker_port,
                                                                           const std::string &client_id,
                                                                           const std::string &client_password,
                                                                           const int32_t keep_alive_interval,
                                                                           const bool clean_session)
    : protocol_type(protocol_type),
      brokerAddress(broker_address),
      brokerPort(broker_port),
      clientId(client_id),
      clientPassword(client_password),
      keepAliveInterval(keep_alive_interval),
      cleanSession(clean_session) {}
MqttClientTestContainerConfiguration::MqttClientTestContainerConfiguration(const MqttBrokerTestContainerConfiguration &broker_config,
                                                                           const std::string &client_id,
                                                                           const std::string &client_password,
                                                                           const int32_t keep_alive_interval,
                                                                           const bool clean_session)
    : protocol_type(GetProtocolTypeFromMqttBrokerProtocolType(broker_config.protocol_type)),
      brokerAddress(broker_config.brokerAddress),
      brokerPort(broker_config.brokerPort),
      clientId(client_id),
      clientPassword(client_password),
      keepAliveInterval(keep_alive_interval),
      cleanSession(clean_session) {}

const MqttClientProtocolTestType MqttClientTestContainerConfiguration::GetProtocolTypeFromMqttBrokerProtocolType(
    MqttBrokerProtocolTestType t) {
  if (t == MqttBrokerProtocolTestType::TCP) {
    return MqttClientProtocolTestType::TCP;
  }
  /*
   * TODO WEBSOCKET
   */
  throw std::exception();
}