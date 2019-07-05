//
// Created by SomeDude on 18.06.2019.
//

#include <gateway/config/gateway_config.h>
#include <bits/types/sig_atomic_t.h>
#include <stdlib.h>
#include <config/starter/starter_helper.h>
#include <platform/platform_compatibility.h>
#include <pthread.h>
#include <signal.h>
#include <network/linux/client/ip/udp/MqttSnClientUdpNetwork.h>
#include <network/linux/shared/ip/MqttSnIpNetworkHelper.h>
#include <network/shared/ip/IpHelper.h>
#include "gateway_starter.h"

static volatile sig_atomic_t keep_running = 1;

static void sig_handler(int _) {
  (void) _;
  keep_running = 0;
}

void *inc_c(void *egw_fcfg_ptr) {
  MqttSnGateway *mqtt_sn_gateway = ((MqttSnGateway_fcfg_ptr *) egw_fcfg_ptr)->msgw_ptr;
  if (MqttSnGatewayConnect(mqtt_sn_gateway) < 0) {
    MqttSnGatewayDisconnect(mqtt_sn_gateway);
#ifdef WITH_LOGGING
    const mqtt_sn_gateway__config *cfg = ((MqttSnGateway_fcfg_ptr *) egw_fcfg_ptr)->msgw_cfg;
    print_program_terminated(&mqtt_sn_gateway->logger, &cfg->msvcfg, cfg->executable_name);
#endif
    MqttSnGatewayDeinitialize(mqtt_sn_gateway);
    return (void *) EXIT_SUCCESS;
  }

  while ((MqttSnGatewayLoop(mqtt_sn_gateway) >= 0) & keep_running) {}
  MqttSnGatewayDisconnect(mqtt_sn_gateway);
#ifdef WITH_LOGGING
  const mqtt_sn_gateway__config *cfg = ((MqttSnGateway_fcfg_ptr *) egw_fcfg_ptr)->msgw_cfg;
  print_program_terminated(&mqtt_sn_gateway->logger, &cfg->msvcfg, cfg->executable_name);
#endif
  MqttSnGatewayDeinitialize(mqtt_sn_gateway);
  return (void *) EXIT_SUCCESS;
}

int start_mqtt_sn_gateway(const mqtt_sn_gateway__config *fcfg,
                          const MqttSnLogger *logger,
                          MqttSnGateway *mqtt_sn_gateway,
                          void *clientNetworkContext) {

  if (clientNetworkContext == NULL) {
#ifdef WITH_LINUX_PLUGIN_NETWORK1
    if (fcfg->cncfg.client_network_plugin_path != NULL) {
      return start_echogateway_client_plugin(fcfg, logger, echo_gateway, clientNetworkContext);
    }
#endif
#ifdef WITH_LINUX_UDP_CLIENT_NETWORK
    if (!strcmp(fcfg->cncfg.client_network_protocol, "udp")) {
      return start_mqtt_sn_gateway_client_udp(fcfg, logger, mqtt_sn_gateway, clientNetworkContext);
    }
#endif
#ifdef WITH_LINUX_TCP_CLIENT_NETWORK1
    if (!strcmp(fcfg->cncfg.client_network_protocol, "tcp")) {
      return start_echogateway_client_tcp(fcfg, logger, echo_gateway, clientNetworkContext);
    }
#endif
    log_str(logger, PSTR("Error init client network unknown protocol:"));
    log_str(logger, fcfg->cncfg.client_network_protocol);
    log_str(logger, PSTR("\n"));
    return EXIT_FAILURE;
  }

  mqtt_sn_gateway->clientNetworkSendTimeout = fcfg->cncfg.client_network_send_timeout;
  mqtt_sn_gateway->clientNetworkReceiveTimeout = fcfg->cncfg.client_network_receive_timeout;
#ifdef WITH_LOGGING
  print_program_started(logger, &fcfg->msvcfg, fcfg->executable_name);
#endif
  MqttSnLogger mqtt_sn_gateway_logger = {0};
  if (MqttSnLoggerInit(&mqtt_sn_gateway_logger, fcfg->mslcfg.log_lvl, stdout_log_init)) {
#ifdef WITH_LOGGING
    // TODO log that logger failed haha
#endif
    return -1;
  }
  // FIXME MqttClient
  if (MqttSnGatewayInitialize(mqtt_sn_gateway,
                              &mqtt_sn_gateway_logger,
                              NULL,
                              clientNetworkContext,
                              &fcfg->gacfg,
                              &fcfg->gccccfg) < 0) {

    MqttSnGatewayDeinitialize(mqtt_sn_gateway);
    return EXIT_FAILURE;
  }

  signal(SIGINT, sig_handler);
  pthread_t thread;
  MqttSnGateway_fcfg_ptr egw_fcfg_ptr = {.msgw_ptr = mqtt_sn_gateway, .msgw_cfg=fcfg};
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
#ifdef WITH_LINUX_UDP_CLIENT_NETWORK

int start_mqtt_sn_gateway_client_udp(const mqtt_sn_gateway__config *fcfg,
                                     const MqttSnLogger *logger,
                                     MqttSnGateway *echo_gateway,
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

  return start_mqtt_sn_gateway(fcfg, logger, echo_gateway, clientNetworkContext);
}
#endif
