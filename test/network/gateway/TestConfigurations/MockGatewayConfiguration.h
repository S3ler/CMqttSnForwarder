//
// Created by bele on 26.02.19.
//

#ifndef CMQTTSNFORWARDER_MOCKGATEWAYCONFIGURATION_H
#define CMQTTSNFORWARDER_MOCKGATEWAYCONFIGURATION_H

#include <cstdint>
#include "../../../../forwarder/global_defines.h"
#include "../MockGatewayNetwork/MockGatewayNetworkInterface.h"
class MockGatewayConfiguration {
 public:
  uint8_t gatewayIdentifier;
  device_address address;
  MockGatewayNetworkInterface* mockGatewayNetworkInterface;

  MockGatewayConfiguration(uint8_t gatewayIdentifier,
                           device_address &address,
                           MockGatewayNetworkInterface *mockGatewayNetworkInterface) : gatewayIdentifier(
      gatewayIdentifier), address(address), mockGatewayNetworkInterface(mockGatewayNetworkInterface) {}
};

#endif //CMQTTSNFORWARDER_MOCKGATEWAYCONFIGURATION_H
