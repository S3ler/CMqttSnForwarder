//
// Created by bele on 07.02.19.
//

#ifndef CMQTTSNFORWARDER_NETWORKLOOPER_H
#define CMQTTSNFORWARDER_NETWORKLOOPER_H

#include <cstdint>
#include <thread>
#include <atomic>
#include "../../../../../network/MqttSnClientNetworkInterface.h"

class ClientNetworkGatewayLooper {
 public:

  bool startNetworkLoop(
      int (*clientNetworkReceive)(MqttSnClientNetworkInterface *,
                                  MqttSnFixedSizeRingBuffer *,
                                  int32_t,
                                  void *),
      int (*clientNetworkSend)(MqttSnClientNetworkInterface *,
                               MqttSnFixedSizeRingBuffer *,
                               int32_t,
                               void *context),
      MqttSnClientNetworkInterface *n,
      MqttSnFixedSizeRingBuffer *receiveBuffer,
      MqttSnFixedSizeRingBuffer *sendBuffer,
      int32_t timeout_ms,
      void *context);

  void stopNetworkLoop();

  void networkLoop();

  bool pauseLoop();

  bool resumeLoop();

  std::atomic<bool> paused{false};
  std::atomic<bool> isPaused{false};

  std::atomic<bool> stopped{false};
  std::atomic<bool> isStopped{true};

 private:
  std::thread thread;

  int (*clientNetworkReceive)(MqttSnClientNetworkInterface *,
                              MqttSnFixedSizeRingBuffer *,
                              int32_t,
                              void *);
  int (*clientNetworkSend)(MqttSnClientNetworkInterface *,
                           MqttSnFixedSizeRingBuffer *,
                           int32_t,
                           void *);
  MqttSnClientNetworkInterface *n;
  MqttSnFixedSizeRingBuffer *receiveBuffer;
  MqttSnFixedSizeRingBuffer *sendBuffer;
  uint32_t timeout_ms;
  void *context;

};

#endif //CMQTTSNFORWARDER_NETWORKLOOPER_H
