//
// Created by SomeDude on 07.04.2019.
//
#include "forwarder_starter.h"
#include <stdlib.h>
#include <network/plugin/plugin_interface.h>
#include <network/plugin/MqttSnGatewayPluginNetwork.h>
#include <string.h>
#include <network/gateway/tcp/MqttSnGatewayTcpNetwork.h>
#include <network/iphelper/MqttSnIpNetworkHelper.h>
#include <network/gateway/udp/MqttSnGatewayUdpNetwork.h>
#include <network/client/udp/MqttSnClientUdpNetwork.h>
#include <network/client/tcp/MqttSnClientTcpNetwork.h>
#include <errno.h>

int convert_string_to_device_address(const char *string, device_address *address) {
  char *cp_string = strdup(string);
  char *token = strtok(cp_string, ".");
  int i = 0;
  int rc = 0;
  while (token != NULL) {
    char *end_prt;
    long int n = strtol(token, &end_prt, 10);
    if (errno == EOVERFLOW) {
      rc = -1;
      break;
    }
    if (*end_prt != '\0') {
      // no conversion performed
      rc = -1;
      break;
    }
    if (n > UINT8_MAX || n < 0) {
      rc = -1;
      break;
    }
    // address->bytes[i++] = atoi(token);
    address->bytes[i++] = n;
    token = strtok(NULL, ".");
  }

  free(cp_string);
  return rc;
}

int start_gateway_plugin(const forwarder_config *fcfg,
                         MqttSnForwarder *mqttSnForwarder,
                         void *gatewayNetworkContext,
                         void *clientNetworkContext) {

  device_address mqttSnGatewayNetworkAddress = {0};
  device_address forwarderGatewayNetworkAddress = {0};

  if (convert_string_to_device_address(fcfg->mqtt_sn_gateway_host, &mqttSnGatewayNetworkAddress)) {
    if (get_device_address_from_hostname(fcfg->mqtt_sn_gateway_host, &mqttSnGatewayNetworkAddress)) {
      fprintf(stderr, "Cannot convert or resolve %s to mqtt-sn gateway address.\n",
              fcfg->mqtt_sn_gateway_host);
      return EXIT_FAILURE;
    }
  }
  if (fcfg->mqtt_sn_gateway_port < 0 && fcfg->mqtt_sn_gateway_port != -1) {
    fprintf(stderr, "Error: Invalid port given: %d\n", fcfg->mqtt_sn_gateway_port);
    return EXIT_FAILURE;
  }
  if (fcfg->mqtt_sn_gateway_port > -1) {
    add_port_to_device_address(fcfg->mqtt_sn_gateway_port, &mqttSnGatewayNetworkAddress);
  }

  if (fcfg->gateway_network_bind_address != NULL) {
    if (convert_string_to_device_address(fcfg->gateway_network_bind_address, &forwarderGatewayNetworkAddress)) {
      if (get_device_address_from_hostname(fcfg->gateway_network_bind_address, &forwarderGatewayNetworkAddress)) {
        fprintf(stderr, "Cannot convert or resolve %s to gateway network address.\n",
                fcfg->gateway_network_bind_address);
        return EXIT_FAILURE;
      }
    }
  }
  if (fcfg->gateway_network_bind_port < 0 && fcfg->gateway_network_bind_port != -1) {
    fprintf(stderr, "Error: Invalid port given: %d\n", fcfg->gateway_network_bind_port);
    return EXIT_FAILURE;
  }
  if (fcfg->gateway_network_bind_port > -1) {
    add_port_to_device_address(fcfg->gateway_network_bind_port, &mqttSnGatewayNetworkAddress);
  }

  const plugin_device_address pluginMqttSnGatewayNetworkAddress = {
      .bytes = mqttSnGatewayNetworkAddress.bytes,
      .length = sizeof(device_address)};

  const plugin_device_address pluginForwarderGatewayNetworkAddress = {
      .bytes = forwarderGatewayNetworkAddress.bytes,
      .length = sizeof(device_address)};

  plugin_config plugin_cfg = {
      .plugin_path = fcfg->gateway_network_plugin_path,
      .protocol = fcfg->gateway_network_protocol,
      .mqtt_sn_gateway_network_address = &pluginMqttSnGatewayNetworkAddress,
      .forwarder_gateway_network_address = &pluginForwarderGatewayNetworkAddress,
      .forwarder_device_address_length = CMQTTSNFORWARDER_DEVICE_ADDRESS_LENGTH,
      .forwarder_maximum_message_length = CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH};

  MqttSnGatewayPluginContext gatewayPluginContext = {
      .plugin_path = fcfg->gateway_network_plugin_path,
      .dl_handle = NULL,
      .plugin_network_init = NULL,
      .plugin_network_disconnect = NULL,
      .plugin_network_connect = NULL,
      .plugin_network_receive = NULL,
      .plugin_network_send = NULL,
      .plugin_context = NULL,
      .plugin_cfg = &plugin_cfg
  };
  if (GatewayNetworkInit(&mqttSnForwarder->gatewayNetwork,
                         &mqttSnGatewayNetworkAddress,
                         &forwarderGatewayNetworkAddress,
                         &gatewayPluginContext,
                         GatewayLinuxPluginInit)) {
    fprintf(stderr, "Error init gateway network\n");
    return EXIT_FAILURE;
  }
  gatewayNetworkContext = &gatewayPluginContext;

  return start_forwarder(fcfg, mqttSnForwarder, gatewayNetworkContext, clientNetworkContext);
}

