//
// Created by SomeDude on 07.04.2019.
//
#ifndef WITH_PLATFORMIO

#include "forwarder_starter.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <bits/types/sig_atomic_t.h>
#include <signal.h>
#include <pthread.h>
#include <network/shared/ip/IpHelper.h>
#include <network/linux/shared/ip/MqttSnIpNetworkHelper.h>
#include <network/linux/gateway/plugin/gateway_network_plugin_interface.h>
#include <network/linux/gateway/plugin/MqttSnGatewayPluginNetwork.h>
#include <logging/linux/stdout/StdoutLogging.h>
#include <network/linux/gateway/ip/tcp/MqttSnGatewayTcpNetwork.h>
#include <network/linux/gateway/ip/udp/MqttSnGatewayUdpNetwork.h>
#include <network/linux/client/plugin/client_network_plugin_interface.h>
#include <network/linux/client/plugin/MqttSnClientPluginNetwork.h>
#include <network/linux/client/ip/tcp/MqttSnClientTcpNetwork.h>
#include <network/linux/client/ip/udp/MqttSnClientUdpNetwork.h>
#include <forwarder/MqttSnForwarderLogging.h>
#include <assert.h>


#ifdef WITH_LINUX_PLUGIN_NETWORK
int start_gateway_plugin(const forwarder_config *fcfg,
                         MqttSnForwarder *mqttSnForwarder,
                         void *gatewayNetworkContext,
                         void *clientNetworkContext) {

  device_address mqttSnGatewayNetworkAddress = {0};
  device_address forwarderGatewayNetworkAddress = {0};
  device_address forwarderGatewayNetworkBroadcastAddress = {0};

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
  if (convert_string_ip_port_to_device_address(fcfg->logger,
                                               fcfg->gateway_network_broadcast_address,
                                               fcfg->gateway_network_broadcast_bind_port,
                                               &forwarderGatewayNetworkBroadcastAddress,
                                               "gateway broadcast")) {
    return EXIT_FAILURE;
  }

  const gateway_plugin_device_address pluginMqttSnGatewayNetworkAddress = {
      .bytes = mqttSnGatewayNetworkAddress.bytes,
      .length = sizeof(device_address)};

  const gateway_plugin_device_address pluginForwarderGatewayNetworkAddress = {
      .bytes = forwarderGatewayNetworkAddress.bytes,
      .length = sizeof(device_address)};

  const gateway_plugin_device_address pluginForwarderGatewayNetworkBroadcastAddress = {
      .bytes = forwarderGatewayNetworkBroadcastAddress.bytes,
      .length = sizeof(device_address)};

  gateway_plugin_config plugin_cfg = {
      .plugin_path = fcfg->gateway_network_plugin_path,
      .protocol = fcfg->gateway_network_protocol,
      .mqtt_sn_gateway_network_address = &pluginMqttSnGatewayNetworkAddress,
      .forwarder_gateway_network_address = &pluginForwarderGatewayNetworkAddress,
      .forwarder_gateway_network_broadcast_address = &pluginForwarderGatewayNetworkBroadcastAddress,
      .gateway_plugin_device_address_length = MQTT_SN_DEVICE_ADDRESS_LENGTH,
      .forwarder_maximum_message_length = MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH};

  MqttSnGatewayPluginContext gatewayPluginContext = {
      .dl_handle = NULL,
      .plugin_network_initialize = NULL,
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
  if (MqttSnLoggerInit(&logger, fcfg->log_lvl, stdout_log_init) != 0) {
    MqttSnLoggerDeinit(&logger);
    return -1;
  }
  mqttSnForwarder->gatewayNetwork.logger = &logger;
#endif

  if (GatewayNetworkInitialize(&mqttSnForwarder->gatewayNetwork,
                               MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH,
                               &mqttSnGatewayNetworkAddress,
                               &forwarderGatewayNetworkAddress,
                               &forwarderGatewayNetworkBroadcastAddress,
                               &gatewayPluginContext,
                               GatewayLinuxPluginInitialize)) {
    fprintf(stderr, "Error init gateway network\n");
    return EXIT_FAILURE;
  }
  gatewayNetworkContext = &gatewayPluginContext;

  return start_forwarder(fcfg, mqttSnForwarder, gatewayNetworkContext, clientNetworkContext);
}
#endif

#ifdef WITH_LINUX_TCP_GATEWAY_NETWORK
int start_gateway_tcp(const forwarder_config *fcfg,
                      MqttSnForwarder *mqttSnForwarder,
                      void *gatewayNetworkContext,
                      void *clientNetworkContext) {

  device_address mqttSnGatewayNetworkAddress = {0};
  device_address forwarderGatewayNetworkAddress = {0};
  device_address forwarderGatewayNetworkBroadcastAddress = {0};
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
  if (convert_string_ip_port_to_device_address(fcfg->logger,
                                               fcfg->gateway_network_broadcast_address,
                                               fcfg->gateway_network_broadcast_bind_port,
                                               &forwarderGatewayNetworkBroadcastAddress,
                                               "gateway broadcast")) {
    return EXIT_FAILURE;
  }

  if (GatewayNetworkInitialize(&mqttSnForwarder->gatewayNetwork,
                               MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH,
                               &mqttSnGatewayNetworkAddress,
                               &forwarderGatewayNetworkAddress,
                               &forwarderGatewayNetworkBroadcastAddress,
                               &tcpGatewayNetworkContext,
                               GatewayLinuxTcpInitialize)) {
    fprintf(stderr, "Error init client network\n");
    return EXIT_FAILURE;
  }
  gatewayNetworkContext = &tcpGatewayNetworkContext;

  return start_forwarder(fcfg, mqttSnForwarder, gatewayNetworkContext, clientNetworkContext);
}
#endif

#ifdef WITH_LINUX_UDP_GATEWAY_NETWORK
int start_gateway_udp(const forwarder_config *fcfg,
                      MqttSnForwarder *mqttSnForwarder,
                      void *gatewayNetworkContext,
                      void *clientNetworkContext) {

  device_address mqttSnGatewayNetworkAddress = {0};
  device_address forwarderGatewayNetworkAddress = {0};
  device_address forwarderGatewayNetworkBroadcastAddress = {0};
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
                                              "gateway unicast")) {
    return EXIT_FAILURE;
  }
  if (convert_string_ip_port_to_device_address(fcfg->logger,
                                               fcfg->gateway_network_broadcast_address,
                                               fcfg->gateway_network_broadcast_bind_port,
                                               &forwarderGatewayNetworkBroadcastAddress,
                                               "gateway broadcast")) {
    return EXIT_FAILURE;
  }

  if (GatewayNetworkInitialize(&mqttSnForwarder->gatewayNetwork,
                               MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH,
                               &mqttSnGatewayNetworkAddress,
                               &forwarderGatewayNetworkAddress,
                               &forwarderGatewayNetworkBroadcastAddress,
                               &udpGatewayNetworkContext,
                               GatewayLinuxUdpInitialize)) {
    fprintf(stderr, "Error init gateway network\n");
    return EXIT_FAILURE;
  }
  gatewayNetworkContext = &udpGatewayNetworkContext;

  return start_forwarder(fcfg, mqttSnForwarder, gatewayNetworkContext, clientNetworkContext);
}
#endif

