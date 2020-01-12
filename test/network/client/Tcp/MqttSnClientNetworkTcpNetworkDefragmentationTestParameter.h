//
// Created by SomeDude on 25.03.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNCLIENTNETWORKTCPNETWORKDEFRAGMENTATIONTESTPARAMETER_H
#define CMQTTSNFORWARDER_MQTTSNCLIENTNETWORKTCPNETWORKDEFRAGMENTATIONTESTPARAMETER_H

#include <TestConfigurations/MqttSnClientNetworkTestValueParameter.h>

class MqttSnClientNetworkTcpNetworkDefragmentationTestParameter
    : public MqttSnClientNetworkTestValueParameter {
 public:
  uint16_t packetSize;
  MqttSnClientNetworkTcpNetworkDefragmentationTestParameter(
      std::vector<MockClientConfiguration> &mockClientIdentifiers,
      uint16_t messageCount,
      uint16_t messageLength,
      uint16_t packetSize,
      MqttSnGatewayClientNetworkTestConfiguration &mqttSnClientNetworkTestFixture,
      uint8_t *(*msgDataGenerator)(uint8_t *, uint16_t) = defaultMessageDataGenerator)
      : MqttSnClientNetworkTestValueParameter(
      mockClientIdentifiers,
      messageCount,
      messageLength,
      mqttSnClientNetworkTestFixture,
      msgDataGenerator),
        packetSize(packetSize) {}
};
#endif //CMQTTSNFORWARDER_MQTTSNCLIENTNETWORKTCPNETWORKDEFRAGMENTATIONTESTPARAMETER_H
