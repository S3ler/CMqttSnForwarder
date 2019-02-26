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

class MqttSnClientNetworkTestTypeParameter {
 public:
  //uint16_t clientCount;
  std::vector<MockClientConfiguration> mockClientIdentifiers;
  uint16_t messageCount;
  uint16_t messageLength;

  MqttSnGatewayClientNetworkTestConfiguration &mqttSnClientNetworkTestFixture;

  MqttSnClientNetworkTestTypeParameter(std::vector<MockClientConfiguration> mockClientIdentifiers,
                                       uint16_t messageCount,
                                       uint16_t messageLength,
                                       MqttSnGatewayClientNetworkTestConfiguration &mqttSnClientNetworkTestFixture)
      : mockClientIdentifiers(std::move(mockClientIdentifiers)),
        messageCount(messageCount),
        messageLength(messageLength),
        mqttSnClientNetworkTestFixture(mqttSnClientNetworkTestFixture) {}

  friend std::ostream &operator<<(std::ostream &os, const MqttSnClientNetworkTestTypeParameter &parameter) {
    os << "clientCount" << parameter.mockClientIdentifiers.size() << "messageCount" << parameter.messageCount << "messageLength"
       << parameter.messageLength << "mqttSnGatewayClientNetworkTestConfiguration" << parameter.mqttSnClientNetworkTestFixture << "";
    return os;
  }
};

#endif //CMQTTSNFORWARDER_MQTTSNCLIENTNETWORKTESTTYPEPARAMETER_H
