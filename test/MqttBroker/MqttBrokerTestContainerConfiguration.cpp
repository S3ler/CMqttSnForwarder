//
// Created by SomeDude on 22.07.2019.
//

#include "MqttBrokerTestContainerConfiguration.h"
MqttBrokerTestContainerConfiguration::MqttBrokerTestContainerConfiguration(const std::string &a_protocol,
                                                                           const std::string &broker_address,
                                                                           const uint16_t broker_port) : protocol(
    a_protocol), brokerAddress(broker_address), brokerPort(broker_port) {}
MqttBrokerTestContainerConfiguration MqttBrokerTestContainerConfiguration::GetDefaultTestContainerConfiguration() {
  return MqttBrokerTestContainerConfiguration(std::string("tcp"), std::string("localhost"), 1883);
}
