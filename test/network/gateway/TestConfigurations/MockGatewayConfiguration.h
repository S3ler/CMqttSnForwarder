//
// Created by bele on 26.02.19.
//

#ifndef CMQTTSNFORWARDER_MOCKGATEWAYCONFIGURATION_H
#define CMQTTSNFORWARDER_MOCKGATEWAYCONFIGURATION_H

#include <cstdint>
#include "platform/device_address.h"
#include "../MockGateway/MockGatewayNetworkInterface.h"

class MockGatewayConfiguration {
 public:
  uint8_t gatewayIdentifier;
  device_address address;
  MockGatewayNetworkInterface *mockGatewayNetworkInterface;

  MockGatewayConfiguration(device_address address,
                           uint8_t gatewayIdentifier,
                           MockGatewayNetworkInterface *mockGatewayNetworkInterface) : gatewayIdentifier(
      gatewayIdentifier), address(address), mockGatewayNetworkInterface(mockGatewayNetworkInterface) {}
};

#endif //CMQTTSNFORWARDER_MOCKGATEWAYCONFIGURATION_H
