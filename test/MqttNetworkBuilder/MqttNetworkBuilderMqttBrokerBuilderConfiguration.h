//
// Created by SomeDude on 23.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTNETWORKBUILDER_MQTTNETWORKBUILDERMQTTBROKERBUILDERCONFIGURATION_H_
#define CMQTTSNFORWARDER_TEST_MQTTNETWORKBUILDER_MQTTNETWORKBUILDERMQTTBROKERBUILDERCONFIGURATION_H_

#include <stdint.h>
#include <string>
#include <test/MqttBroker/MqttBrokerProtocolTestType.h>
#include "../MqttBroker/MqttBrokerTestType.h"

class MqttNetworkBuilderMqttBrokerBuilderConfiguration {
 public:
  const MqttBrokerTestType type;
  const int32_t maxBrokerCount;

  const MqttBrokerProtocolTestType protocol_type; // example tcp
  const std::string brokerAddress; // example localhost
  const uint16_t minBrokerPort; // example 1883
  MqttNetworkBuilderMqttBrokerBuilderConfiguration(const MqttBrokerTestType type,
                                            const int32_t max_broker_count,
                                            const MqttBrokerProtocolTestType protocol_type,
                                            const std::string &broker_address,
                                            const uint16_t min_broker_port);
  static const MqttNetworkBuilderMqttBrokerBuilderConfiguration GetDefaultConfiguration();
};

#endif //CMQTTSNFORWARDER_TEST_MQTTNETWORKBUILDER_MQTTNETWORKBUILDERMQTTBROKERBUILDERCONFIGURATION_H_
