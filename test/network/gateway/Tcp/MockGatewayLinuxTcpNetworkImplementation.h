//
// Created by bele on 03.02.19.
//

#ifndef CMQTTSNFORWARDER_LINUXTCPFAKESOCKET_H
#define CMQTTSNFORWARDER_LINUXTCPFAKESOCKET_H

#include "../MockGateway/MockGatewayNetworkInterface.h"

#define MockGatewayLinuxTcpNetworkImplementation_BUFFER_SIZE 1024

class MockGatewayLinuxTcpNetworkImplementation : public MockGatewayNetworkInterface {
 private:
  int master_socket_fd = -1;
  int forwarder_socket_fd = -1;
  device_address *gatewayDeviceAddress;
 public:
  bool isNetworkConnected() override;

  bool isNetworkDisconnected() override;

  device_address getForwarderDeviceAddress() override;

  int sendNetwork(const device_address *to, const uint8_t *data, uint16_t dataLength) override;

  bool connectNetwork(device_address *gatewayDeviceAddress, device_address *forwarderDeviceAddress) override;

  void disconnectNetwork() override;

  int loopNetwork(MockGatewayNetworkReceiver *receiver) override;

 private:
  int awaitForwarderConnection(MockGatewayNetworkReceiver *receiver);
  int receiveForwarderMessage(MockGatewayNetworkReceiver *receiver);
  int getDeviceAddressFromFileDescriptor(int peer_fd, device_address *peerAddress);
};

#endif //CMQTTSNFORWARDER_LINUXTCPFAKESOCKET_H
