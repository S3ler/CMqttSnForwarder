//
// Created by bele on 07.02.19.
//

#include "GatewayNetworkLooper.h"

bool GatewayNetworkLooper::startNetworkLoop(int (*ClientNetworkReceive)(MqttSnClientNetworkInterface *,
                                                                         MqttSnFixedSizeRingBuffer *,
                                                                         uint32_t,
                                                                         void *),
                                            MqttSnClientNetworkInterface *n,
                                            MqttSnFixedSizeRingBuffer *receiveBuffer,
                                            uint32_t timeout_ms,
                                            void *context) {
  if (ClientNetworkReceive == nullptr) {

  }
  if (n == nullptr) {

  }
  if (receiveBuffer == nullptr) {

  }
  if (timeout_ms < 0) {

  }

  this->clientNetworkReceive = ClientNetworkReceive;
  this->n = n;
  this->receiveBuffer = receiveBuffer;
  this->timeout_ms = timeout_ms;
  this->context = context;

  this->thread = std::thread(&GatewayNetworkLooper::networkLoop, this);
  this->thread.detach();
  return true;

}
void GatewayNetworkLooper::stopNetworkLoop() {
  stopped = true;

}
void GatewayNetworkLooper::networkLoop() {
  while (!stopped) {
    if(this->clientNetworkReceive(n, receiveBuffer, timeout_ms, context) < 0){
      break;
    }
  }
}
