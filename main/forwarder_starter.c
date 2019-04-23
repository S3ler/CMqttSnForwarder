//
// Created by SomeDude on 07.04.2019.
//
#include "forwarder_starter.h"
#include <MqttSnForwarderLogging.h>
#include <network/shared/ip/MqttSnIpNetworkHelper.h>
#include <MqttSnGatewayPluginNetwork.h>
#include <MqttSnGatewayTcpNetwork.h>
#include <MqttSnGatewayUdpNetwork.h>
#include <MqttSnClientPluginNetwork.h>
#include <MqttSnClientTcpNetwork.h>
#include <MqttSnClientUdpNetwork.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <bits/types/sig_atomic_t.h>
#include <signal.h>
#include <pthread.h>

int convert_string_to_device_address(const char *string, device_address *address) {
  char *cp_string = strdup(string);
  char *token = strtok(cp_string, ".");
  size_t i = 0;
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
    if (i + 1 > sizeof(device_address)) {
      // given string address is too long
      rc = -1;
      break;
    }
    address->bytes[i++] = n;
    token = strtok(NULL, ".");
  }

  free(cp_string);
  return rc;
}

int convert_hostname_port_to_device_address(const char *hostname,
                                            int port,
                                            device_address *address,
                                            const char *address_name) {
  if (hostname == NULL) {
    memset(address, 0, sizeof(device_address));
  } else {
    if (convert_string_to_device_address(hostname, address)) {
      if (get_device_address_from_hostname(hostname, address)) {
        fprintf(stderr, "Cannot convert or resolve %s to %s network address.\n",
                hostname, address_name);
        return EXIT_FAILURE;
      }
    }
  }

  if (port < -1 || port > 65535) {
    fprintf(stderr, "Error: Invalid port given: %d\n", port);
    return EXIT_FAILURE;
  }
  if (port > -1) {
    add_port_to_device_address(port, address);
  }
  return EXIT_SUCCESS;
}

