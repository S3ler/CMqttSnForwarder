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
#include "ComparableClientMqttSnMessageData.h"
#include "MockClientNetworkReceiver.h"
#include <thread>

class MockClientNetworkInterface;

class MockClient {
 public:
  MockClient(uint16_t identifier,
             device_address networkAddress,
             device_address *forwarderAddress,
             MockClientNetworkInterface *mockClientNetworkInterface,
             MockClientNetworkReceiver *receiver);

  void loop();

  bool start_loop();

  void stop_loop();

  bool isNetworkConnected();

  int send(const device_address *to, const uint8_t *data, uint16_t length);

  int send(ComparableClientMqttSnMessageData *comparableClientMqttSnMessageData);

  MockClientNetworkReceiver *getMockClientNetworkReceiver();

  uint16_t getIdentifier();

  device_address *getClientDeviceAddress();

  const std::atomic<bool> &getDone() const;

  device_address getForwarderDeviceAddress() const;

 private:
  uint16_t identifier;
  MockClientNetworkReceiver *receiver;
  std::thread thread;
  device_address networkAddress;
  std::atomic<bool> stopped{false};
  std::atomic<bool> done{false};
  MockClientNetworkInterface *mockClientNetworkInterface = nullptr;
  device_address *forwarderAddress = nullptr;
};

#endif //CMQTTSNFORWARDER_MOCKCLIENT_H