int start_gateway_tcp(const forwarder_config *fcfg,
                      MqttSnForwarder *mqttSnForwarder,
                      void *gatewayNetworkContext,
                      void *clientNetworkContext) {

  device_address mqttSnGatewayNetworkAddress = {0};
  device_address forwarderGatewayNetworkAddress = {0};
  MqttSnGatewayTcpNetwork tcpGatewayNetworkContext = {0};

  if (get_device_address_from_hostname(fcfg->mqtt_sn_gateway_host, &mqttSnGatewayNetworkAddress)) {
    fprintf(stderr, "Cannot resolve %s.\n", fcfg->mqtt_sn_gateway_host);
    return EXIT_FAILURE;
  }
  if (fcfg->mqtt_sn_gateway_port < 1 || fcfg->mqtt_sn_gateway_port > 65535) {
    fprintf(stderr, "Error: Invalid port given: %d\n", fcfg->mqtt_sn_gateway_port);
    return EXIT_FAILURE;
  }
  add_port_to_device_address(fcfg->mqtt_sn_gateway_port, &mqttSnGatewayNetworkAddress);

  if (fcfg->gateway_network_bind_address != NULL) {
    if (get_device_address_from_hostname(fcfg->gateway_network_bind_address, &forwarderGatewayNetworkAddress)) {
      fprintf(stderr, "Cannot resolve %s.\n", fcfg->gateway_network_bind_address);
      return EXIT_FAILURE;
    }
  }
  if (fcfg->gateway_network_bind_port < 1 || fcfg->gateway_network_bind_port > 65535) {
    fprintf(stderr, "Error: Invalid port given: %d\n", fcfg->gateway_network_bind_port);
    return EXIT_FAILURE;
  }
  add_port_to_device_address(fcfg->gateway_network_bind_port, &forwarderGatewayNetworkAddress);

  if (GatewayNetworkInit(&mqttSnForwarder->gatewayNetwork,
                         &mqttSnGatewayNetworkAddress,
                         &forwarderGatewayNetworkAddress,
                         &tcpGatewayNetworkContext,
                         GatewayLinuxTcpInit)) {
    fprintf(stderr, "Error init client network\n");
    return EXIT_FAILURE;
  }
  gatewayNetworkContext = &tcpGatewayNetworkContext;

  return start_forwarder(fcfg, mqttSnForwarder, gatewayNetworkContext, clientNetworkContext);
}

