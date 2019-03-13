//
// Created by bele on 05.02.19.
//

#ifndef CMQTTSNFORWARDER_MOCKCLIENTNETWORKINTERFACE_H
#define CMQTTSNFORWARDER_MOCKCLIENTNETWORKINTERFACE_H

#include <cstdint>
#include "MockClient.h"
#include <global_defines.h>
#include "../MockClient/MockClientNetworkReceiver.h"

class MockClient;

class MockClientNetworkInterface {
 public:
  virtual void setMockClient(MockClient *client)=0;

  virtual void setNetworkAddress(device_address *address)=0;

  virtual bool isNetworkConnected() = 0;

  virtual bool isNetworkDisconnected() =0;

  virtual int sendToNetwork(device_address* to, const uint8_t *buf, uint8_t len)=0;

  virtual bool connectNetwork(device_address *address)=0;

  virtual void disconnectNetwork()=0;

  virtual int loopNetwork(MockClientNetworkReceiver *pReceiver) =0;
};

#endif //CMQTTSNFORWARDER_MOCKCLIENTNETWORKINTERFACE_H
