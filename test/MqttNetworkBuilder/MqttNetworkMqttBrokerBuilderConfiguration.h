//
// Created by SomeDude on 23.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTNETWORKBUILDER_MQTTNETWORKMQTTBROKERBUILDERCONFIGURATION_H_
#define CMQTTSNFORWARDER_TEST_MQTTNETWORKBUILDER_MQTTNETWORKMQTTBROKERBUILDERCONFIGURATION_H_

#include <stdint.h>
#include <string>
#include "../MqttBroker/MqttBrokerTestType.h"

class MqttNetworkMqttBrokerBuilderConfiguration {
 public:
  const MqttBrokerTestType type;
  const int32_t maxBrokerCount;

  const std::string protocol; // example tcp
  const std::string brokerAddress; // example localhost
  const uint16_t minBrokerPort; // example 1883
  MqttNetworkMqttBrokerBuilderConfiguration(const MqttBrokerTestType type,
                                            const int32_t max_broker_count,
                                            const std::string &a_protocol,
                                            const std::string &broker_address,
                                            const uint16_t min_broker_port);
  static const MqttNetworkMqttBrokerBuilderConfiguration GetDefaultConfiguration();
};

#endif //CMQTTSNFORWARDER_TEST_MQTTNETWORKBUILDER_MQTTNETWORKMQTTBROKERBUILDERCONFIGURATION_H_
