//
// Created by bele on 05.02.19.
//

#ifndef CMQTTSNFORWARDER_MOCKCLIENT_H
#define CMQTTSNFORWARDER_MOCKCLIENT_H

#include <cstdint>
#include <atomic>
#include "MockClientNetworkInterface.h"
#include "MockClientNetworkReceiverInterface.h"
#include <thread>

class MockClientNetworkInterface;

class MockClient {
 public:
  void loop();

  bool start_loop();

  void stop_loop();

  void send(uint8_t *data, uint16_t length);

  void receive(uint8_t *data, uint16_t length);

  void setNetworkAddress(device_address *networkAddress);

  device_address* getNetworkAddress();

  void setForwarderAddress(device_address *forwarderAddress);

  void setMockClientNetworkReceiverInterface(MockClientNetworkReceiverInterface *receiverInterface);

  void setMockClientNetworkInterface(MockClientNetworkInterface *mockClientNetworkInterface);

 private:
  MockClientNetworkReceiverInterface *receiver = nullptr;
  std::thread thread;
  device_address *networkAddress = nullptr;
  std::atomic<bool> stopped{false};
  MockClientNetworkInterface *mockClientNetworkInterface = nullptr;
  device_address *forwarderAddress = nullptr;
};

#endif //CMQTTSNFORWARDER_MOCKCLIENT_H