#ifdef WITH_PLUGIN
int start_gateway_plugin(const forwarder_config *fcfg,
                         MqttSnForwarder *mqttSnForwarder,
                         void *gatewayNetworkContext,
                         void *clientNetworkContext) {

  device_address mqttSnGatewayNetworkAddress = {0};
  device_address forwarderGatewayNetworkAddress = {0};

  if (convert_hostname_port_to_device_address(fcfg->mqtt_sn_gateway_host,
                                              fcfg->mqtt_sn_gateway_port,
                                              &mqttSnGatewayNetworkAddress,
                                              "mqtt-sn gateway")) {
    return EXIT_FAILURE;
  }
  if (convert_hostname_port_to_device_address(fcfg->gateway_network_bind_address,
                                              fcfg->gateway_network_bind_port,
                                              &forwarderGatewayNetworkAddress,
                                              "gateway")) {
    return EXIT_FAILURE;
  }

  const gateway_plugin_device_address pluginMqttSnGatewayNetworkAddress = {
      .bytes = mqttSnGatewayNetworkAddress.bytes,
      .length = sizeof(device_address)};

  const gateway_plugin_device_address pluginForwarderGatewayNetworkAddress = {
      .bytes = forwarderGatewayNetworkAddress.bytes,
      .length = sizeof(device_address)};

  gateway_plugin_config plugin_cfg = {
      .plugin_path = fcfg->gateway_network_plugin_path,
      .protocol = fcfg->gateway_network_protocol,
      .mqtt_sn_gateway_network_address = &pluginMqttSnGatewayNetworkAddress,
      .forwarder_gateway_network_address = &pluginForwarderGatewayNetworkAddress,
      .gateway_plugin_device_address_length = CMQTTSNFORWARDER_DEVICE_ADDRESS_LENGTH,
      .forwarder_maximum_message_length = CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH};

  MqttSnGatewayPluginContext gatewayPluginContext = {
      .dl_handle = NULL,
      .plugin_network_init = NULL,
      .plugin_network_disconnect = NULL,
      .plugin_network_connect = NULL,
      .plugin_network_receive = NULL,
      .plugin_network_send = NULL,
      .plugin_context = NULL,
      .plugin_cfg = &plugin_cfg
  };

#ifdef WITH_LOGGING
  // logger only for ClientNetworkInit
  MqttSnLogger logger = {0};
  if (MqttSnLoggerInit(&logger, fcfg->log_lvl) != 0) {
    MqttSnLoggerDeinit(&logger);
    return -1;
  }
  mqttSnForwarder->gatewayNetwork.logger = &logger;
#endif

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
#endif

#ifdef WITH_LINUX_GATEWAY_NETWORK_TCP
int start_gateway_tcp(const forwarder_config *fcfg,
                      MqttSnForwarder *mqttSnForwarder,
                      void *gatewayNetworkContext,
                      void *clientNetworkContext) {

  device_address mqttSnGatewayNetworkAddress = {0};
  device_address forwarderGatewayNetworkAddress = {0};
  MqttSnGatewayTcpNetwork tcpGatewayNetworkContext = {0};

  if (convert_hostname_port_to_device_address(fcfg->mqtt_sn_gateway_host,
                                              fcfg->mqtt_sn_gateway_port,
                                              &mqttSnGatewayNetworkAddress,
                                              "mqtt-sn gateway")) {
    return EXIT_FAILURE;
  }
  if (convert_hostname_port_to_device_address(fcfg->gateway_network_bind_address,
                                              fcfg->gateway_network_bind_port,
                                              &forwarderGatewayNetworkAddress,
                                              "gateway")) {
    return EXIT_FAILURE;
  }

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
#endif

#ifdef WITH_LINUX_GATEWAY_NETWORK_UDP
int start_gateway_udp(const forwarder_config *fcfg,
                      MqttSnForwarder *mqttSnForwarder,
                      void *gatewayNetworkContext,
                      void *clientNetworkContext) {

  device_address mqttSnGatewayNetworkAddress = {0};
  device_address forwarderGatewayNetworkAddress = {0};
  MqttSnGatewayUdpNetwork udpGatewayNetworkContext = {0};

  if (convert_hostname_port_to_device_address(fcfg->mqtt_sn_gateway_host,
                                              fcfg->mqtt_sn_gateway_port,
                                              &mqttSnGatewayNetworkAddress,
                                              "mqtt-sn gateway")) {
    return EXIT_FAILURE;
  }
  if (convert_hostname_port_to_device_address(fcfg->gateway_network_bind_address,
                                              fcfg->gateway_network_bind_port,
                                              &forwarderGatewayNetworkAddress,
                                              "gateway")) {
    return EXIT_FAILURE;
  }

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
#endif

#ifdef WITH_PLUGIN
int start_client_plugin(const forwarder_config *fcfg,
                        MqttSnForwarder *mqttSnForwarder,
                        void *gatewayNetworkContext,
                        void *clientNetworkContext) {

  device_address mqttSnGatewayNetworkAddress = {0};
  device_address forwarderClientNetworkAddress = {0};

  if (convert_hostname_port_to_device_address(fcfg->mqtt_sn_gateway_host,
                                              fcfg->mqtt_sn_gateway_port,
                                              &mqttSnGatewayNetworkAddress,
                                              "mqtt-sn gateway")) {
    return EXIT_FAILURE;
  }

  if (convert_hostname_port_to_device_address(fcfg->client_network_bind_address,
                                              fcfg->client_network_bind_port,
                                              &forwarderClientNetworkAddress,
                                              "client")) {
    return EXIT_FAILURE;
  }

  const client_plugin_device_address pluginMqttSnForwarderNetworkAddress = {
      .bytes = forwarderClientNetworkAddress.bytes,
      .length = sizeof(device_address)};

  client_plugin_config plugin_cfg = {
      .plugin_path = fcfg->client_network_plugin_path,
      .protocol = fcfg->client_network_protocol,
      .forwarder_client_network_address = &pluginMqttSnForwarderNetworkAddress,
      .client_plugin_device_address_length = CMQTTSNFORWARDER_DEVICE_ADDRESS_LENGTH,
      .forwarder_maximum_message_length = CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH};

  MqttSnClientPluginContext clientPluginContext = {
      .dl_handle = NULL,
      .plugin_network_init = NULL,
      .plugin_network_disconnect = NULL,
      .plugin_network_connect = NULL,
      .plugin_network_receive = NULL,
      .plugin_network_send = NULL,
      .plugin_context = NULL,
      .plugin_cfg = &plugin_cfg
  };

#ifdef WITH_LOGGING
  // logger only for ClientNetworkInit
  MqttSnLogger logger = {0};
  if (MqttSnLoggerInit(&logger, fcfg->log_lvl) != 0) {
    MqttSnLoggerDeinit(&logger);
    return -1;
  }
  mqttSnForwarder->clientNetwork.logger = &logger;
#endif

  if (ClientNetworkInit(&mqttSnForwarder->clientNetwork,
                        &mqttSnGatewayNetworkAddress,
                        &forwarderClientNetworkAddress,
                        &clientPluginContext,
                        ClientLinuxPluginInit)) {
    fprintf(stderr, "Error init client network\n");
    return EXIT_FAILURE;
  }
  clientNetworkContext = &clientPluginContext;

  return start_forwarder(fcfg, mqttSnForwarder, gatewayNetworkContext, clientNetworkContext);

}
#endif

#ifdef WITH_LINUX_CLIENT_NETWORK_TCP
int start_client_tcp(const forwarder_config *fcfg,
                     MqttSnForwarder *mqttSnForwarder,
                     void *gatewayNetworkContext,
                     void *clientNetworkContext) {

  device_address mqttSnGatewayNetworkAddress = {0};
  device_address forwarderClientNetworkAddress = {0};
  MqttSnClientTcpNetwork tcpClientNetworkContext = {0};

  if (convert_hostname_port_to_device_address(fcfg->mqtt_sn_gateway_host,
                                              fcfg->mqtt_sn_gateway_port,
                                              &mqttSnGatewayNetworkAddress,
                                              "mqtt-sn gateway")) {
    return EXIT_FAILURE;
  }

  if (convert_hostname_port_to_device_address(fcfg->client_network_bind_address,
                                              fcfg->client_network_bind_port,
                                              &forwarderClientNetworkAddress,
                                              "client")) {
    return EXIT_FAILURE;
  }

  if (ClientNetworkInit(&mqttSnForwarder->clientNetwork,
                        &mqttSnGatewayNetworkAddress,
                        &forwarderClientNetworkAddress,
                        &tcpClientNetworkContext,
                        ClientLinuxTcpInit)) {
    fprintf(stderr, "Error init client network\n");
    return EXIT_FAILURE;
  }
  clientNetworkContext = &tcpClientNetworkContext;

  return start_forwarder(fcfg, mqttSnForwarder, gatewayNetworkContext, clientNetworkContext);
}
#endif

#ifdef WITH_LINUX_CLIENT_NETWORK_UDP
int start_client_udp(const forwarder_config *fcfg,
                     MqttSnForwarder *mqttSnForwarder,
                     void *gatewayNetworkContext,
                     void *clientNetworkContext) {

  device_address forwarderClientNetworkAddress = {0};
  device_address mqttSnGatewayNetworkAddress = {0};
  MqttSnClientUdpNetwork udpClientNetworkContext = {0};

  if (convert_hostname_port_to_device_address(fcfg->mqtt_sn_gateway_host,
                                              fcfg->mqtt_sn_gateway_port,
                                              &mqttSnGatewayNetworkAddress,
                                              "mqtt-sn gateway")) {
    return EXIT_FAILURE;
  }

  if (convert_hostname_port_to_device_address(fcfg->client_network_bind_address,
                                              fcfg->client_network_bind_port,
                                              &forwarderClientNetworkAddress,
                                              "client")) {
    return EXIT_FAILURE;
  }

  if (ClientNetworkInit(&mqttSnForwarder->clientNetwork,
                        &mqttSnGatewayNetworkAddress,
                        &forwarderClientNetworkAddress,
                        &udpClientNetworkContext,
                        ClientLinuxUdpInit)) {
    fprintf(stderr, "Error init client network\n");
    return EXIT_FAILURE;
  }
  clientNetworkContext = &udpClientNetworkContext;

  return start_forwarder(fcfg, mqttSnForwarder, gatewayNetworkContext, clientNetworkContext);
}
#endif

static volatile sig_atomic_t keep_running = 1;

static void sig_handler(int _) {
  (void) _;
  keep_running = 0;
}

void *inc_c(void *mqttSnForwarderFcfgPtr_ptr) {
  MqttSnForwarder_fcfg_ptr *mqttSnForwarderFcfgPtr = (MqttSnForwarder_fcfg_ptr *) mqttSnForwarderFcfgPtr_ptr;
  MqttSnForwarder *mqttSnForwarder = mqttSnForwarderFcfgPtr->mqttSnForwarder_ptr;

  while ((MqttSnForwarderLoop(mqttSnForwarder) == 0) & keep_running) {}

#ifdef WITH_LOGGING
  const forwarder_config *fcfg = mqttSnForwarderFcfgPtr->fcfg_ptr;
  if (log_forwarder_terminated(&mqttSnForwarder->logger,
                               fcfg->version,
                               fcfg->major,
                               fcfg->minor,
                               fcfg->tweak)) {
    return (void *) EXIT_FAILURE;
  }
#endif

  MqttSnForwarderDeinit(mqttSnForwarder);
  return (void *) EXIT_SUCCESS;
}

int start_forwarder(const forwarder_config *fcfg,
                    MqttSnForwarder *mqttSnForwarder,
                    void *gatewayNetworkContext,
                    void *clientNetworkContext) {

  if ((gatewayNetworkContext == NULL && clientNetworkContext != NULL)
      || (gatewayNetworkContext == NULL && clientNetworkContext == NULL)) {

#ifdef WITH_PLUGIN
    if (fcfg->gateway_network_plugin_path != NULL) {
      return start_gateway_plugin(fcfg, mqttSnForwarder, gatewayNetworkContext, clientNetworkContext);
    }
#endif
#ifdef WITH_LINUX_GATEWAY_NETWORK_UDP
    if (!strncasecmp(fcfg->gateway_network_protocol, "udp", 3)) {
      return start_gateway_udp(fcfg, mqttSnForwarder, gatewayNetworkContext, clientNetworkContext);
    }
#endif
#ifdef WITH_LINUX_GATEWAY_NETWORK_TCP
    if (!strncasecmp(fcfg->gateway_network_protocol, "tcp", 3)) {
      return start_gateway_tcp(fcfg, mqttSnForwarder, gatewayNetworkContext, clientNetworkContext);
    }
#endif
    fprintf(stderr, "Error init gateway network unknown protocol: %s\n", fcfg->client_network_protocol);
    return EXIT_FAILURE;
  }

  if (gatewayNetworkContext != NULL && clientNetworkContext == NULL) {
#ifdef WITH_PLUGIN
    if (fcfg->client_network_plugin_path != NULL) {
      return start_client_plugin(fcfg, mqttSnForwarder, gatewayNetworkContext, clientNetworkContext);
    }
#endif
#ifdef WITH_LINUX_CLIENT_NETWORK_UDP
    if (!strncasecmp(fcfg->client_network_protocol, "udp", 3)) {
      return start_client_udp(fcfg, mqttSnForwarder, gatewayNetworkContext, clientNetworkContext);
    }
#endif
#ifdef WITH_LINUX_CLIENT_NETWORK_TCP
    if (!strncasecmp(fcfg->client_network_protocol, "tcp", 3)) {
      return start_client_tcp(fcfg, mqttSnForwarder, gatewayNetworkContext, clientNetworkContext);
    }
#endif
    fprintf(stderr, "Error init client network unknown protocol: %s\n", fcfg->client_network_protocol);
    return EXIT_FAILURE;
  }

#ifdef WITH_LOGGING
  // logger only for ClientNetworkInit
  MqttSnLogger logger = {0};
  if (MqttSnLoggerInit(&logger, fcfg->log_lvl) != 0) {
    MqttSnLoggerDeinit(&logger);
    return -1;
  }
  if (log_forwarder_started(&logger,
                            fcfg->version,
                            fcfg->major,
                            fcfg->minor,
                            fcfg->tweak,
                            fcfg->build_date)) {
    return EXIT_FAILURE;
  }
#endif

  mqttSnForwarder->clientNetworkSendTimeout = fcfg->client_network_send_timeout;
  mqttSnForwarder->clientNetworkReceiveTimeout = fcfg->client_network_receive_timeout;
  mqttSnForwarder->gatewayNetworkSendTimeout = fcfg->gateway_network_send_timeout;
  mqttSnForwarder->gatewayNetworkReceiveTimeout = fcfg->gateway_network_receive_timeout;

  if (MqttSnForwarderInit(mqttSnForwarder, fcfg->log_lvl, clientNetworkContext, gatewayNetworkContext) != 0) {
    MqttSnForwarderDeinit(mqttSnForwarder);
    return EXIT_FAILURE;
  }

  signal(SIGINT, sig_handler);
  pthread_t mqttSnForwarder_thread;
  MqttSnForwarder_fcfg_ptr mqttSnForwarderFcfgPtr = {.mqttSnForwarder_ptr = mqttSnForwarder, .fcfg_ptr = fcfg};
  if (pthread_create(&mqttSnForwarder_thread, NULL, inc_c, &mqttSnForwarderFcfgPtr)) {
    fprintf(stderr, "Error creating thread\n");
    return EXIT_FAILURE;
  }

  int rc;
  if (pthread_join(mqttSnForwarder_thread, (void *) &rc)) {
    fprintf(stderr, "Error joining thread\n");
    return EXIT_FAILURE;
  }

  return rc;
}
