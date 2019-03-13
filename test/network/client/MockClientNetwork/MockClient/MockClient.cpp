//
// Created by bele on 05.02.19.
//

#include <stdexcept>
#include "MockClient.h"

void MockClient::loop() {
  done = false;
  while (!stopped & mockClientNetworkInterface->isNetworkConnected()) {
    if (mockClientNetworkInterface->loopNetwork(receiver) != 0) {
      break;
    }
  }
  mockClientNetworkInterface->disconnectNetwork();
  done = true;
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
  this->receiver->receive_any_message(nullptr, data, length);
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

int MockClient::send(CompareableMqttSnMessageData* compareableMqttSnMessageData){
  if (mockClientNetworkInterface == nullptr) {
    throw std::invalid_argument("mockClientNetworkInterface not initialized");
  }
  if (receiver == nullptr) {
    throw std::invalid_argument("receiver not initialized");
  }
  if (this->networkAddress == nullptr) {
    throw std::invalid_argument("gateway networkAddress not set");
  }
  return mockClientNetworkInterface->sendToNetwork(forwarderAddress,
      &compareableMqttSnMessageData->data[0],
      compareableMqttSnMessageData->data_length);
}


MockClientNetworkReceiver *MockClient::getMockClientNetworkReceiver() {
  return receiver;
}

MockClient::MockClient(uint16_t identifier,
                       device_address *networkAddress,
                       device_address *forwarderAddress,
                       MockClientNetworkInterface *mockClientNetworkInterface,
                       MockClientNetworkReceiver *receiver) :
    identifier(identifier),
    networkAddress(networkAddress),
    forwarderAddress(forwarderAddress),
    mockClientNetworkInterface(mockClientNetworkInterface),
    receiver(receiver) {}

uint16_t MockClient::getIdentifier() {
  return identifier;
}
const std::atomic<bool> &MockClient::getDone() const {
  return done;
}
device_address *MockClient::getNetworkAddress() const {
  return networkAddress;
}
