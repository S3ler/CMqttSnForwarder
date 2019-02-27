//
// Created by bele on 05.02.19.
//

#include <stdexcept>
#include "MockClient.h"

void MockClient::loop() {
  while (!stopped) {
    mockClientNetworkInterface->loopNetwork();
  }
  mockClientNetworkInterface->disconnectNetwork();
}

bool MockClient::start_loop() {
  if (mockClientNetworkInterface == nullptr) {
    throw std::invalid_argument("mockClientNetworkInterface not initialized");
  }
  if (receiver == nullptr) {
    throw std::invalid_argument("receiver not initialized");
  }
  if (this->networkAddress == nullptr) {
    throw std::invalid_argument("gateway networkAddress not set");
  }

  mockClientNetworkInterface->setMockClient(this);
  if (!mockClientNetworkInterface->connectNetwork(forwarderAddress)) {
    return false;
  }
  this->thread = std::thread(&MockClient::loop, this);
  this->thread.detach();
  return true;
}

bool MockClient::isNetworkConnected() {
  return mockClientNetworkInterface->isNetworkConnected();
}

void MockClient::stop_loop() {
  this->stopped = true;
  if (this->thread.joinable()) {
    this->thread.join();
  }
  if (mockClientNetworkInterface->isNetworkConnected()) {
    mockClientNetworkInterface->disconnectNetwork();
  }

}

void MockClient::receive(uint8_t *data, uint16_t length) {
  if (mockClientNetworkInterface == nullptr) {
    throw std::invalid_argument("mockClientNetworkInterface not initialized");
  }
  if (receiver == nullptr) {
    throw std::invalid_argument("receiver not initialized");
  }
  if (this->networkAddress == nullptr) {
    throw std::invalid_argument("gateway networkAddress not set");
  }
  this->receiver->receive_any_message(data, length);
}

void MockClient::setForwarderAddress(device_address *forwarderAddress) {
  this->forwarderAddress = forwarderAddress;
}

void MockClient::setNetworkAddress(device_address *networkAddress) {
  this->networkAddress = networkAddress;
}

void MockClient::setMockClientNetworkInterface(MockClientNetworkInterface *mockClientNetworkInterface) {
  this->mockClientNetworkInterface = mockClientNetworkInterface;
}

int MockClient::send(uint8_t *data, uint16_t length) {
  if (mockClientNetworkInterface == nullptr) {
    throw std::invalid_argument("mockClientNetworkInterface not initialized");
  }
  if (receiver == nullptr) {
    throw std::invalid_argument("receiver not initialized");
  }
  if (this->networkAddress == nullptr) {
    throw std::invalid_argument("gateway networkAddress not set");
  }
  return mockClientNetworkInterface->sendToNetwork(forwarderAddress, data, length);
}

MockClientNetworkReceiverInterface *MockClient::getMockClientNetworkReceiverInterface() {
  return receiver;
}
void MockClient::setMockClientNetworkReceiverInterface(MockClientNetworkReceiverInterface *receiver) {
  this->receiver = receiver;
}

MockClient::MockClient(uint16_t identifier,
                       const device_address *networkAddress,
                       device_address *forwarderAddress,
                       MockClientNetworkInterface *mockClientNetworkInterface,
                       MockClientNetworkReceiverInterface *receiver) :
    identifier(identifier),
    networkAddress(networkAddress),
    forwarderAddress(forwarderAddress),
    mockClientNetworkInterface(mockClientNetworkInterface),
    receiver(receiver) {}

uint16_t MockClient::getIdentifier() {
  return identifier;
}
