//
// Created by bele on 05.02.19.
//

#include "MockClient.h"

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

bool MockClient::isNetworkConnected() {
  return mockClientNetworkInterface->isNetworkConnected();
}

bool MockClient::start_loop() {
  if (!mockClientNetworkInterface->connectNetwork(forwarderAddress)) {
    return false;
  }
  this->thread = std::thread(&MockClient::loop, this);
  this->thread.detach();
  return true;
}

void MockClient::stop_loop() {
  this->stopped = true;
  if (this->thread.joinable()) {
    this->thread.join();
  }
}

int MockClient::send(const device_address *to, const uint8_t *data, uint16_t length) {
  return mockClientNetworkInterface->sendToNetwork(to, data, length);
}

int MockClient::send(ComparableClientMqttSnMessageData *comparableClientMqttSnMessageData) {
  return send(forwarderAddress, // TODO remove forwarderAddress
              &comparableClientMqttSnMessageData->data[0],
              comparableClientMqttSnMessageData->data_length);
}

MockClientNetworkReceiver *MockClient::getMockClientNetworkReceiver() {
  return receiver;
}

uint16_t MockClient::getIdentifier() {
  return identifier;
}

const std::atomic<bool> &MockClient::getDone() const {
  return done;
}

device_address *MockClient::getClientDeviceAddress() const {
  return networkAddress;
}
device_address MockClient::getForwarderDeviceAddress() const {
  return mockClientNetworkInterface->getForwarderDeviceAddress();
}
