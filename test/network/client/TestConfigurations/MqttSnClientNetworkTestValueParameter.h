#include <utility>

#include <utility>

//
// Created by bele on 24.02.19.
//

#ifndef CMQTTSNFORWARDER_MQTTSNCLIENTNETWORKTESTTYPEPARAMETER_H
#define CMQTTSNFORWARDER_MQTTSNCLIENTNETWORKTESTTYPEPARAMETER_H

#include <cstdint>
#include <ostream>
#include "MqttSnGatewayClientNetworkTestConfiguration.h"
#include "MockClientConfiguration.h"

uint8_t *defaultMessageDataGenerator(uint8_t *arr, uint16_t len) {
  memset(arr, 0, len * sizeof(uint8_t));
  for (uint16_t i = 0; i < len; i++) {
    arr[i] = static_cast<uint8_t>(i % UINT8_MAX);
  }
  return arr;
}

class MqttSnClientNetworkTestValueParameter {
 public:
  //uint16_t clientCount;
  std::vector<MockClientConfiguration> mockClientConfigurations;
  uint16_t messageCount;
  uint16_t messageLength;
  uint8_t *(*messageDataGenerator)(uint8_t* arr, uint16_t len);

  MqttSnGatewayClientNetworkTestConfiguration &mqttSnClientNetworkTestFixture;

  MqttSnClientNetworkTestValueParameter(std::vector<MockClientConfiguration> mockClientIdentifiers,
                                        uint16_t messageCount,
                                        uint16_t messageLength,
                                        MqttSnGatewayClientNetworkTestConfiguration &mqttSnClientNetworkTestFixture,
                                        uint8_t *(*messageDataGenerator)(uint8_t*, uint16_t) = defaultMessageDataGenerator)
      : mockClientConfigurations(std::move(mockClientIdentifiers)),
        messageCount(messageCount),
        messageLength(messageLength),
        mqttSnClientNetworkTestFixture(mqttSnClientNetworkTestFixture),
        messageDataGenerator(messageDataGenerator) {}

  friend std::ostream &operator<<(std::ostream &os, const MqttSnClientNetworkTestValueParameter &parameter) {
    os << "clientCount" << parameter.mockClientConfigurations.size() << "messageCount" << parameter.messageCount
       << "messageLength"
       << parameter.messageLength << "mqttSnGatewayClientNetworkTestConfiguration"
       << parameter.mqttSnClientNetworkTestFixture << "";
    return os;
  }
};

#endif //CMQTTSNFORWARDER_MQTTSNCLIENTNETWORKTESTTYPEPARAMETER_H
