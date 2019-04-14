#include <utility>

//
// Created by bele on 26.02.19.
//

#ifndef CMQTTSNFORWARDER_MQTTSNGATEWAYNETWORKVALUEPARAMETER_H
#define CMQTTSNFORWARDER_MQTTSNGATEWAYNETWORKVALUEPARAMETER_H

#include <cstdint>
#include <vector>
#include <ostream>
#include <cstring>
#include "MockGatewayConfiguration.h"
#include "MqttSnForwarderGatewayNetworkTestConfiguration.h"
#include "MqttSnGatewayNetworkMessageDataGenerator.h"

class MqttSnGatewayNetworkValueParameter {
 public:
  std::vector<MockGatewayConfiguration> mockGatewayConfigurations;
  uint16_t messageCount;
  uint16_t messageLength;
  uint8_t *(*messageDataGenerator)(uint8_t *arr, uint16_t len);
  device_address gatewayToConnectAddress;
  bool searchGateway = false;

  MqttSnForwarderGatewayNetworkTestConfiguration &mqttSnForwarderGatewayNetworkTestConfiguration;

  MqttSnGatewayNetworkValueParameter(std::vector<MockGatewayConfiguration> mockGatewayConfigurations,
                                     uint16_t messageCount,
                                     uint16_t messageLength,
                                     MqttSnForwarderGatewayNetworkTestConfiguration &mqttSnForwarderGatewayNetworkTestConfiguration,
                                     device_address mqttSnGatewayNetworkAddress = {0},
                                     uint8_t *(*msgDataGenerator)(uint8_t *, uint16_t) = defaultMessageDataGenerator)
      : mockGatewayConfigurations(std::move(mockGatewayConfigurations)),
        messageCount(messageCount),
        messageLength(messageLength),
        messageDataGenerator(msgDataGenerator),
        gatewayToConnectAddress(mqttSnGatewayNetworkAddress),
        mqttSnForwarderGatewayNetworkTestConfiguration(mqttSnForwarderGatewayNetworkTestConfiguration) {
    device_address emptyDeviceAddress = {0};
    searchGateway = memcmp(emptyDeviceAddress.bytes, mqttSnGatewayNetworkAddress.bytes, sizeof(device_address)) == 0;
  }

  friend std::ostream &operator<<(std::ostream &os, const MqttSnGatewayNetworkValueParameter &parameter) {
    os << "mockGatewayCount" << parameter.mockGatewayConfigurations.size()
       << "messageCount" << parameter.messageCount
       << "messageLength" << parameter.messageLength;
    return os;
  }
};

#endif //CMQTTSNFORWARDER_MQTTSNGATEWAYNETWORKVALUEPARAMETER_H
