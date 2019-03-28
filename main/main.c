#include <stdio.h>
#include "../forwarder/MqttSnFixedSizeRingBuffer.h"
#include "../forwarder/MqttSnForwarder.h"
#include "../forwarder/network/gateway/tcp/MqttSnGatewayTcpNetwork.h"
#include "../forwarder/network/client/tcp/MqttSnClientTcpNetwork.h"

#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

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

  signal(SIGINT, sig_handler);
  pthread_t mqttSnForwarder_thread;

  MqttSnForwarder mqttSnForwarder = {0};
  MqttSnGatewayTcpNetwork gatewayNetworkContext = {0};
  device_address forwarderGatewayNetworkAddress = {0};
  MqttSnClientTcpNetwork clientNetworkContext = {0};

  uint16_t gatewayNetworkPort = 8888;
  device_address
      mqttSnGatewayNetworkAddress =
      {127, 0, 0, 1, (uint8_t) (gatewayNetworkPort >> 8), (uint8_t) (gatewayNetworkPort >> 0)};
  GatewayNetworkInit(&mqttSnForwarder.gatewayNetwork,
                     &forwarderGatewayNetworkAddress,
                     &mqttSnGatewayNetworkAddress,
                     &gatewayNetworkContext,
                     GatewayLinuxTcpInit);

  uint16_t clientNetworkPort = 9999;
  device_address
      clientNetworkAddress = {127, 0, 0, 1, (uint8_t) (clientNetworkPort >> 8), (uint8_t) (clientNetworkPort >> 0)};
  ClientNetworkInit(&mqttSnForwarder.clientNetwork,
                    &clientNetworkAddress,
                    &clientNetworkContext,
                    ClientLinuxTcpInit);

  if (MqttSnForwarderInit(&mqttSnForwarder, &clientNetworkContext, &gatewayNetworkContext) != 0) {
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

