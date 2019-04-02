//
// Created by bele on 03.02.19.
//

#ifndef CMQTTSNFORWARDER_FAKESOCKETINTERFACE_H
#define CMQTTSNFORWARDER_FAKESOCKETINTERFACE_H

#include "MockGateway.h"
#include "../MockGateway/MockGatewayNetworkReceiver.h"

class MockGatewayNetworkReceiver;

class MockGatewayNetworkInterface {
 public:
  virtual bool isNetworkConnected() = 0;

  virtual bool isNetworkDisconnected() = 0;

  virtual device_address getForwarderDeviceAddress() = 0;

  virtual int sendNetwork(const device_address *to, const uint8_t *buf, uint16_t len) = 0;

  virtual bool connectNetwork(device_address *gatewayDeviceAddress, device_address *forwarderDeviceAddress) = 0;

  virtual void disconnectNetwork() = 0;

  virtual int loopNetwork(MockGatewayNetworkReceiver *receiver) = 0;

  virtual ~MockGatewayNetworkInterface() {};
};

#endif //CMQTTSNFORWARDER_FAKESOCKETINTERFACE_H
