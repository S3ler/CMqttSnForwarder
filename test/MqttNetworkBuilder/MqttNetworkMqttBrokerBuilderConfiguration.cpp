//
// Created by SomeDude on 23.07.2019.
//

#include "MqttNetworkMqttBrokerBuilderConfiguration.h"
MqttNetworkMqttBrokerBuilderConfiguration::MqttNetworkMqttBrokerBuilderConfiguration(const MqttBrokerTestType type,
                                                                                     const int32_t max_broker_count,
                                                                                     const std::string &a_protocol,
                                                                                     const std::string &broker_address,
                                                                                     const uint16_t min_broker_port)
    : type(type),
      maxBrokerCount(max_broker_count),
      protocol(a_protocol),
      brokerAddress(broker_address),
      minBrokerPort(min_broker_port) {}
const MqttNetworkMqttBrokerBuilderConfiguration MqttNetworkMqttBrokerBuilderConfiguration::GetDefaultConfiguration() {
  return MqttNetworkMqttBrokerBuilderConfiguration(MqttBrokerTestType::MOSQUITTO_DOCKER,
                                                   1,
                                                   std::string("tcp"),
                                                   std::string("localhost"),
                                                   1883);
}