int start_gateway_udp(const forwarder_config *fcfg,
                      MqttSnForwarder *mqttSnForwarder,
                      void *gatewayNetworkContext,
                      void *clientNetworkContext) {

  device_address mqttSnGatewayNetworkAddress = {0};
  device_address forwarderGatewayNetworkAddress = {0};
  MqttSnGatewayUdpNetwork udpGatewayNetworkContext = {0};

  if (get_device_address_from_hostname(fcfg->mqtt_sn_gateway_host, &mqttSnGatewayNetworkAddress)) {
    fprintf(stderr, "Cannot resolve %s.\n", fcfg->mqtt_sn_gateway_host);
    return EXIT_FAILURE;
  }
  if (fcfg->mqtt_sn_gateway_port < 1 || fcfg->mqtt_sn_gateway_port > 65535) {
    fprintf(stderr, "Error: Invalid port given: %d\n", fcfg->mqtt_sn_gateway_port);
    return EXIT_FAILURE;
  }
  add_port_to_device_address(fcfg->mqtt_sn_gateway_port, &mqttSnGatewayNetworkAddress);

  if (fcfg->gateway_network_bind_address != NULL) {
    if (get_device_address_from_hostname(fcfg->gateway_network_bind_address, &forwarderGatewayNetworkAddress)) {
      fprintf(stderr, "Cannot resolve %s.\n", fcfg->gateway_network_bind_address);
      return EXIT_FAILURE;
    }
  }
  if (fcfg->gateway_network_bind_port < 1 || fcfg->gateway_network_bind_port > 65535) {
    fprintf(stderr, "Error: Invalid port given: %d\n", fcfg->gateway_network_bind_port);
    return EXIT_FAILURE;
  }
  add_port_to_device_address(fcfg->gateway_network_bind_port, &forwarderGatewayNetworkAddress);

  if (GatewayNetworkInit(&mqttSnForwarder->gatewayNetwork,
                         &mqttSnGatewayNetworkAddress,
                         &forwarderGatewayNetworkAddress,
                         &udpGatewayNetworkContext,
                         GatewayLinuxUdpInit)) {
    fprintf(stderr, "Error init gateway network\n");
    return EXIT_FAILURE;
  }
  gatewayNetworkContext = &udpGatewayNetworkContext;

  return start_forwarder(fcfg, mqttSnForwarder, gatewayNetworkContext, clientNetworkContext);
}

int start_client_plugin(const forwarder_config *fcfg,
                        MqttSnForwarder *mqttSnForwarder,
                        void *gatewayNetworkContext,
                        void *clientNetworkContext) {

  fprintf(stderr, "Client network plugin is notimplemented yet.\n");
  return EXIT_FAILURE;

  // TODO
  // return start_forwarder(fcfg, mqttSnForwarder, gatewayNetworkContext, clientNetworkContext);
}

int start_client_tcp(const forwarder_config *fcfg,
                     MqttSnForwarder *mqttSnForwarder,
                     void *gatewayNetworkContext,
                     void *clientNetworkContext) {

  device_address forwarderClientNetworkAddress = {0};
  MqttSnClientTcpNetwork tcpClientNetworkContext = {0};

  if (fcfg->client_network_bind_address != NULL) {
    if (get_device_address_from_hostname(fcfg->client_network_bind_address, &forwarderClientNetworkAddress)) {
      fprintf(stderr, "Cannot resolve %s.\n", fcfg->client_network_bind_address);
      return EXIT_FAILURE;
    }
  }
  if (fcfg->client_network_bind_port < 1 || fcfg->client_network_bind_port > 65535) {
    fprintf(stderr, "Error: Invalid port given: %d\n", fcfg->client_network_bind_port);
    return EXIT_FAILURE;
  }
  add_port_to_device_address(fcfg->client_network_bind_port, &forwarderClientNetworkAddress);

  if (ClientNetworkInit(&mqttSnForwarder->clientNetwork,
                        &forwarderClientNetworkAddress,
                        &tcpClientNetworkContext,
                        ClientLinuxTcpInit)) {
    fprintf(stderr, "Error init client network\n");
    return EXIT_FAILURE;
  }
  clientNetworkContext = &tcpClientNetworkContext;

  return start_forwarder(fcfg, mqttSnForwarder, gatewayNetworkContext, clientNetworkContext);
}
int start_client_udp(const forwarder_config *fcfg,
                     MqttSnForwarder *mqttSnForwarder,
                     void *gatewayNetworkContext,
                     void *clientNetworkContext) {

  device_address forwarderClientNetworkAddress = {0};
  MqttSnClientUdpNetwork udpClientNetworkContext = {0};

  if (fcfg->client_network_bind_address != NULL) {
    if (get_device_address_from_hostname(fcfg->client_network_bind_address, &forwarderClientNetworkAddress)) {
      fprintf(stderr, "Cannot resolve %s.\n", fcfg->client_network_bind_address);
      return EXIT_FAILURE;
    }
  }
  add_port_to_device_address(fcfg->client_network_bind_port, &forwarderClientNetworkAddress);

  if (ClientNetworkInit(&mqttSnForwarder->clientNetwork,
                        &forwarderClientNetworkAddress,
                        &udpClientNetworkContext,
                        ClientLinuxUdpInit)) {
    fprintf(stderr, "Error init client network\n");
    return EXIT_FAILURE;
  }
  clientNetworkContext = &udpClientNetworkContext;

  return start_forwarder(fcfg, mqttSnForwarder, gatewayNetworkContext, clientNetworkContext);
}

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

