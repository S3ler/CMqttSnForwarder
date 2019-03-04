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

  bool startNetworkLoop(int (*clientNetworkReceive)(MqttSnClientNetworkInterface *n,
                                                    MqttSnFixedSizeRingBuffer *receiveBuffer,
                                                    uint32_t timeout_ms,
                                                    void *context),
                        MqttSnClientNetworkInterface *n,
                        MqttSnFixedSizeRingBuffer *receiveBuffer,
                        uint32_t timeout_ms,
                        void *context);

  void stopNetworkLoop();

  void networkLoop();

  std::atomic<bool> stopped{false};
  std::atomic<bool> isStopped{true};
  std::thread thread;

 private:
  int (*clientNetworkReceive)(MqttSnClientNetworkInterface *,
                              MqttSnFixedSizeRingBuffer *,
                              uint32_t,
                              void *);
  MqttSnClientNetworkInterface *n;
  MqttSnFixedSizeRingBuffer *receiveBuffer;
  uint32_t timeout_ms;
  void *context;
};

#endif //CMQTTSNFORWARDER_NETWORKLOOPER_H
