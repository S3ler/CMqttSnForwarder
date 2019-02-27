//
// Created by bele on 03.02.19.
//

#include "MockGateway.h"
#include "MockGatewayNetworkReceiverInterface.h"
#include "../Tcp/MockGatewayLinuxTcpNetworkImplementation.h"

void MockGateway::loop() {
  while (!stopped) {
    mockGatewayNetworkInterface->loopNetwork();
  }
  mockGatewayNetworkInterface->disconnectNetwork();
  // free(mockGatewayNetworkInterface);
}

bool MockGateway::start_loop() {
  if (mockGatewayNetworkInterface == nullptr) {
    throw std::invalid_argument("mockGatewayNetworkInterface not initialized");
  }
  if (receiver == nullptr) {
    throw std::invalid_argument("receiver not initialized");
  }
  if (this->networkAddress == nullptr) {
    throw std::invalid_argument("gateway networkAddress not set");
  }

  mockGatewayNetworkInterface->setMockGateway(this);
  if (!mockGatewayNetworkInterface->connectNetwork(const_cast<device_address *>(networkAddress))) {
    return false;
  }
  this->thread = std::thread(&MockGateway::loop, this);
  this->thread.detach();
  return true;
}

void MockGateway::stop_loop() {
  this->stopped = true;
  if (this->thread.joinable()) {
    this->thread.join();
  }
  if (!mockGatewayNetworkInterface->isNetworkDisconnected()) {
    mockGatewayNetworkInterface->disconnectNetwork();
  }
}

void MockGateway::send(uint8_t *data, uint16_t length) {
  if (mockGatewayNetworkInterface == nullptr) {
    throw std::invalid_argument("mockGatewayNetworkInterface not initialized");
  }
  if (receiver == nullptr) {
    throw std::invalid_argument("receiver not initialized");
  }
  if (this->networkAddress == nullptr) {
    throw std::invalid_argument("gateway networkAddress not set");
  }
  // TODO check forwarderAddress
  mockGatewayNetworkInterface->sendNetwork(data, length);
}

void MockGateway::receive(uint8_t *data, uint16_t length) {
  if (mockGatewayNetworkInterface == nullptr) {
    throw std::invalid_argument("mockGatewayNetworkInterface not initialized");
  }
  if (receiver == nullptr) {
    throw std::invalid_argument("receiver not initialized");
  }
  if (this->networkAddress == nullptr) {
    throw std::invalid_argument("gateway networkAddress not set");
  }
  this->receiver->receive_any_message(data, length);
}

MockGatewayNetworkReceiverInterface *MockGateway::getReceiver() {
  return receiver;
}

MockGateway::MockGateway(uint8_t gatewayIdentifier,
                         const device_address *networkAddress,
                         MockGatewayNetworkInterface *mockGatewayNetworkInterface,
                         MockGatewayNetworkReceiverInterface *receiver)
    : receiver(receiver),
      networkAddress(networkAddress),
      mockGatewayNetworkInterface(mockGatewayNetworkInterface),
      gatewayIdentifier(gatewayIdentifier) {}

bool MockGateway::isNetworkConnected() {
  return mockGatewayNetworkInterface->isNetworkConnected();
}

