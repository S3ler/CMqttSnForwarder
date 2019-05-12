//
// Created by bele on 27.02.19.
//

#ifndef CMQTTSNFORWARDER_MOCKFORWARDERGATEWAYNETWORKLOOPER_H
#define CMQTTSNFORWARDER_MOCKFORWARDERGATEWAYNETWORKLOOPER_H

#include <atomic>
#include <thread>
#include "../../../../network/MqttSnGatewayNetworkInterface.h"
class MockForwarderGatewayNetworkLooper {
 public:
  bool startNetworkLoop(
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
      void *context);

  void stopNetworkLoop();

  void networkLoop();

  bool pauseLoop();

  bool resumeLoop();

  std::atomic<bool> paused{false};
  std::atomic<bool> isPaused{false};

  std::atomic_bool stopped{false};
  std::atomic_bool isStopped{true};

 private:
  std::thread thread;

  int (*gatewayNetworkReceive)(MqttSnGatewayNetworkInterface *n,
                               MqttSnFixedSizeRingBuffer *receiveBuffer,
                               int32_t timeout_ms,
                               void *context);
  int (*gatewayNetworkSend)(MqttSnGatewayNetworkInterface *n,
                            MqttSnFixedSizeRingBuffer *receiveBuffer,
                            int32_t timeout_ms,
                            void *context);
  MqttSnGatewayNetworkInterface *n;
  MqttSnFixedSizeRingBuffer *receiveBuffer;
  MqttSnFixedSizeRingBuffer *sendBuffer;
  uint32_t timeout_ms;
  void *context;
};

#endif //CMQTTSNFORWARDER_MOCKFORWARDERGATEWAYNETWORKLOOPER_H
