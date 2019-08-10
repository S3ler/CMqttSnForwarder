//
// Created by SomeDude on 23.07.2019.
//

#include "MqttNetworkBuilderMqttBrokerBuilderConfiguration.h"
const MqttNetworkBuilderMqttBrokerBuilderConfiguration MqttNetworkBuilderMqttBrokerBuilderConfiguration::GetDefaultConfiguration() {
  return MqttNetworkBuilderMqttBrokerBuilderConfiguration(MqttBrokerTestType::MOSQUITTO_DOCKER,
                                                   1,
                                                   MqttBrokerProtocolTestType::TCP,
                                                   std::string("localhost"),
                                                   1883);
}
MqttNetworkBuilderMqttBrokerBuilderConfiguration::MqttNetworkBuilderMqttBrokerBuilderConfiguration(const MqttBrokerTestType type,
                                                                                     const int32_t max_broker_count,
                                                                                     const MqttBrokerProtocolTestType protocol_type,
                                                                                     const std::string &broker_address,
                                                                                     const uint16_t min_broker_port)
    : type(type),
      maxBrokerCount(max_broker_count),
      protocol_type(protocol_type),
      brokerAddress(broker_address),
      minBrokerPort(min_broker_port) {}
