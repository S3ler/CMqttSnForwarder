//
// Created by bele on 27.02.19.
//

#ifndef CMQTTSNFORWARDER_MOCKFORWARDERGATEWAYNETWORKLOOPER_H
#define CMQTTSNFORWARDER_MOCKFORWARDERGATEWAYNETWORKLOOPER_H

#include <atomic>
#include <thread>
#include "../../../../forwarder/MqttSnGatewayNetworkInterface.h"
class MockForwarderGatewayNetworkLooper {
 public:
  bool startNetworkLoop(int (*gatewayNetworkReceive)(MqttSnGatewayNetworkInterface *,
                                                     MqttSnFixedSizeRingBuffer *,
                                                     uint32_t,
                                                     void *),
                          MqttSnGatewayNetworkInterface *n,
                          MqttSnFixedSizeRingBuffer *receiveBuffer,
                          uint32_t timeout_ms,
                          void *context);
  void stopNetworkLoop();
  void networkLoop();

  std::atomic_bool stopped{false};
  std::atomic_bool isStopped{true};
  std::thread thread;
 private:
  int (*gatewayNetworkReceive)(MqttSnGatewayNetworkInterface *n,
                            MqttSnFixedSizeRingBuffer *receiveBuffer,
                            uint32_t timeout_ms,
                            void * context);
  MqttSnGatewayNetworkInterface *n;
  MqttSnFixedSizeRingBuffer *receiveBuffer;
  uint32_t timeout_ms;
  void* context;
};

#endif //CMQTTSNFORWARDER_MOCKFORWARDERGATEWAYNETWORKLOOPER_H
