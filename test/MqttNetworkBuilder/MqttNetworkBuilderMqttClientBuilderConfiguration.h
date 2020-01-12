//
// Created by SomeDude on 23.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTNETWORKBUILDER_MQTTNETWORKBUILDERMQTTCLIENTBUILDERCONFIGURATION_H_
#define CMQTTSNFORWARDER_TEST_MQTTNETWORKBUILDER_MQTTNETWORKBUILDERMQTTCLIENTBUILDERCONFIGURATION_H_

#include <stdint.h>
#include <string>
#include <vector>
#include "../MqttClient/MqttClientTestType.h"

class MqttNetworkBuilderMqttClientBuilderConfiguration {
 public:
  const MqttClientTestType type;
  const uint16_t maxClientCount;

  const std::string baseClientId;
  const std::string baseClientPassword;

  const int32_t keepAliveInterval;
  const bool cleanSession;

  const std::string baseWillTopic;
  const std::vector<uint8_t> baseWillMessage;
  const int32_t willQoS;
  const bool willRetain;

  const int64_t publishReceivePublishTimeout;
  MqttNetworkBuilderMqttClientBuilderConfiguration(const MqttClientTestType &type, const uint16_t max_client_count, const std::string &base_client_id, const std::string &base_client_password,
                                                   const int32_t keep_alive_interval, const bool clean_session, const std::string &baseWillTopic, const std::vector<uint8_t> &baseWillMessage,
                                                   const int32_t willQoS, const bool willRetain, const int64_t publishReceivePublishTimeout);
  MqttNetworkBuilderMqttClientBuilderConfiguration(const MqttClientTestType &type, const uint16_t max_client_count, const std::string &base_client_id, const std::string &base_client_password,
                                                   const int32_t keep_alive_interval, const bool clean_session, const int64_t publishReceivePublishTimeout);
  static const MqttNetworkBuilderMqttClientBuilderConfiguration GetDefaultConfiguration();
};

#endif  // CMQTTSNFORWARDER_TEST_MQTTNETWORKBUILDER_MQTTNETWORKBUILDERMQTTCLIENTBUILDERCONFIGURATION_H_
