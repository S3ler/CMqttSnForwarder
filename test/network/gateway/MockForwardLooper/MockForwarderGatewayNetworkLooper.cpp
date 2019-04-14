//
// Created by bele on 27.02.19.
//

#include "MockForwarderGatewayNetworkLooper.h"

bool MockForwarderGatewayNetworkLooper::startNetworkLoop(
    int (*gatewayNetworkReceive)(MqttSnGatewayNetworkInterface *,
                                 MqttSnFixedSizeRingBuffer *,
                                 int32_t,
                                 void *),
    int (*gatewayNetworkSend)(MqttSnGatewayNetworkInterface *,
                              MqttSnFixedSizeRingBuffer *,
                              int32_t,
                              void *),
    MqttSnGatewayNetworkInterface *n,
    MqttSnFixedSizeRingBuffer *receiveBuffer,
    MqttSnFixedSizeRingBuffer *sendBuffer,
    uint32_t timeout_ms,
    void *context) {

  if (gatewayNetworkReceive == nullptr || gatewayNetworkSend == nullptr) {
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

  this->gatewayNetworkReceive = gatewayNetworkReceive;
  this->gatewayNetworkSend = gatewayNetworkSend;

  this->receiveBuffer = receiveBuffer;
  this->sendBuffer = sendBuffer;

  this->n = n;
  this->timeout_ms = timeout_ms;
  this->context = context;

  this->thread = std::thread(&MockForwarderGatewayNetworkLooper::networkLoop, this);
  this->thread.detach();
  return true;
}

void MockForwarderGatewayNetworkLooper::stopNetworkLoop() {
  stopped = true;
}

void MockForwarderGatewayNetworkLooper::networkLoop() {
  isStopped = false;
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
    if (gatewayNetworkReceive(n, receiveBuffer, timeout_ms, context) < 0) {
      break;
    }
    if (gatewayNetworkSend(n, sendBuffer, timeout_ms, context) < 0) {
      break;
    }
  }
  isStopped = true;
}

bool MockForwarderGatewayNetworkLooper::pauseLoop() {
  paused = true;
  while (!isPaused) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    if (isStopped) {
      break;
    }
  }
  return true;
}

bool MockForwarderGatewayNetworkLooper::resumeLoop() {
  paused = false;
  while (isPaused) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    if (isStopped) {
      break;
    }
  }
  return true;
}
