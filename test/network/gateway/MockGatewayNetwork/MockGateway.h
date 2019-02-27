//
// Created by bele on 03.02.19.
//

#ifndef CMQTTSNFORWARDER_MOCKGATEWAYNETWORKINTERFACE_H
#define CMQTTSNFORWARDER_MOCKGATEWAYNETWORKINTERFACE_H

#include <atomic>
#include <thread>
#include "../../../../forwarder/global_defines.h"
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

  MockGatewayNetworkReceiverInterface *getReceiver();

  bool isNetworkConnected();

  MockGateway(uint8_t gatewayIdentifier,
                const device_address *networkAddress,
                MockGatewayNetworkInterface *mockGatewayNetworkInterface,
                MockGatewayNetworkReceiverInterface *receiver);
 private:

  MockGatewayNetworkReceiverInterface *receiver = nullptr;
  std::thread thread;
  const device_address *networkAddress = nullptr;
  std::atomic<bool> stopped{false};
  MockGatewayNetworkInterface* mockGatewayNetworkInterface = nullptr;
  uint8_t gatewayIdentifier;
};

#endif //CMQTTSNFORWARDER_MOCKGATEWAYNETWORKINTERFACE_H
