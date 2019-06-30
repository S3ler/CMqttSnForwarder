//
// Created by SomeDude on 31.05.2019.
//

#include "publish_client_starter.h"
#include <config/starter/starter_helper.h>
#include <stddef.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include <network/linux/shared/ip/MqttSnIpNetworkHelper.h>
#include <network/shared/ip/IpHelper.h>
#include <network/linux/gateway/plugin/gateway_network_plugin_interface.h>
#include <network/linux/gateway/plugin/MqttSnGatewayPluginNetwork.h>
#include <network/linux/gateway/ip/tcp/MqttSnGatewayTcpNetwork.h>
#include <client/MqttSnClientLogger.h>

static volatile sig_atomic_t keep_running = 1;

int32_t send_single_mqtt_sn_m1_publish(MqttSnClient *publish_client, const publish_client_config *cfg);
static void sig_handler(int _) {
  (void) _;
  keep_running = 0;
}

void *publish_client_thread_function(void *ptr) {
  MqttSnClient *publish_client = ((PublishClient_cfg_ptr *) ptr)->c_ptr;
  while ((MqttSnClientLoop(publish_client) >= 0) & keep_running) {}
  return (void *) EXIT_SUCCESS;
}

int32_t start_publish_client(const publish_client_config *cfg,
                             const MqttSnLogger *logger,
                             MqttSnClient *publish_client,
                             void *gatewayNetworkContext) {

  if (gatewayNetworkContext == NULL) {
#ifdef WITH_LINUX_PLUGIN_NETWORK
    if (cfg->gncfg.gateway_network_plugin_path != NULL) {
      return start_publish_client_plugin(cfg, publish_client, logger, gatewayNetworkContext);
    }
#endif
#ifdef WITH_LINUX_UDP_GATEWAY_NETWORK
    if (!strcmp(cfg->gncfg.gateway_network_protocol, "udp")) {
      return start_publish_client_udp(cfg, publish_client, logger, gatewayNetworkContext);
    }
#endif
#ifdef WITH_LINUX_TCP_GATEWAY_NETWORK
    if (!strcmp(cfg->gncfg.gateway_network_protocol, "tcp")) {
      return start_publish_client_tcp(cfg, publish_client, logger, gatewayNetworkContext);
    }
#endif
    log_str(logger, "Error init gateway network unknown protocol: ");
    log_str(logger, cfg->gncfg.gateway_network_protocol);
    log_str(logger, "\n");
    return EXIT_FAILURE;
  }

  publish_client->gatewayNetworkSendTimeout = cfg->gncfg.gateway_network_send_timeout;
  publish_client->gatewayNetworkReceiveTimeout = cfg->gncfg.gateway_network_receive_timeout;
#ifdef WITH_LOGGING
  print_program_started(logger, &cfg->msvcfg, cfg->executable_name);
#endif
  if (MqttSnClientInit(publish_client, cfg->mslcfg.log_lvl, gatewayNetworkContext) < 0) {
    MqttSnClientDeinit(publish_client);
    return EXIT_FAILURE;
  }

  // configure Mqtt-Sn Client

  uint8_t only_M1_publishes = 1;
  if (cfg->cpcfg.publish_list_len == 0) {
    only_M1_publishes = 0;
  } else {
    for (uint16_t i = 0; i < cfg->cpcfg.publish_list_len; i++) {
      if (cfg->cpcfg.publish_list[i].qos != -1) {
        only_M1_publishes = 0;
        break;
      }
    }
  }

  if (only_M1_publishes && cfg->cpcfg.default_qos == -1 && cfg->cpcfg.console_in) {
    if (cfg->cpcfg.console_in == 1) {
      // ony one message
      // TODO
      // read from console in until STRG+C?
      // publish
      // return
    } else {
      // multiple messages
      // TODO
      // read from console in until STRG+C?
      // publish
      // return
    }
#ifdef WITH_LOGGING
    print_program_terminated(&publish_client->logger, &cfg->msvcfg, cfg->executable_name);
#endif
    return EXIT_SUCCESS;
  }

  if (only_M1_publishes) {
    return send_single_mqtt_sn_m1_publish(publish_client, cfg);
  }


  MQTT_SN_CLIENT_RETURN_CODE connect_rc;
  MqttSnClientTimeoutOffset(publish_client, cfg->cccfg.connect_timeout_offset);
  if ((connect_rc = MqttSnClientConnect(publish_client,
                                         publish_client->gatewayNetwork.mqtt_sn_gateway_address,
                                         cfg->cccfg.client_connect_timeout_ms,
                                         cfg->cccfg.clean_session,
                                         cfg->cccfg.client_id,
                                         cfg->cccfg.connect_duration)) != MQTT_SN_CLIENT_RETURN_SUCCESS) {

    log_mqtt_sn_client(&publish_client->logger, connect_rc);
    // TODO log error here
#ifdef WITH_LOGGING
    print_program_terminated(&publish_client->logger, &cfg->msvcfg, cfg->executable_name);
#endif
    MqttSnClientDeinit(publish_client);
    return EXIT_FAILURE;
  }

  signal(SIGINT, sig_handler);
  pthread_t thread;
  PublishClient_cfg_ptr ptr = {.c_ptr = publish_client, .cfg_ptr=cfg};
  if (pthread_create(&thread, NULL, publish_client_thread_function, &ptr)) {
    log_str(logger, "Error creating thread\n");
    return EXIT_FAILURE;
  }

  int rc;
  if (pthread_join(thread, (void *) &rc)) {
    log_str(logger, "Error joining thread\n");
    return EXIT_FAILURE;
  }

#ifdef WITH_LOGGING
  print_program_terminated(&publish_client->logger, &cfg->msvcfg, cfg->executable_name);
#endif
  MqttSnClientDeinit(publish_client);

  return rc;
}

