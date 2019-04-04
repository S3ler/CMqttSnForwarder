//
// Created by bele on 06.02.19.
//

#ifndef CMQTTSNFORWARDER_MOCKCLIENTLINUXTCPNETWORKIMPLEMENTATION_H
#define CMQTTSNFORWARDER_MOCKCLIENTLINUXTCPNETWORKIMPLEMENTATION_H

#include "../MockClientNetwork/MockClient/MockClientNetworkInterface.h"

class MockClientLinuxTcpNetworkImplementation : public MockClientNetworkInterface {
 private:
  int forwarder_socket_fd = -1;
  device_address *forwarderAddress;

 public:
  bool isNetworkConnected() override;

  bool isNetworkDisconnected() override;

  device_address getForwarderDeviceAddress() override;

  int sendToNetwork(const device_address *to, const uint8_t *buf, uint16_t dataLength) override;

  bool connectNetwork(device_address *forwarderAddress, device_address *clientAddress) override;

  void disconnectNetwork() override;

  int loopNetwork(MockClientNetworkReceiver *receiver) override;

 private:
  int getDeviceAddressFromFileDescriptor(int peer_fd, device_address *peerAddress);
};

#endif //CMQTTSNFORWARDER_MOCKCLIENTLINUXTCPNETWORKIMPLEMENTATION_H
