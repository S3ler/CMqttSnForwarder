//
// Created by bele on 27.02.19.
//

#include "MockForwarderGatewayNetworkLooper.h"
bool MockForwarderGatewayNetworkLooper::startNetworkLoop(int (*gatewayNetworkReceive)(MqttSnGatewayNetworkInterface *,
                                                                                      MqttSnFixedSizeRingBuffer *,
                                                                                      uint32_t,
                                                                                      void *),
                                                         MqttSnGatewayNetworkInterface *n,
                                                         MqttSnFixedSizeRingBuffer *receiveBuffer,
                                                         uint32_t timeout_ms,
                                                         void *context)
                                                         {
                                                           if (gatewayNetworkReceive == nullptr) {
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
                                                           this->gatewayNetworkReceive = gatewayNetworkReceive;
                                                           this->n = n;
                                                           this->receiveBuffer = receiveBuffer;
                                                           this->timeout_ms = timeout_ms;
                                                           this->context = context;

                                                           this->thread = std::thread(&MockForwarderGatewayNetworkLooper::networkLoop, this);
                                                           this->thread.detach();
  return true;
}
void MockForwarderGatewayNetworkLooper::stopNetworkLoop() {
  stopped = true;
  isStopped = false;
}
void MockForwarderGatewayNetworkLooper::networkLoop() {
  while (!stopped) {
    if (this->gatewayNetworkReceive(n, receiveBuffer, timeout_ms, context) < 0) {
      break;
    }
  }
  isStopped = true;
}
