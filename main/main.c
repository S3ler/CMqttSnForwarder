#include <stdio.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <getopt.h>
#include <string.h>
#include <network/client/udp/MqttSnClientUdpNetwork.h>
#include <MqttSnForwarder.h>
#include <network/gateway/udp/MqttSnGatewayUdpNetwork.h>
#include <network/iphelper/MqttSnIpNetworkHelper.h>
#include <network/gateway/plugin/gateway_plugin_interface.h>
#include <network/gateway/plugin/MqttSnGatewayPluginNetwork.h>
#include <network/gateway/tcp/MqttSnGatewayTcpNetwork.h>
#include <network/client/tcp/MqttSnClientTcpNetwork.h>
#include "forwarder_config.h"
#include "forwarder_starter.h"

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

  MqttSnForwarder mqttSnForwarder = {0};
  void *gatewayNetworkContext = NULL;
  void *clientNetworkContext = NULL;
  rc = start_forwarder(&fcfg, &mqttSnForwarder, gatewayNetworkContext, clientNetworkContext);
  forwarder_config_cleanup(&fcfg);

  return rc;
}
