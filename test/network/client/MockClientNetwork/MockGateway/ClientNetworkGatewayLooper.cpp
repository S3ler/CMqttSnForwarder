//
// Created by bele on 07.02.19.
//

#include "ClientNetworkGatewayLooper.h"

bool ClientNetworkGatewayLooper::startNetworkLoop(int (*clientNetworkReceive)(MqttSnClientNetworkInterface *,
                                                                         MqttSnFixedSizeRingBuffer *,
                                                                         uint32_t,
                                                                         void *),
                                            MqttSnClientNetworkInterface *n,
                                            MqttSnFixedSizeRingBuffer *receiveBuffer,
                                            uint32_t timeout_ms,
                                            void *context) {
  if (clientNetworkReceive == nullptr) {
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

  this->clientNetworkReceive = clientNetworkReceive;
  this->n = n;
  this->receiveBuffer = receiveBuffer;
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
    if(this->clientNetworkReceive(n, receiveBuffer, timeout_ms, context) < 0){
      break;
    }
  }
  isStopped = true;
}
