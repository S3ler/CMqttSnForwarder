#include <stdio.h>
#include "../forwarder/MqttSnFixedSizeRingBuffer.h"
#include "../forwarder/MqttSnForwarder.h"
#include "../forwarder/network/gateway/tcp/MqttSnGatewayTcpNetwork.h"
#include "../forwarder/network/client/tcp/MqttSnClientTcpNetwork.h"

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
//https://stackoverflow.com/questions/4217037/catch-ctrl-c-in-c
static volatile sig_atomic_t keep_running = 1;

static void sig_handler(int _) {
  (void) _;
  keep_running = 0;
}

int main() {

  signal(SIGINT, sig_handler);

  MqttSnForwarder mqttSnForwarder;

  MqttSnGatewayTcpNetwork gatewayNetworkContext = {0};
  /*
  gatewayNetworkContext.ip = "127.0.0.1";
  gatewayNetworkContext.port = 8888;
  */
  device_address forwarderGatewayNetworkAddress = {0, 0, 0, 0, 0, 0};
  uint16_t gatewayNetworkPort = 8888;
  device_address
      mqttSnGatewayNetworkAddress =
      {127, 0, 0, 1, (uint8_t) gatewayNetworkPort >> 8, (uint8_t) gatewayNetworkPort >> 0};
  GatewayNetworkInit(&mqttSnForwarder.gatewayNetwork,
                     &forwarderGatewayNetworkAddress,
                     &mqttSnGatewayNetworkAddress,
                     &gatewayNetworkContext,
                     GatewayLinuxTcpInit);

  MqttSnClientTcpNetwork clientNetworkContext = {0};
  uint16_t clientNetworkPort = 9999;
  device_address
      clientNetworkAddress = {127, 0, 0, 1, (uint8_t) clientNetworkPort >> 8, (uint8_t) clientNetworkPort >> 0};
  ClientNetworkInit(&mqttSnForwarder.clientNetwork, &clientNetworkAddress, &clientNetworkContext, ClientLinuxTcpInit);

  MqttSnForwarderStart(&mqttSnForwarder, &clientNetworkContext, &gatewayNetworkContext, &keep_running);

  return EXIT_SUCCESS;
}

