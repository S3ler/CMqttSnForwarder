//
// Created by SomeDude on 01.04.2019.
//

#ifndef CMQTTSNFORWARDER_MOCKGATEWAYLINUXUDPNETWORKIMPLEMENTATION_H
#define CMQTTSNFORWARDER_MOCKGATEWAYLINUXUDPNETWORKIMPLEMENTATION_H

#include <MockGateway/MockGatewayNetworkInterface.h>

#define MockGatewayLinuxUdpNetworkImplementation_BUFFER_SIZE 1024

class MockGatewayLinuxUdpNetworkImplementation : public MockGatewayNetworkInterface {
 private:
  int master_socket_fd = -1;
  device_address *gatewayDeviceAddress = nullptr;
  device_address forwarderDeviceAddress;

 public:
  bool isNetworkConnected() override;
  bool isNetworkDisconnected() override;
  device_address getForwarderDeviceAddress() override;
  int sendNetwork(const device_address *to, const uint8_t *buf, uint16_t len) override;
  bool connectNetwork(device_address *gatewayDeviceAddress, device_address *forwarderDeviceAddress) override;
  void disconnectNetwork() override;
  int loopNetwork(MockGatewayNetworkReceiver *receiver) override;
 private:
  int receiveForwarderMessage(MockGatewayNetworkReceiver *receiver);
};

#endif //CMQTTSNFORWARDER_MOCKGATEWAYLINUXUDPNETWORKIMPLEMENTATION_H
