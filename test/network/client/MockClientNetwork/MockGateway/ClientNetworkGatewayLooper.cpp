//
// Created by bele on 07.02.19.
//

#include "ClientNetworkGatewayLooper.h"

bool ClientNetworkGatewayLooper::startNetworkLoop(
    int (*clientNetworkReceive)(MqttSnClientNetworkInterface *,
                                MqttSnFixedSizeRingBuffer *,
                                uint32_t,
                                void *),

    int (*clientNetworkSend)(struct MqttSnClientNetworkInterface *,
                             MqttSnFixedSizeRingBuffer *,
                             uint32_t,
                             void *context),

    MqttSnClientNetworkInterface *n,
    MqttSnFixedSizeRingBuffer *receiveBuffer,
    MqttSnFixedSizeRingBuffer *sendBuffer,
    uint32_t timeout_ms,
    void *context) {

  if (clientNetworkReceive == nullptr && clientNetworkSend == nullptr) {
    return false;
  }
  if (n == nullptr) {
    return false;
  }
  if (receiveBuffer == nullptr) {
    return false;
  }
  if (timeout_ms < 0) {
    return false;
  }

  if (clientNetworkReceive != nullptr && clientNetworkSend == nullptr) {
    this->clientNetworkReceive = clientNetworkReceive;
    this->clientNetworkSend = nullptr;

    this->receiveBuffer = receiveBuffer;
    this->sendBuffer = nullptr;
  }
  if (clientNetworkReceive == nullptr && clientNetworkSend != nullptr) {
    this->clientNetworkReceive = nullptr;
    this->clientNetworkSend = clientNetworkSend;

    this->receiveBuffer = nullptr;
    this->sendBuffer = sendBuffer;
  }
  if (clientNetworkReceive != nullptr && clientNetworkSend != nullptr) {
    this->clientNetworkReceive = clientNetworkReceive;
    this->clientNetworkSend = clientNetworkSend;

    this->receiveBuffer = receiveBuffer;
    this->sendBuffer = sendBuffer;
  }

  this->n = n;
  this->timeout_ms = timeout_ms;
  this->context = context;

  this->thread = std::thread(&ClientNetworkGatewayLooper::networkLoop, this);
  this->thread.detach();
  return true;

}
void ClientNetworkGatewayLooper::stopNetworkLoop() {
  stopped = true;
  isStopped = false;
}
void ClientNetworkGatewayLooper::networkLoop() {
  while (!stopped) {
    if (paused) {
      isPaused = true;
      while (paused) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
      }
      isPaused = false;
    }

    if (clientNetworkReceive != nullptr) {
      if (clientNetworkReceive(n, receiveBuffer, timeout_ms, context) < 0) {
        break;
      }
    }
    if (clientNetworkSend != nullptr) {
      if (clientNetworkSend(n, sendBuffer, timeout_ms, context) < 0) {
        break;
      }
    }
  }
  isStopped = true;
}
bool ClientNetworkGatewayLooper::pauseLoop() {
  paused = true;
  while (!isPaused) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  return true;
}
bool ClientNetworkGatewayLooper::resumeLoop() {
  paused = false;
  while (isPaused) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  return true;
}