int32_t send_single_mqtt_sn_m1_publish(MqttSnClient *publish_client,
                                       const publish_client_config *cfg) {// no connect needed
  for (uint32_t i = 0; i < cfg->cpcfg.publish_list_len; i++) {
    single_client_publish_config publish = cfg->cpcfg.publish_list[i];
    if (MqttSnClientPublishPredefinedM1(publish_client, publish.topic_id, 0, publish.data, publish.data_length) < 0) {
      MqttSnClientDeinit(publish_client);
      return EXIT_FAILURE;
    }
  }
#ifdef WITH_LOGGING
  print_program_terminated(&publish_client->logger, &cfg->msvcfg, cfg->executable_name);
#endif
  return EXIT_SUCCESS;
}
#ifdef WITH_LINUX_PLUGIN_NETWORK
int32_t start_publish_client_plugin(const publish_client_config *cfg,
                                    MqttSnClient *publish_client,
                                    const MqttSnLogger *logger,
                                    void *gatewayNetworkContext) {
  device_address mqttSnGatewayNetworkAddress = {0};
  device_address forwarderGatewayNetworkAddress = {0};
  device_address forwarderGatewayNetworkBroadcastAddress = {0};

  if (convert_hostname_port_to_device_address(cfg->msgcfg.mqtt_sn_gateway_host,
                                              cfg->msgcfg.mqtt_sn_gateway_port,
                                              &mqttSnGatewayNetworkAddress,
                                              "mqtt-sn gateway")) {
    return EXIT_FAILURE;
  }
  if (convert_hostname_port_to_device_address(cfg->gncfg.gateway_network_bind_address,
                                              cfg->gncfg.gateway_network_bind_port,
                                              &forwarderGatewayNetworkAddress,
                                              "gateway")) {
    return EXIT_FAILURE;
  }
  if (convert_string_ip_port_to_device_address(logger,
                                               cfg->gncfg.gateway_network_broadcast_address,
                                               cfg->gncfg.gateway_network_broadcast_bind_port,
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
      .plugin_path = cfg->gncfg.gateway_network_plugin_path,
      .protocol = cfg->gncfg.gateway_network_protocol,
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

  if (GatewayNetworkInitialize(&publish_client->gatewayNetwork,
                               MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH,
                               &mqttSnGatewayNetworkAddress,
                               &forwarderGatewayNetworkAddress,
                               &forwarderGatewayNetworkBroadcastAddress,
                               &gatewayPluginContext,
                               GatewayLinuxPluginInitialize)) {
    log_str(logger, "Error init gateway network\n");
    return EXIT_FAILURE;
  }
  gatewayNetworkContext = &gatewayPluginContext;

  return start_publish_client(cfg, logger, publish_client, gatewayNetworkContext);
}
#endif
#ifdef WITH_LINUX_UDP_GATEWAY_NETWORK
int32_t start_publish_client_udp(const publish_client_config *cfg,
                                 MqttSnClient *publish_client,
                                 const MqttSnLogger *logger,
                                 void *gatewayNetworkContext) {

  device_address mqttSnGatewayNetworkAddress = {0};
  device_address forwarderGatewayNetworkAddress = {0};
  device_address forwarderGatewayNetworkBroadcastAddress = {0};
  MqttSnGatewayUdpNetwork udpGatewayNetworkContext = {0};

  if (convert_hostname_port_to_device_address(cfg->msgcfg.mqtt_sn_gateway_host,
                                              cfg->msgcfg.mqtt_sn_gateway_port,
                                              &mqttSnGatewayNetworkAddress,
                                              "mqtt-sn gateway")) {
    return EXIT_FAILURE;
  }
  if (convert_hostname_port_to_device_address(cfg->gncfg.gateway_network_bind_address,
                                              cfg->gncfg.gateway_network_bind_port,
                                              &forwarderGatewayNetworkAddress,
                                              "gateway unicast")) {
    return EXIT_FAILURE;
  }
  if (convert_string_ip_port_to_device_address(logger,
                                               cfg->gncfg.gateway_network_broadcast_address,
                                               cfg->gncfg.gateway_network_broadcast_bind_port,
                                               &forwarderGatewayNetworkBroadcastAddress,
                                               "gateway broadcast")) {
    return EXIT_FAILURE;
  }

  if (GatewayNetworkInitialize(&publish_client->gatewayNetwork,
                               MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH,
                               &mqttSnGatewayNetworkAddress,
                               &forwarderGatewayNetworkAddress,
                               &forwarderGatewayNetworkBroadcastAddress,
                               &udpGatewayNetworkContext,
                               GatewayLinuxUdpInitialize)) {
    log_str(logger, "Error init gateway network\n");
    return EXIT_FAILURE;
  }
  gatewayNetworkContext = &udpGatewayNetworkContext;

  return start_publish_client(cfg, logger, publish_client, gatewayNetworkContext);
}
#endif
#ifdef WITH_LINUX_TCP_GATEWAY_NETWORK
int32_t start_publish_client_tcp(const publish_client_config *cfg,
                                 MqttSnClient *publish_client,
                                 const MqttSnLogger *logger,
                                 void *gatewayNetworkContext) {
  device_address mqttSnGatewayNetworkAddress = {0};
  device_address forwarderGatewayNetworkAddress = {0};
  device_address forwarderGatewayNetworkBroadcastAddress = {0};
  MqttSnGatewayTcpNetwork tcpGatewayNetworkContext = {0};

  if (convert_hostname_port_to_device_address(cfg->msgcfg.mqtt_sn_gateway_host,
                                              cfg->msgcfg.mqtt_sn_gateway_port,
                                              &mqttSnGatewayNetworkAddress,
                                              "mqtt-sn gateway")) {
    return EXIT_FAILURE;
  }
  if (convert_hostname_port_to_device_address(cfg->gncfg.gateway_network_bind_address,
                                              cfg->gncfg.gateway_network_bind_port,
                                              &forwarderGatewayNetworkAddress,
                                              "gateway")) {
    return EXIT_FAILURE;
  }
  if (convert_string_ip_port_to_device_address(logger,
                                               cfg->gncfg.gateway_network_broadcast_address,
                                               cfg->gncfg.gateway_network_broadcast_bind_port,
                                               &forwarderGatewayNetworkBroadcastAddress,
                                               "gateway broadcast")) {
    return EXIT_FAILURE;
  }

  if (GatewayNetworkInitialize(&publish_client->gatewayNetwork,
                               MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH,
                               &mqttSnGatewayNetworkAddress,
                               &forwarderGatewayNetworkAddress,
                               &forwarderGatewayNetworkBroadcastAddress,
                               &tcpGatewayNetworkContext,
                               GatewayLinuxTcpInitialize)) {
    log_str(logger, "Error init client network\n");
    return EXIT_FAILURE;
  }
  gatewayNetworkContext = &tcpGatewayNetworkContext;

  return start_publish_client(cfg, logger, publish_client, gatewayNetworkContext);
}
#endif
