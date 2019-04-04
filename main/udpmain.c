#include <stdio.h>
#include "../forwarder/MqttSnFixedSizeRingBuffer.h"
#include "../forwarder/MqttSnForwarder.h"
#include "../forwarder/network/gateway/tcp/MqttSnGatewayTcpNetwork.h"
#include "../forwarder/network/gateway/udp/MqttSnGatewayUdpNetwork.h"
#include "../forwarder/network/client/tcp/MqttSnClientTcpNetwork.h"

#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <network/client/udp/MqttSnClientUdpNetwork.h>

static volatile sig_atomic_t keep_running = 1;

static void sig_handler(int _) {
  (void) _;
  keep_running = 0;
}

void *inc_c(void *mqttSnForwarder_ptr) {
  MqttSnForwarder *mqttSnForwarder = (MqttSnForwarder *) mqttSnForwarder_ptr;
  while ((MqttSnForwarderLoop(mqttSnForwarder) == 0) & keep_running) {}
  MqttSnForwarderDeinit(mqttSnForwarder);
  return NULL;
}

int main(int argc, char *argv[]) {
  MqttSnGatewayUdpNetwork udpGatewayNetworkContext = {0};
  MqttSnClientUdpNetwork udpClientNetworkContext = {0};

  signal(SIGINT, sig_handler);
  pthread_t mqttSnForwarder_thread;

  MqttSnForwarder mqttSnForwarder = {0};
  uint16_t mqttSnGatewayNetworkPort = 22222;
  device_address
      mqttSnGatewayNetworkAddress =
      {127, 0, 0, 1, (uint8_t) (mqttSnGatewayNetworkPort >> 8), (uint8_t) (mqttSnGatewayNetworkPort >> 0)};
  uint16_t forwarderGatewayNetworkPort = 8888;
  device_address
      forwarderGatewayNetworkAddress =
      {127, 0, 0, 1, (uint8_t) (forwarderGatewayNetworkPort >> 8), (uint8_t) (forwarderGatewayNetworkPort >> 0)};

  uint16_t forwarderClientNetworkPort = 9999;
  device_address
      forwarderClientNetworkAddress =
      {127, 0, 0, 1, (uint8_t) (forwarderClientNetworkPort >> 8), (uint8_t) (forwarderClientNetworkPort >> 0)};

  void *gatewayNetworkContext = NULL;
  void *clientNetworkContext = NULL;

  GatewayNetworkInit(&mqttSnForwarder.gatewayNetwork,
                     &mqttSnGatewayNetworkAddress,
                     &forwarderGatewayNetworkAddress,
                     &udpGatewayNetworkContext,
                     GatewayLinuxUdpInit);
  gatewayNetworkContext = &udpGatewayNetworkContext;

  ClientNetworkInit(&mqttSnForwarder.clientNetwork,
                    &forwarderClientNetworkAddress,
                    &udpClientNetworkContext,
                    ClientLinuxUdpInit);
  clientNetworkContext = &udpClientNetworkContext;

  if (MqttSnForwarderInit(&mqttSnForwarder, clientNetworkContext, gatewayNetworkContext) != 0) {
    fprintf(stderr,
            "Error init MqttSnForwarder\n");
    MqttSnForwarderDeinit(&mqttSnForwarder);
    return EXIT_FAILURE;
  }

  if (pthread_create(&mqttSnForwarder_thread, NULL, inc_c, &mqttSnForwarder)) {
    fprintf(stderr,
            "Error creating thread\n");
    return EXIT_FAILURE;
  }

  if (
      pthread_join(mqttSnForwarder_thread, NULL)) {
    fprintf(stderr,
            "Error joining thread\n");
    return EXIT_FAILURE;
  }

  printf("MqttSnForwarder finished\n");
  return EXIT_SUCCESS;
}

