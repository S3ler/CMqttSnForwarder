//
// Created by bele on 07.02.19.
//

#include "ClientNetworkGatewayLooper.h"

bool ClientNetworkGatewayLooper::startNetworkLoop(
    int (*clientNetworkReceive)(MqttSnClientNetworkInterface *,
                                MqttSnFixedSizeRingBuffer *,
                                uint32_t,
                                void *),

    int (*clientNetworkSend)(MqttSnClientNetworkInterface *,
                             MqttSnFixedSizeRingBuffer *,
                             uint32_t,
                             void *),
    MqttSnClientNetworkInterface *n,
    MqttSnFixedSizeRingBuffer *receiveBuffer,
    MqttSnFixedSizeRingBuffer *sendBuffer,
    uint32_t timeout_ms,
    void *context) {

  if (clientNetworkReceive == nullptr || clientNetworkSend == nullptr) {
    return false;
  }
  if (n == nullptr) {
    return false;
  }
  if (receiveBuffer == nullptr) {
    return false;
  }
  if (sendBuffer == nullptr) {
    return false;
  }
  if (timeout_ms < 0) {
    return false;
  }

  this->clientNetworkReceive = clientNetworkReceive;
  this->clientNetworkSend = clientNetworkSend;

  this->receiveBuffer = receiveBuffer;
  this->sendBuffer = sendBuffer;

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
      while (paused & !stopped) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
      }
      isPaused = false;
      if (stopped) {
        break;
      }
    }
    if (clientNetworkReceive(n, receiveBuffer, timeout_ms, context) < 0) {
      break;
    }
    if (clientNetworkSend(n, sendBuffer, timeout_ms, context) < 0) {
      break;
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
