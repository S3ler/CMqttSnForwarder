//
// Created by SomeDude on 22.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTCONNECTACTION_H_
#define CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTCONNECTACTION_H_

#include <stdint.h>
#include <string>
#include <vector>
#include "../MqttBroker/MqttBrokerTestContainerConfiguration.h"
#include "MqttClientAction.h"

class MqttClientConnectAction : public MqttClientAction {
 public:
  const std::string protocol; // example tcp
  const std::string brokerAddress; // example localhost
  const uint16_t brokerPort; // example 1883

  const std::string clientId; // not optional
  const std::string clientPassword; // optional

  const int32_t keepAliveInterval;
  const bool cleanSession;

  // TODO will
  //const std::string willTopic;
  //const std::vector<uint8_t> willMessage;
  //const int32_t willQoS;
  MqttClientConnectAction(const MqttBrokerTestContainerConfiguration &broker_config,
                          const std::string &client_id,
                          const std::string &client_password,
                          const int32_t keep_alive_interval,
                          const bool clean_session);
  MqttClientConnectAction(const std::string &a_protocol,
                          const std::string &broker_address,
                          const uint16_t broker_port,
                          const std::string &client_id,
                          const std::string &client_password,
                          const int32_t keep_alive_interval,
                          const bool clean_session);

};

#endif //CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTCONNECTACTION_H_
