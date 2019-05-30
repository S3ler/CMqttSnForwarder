//
// Created by SomeDude on 30.05.2019.
//

#include <stddef.h>
#include <string.h>
#include <platform/platform_compatibility.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <network/linux/client/ip/tcp/MqttSnClientTcpNetwork.h>
#include <network/linux/shared/ip/MqttSnIpNetworkHelper.h>
#include <network/shared/ip/IpHelper.h>
#include <network/linux/client/plugin/client_network_plugin_interface.h>
#include <network/linux/client/plugin/MqttSnClientPluginNetwork.h>
#include "echogateway_starter.h"

static volatile sig_atomic_t keep_running = 1;

static void sig_handler(int _) {
  (void) _;
  keep_running = 0;
}

void *inc_c(void *egw_fcfg_ptr) {
  EchoGateway *egw = ((EchoGateway_fcfg_ptr *) egw_fcfg_ptr)->egw_ptr;

  while ((EchoGatewayLoop(egw) >= 0) & keep_running) {}
#ifdef WITH_LOGGING
  log_echogateway_terminated(&egw->logger, &((EchoGateway_fcfg_ptr *) egw_fcfg_ptr)->egw_cfg->msvcfg);
#endif
  EchoGatewayDeinit(egw);
  return (void *) EXIT_SUCCESS;
}

int start_echogateway(const echogateway_config *fcfg,
                      const MqttSnLogger *logger,
                      EchoGateway *echo_gateway,
                      void *clientNetworkContext) {

  if (clientNetworkContext == NULL) {
#ifdef WITH_LINUX_PLUGIN_NETWORK
    if (fcfg->cncfg.client_network_plugin_path != NULL) {
      return start_echogateway_client_plugin(fcfg, logger, echo_gateway, clientNetworkContext);
    }
#endif
#ifdef WITH_LINUX_UDP_CLIENT_NETWORK
    if (!strcmp(fcfg->cncfg.client_network_protocol, "udp")) {
      return start_echogateway_client_udp(fcfg, logger, echo_gateway, clientNetworkContext);
    }
#endif
#ifdef WITH_LINUX_TCP_CLIENT_NETWORK
    if (!strcmp(fcfg->cncfg.client_network_protocol, "tcp")) {
      return start_echogateway_client_tcp(fcfg, logger, echo_gateway, clientNetworkContext);
    }
#endif
    log_str(logger, PSTR("Error init client network unknown protocol:"));
    log_str(logger, fcfg->cncfg.client_network_protocol);
    log_str(logger, PSTR("\n"));
    return EXIT_FAILURE;
  }

  echo_gateway->clientNetworkSendTimeout = fcfg->cncfg.client_network_send_timeout;
  echo_gateway->clientNetworkReceiveTimeout = fcfg->cncfg.client_network_receive_timeout;
#ifdef WITH_LOGGING
  log_echogateway_started(logger, &fcfg->msvcfg);
#endif
  if (EchoGatewayInit(echo_gateway, fcfg->mslcfg.log_lvl, clientNetworkContext) < 0) {
    EchoGatewayDeinit(echo_gateway);
    return EXIT_FAILURE;
  }

  signal(SIGINT, sig_handler);
  pthread_t thread;
  EchoGateway_fcfg_ptr egw_fcfg_ptr = {.egw_ptr = echo_gateway, .egw_cfg=fcfg};
  if (pthread_create(&thread, NULL, inc_c, &egw_fcfg_ptr)) {
    log_str(logger, "Error creating thread\n");
    return EXIT_FAILURE;
  }

  int rc;
  if (pthread_join(thread, (void *) &rc)) {
    log_str(logger, "Error joining thread\n");
    return EXIT_FAILURE;
  }

  return rc;
}

