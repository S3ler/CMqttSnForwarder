
//
// Created by bele on 05.02.19.
//

#ifndef CMQTTSNFORWARDER_MOCKCLIENTNETWORKINTERFACE_H
#define CMQTTSNFORWARDER_MOCKCLIENTNETWORKINTERFACE_H

#include <cstdint>
#include <global_defines.h>
#include "../MockClient/MockClientNetworkReceiver.h"

class MockClientNetworkInterface {
 public:
  virtual bool isNetworkConnected() = 0;

  virtual bool isNetworkDisconnected() = 0;

  virtual device_address getForwarderDeviceAddress() = 0;

  virtual int sendToNetwork(const device_address *to, const uint8_t *data, uint16_t dataLength) = 0;

  virtual bool connectNetwork(device_address *forwarderAddress, device_address *clientAddress) = 0;

  virtual void disconnectNetwork() = 0;

  virtual int loopNetwork(MockClientNetworkReceiver *receiver) = 0;

  virtual ~MockClientNetworkInterface() {};
};

#endif //CMQTTSNFORWARDER_MOCKCLIENTNETWORKINTERFACE_H
