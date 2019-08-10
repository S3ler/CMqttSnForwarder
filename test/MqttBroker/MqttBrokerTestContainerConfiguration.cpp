//
// Created by SomeDude on 22.07.2019.
//

#include "MqttBrokerTestContainerConfiguration.h"
const MqttBrokerTestContainerConfiguration MqttBrokerTestContainerConfiguration::GetDefaultTestContainerConfiguration() {
  return MqttBrokerTestContainerConfiguration(MqttBrokerProtocolTestType ::TCP,
                                              std::string("localhost"),
                                              1883);
}
MqttBrokerTestContainerConfiguration::MqttBrokerTestContainerConfiguration(const MqttBrokerProtocolTestType protocol_type,
                                                                           const std::string &broker_address,
                                                                           const uint16_t broker_port) : protocol_type(
    protocol_type), brokerAddress(broker_address), brokerPort(broker_port) {}