#ifdef WITH_LINUX_PLUGIN_NETWORK
int start_client_plugin(const forwarder_config *fcfg,
                        MqttSnForwarder *mqttSnForwarder,
                        void *gatewayNetworkContext,
                        void *clientNetworkContext) {

  device_address mqttSnGatewayNetworkAddress = {0};
  device_address forwarderClientNetworkAddress = {0};
  device_address forwarderClientNetworkBroadcastAddress = {0};

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
  if (convert_string_ip_port_to_device_address(fcfg->logger,
                                               fcfg->client_network_broadcast_address,
                                               fcfg->client_network_broadcast_bind_port,
                                               &forwarderClientNetworkBroadcastAddress,
                                               "client broadcast")) {
    return EXIT_FAILURE;
  }

  const client_plugin_device_address pluginMqttSnForwarderNetworkAddress = {
      .bytes = forwarderClientNetworkAddress.bytes,
      .length = sizeof(device_address)};

  client_plugin_config plugin_cfg = {
      .plugin_path = fcfg->client_network_plugin_path,
      .protocol = fcfg->client_network_protocol,
      .forwarder_client_network_address = &pluginMqttSnForwarderNetworkAddress,
      .client_plugin_device_address_length = MQTT_SN_DEVICE_ADDRESS_LENGTH,
      .forwarder_maximum_message_length = MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH};

  MqttSnClientPluginContext clientPluginContext = {
      .dl_handle = NULL,
      .plugin_network_initialize = NULL,
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
  if (MqttSnLoggerInit(&logger, fcfg->log_lvl, stdout_log_init) != 0) {
    MqttSnLoggerDeinit(&logger);
    return -1;
  }
  mqttSnForwarder->clientNetwork.logger = &logger;
#endif

  if (ClientNetworkInitialize(&mqttSnForwarder->clientNetwork,
                              MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH,
                              &mqttSnGatewayNetworkAddress,
                              &forwarderClientNetworkAddress,
                              &forwarderClientNetworkBroadcastAddress,
                              &clientPluginContext,
                              ClientLinuxPluginInitialize)) {
    fprintf(stderr, "Error init client network\n");
    return EXIT_FAILURE;
  }
  clientNetworkContext = &clientPluginContext;

  return start_forwarder(fcfg, mqttSnForwarder, gatewayNetworkContext, clientNetworkContext);

}
#endif

#ifdef WITH_LINUX_TCP_CLIENT_NETWORK
int start_client_tcp(const forwarder_config *fcfg,
                     MqttSnForwarder *mqttSnForwarder,
                     void *gatewayNetworkContext,
                     void *clientNetworkContext) {

  device_address mqttSnGatewayNetworkAddress = {0};
  device_address forwarderClientNetworkAddress = {0};
  device_address forwarderClientNetworkBroadcastAddress = {0};
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
  if (convert_string_ip_port_to_device_address(fcfg->logger,
                                               fcfg->client_network_broadcast_address,
                                               fcfg->client_network_broadcast_bind_port,
                                               &forwarderClientNetworkBroadcastAddress,
                                               "client broadcast")) {
    return EXIT_FAILURE;
  }

  if (ClientNetworkInitialize(&mqttSnForwarder->clientNetwork,
                              MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH,
                              &mqttSnGatewayNetworkAddress,
                              &forwarderClientNetworkAddress,
                              &forwarderClientNetworkBroadcastAddress,
                              &tcpClientNetworkContext,
                              ClientLinuxTcpInitialize)) {
    fprintf(stderr, "Error init client network\n");
    return EXIT_FAILURE;
  }
  clientNetworkContext = &tcpClientNetworkContext;

  return start_forwarder(fcfg, mqttSnForwarder, gatewayNetworkContext, clientNetworkContext);
}
#endif

#ifdef WITH_LINUX_UDP_CLIENT_NETWORK
int start_client_udp(const forwarder_config *fcfg,
                     MqttSnForwarder *mqttSnForwarder,
                     void *gatewayNetworkContext,
                     void *clientNetworkContext) {

  device_address forwarderClientNetworkAddress = {0};
  device_address mqttSnGatewayNetworkAddress = {0};
  device_address forwarderClientNetworkBroadcastAddress = {0};
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
                                              "client unicast")) {
    return EXIT_FAILURE;
  }
  if (convert_string_ip_port_to_device_address(fcfg->logger,
                                               fcfg->client_network_broadcast_address,
                                               fcfg->client_network_broadcast_bind_port,
                                               &forwarderClientNetworkBroadcastAddress,
                                               "client broadcast")) {
    return EXIT_FAILURE;
  }

  if (ClientNetworkInitialize(&mqttSnForwarder->clientNetwork,
                              MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH,
                              &mqttSnGatewayNetworkAddress,
                              &forwarderClientNetworkAddress,
                              &forwarderClientNetworkBroadcastAddress,
                              &udpClientNetworkContext,
                              ClientLinuxUdpInitialize)) {
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

#ifdef WITH_LINUX_PLUGIN_NETWORK
    if (fcfg->gateway_network_plugin_path != NULL) {
      return start_gateway_plugin(fcfg, mqttSnForwarder, gatewayNetworkContext, clientNetworkContext);
    }
#endif
#ifdef WITH_LINUX_UDP_GATEWAY_NETWORK
    if (!strcmp(fcfg->gateway_network_protocol, "udp")) {
      return start_gateway_udp(fcfg, mqttSnForwarder, gatewayNetworkContext, clientNetworkContext);
    }
#endif
#ifdef WITH_LINUX_TCP_GATEWAY_NETWORK
    if (!strcmp(fcfg->gateway_network_protocol, "tcp")) {
      return start_gateway_tcp(fcfg, mqttSnForwarder, gatewayNetworkContext, clientNetworkContext);
    }
#endif
    fprintf(stderr, "Error init gateway network unknown protocol: %s\n", fcfg->gateway_network_protocol);
    return EXIT_FAILURE;
  }

  if (gatewayNetworkContext != NULL && clientNetworkContext == NULL) {
#ifdef WITH_LINUX_PLUGIN_NETWORK
    if (fcfg->client_network_plugin_path != NULL) {
      return start_client_plugin(fcfg, mqttSnForwarder, gatewayNetworkContext, clientNetworkContext);
    }
#endif
#ifdef WITH_LINUX_UDP_CLIENT_NETWORK
    if (!strcmp(fcfg->client_network_protocol, "udp")) {
      return start_client_udp(fcfg, mqttSnForwarder, gatewayNetworkContext, clientNetworkContext);
    }
#endif
#ifdef WITH_LINUX_TCP_CLIENT_NETWORK
    if (!strcmp(fcfg->client_network_protocol, "tcp")) {
      return start_client_tcp(fcfg, mqttSnForwarder, gatewayNetworkContext, clientNetworkContext);
    }
#endif
    fprintf(stderr, "Error init client network unknown protocol: %s\n", fcfg->client_network_protocol);
    return EXIT_FAILURE;
  }

#ifdef WITH_LOGGING
  // logger only for ClientNetworkInit
  MqttSnLogger logger = {0};
  if (MqttSnLoggerInit(&logger, fcfg->log_lvl, stdout_log_init) != 0) {
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

#endif
