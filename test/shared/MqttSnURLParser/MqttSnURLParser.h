//
// Created by SomeDude on 30.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_SHARED_MQTTSNURLPARSER_H_
#define CMQTTSNFORWARDER_TEST_SHARED_MQTTSNURLPARSER_H_

#include <stdint.h>
#include <vector>
#include <string>
#include <memory.h>

class MqttSnURLParser {
 public:
  static std::vector<uint8_t> GetBrokerAddressFromUriAndPort(const std::string &URI, const int32_t brokerPort);
  static std::vector<uint8_t> GetBrokerAddressFromString(const std::string &addressString);
  static std::vector<uint8_t> GetBrokerAddressFromUri(const std::string &URI);
  static std::vector<uint8_t> GetPortVectorFromInt32(const int32_t brokerPort);

  static bool isLocalhost(const std::vector<uint8_t> &vector);
  static std::string toString(const std::vector<uint8_t> &vector, uint32_t elements);
  static uint16_t getPort(const std::vector<uint8_t> &vector);
};

#endif //CMQTTSNFORWARDER_TEST_SHARED_MQTTSNURLPARSER_H_
