//
// Created by bele on 03.02.19.
//

#ifndef CMQTTSNFORWARDER_MOCKGATEWAYNETWORKINTERFACE_H
#define CMQTTSNFORWARDER_MOCKGATEWAYNETWORKINTERFACE_H

#include <atomic>
#include <thread>
#include "../../../../forwarder/global_defines.h"
#include "ComparableGatewayMqttSnMessageData.h"
#include "MockGatewayNetworkReceiver.h"
#include "MockGatewayNetworkInterface.h"

class MockGatewayNetworkReceiver;
class MockGatewayNetworkInterface;

class MockGateway {
 public:
  MockGateway(uint8_t identifier,
              device_address *gatewayAddress,
              MockGatewayNetworkInterface *mockGatewayNetworkInterface,
              MockGatewayNetworkReceiver *receiver);

  void loop();

  bool start_loop();

  void stop_loop();

  bool isNetworkConnected();

  int send(const device_address *to, const uint8_t *data, uint16_t length);

  int send(ComparableGatewayMqttSnMessageData *comparableGatewayMqttSnMessageData);

  MockGatewayNetworkReceiver *getReceiver();

  uint8_t getIdentifier();

  device_address *getGatewayDeviceAddress() const;

  device_address getForwarderDeviceAddress() const;

  const std::atomic<bool> &getDone() const;

 private:
  uint8_t identifier;
  MockGatewayNetworkReceiver *receiver = nullptr;
  std::thread thread;
  //device_address *forwarderAddress = nullptr;
  device_address *gatewayAddress = nullptr;
  std::atomic<bool> stopped{false};
  std::atomic<bool> done{false};
  MockGatewayNetworkInterface *mockGatewayNetworkInterface = nullptr;
};

#endif //CMQTTSNFORWARDER_MOCKGATEWAYNETWORKINTERFACE_H