#ifdef WITH_LINUX_PLUGIN_NETWORK
int start_echogateway_client_plugin(const echogateway_config *fcfg,
                                    const MqttSnLogger *logger,
                                    EchoGateway *echo_gateway,
                                    void *clientNetworkContext) {

  device_address mqttSnGatewayNetworkAddress = {0};
  device_address forwarderClientNetworkAddress = {0};
  device_address forwarderClientNetworkBroadcastAddress = {0};

  /*if (convert_hostname_port_to_device_address(fcfg->mqtt_sn_gateway_host,
                                              fcfg->mqtt_sn_gateway_port,
                                              &mqttSnGatewayNetworkAddress,
                                              "mqtt-sn gateway")) {
    return EXIT_FAILURE;
  }*/

  if (convert_hostname_port_to_device_address(fcfg->cncfg.client_network_bind_address,
                                              fcfg->cncfg.client_network_bind_port,
                                              &forwarderClientNetworkAddress,
                                              "client")) {
    return EXIT_FAILURE;
  }
  if (convert_string_ip_port_to_device_address(logger,
                                               fcfg->cncfg.client_network_broadcast_address,
                                               fcfg->cncfg.client_network_broadcast_bind_port,
                                               &forwarderClientNetworkBroadcastAddress,
                                               "client broadcast")) {
    return EXIT_FAILURE;
  }

  const client_plugin_device_address pluginMqttSnForwarderNetworkAddress = {
      .bytes = forwarderClientNetworkAddress.bytes,
      .length = sizeof(device_address)};

  client_plugin_config plugin_cfg = {
      .plugin_path = fcfg->cncfg.client_network_plugin_path,
      .protocol = fcfg->cncfg.client_network_protocol,
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

  if (ClientNetworkInitialize(&echo_gateway->clientNetwork,
                              MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH,
                              &mqttSnGatewayNetworkAddress,
                              &forwarderClientNetworkAddress,
                              &forwarderClientNetworkBroadcastAddress,
                              &clientPluginContext,
                              ClientLinuxPluginInitialize)) {
    log_str(logger, "Error init client network\n");
    return EXIT_FAILURE;
  }
  clientNetworkContext = &clientPluginContext;

  return start_echogateway(fcfg, logger, echo_gateway, clientNetworkContext);

}
#endif

#ifdef WITH_LINUX_UDP_CLIENT_NETWORK
int start_echogateway_client_udp(const echogateway_config *fcfg,
                                 const MqttSnLogger *logger,
                                 EchoGateway *echo_gateway,
                                 void *clientNetworkContext) {

  device_address mqttSnGatewayNetworkAddress = {0};
  device_address forwarderClientNetworkAddress = {0};
  device_address forwarderClientNetworkBroadcastAddress = {0};
  MqttSnClientUdpNetwork udpClientNetworkContext = {0};

  /*if (convert_hostname_port_to_device_address(fcfg->mqtt_sn_gateway_host,
                                              fcfg->mqtt_sn_gateway_port,
                                              &mqttSnGatewayNetworkAddress,
                                              "mqtt-sn gateway")) {
    return EXIT_FAILURE;
  }*/

  if (convert_hostname_port_to_device_address(fcfg->cncfg.client_network_bind_address,
                                              fcfg->cncfg.client_network_bind_port,
                                              &forwarderClientNetworkAddress,
                                              "client unicast")) {
    return EXIT_FAILURE;
  }
  if (convert_string_ip_port_to_device_address(logger,
                                               fcfg->cncfg.client_network_broadcast_address,
                                               fcfg->cncfg.client_network_broadcast_bind_port,
                                               &forwarderClientNetworkBroadcastAddress,
                                               "client broadcast")) {
    return EXIT_FAILURE;
  }

  if (ClientNetworkInitialize(&echo_gateway->clientNetwork,
                              MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH,
                              &mqttSnGatewayNetworkAddress,
                              &forwarderClientNetworkAddress,
                              &forwarderClientNetworkBroadcastAddress,
                              &udpClientNetworkContext,
                              ClientLinuxUdpInitialize)) {
    log_str(logger, "Error init client network\n");
    return EXIT_FAILURE;
  }
  clientNetworkContext = &udpClientNetworkContext;

  return start_echogateway(fcfg, logger, echo_gateway, clientNetworkContext);
}
#endif
#ifdef WITH_LINUX_TCP_CLIENT_NETWORK
int start_echogateway_client_tcp(const echogateway_config *fcfg,
                                 const MqttSnLogger *logger,
                                 EchoGateway *echo_gateway,
                                 void *clientNetworkContext) {

  device_address mqttSnGatewayNetworkAddress = {0};
  device_address forwarderClientNetworkAddress = {0};
  device_address forwarderClientNetworkBroadcastAddress = {0};
  MqttSnClientTcpNetwork tcpClientNetworkContext = {0};

  /*if (convert_hostname_port_to_device_address(fcfg->mqtt_sn_gateway_host,
                                              fcfg->mqtt_sn_gateway_port,
                                              &mqttSnGatewayNetworkAddress,
                                              "mqtt-sn gateway")) {
    return EXIT_FAILURE;
  }*/
  if (convert_hostname_port_to_device_address(fcfg->cncfg.client_network_bind_address,
                                              fcfg->cncfg.client_network_bind_port,
                                              &forwarderClientNetworkAddress,
                                              "client")) {
    return EXIT_FAILURE;
  }
  if (convert_string_ip_port_to_device_address(logger,
                                               fcfg->cncfg.client_network_broadcast_address,
                                               fcfg->cncfg.client_network_broadcast_bind_port,
                                               &forwarderClientNetworkBroadcastAddress,
                                               "client broadcast")) {
    return EXIT_FAILURE;
  }
  if (ClientNetworkInitialize(&echo_gateway->clientNetwork,
                              MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH,
                              &mqttSnGatewayNetworkAddress,
                              &forwarderClientNetworkAddress,
                              &forwarderClientNetworkBroadcastAddress,
                              &tcpClientNetworkContext,
                              ClientLinuxTcpInitialize)) {
    log_str(logger, "Error init client network\n");
    return EXIT_FAILURE;
  }
  clientNetworkContext = &tcpClientNetworkContext;

  return start_echogateway(fcfg, logger, echo_gateway, clientNetworkContext);
}
#endif
