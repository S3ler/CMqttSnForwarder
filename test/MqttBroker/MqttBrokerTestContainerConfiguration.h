//
// Created by SomeDude on 22.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTBROKER_MQTTBROKERTESTCONTAINERCONFIGURATION_H_
#define CMQTTSNFORWARDER_TEST_MQTTBROKER_MQTTBROKERTESTCONTAINERCONFIGURATION_H_

#include <string>
#include "MqttBrokerTestType.h"
#include "MqttBrokerProtocolTestType.h"

class MqttBrokerTestContainerConfiguration {
 public:
  const MqttBrokerProtocolTestType protocol_type; // example tcp
  const std::string brokerAddress; // example localhost
  const uint16_t brokerPort; // example 1883

  MqttBrokerTestContainerConfiguration(const MqttBrokerProtocolTestType protocol_type,
                                       const std::string &broker_address,
                                       const uint16_t broker_port);
  static const MqttBrokerTestContainerConfiguration GetDefaultTestContainerConfiguration();
};

#endif //CMQTTSNFORWARDER_TEST_MQTTBROKER_MQTTBROKERTESTCONTAINERCONFIGURATION_H_
