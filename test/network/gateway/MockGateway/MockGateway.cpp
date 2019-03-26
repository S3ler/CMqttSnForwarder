//
// Created by bele on 03.02.19.
//

#include "MockGateway.h"

MockGateway::MockGateway(uint8_t identifier,
                         device_address *gatewayAddress,
                         MockGatewayNetworkInterface *mockGatewayNetworkInterface,
                         MockGatewayNetworkReceiver *receiver)
    : receiver(receiver),
      gatewayAddress(gatewayAddress),
      mockGatewayNetworkInterface(mockGatewayNetworkInterface),
      identifier(identifier) {}

void MockGateway::loop() {
  done = false;
  while (!stopped & mockGatewayNetworkInterface->isNetworkConnected()) {
    if (mockGatewayNetworkInterface->loopNetwork(receiver) != 0) {
      break;
    }
  }
  mockGatewayNetworkInterface->disconnectNetwork();
  done = true;
}

bool MockGateway::start_loop() {
  if (!mockGatewayNetworkInterface->connectNetwork(gatewayAddress)) {
    return false;
  }
  this->thread = std::thread(&MockGateway::loop, this);
  this->thread.detach();
  return true;
}

bool MockGateway::isNetworkConnected() {
  return mockGatewayNetworkInterface->isNetworkConnected();
}

void MockGateway::stop_loop() {
  this->stopped = true;
  if (this->thread.joinable()) {
    this->thread.join();
  }
}

int MockGateway::send(const device_address *to, const uint8_t *data, uint16_t length) {
  mockGatewayNetworkInterface->sendNetwork(to, data, length);
}

int MockGateway::send(ComparableGatewayMqttSnMessageData *comparableGatewayMqttSnMessageData) {
  return send(&comparableGatewayMqttSnMessageData->address,
              &comparableGatewayMqttSnMessageData->data[0],
              comparableGatewayMqttSnMessageData->data_length);
}

MockGatewayNetworkReceiver *MockGateway::getReceiver() {
  return receiver;
}

uint8_t MockGateway::getIdentifier() {
  return identifier;
}

device_address *MockGateway::getGatewayDeviceAddress() const {
  return gatewayAddress;
}

const std::atomic<bool> &MockGateway::getDone() const {
  return done;
}
device_address MockGateway::getForwarderDeviceAddress() const {
  return mockGatewayNetworkInterface->getForwarderDeviceAddress();
}