int start_forwarder(const forwarder_config *fcfg,
                    MqttSnForwarder *mqttSnForwarder,
                    void *gatewayNetworkContext,
                    void *clientNetworkContext) {

  if ((gatewayNetworkContext == NULL && clientNetworkContext != NULL)
      || (gatewayNetworkContext == NULL && clientNetworkContext == NULL)) {
    if (fcfg->gateway_network_plugin_path != NULL) {
      return start_gateway_plugin(fcfg, mqttSnForwarder, gatewayNetworkContext, clientNetworkContext);
    }
    if (!strncasecmp(fcfg->gateway_network_protocol, "udp", 3)) {
      return start_gateway_udp(fcfg, mqttSnForwarder, gatewayNetworkContext, clientNetworkContext);
    }
    if (!strncasecmp(fcfg->gateway_network_protocol, "tcp", 3)) {
      return start_gateway_tcp(fcfg, mqttSnForwarder, gatewayNetworkContext, clientNetworkContext);
    }
    fprintf(stderr, "Error init gateway network no protocol or plugin\n");
    return EXIT_FAILURE;
  }

  if (gatewayNetworkContext != NULL && clientNetworkContext == NULL) {
    if (fcfg->client_network_plugin_path != NULL) {
      return start_client_plugin(fcfg, mqttSnForwarder, gatewayNetworkContext, clientNetworkContext);
    }
    if (!strncasecmp(fcfg->client_network_protocol, "udp", 3)) {
      return start_client_udp(fcfg, mqttSnForwarder, gatewayNetworkContext, clientNetworkContext);
    }
    if (!strncasecmp(fcfg->client_network_protocol, "tcp", 3)) {
      return start_client_tcp(fcfg, mqttSnForwarder, gatewayNetworkContext, clientNetworkContext);
    }
    fprintf(stderr, "Error init gateway network no protocol or plugin\n");
    return EXIT_FAILURE;
  }

  mqttSnForwarder->clientNetworkSendTimeout = fcfg->client_network_send_timeout;
  mqttSnForwarder->clientNetworkReceiveTimeout = fcfg->client_network_receive_timeout;
  mqttSnForwarder->gatewayNetworkSendTimeout = fcfg->gateway_network_send_timeout;
  mqttSnForwarder->gatewayNetworkReceiveTimeout = fcfg->gateway_network_receive_timeout;

  if (MqttSnForwarderInit(mqttSnForwarder, clientNetworkContext, gatewayNetworkContext) != 0) {
    fprintf(stderr, "Error init mqtt-sn forwarder\n");
    MqttSnForwarderDeinit(mqttSnForwarder);
    return EXIT_FAILURE;
  }
  signal(SIGINT, sig_handler);
  pthread_t mqttSnForwarder_thread;
  if (pthread_create(&mqttSnForwarder_thread, NULL, inc_c, mqttSnForwarder)) {
    fprintf(stderr, "Error creating thread\n");
    return EXIT_FAILURE;
  }
  if (pthread_join(mqttSnForwarder_thread, NULL)) {
    fprintf(stderr, "Error joining thread\n");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
// TODO check every value here again...
