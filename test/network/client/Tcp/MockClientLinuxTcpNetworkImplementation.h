//
// Created by bele on 06.02.19.
//

#ifndef CMQTTSNFORWARDER_MOCKCLIENTLINUXTCPNETWORKIMPLEMENTATION_H
#define CMQTTSNFORWARDER_MOCKCLIENTLINUXTCPNETWORKIMPLEMENTATION_H

#include "../MockClientNetwork/MockClient/MockClientNetworkInterface.h"

class MockClientLinuxTcpNetworkImplementation : public MockClientNetworkInterface {
 private:
  MockClient *mockClient;
  int forwarder_socket_fd = -1;
  device_address *address;
 public:
  void setMockClient(MockClient *client) override;
  void setNetworkAddress(device_address *address)override;
  bool isNetworkConnected() override;
  bool isNetworkDisconnected() override;
  int sendToNetwork(device_address *to, const uint8_t *buf, uint8_t len) override;
  bool connectNetwork(device_address *address) override;
  void disconnectNetwork() override;
  int loopNetwork(MockClientNetworkReceiver *pReceiver) override;

};

#endif //CMQTTSNFORWARDER_MOCKCLIENTLINUXTCPNETWORKIMPLEMENTATION_H
