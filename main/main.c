#include <stdio.h>

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <network/client/udp/MqttSnClientUdpNetwork.h>
#include <MqttSnForwarder.h>
#include <network/gateway/udp/MqttSnGatewayUdpNetwork.h>
#include <ctype.h>
#include <getopt.h>
#include <network/iphelper/MqttSnIpNetworkHelper.h>
#include "forwarder_config.h"

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

  forwarder_config fcfg = {0};
  forwarder_config_init(&fcfg);
  int rc = process_forwarder_config_line(&fcfg, argc, argv);
  if (rc) {
    forwarder_config_cleanup(&fcfg);
    if (rc == 2) {
      print_usage();
    } else {
      fprintf(stderr, "\nUse 'cmqttsnforwarder --help' to see usage.\n");
    }
    return EXIT_SUCCESS;
  }

  device_address mqttSnGatewayNetworkAddress = {0};
  if (get_device_address_from_hostname(fcfg.mqtt_sn_gateway_host, &mqttSnGatewayNetworkAddress)) {
    fprintf(stderr, "Cannot resolve %s.\n", fcfg.mqtt_sn_gateway_host);
    return EXIT_FAILURE;
  }
  add_port_to_device_address(fcfg.mqtt_sn_gateway_port, &mqttSnGatewayNetworkAddress);

  device_address forwarderClientNetworkAddress = {0};
  if (fcfg.client_network_bind_address != NULL) {
    if (get_device_address_from_hostname(fcfg.client_network_bind_address, &forwarderClientNetworkAddress)) {
      fprintf(stderr, "Cannot resolve %s.\n", fcfg.client_network_bind_address);
      return EXIT_FAILURE;
    }
  }
  add_port_to_device_address(fcfg.client_network_bind_port, &forwarderClientNetworkAddress);

  device_address forwarderGatewayNetworkAddress = {0};
  if (fcfg.gateway_network_bind_address != NULL) {
    if (get_device_address_from_hostname(fcfg.gateway_network_bind_address, &forwarderGatewayNetworkAddress)) {
      fprintf(stderr, "Cannot resolve %s.\n", fcfg.client_network_bind_address);
      return EXIT_FAILURE;
    }
  }
  add_port_to_device_address(fcfg.gateway_network_bind_port, &forwarderGatewayNetworkAddress);

  MqttSnGatewayUdpNetwork udpGatewayNetworkContext = {0};
  MqttSnClientUdpNetwork udpClientNetworkContext = {0};

  signal(SIGINT, sig_handler);
  pthread_t mqttSnForwarder_thread;

  MqttSnForwarder mqttSnForwarder = {0};

  void *gatewayNetworkContext = NULL;
  void *clientNetworkContext = NULL;

  if (GatewayNetworkInit(&mqttSnForwarder.gatewayNetwork,
                         &mqttSnGatewayNetworkAddress,
                         &forwarderGatewayNetworkAddress,
                         &udpGatewayNetworkContext,
                         GatewayLinuxUdpInit)) {
    fprintf(stderr, "Error init gateway network\n");
    return EXIT_FAILURE;
  }
  gatewayNetworkContext = &udpGatewayNetworkContext;

  if(ClientNetworkInit(&mqttSnForwarder.clientNetwork,
                    &forwarderClientNetworkAddress,
                    &udpClientNetworkContext,
                    ClientLinuxUdpInit)){
    fprintf(stderr, "Error init client network\n");
    return EXIT_FAILURE;
  }
  clientNetworkContext = &udpClientNetworkContext;

  if (MqttSnForwarderInit(&mqttSnForwarder, clientNetworkContext, gatewayNetworkContext) != 0) {
    fprintf(stderr, "Error init mqtt-sn forwarder\n");
    MqttSnForwarderDeinit(&mqttSnForwarder);
    return EXIT_FAILURE;
  }

  if (pthread_create(&mqttSnForwarder_thread, NULL, inc_c, &mqttSnForwarder)) {
    fprintf(stderr, "Error creating thread\n");
    return EXIT_FAILURE;
  }

  if (
      pthread_join(mqttSnForwarder_thread, NULL)) {
    fprintf(stderr, "Error joining thread\n");
    return EXIT_FAILURE;
  }

  printf("MqttSnForwarder finished\n");
  return EXIT_SUCCESS;
}
