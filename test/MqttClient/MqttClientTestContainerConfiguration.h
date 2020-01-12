//
// Created by SomeDude on 30.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTTESTCONTAINERCONFIGURATION_H_
#define CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTTESTCONTAINERCONFIGURATION_H_

#include <stdint.h>
#include <string>
#include <vector>
#include "MqttClientAction.h"
#include "MqttClientProtocolTestType.h"
#include <test/MqttBroker/MqttBrokerTestContainerConfiguration.h>

class MqttClientTestContainerConfiguration {
 public:
  const MqttClientProtocolTestType protocol_type;
  const std::string brokerAddress;
  const uint16_t brokerPort;

  const std::string clientId;
  const std::string clientPassword;

  const int32_t keepAliveInterval;
  const bool cleanSession;

  const std::string willTopic;
  const std::vector<uint8_t> willMessage;
  const int32_t willQoS;
  const bool willRetain;

  const int64_t publishReceivePublishTimeout;

  MqttClientTestContainerConfiguration();

  MqttClientTestContainerConfiguration(const MqttClientProtocolTestType protocol_type, const std::string &broker_address, const uint16_t broker_port, const std::string &client_id,
                                       const std::string &client_password, const int32_t keep_alive_interval, const bool clean_session, const std::string &willTopic,
                                       const std::vector<uint8_t> &willMessage, const int32_t willQoS, const bool willRetain, const int64_t publishReceivePublishTimeout);
  MqttClientTestContainerConfiguration(const MqttBrokerTestContainerConfiguration &broker_config, const std::string &client_id, const std::string &client_password, const int32_t keep_alive_interval,
                                       const bool clean_session, const std::string &willTopic, const std::vector<uint8_t> &willMessage, const int32_t willQoS, const bool willRetain,
                                       const int64_t publishReceivePublishTimeout);
  MqttClientTestContainerConfiguration(const MqttBrokerTestContainerConfiguration &broker_config, const std::string &client_id, const std::string &client_password, const int32_t keep_alive_interval,
                                       const bool clean_session, const int64_t publishReceivePublishTimeout);

 private:
  MqttClientProtocolTestType GetProtocolTypeFromMqttBrokerProtocolType(MqttBrokerProtocolTestType t);
};

#endif  // CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTTESTCONTAINERCONFIGURATION_H_
