//
// Created by SomeDude on 03.04.2019.
//

#ifndef CMQTTSNFORWARDER_MOCKCLIENTLINUXUDPNETWORKIMPLEMENTATION_H
#define CMQTTSNFORWARDER_MOCKCLIENTLINUXUDPNETWORKIMPLEMENTATION_H

#include <global_defines.h>
#include <MockClientNetwork/MockClient/MockClientNetworkInterface.h>

#define MockClientLinuxUdpNetworkImplementation_BUFFER_SIZE 1024

class MockClientLinuxUdpNetworkImplementation : public MockClientNetworkInterface {
 private:
  int master_socket_fd = -1;
  device_address forwarderDeviceAddress;
  device_address *clientDeviceAddress;
 public:
  bool isNetworkConnected() override;
  bool isNetworkDisconnected() override;
  device_address getForwarderDeviceAddress() override;
  int sendToNetwork(const device_address *to, const uint8_t *data, uint16_t dataLength) override;
  bool connectNetwork(device_address *forwarderAddress, device_address *clientAddress) override;
  void disconnectNetwork() override;
  int loopNetwork(MockClientNetworkReceiver *receiver) override;
};

#endif //CMQTTSNFORWARDER_MOCKCLIENTLINUXUDPNETWORKIMPLEMENTATION_H
