//
// Created by bele on 03.02.19.
//

#ifndef CMQTTSNFORWARDER_LINUXTCPFAKESOCKET_H
#define CMQTTSNFORWARDER_LINUXTCPFAKESOCKET_H

#include "../MockGatewayNetworkInterface.h"

#define MockGatewayLinuxTcpNetworkImplementation_BUFFER_SIZE 255

class MockGatewayLinuxTcpNetworkImplementation : public MockGatewayNetworkInterface {
 private:
  MockGateway *mockGateway;
  int master_socket_fd = -1;
  int client_socket_fd = -1;

 public:
  virtual void setMockGateway(MockGateway *mockGateway) ;
  virtual bool isNetworkDisconnected() ;
  virtual ssize_t sendNetwork(const uint8_t *buf, uint8_t len) ;
  virtual bool connectNetwork(device_address *address) ;
  virtual void disconnectNetwork() ;
  virtual void loopNetwork() ;
};

#endif //CMQTTSNFORWARDER_LINUXTCPFAKESOCKET_H
