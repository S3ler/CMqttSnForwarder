//
// Created by SomeDude on 24.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTCONNECTACTION_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTCONNECTACTION_H_

#include <stdint.h>
#include <string>
#include <vector>
#include "MqttSnClientAction.h"
#include "MqttSnClientProtocolType.h"

class MqttSnClientConnectAction : public MqttSnClientAction {
 public:
  const MqttSnClientProtocolType protocol; // example UDP/BLE/B5.0/ZigBee
  const std::vector<uint8_t> brokerAddress;

  const std::string clientId; // not optional
  const std::string clientPassword; // optional

  const int32_t keepAliveInterval;
  const bool cleanSession;

  // TODO will
  MqttSnClientConnectAction(const MqttSnClientProtocolType a_protocol,
                            const std::vector<uint8_t> &broker_address,
                            const std::string &client_id,
                            const std::string &client_password,
                            const int32_t keep_alive_interval,
                            const bool clean_session);
  MqttSnClientConnectAction(const MqttSnClientProtocolType a_protocol,
                            const std::string &URI,
                            const int32_t brokerPort,
                            const std::string &client_id,
                            const std::string &client_password,
                            const int32_t keep_alive_interval,
                            const bool clean_session);
  static std::vector<uint8_t> GetBrokerAddressFromUriAndPort(const std::string &URI, const int32_t brokerPort);
  static std::vector<uint8_t> GetBrokerAddressFromString(const std::string &addressString);
  static std::vector<uint8_t> GetBrokerAddressFromUri(const std::string &URI);
  static std::vector<uint8_t> GetPortVectorFromInt32(const int32_t brokerPort);
};

#endif //CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTCONNECTACTION_H_
