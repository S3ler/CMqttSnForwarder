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
#include "CompareableMqttSnMessageData.h"
#include "MockClientNetworkReceiver.h"
#include <thread>

class MockClientNetworkInterface;

class MockClient {
 public:
  void loop();

  bool start_loop();

  bool isNetworkConnected();

  void stop_loop();

  int send(uint8_t *data, uint16_t length);

  int send(CompareableMqttSnMessageData* compareableMqttSnMessageData);

  void receive(uint8_t *data, uint16_t length);

  void setNetworkAddress(device_address *networkAddress);

  void setForwarderAddress(device_address *forwarderAddress);

  void setMockClientNetworkInterface(MockClientNetworkInterface *mockClientNetworkInterface);

  MockClientNetworkReceiver* getMockClientNetworkReceiver();

  uint16_t getIdentifier();

  device_address *getNetworkAddress() const;

  const std::atomic<bool> &getDone() const;

  MockClient(uint16_t identifier,
             device_address *networkAddress,
             device_address *forwarderAddress,
             MockClientNetworkInterface *mockClientNetworkInterface,
             MockClientNetworkReceiver *receiver);

 private:
  uint16_t identifier;
  MockClientNetworkReceiver* receiver;
  std::thread thread;
  device_address *networkAddress;
  std::atomic<bool> stopped{false};
  std::atomic<bool> done{false};
  MockClientNetworkInterface *mockClientNetworkInterface = nullptr;
  device_address *forwarderAddress = nullptr;
};

#endif //CMQTTSNFORWARDER_MOCKCLIENT_H
