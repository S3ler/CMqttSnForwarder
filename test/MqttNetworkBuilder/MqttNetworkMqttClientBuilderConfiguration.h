//
// Created by SomeDude on 23.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTNETWORKBUILDER_MQTTNETWORKMQTTCLIENTBUILDERCONFIGURATION_H_
#define CMQTTSNFORWARDER_TEST_MQTTNETWORKBUILDER_MQTTNETWORKMQTTCLIENTBUILDERCONFIGURATION_H_

#include <stdint.h>
#include <string>
#include "../MqttClient/MqttClientTestType.h"

class MqttNetworkMqttClientBuilderConfiguration {
 public:
  const MqttClientTestType type;
  const int32_t maxClientCount;

  const std::string baseClientId;
  const std::string baseClientPassword;

  const int32_t keepAliveInterval;
  const bool cleanSession;

  // TODO will
  //const std::string baseWillTopic;
  //const std::vector<uint8_t> baseWillMessage;
  //const int32_t willQoS;

  MqttNetworkMqttClientBuilderConfiguration(const MqttClientTestType &type,
                                            const int32_t max_client_count,
                                            const std::string &base_client_id,
                                            const std::string &base_client_password,
                                            const int32_t keep_alive_interval,
                                            const bool clean_session);
  static const MqttNetworkMqttClientBuilderConfiguration GetDefaultConfiguration();
};

#endif //CMQTTSNFORWARDER_TEST_MQTTNETWORKBUILDER_MQTTNETWORKMQTTCLIENTBUILDERCONFIGURATION_H_
