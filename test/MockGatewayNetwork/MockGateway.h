//
// Created by bele on 03.02.19.
//

#ifndef CMQTTSNFORWARDER_MOCKGATEWAYNETWORKINTERFACE_H
#define CMQTTSNFORWARDER_MOCKGATEWAYNETWORKINTERFACE_H

#include <atomic>
#include <thread>
#include "../../forwarder/global_defines.h"
#include "MockGatewayNetworkInterface.h"

class MockGatewayNetworkReceiverInterface;
class MockGatewayNetworkInterface;

class MockGateway {
 public:

 public:
  void loop();

  bool start_loop();

  void stop_loop();

  void send(uint8_t *data, uint16_t length);

  void receive(uint8_t *data, uint16_t length);

  void setNetworkAddress(device_address *networkAddress);

  void setForwarderNetworkAddress(device_address *forwarderAddress);

  void setMockGatewayNetworkReceiverInterface(MockGatewayNetworkReceiverInterface *receiverInterface);

  void setMockGatewayNetworkInterface(MockGatewayNetworkInterface *mockGatewayNetworkInterface);

 private:

  MockGatewayNetworkReceiverInterface *receiver = nullptr;
  std::thread thread;
  device_address *networkAddress = nullptr;
  std::atomic<bool> stopped{false};
  MockGatewayNetworkInterface* mockGatewayNetworkInterface = nullptr;
  device_address *forwarderAddress;
};

#endif //CMQTTSNFORWARDER_MOCKGATEWAYNETWORKINTERFACE_H
