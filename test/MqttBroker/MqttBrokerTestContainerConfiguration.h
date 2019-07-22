//
// Created by SomeDude on 22.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTBROKER_MQTTBROKERTESTCONTAINERCONFIGURATION_H_
#define CMQTTSNFORWARDER_TEST_MQTTBROKER_MQTTBROKERTESTCONTAINERCONFIGURATION_H_

#include <string>

class MqttBrokerTestContainerConfiguration {
 public:
  const std::string protocol; // example tcp
  const std::string brokerAddress; // example localhost
  const uint16_t brokerPort; // example 1883
  MqttBrokerTestContainerConfiguration(const std::string &a_protocol,
                                       const std::string &broker_address,
                                       const uint16_t broker_port);
  static MqttBrokerTestContainerConfiguration GetDefaultTestContainerConfiguration();
};

#endif //CMQTTSNFORWARDER_TEST_MQTTBROKER_MQTTBROKERTESTCONTAINERCONFIGURATION_H_
