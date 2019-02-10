//
// Created by bele on 03.02.19.
//

#ifndef CMQTTSNFORWARDER_FAKESOCKETINTERFACE_H
#define CMQTTSNFORWARDER_FAKESOCKETINTERFACE_H

#include "MockGateway.h"

class MockGateway;

class MockGatewayNetworkInterface {
 public:
  virtual void setMockGateway(MockGateway *fakeClient)=0;

  virtual bool isNetworkDisconnected() =0;

  virtual ssize_t sendNetwork(const uint8_t *buf, uint8_t len)=0;

  virtual bool connectNetwork(device_address *address)=0;

  virtual void disconnectNetwork()=0;

  virtual void loopNetwork()=0;
};

#endif //CMQTTSNFORWARDER_FAKESOCKETINTERFACE_H
