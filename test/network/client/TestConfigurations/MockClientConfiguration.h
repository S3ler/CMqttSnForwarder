//
// Created by bele on 25.02.19.
//

#ifndef CMQTTSNFORWARDER_MOCKCLIENTIDENTIFIER_H
#define CMQTTSNFORWARDER_MOCKCLIENTIDENTIFIER_H

#include "../../../../forwarder/global_defines.h"
#include "../MockClientNetwork/MockClient/MockClientNetworkInterface.h"

class MockClientConfiguration {
 public:
  device_address address;
  uint16_t identifier;
  MockClientNetworkInterface *mockClientNetworkInterface;

  MockClientConfiguration(device_address address,
                          uint16_t identifier,
                          MockClientNetworkInterface *mockClientNetworkInterface)
      : address(address), identifier(identifier), mockClientNetworkInterface(mockClientNetworkInterface) {}
};

#endif //CMQTTSNFORWARDER_MOCKCLIENTIDENTIFIER_H
