//
// Created by bele on 05.02.19.
//

#ifndef CMQTTSNFORWARDER_MOCKCLIENT_H
#define CMQTTSNFORWARDER_MOCKCLIENT_H

#include <cstdint>
#include <atomic>
#include "MockClientNetworkInterface.h"
#include "MockClientNetworkReceiverInterface.h"
#include "../../../../../forwarder/global_defines.h"
#include <thread>

class MockClientNetworkInterface;

class MockClient {
 public:
  void loop();

  bool start_loop();

  bool isNetworkConnected();

  void stop_loop();

  int send(uint8_t *data, uint16_t length);

  void receive(uint8_t *data, uint16_t length);

  void setNetworkAddress(device_address *networkAddress);

  device_address* getNetworkAddress();

  void setForwarderAddress(device_address *forwarderAddress);

  void setMockClientNetworkInterface(MockClientNetworkInterface *mockClientNetworkInterface);

  MockClientNetworkReceiverInterface* getMockClientNetworkReceiverInterface();

  void setMockClientNetworkReceiverInterface(MockClientNetworkReceiverInterface* receiver);

  uint16_t getIdentifier() const;

  MockClient();

  MockClient(uint16_t identifier,
             device_address *networkAddress,
             device_address *forwarderAddress,
             MockClientNetworkReceiverInterface *receiver);
 private:
  uint16_t identifier;
  MockClientNetworkReceiverInterface* receiver;
  std::thread thread;
  device_address *networkAddress = nullptr;
  std::atomic<bool> stopped{false};
  MockClientNetworkInterface *mockClientNetworkInterface = nullptr;
  device_address *forwarderAddress = nullptr;
};

#endif //CMQTTSNFORWARDER_MOCKCLIENT_H
