//
// Created by bele on 07.02.19.
//

#ifndef CMQTTSNFORWARDER_NETWORKLOOPER_H
#define CMQTTSNFORWARDER_NETWORKLOOPER_H

#include <cstdint>
#include <thread>
#include <atomic>
#include "../../../../../forwarder/MqttSnClientNetworkInterface.h"

class ClientNetworkGatewayLooper {
 public:

  bool startNetworkLoop(
      int (*clientNetworkReceive)(MqttSnClientNetworkInterface *,
                                  MqttSnFixedSizeRingBuffer *,
                                  uint32_t,
                                  void *),

      int (*client_network_send)(struct MqttSnClientNetworkInterface *,
                                 MqttSnFixedSizeRingBuffer *,
                                 uint32_t,
                                 void *context),

      MqttSnClientNetworkInterface *n,
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

  std::atomic<bool> stopped{false};
  std::atomic<bool> isStopped{true};
  std::thread thread;

 private:
  int (*clientNetworkReceive)(MqttSnClientNetworkInterface *,
                              MqttSnFixedSizeRingBuffer *,
                              uint32_t,
                              void *);
  int (*clientNetworkSend)(MqttSnClientNetworkInterface *,
                           MqttSnFixedSizeRingBuffer *,
                           uint32_t,
                           void *);
  MqttSnClientNetworkInterface *n;
  MqttSnFixedSizeRingBuffer *receiveBuffer;
  MqttSnFixedSizeRingBuffer *sendBuffer;
  uint32_t timeout_ms;
  void *context;


};

#endif //CMQTTSNFORWARDER_NETWORKLOOPER_H
