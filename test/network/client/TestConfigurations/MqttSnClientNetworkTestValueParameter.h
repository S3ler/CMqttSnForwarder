#include <utility>

#include <utility>

//
// Created by bele on 24.02.19.
//

#ifndef CMQTTSNFORWARDER_MQTTSNCLIENTNETWORKTESTTYPEPARAMETER_H
#define CMQTTSNFORWARDER_MQTTSNCLIENTNETWORKTESTTYPEPARAMETER_H

#include <cstdint>
#include <vector>
#include <ostream>
#include "MockClientConfiguration.h"
#include "MqttSnGatewayClientNetworkTestConfiguration.h"
#include "ClientNetworkMessageDataGenerator.h"

class MqttSnClientNetworkTestValueParameter {
 public:
  std::vector<MockClientConfiguration> mockClientConfigurations;
  uint16_t messageCount;
  uint16_t messageLength;
  uint8_t *(*messageDataGenerator)(uint8_t *arr, uint16_t len);

  MqttSnGatewayClientNetworkTestConfiguration &mqttSnClientNetworkTestFixture;

  MqttSnClientNetworkTestValueParameter(std::vector<MockClientConfiguration> mockClientIdentifiers,
                                        uint16_t messageCount,
                                        uint16_t messageLength,
                                        MqttSnGatewayClientNetworkTestConfiguration &mqttSnClientNetworkTestFixture,
                                        uint8_t *(*msgDataGenerator)(uint8_t *, uint16_t) = defaultMessageDataGenerator)
      : mockClientConfigurations(std::move(mockClientIdentifiers)),
        messageCount(messageCount),
        messageLength(messageLength),
        messageDataGenerator(msgDataGenerator),
        mqttSnClientNetworkTestFixture(mqttSnClientNetworkTestFixture) {}

  friend std::ostream &operator<<(std::ostream &os, const MqttSnClientNetworkTestValueParameter &parameter) {
    os << "mockClientCount" << parameter.mockClientConfigurations.size()
       << "messageCount" << parameter.messageCount
       << "messageLength" << parameter.messageLength;
    return os;
  }
};

#endif //CMQTTSNFORWARDER_MQTTSNCLIENTNETWORKTESTTYPEPARAMETER_H
