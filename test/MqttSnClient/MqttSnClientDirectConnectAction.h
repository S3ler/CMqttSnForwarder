//
// Created by SomeDude on 01.08.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTDIRECTCONNECTACTION_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTDIRECTCONNECTACTION_H_

#include <stdint.h>
#include <string>
#include <vector>
#include "MqttSnClientConnectAction.h"

class MqttSnClientDirectConnectAction : public MqttSnClientConnectAction {
 public:
  const std::vector<uint8_t> brokerAddress;

  MqttSnClientDirectConnectAction(const std::vector<uint8_t> &broker_address, const std::string &client_id, const std::string &client_password, const int32_t keep_alive_interval,
                                  const bool clean_session);
  MqttSnClientDirectConnectAction(const std::string &uri, const int32_t broker_port, const std::string &client_id, const std::string &client_password, const int32_t keep_alive_interval,
                                  const bool clean_session);
  MqttSnClientDirectConnectAction(const std::string &uri, const int32_t broker_port, const std::string &client_id, const std::string &client_password, const int32_t keep_alive_interval,
                                  const bool clean_session, const std::string &willTopic, const std::vector<uint8_t> &willMessage, const bool willRetain);
  MqttSnClientDirectConnectAction(const std::vector<uint8_t> &broker_address, const std::string &client_id, const std::string &client_password, const int32_t keep_alive_interval,
                                  const bool clean_session, const std::string &willTopic, const std::vector<uint8_t> &willMessage, const bool willRetain);
};

#endif  // CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTDIRECTCONNECTACTION_H_
