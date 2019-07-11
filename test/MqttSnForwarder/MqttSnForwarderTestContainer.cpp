//
// Created by SomeDude on 09.07.2019.
//

#include <logging/linux/stdout/StdoutLogging.h>
#include <forwarder/config/forwarder_config.h>
#include <config/common/config_command_helper.h>
#include "MqttSnForwarderTestContainer.h"
#include <string.h>
#include <config/starter/starter_helper.h>
#include <network/linux/shared/ip/MqttSnIpNetworkHelper.h>
#include <network/shared/ip/IpHelper.h>
#include <network/linux/client/plugin/client_network_plugin_interface.h>
#include <network/linux/client/plugin/MqttSnClientPluginNetwork.h>
#include <network/linux/client/ip/tcp/MqttSnClientTcpNetwork.h>
#include <network/linux/gateway/plugin/gateway_network_plugin_interface.h>
#include <network/linux/gateway/plugin/MqttSnGatewayPluginNetwork.h>
#include <network/linux/gateway/ip/tcp/MqttSnGatewayTcpNetwork.h>

MqttSnForwarderTestContainer::MqttSnForwarderTestContainer(const string &identifier, const string &cmd)
    : identifier(identifier), cmd(cmd) {}

int32_t MqttSnForwarderTestContainer::initialize() {
  if (isRunning()) {
    return -1;
  }

  if (MqttSnLoggerInit(&fw_logger, LOG_LEVEL_VERBOSE, stdout_log_init)) {
    return -1;
  }

  if (forwarder_config_init(&fw_cfg) < 0) {
    return EXIT_FAILURE;
  }

  int32_t rc = process_config_file_line(&fw_logger,
                                        cmd.data(),
                                        cmd.length() + 1,
                                        identifier.data(),
                                        &fw_cfg,
                                        forwarder_config_file_process_command_callback);
  if (rc) {
    forwarder_config_cleanup(&fw_cfg);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
int32_t MqttSnForwarderTestContainer::start() {
  const forwarder_config *fcfg = &fw_cfg;
  const MqttSnLogger *logger = &fw_logger;
  MqttSnForwarder *mqttSnForwarder = &forwarder;

  if ((gatewayNetworkContext == NULL && clientNetworkContext != NULL)
      || (gatewayNetworkContext == NULL && clientNetworkContext == NULL)) {

#ifdef WITH_LINUX_PLUGIN_NETWORK
    if (fcfg->gncfg.gateway_network_plugin_path != NULL) {
      return start_gateway_plugin(fcfg, logger, mqttSnForwarder);
    }
#endif
#ifdef WITH_LINUX_UDP_GATEWAY_NETWORK
    if (!strcmp(fcfg->gncfg.gateway_network_protocol, "udp")) {
      return start_gateway_udp(fcfg, logger, mqttSnForwarder);
    }
#endif
#ifdef WITH_LINUX_TCP_GATEWAY_NETWORK
    if (!strcmp(fcfg->gncfg.gateway_network_protocol, "tcp")) {
      return start_gateway_tcp(fcfg, logger, mqttSnForwarder);
    }
#endif
    log_str(logger, "Error init gateway network unknown protocol: ");
    log_str(logger, fcfg->gncfg.gateway_network_protocol);
    log_str(logger, "\n");
  }

  if (gatewayNetworkContext != NULL && clientNetworkContext == NULL) {
#ifdef WITH_LINUX_PLUGIN_NETWORK
    if (fcfg->cncfg.client_network_plugin_path != NULL) {
      return start_client_plugin(fcfg, logger, mqttSnForwarder);
    }
#endif
#ifdef WITH_LINUX_UDP_CLIENT_NETWORK
    if (!strcmp(fcfg->cncfg.client_network_protocol, "udp")) {
      return start_client_udp(fcfg, logger, mqttSnForwarder);
    }
#endif
#ifdef WITH_LINUX_TCP_CLIENT_NETWORK
    if (!strcmp(fcfg->cncfg.client_network_protocol, "tcp")) {
      return start_client_tcp(fcfg, logger, mqttSnForwarder);
    }
#endif
    log_str(logger, "Error init client network unknown protocol: ");
    log_str(logger, fcfg->cncfg.client_network_protocol);
    log_str(logger, "\n");
    return EXIT_FAILURE;
  }

  mqttSnForwarder->clientNetworkSendTimeout = fcfg->cncfg.client_network_send_timeout;
  mqttSnForwarder->clientNetworkReceiveTimeout = fcfg->cncfg.client_network_receive_timeout;
  mqttSnForwarder->gatewayNetworkSendTimeout = fcfg->gncfg.gateway_network_send_timeout;
  mqttSnForwarder->gatewayNetworkReceiveTimeout = fcfg->gncfg.gateway_network_receive_timeout;

  MqttSnLogger forwarder_logger = {0};
  if (MqttSnLoggerInit(&forwarder_logger, fcfg->mslcfg.log_lvl, stdout_log_init)) {
    MqttSnLoggerDeinit(&forwarder_logger);
    return EXIT_FAILURE;
  }

#ifdef WITH_LOGGING
  print_program_started(logger, &fw_cfg.msvcfg, fw_cfg.executable_name);
#endif

  if (MqttSnForwarderInit(mqttSnForwarder,
                          &forwarder_logger,
                          fcfg->mslcfg.log_lvl,
                          clientNetworkContext,
                          gatewayNetworkContext) != 0) {
    MqttSnForwarderDeinit(mqttSnForwarder);
    return EXIT_FAILURE;
  }

  runner = thread(&MqttSnForwarderTestContainer::loop, this);
  runner.detach();

  return EXIT_SUCCESS;
}
void MqttSnForwarderTestContainer::stop() {
  stopped = true;
}
bool MqttSnForwarderTestContainer::isStopped() {
  return stopped;
}
bool MqttSnForwarderTestContainer::isRunning() {
  return running;
}
void MqttSnForwarderTestContainer::loop() {
  running = false;
  stopped = false;

  while ((MqttSnForwarderLoop(&forwarder) == 0) & !stopped) {}

#ifdef WITH_LOGGING
  print_program_terminated(&forwarder.logger, &fw_cfg.msvcfg, fw_cfg.executable_name);
#endif

  MqttSnForwarderDeinit(&forwarder);

  forwarder_config_cleanup(&fw_cfg);
  running = false;
}

#ifdef WITH_LINUX_PLUGIN_NETWORK
int32_t MqttSnForwarderTestContainer::start_client_plugin(const forwarder_config *fcfg,
                                                          const MqttSnLogger *logger,
                                                          MqttSnForwarder *mqttSnForwarder) {

  device_address mqttSnGatewayNetworkAddress = {0};
  device_address forwarderClientNetworkAddress = {0};
  device_address forwarderClientNetworkBroadcastAddress = {0};

  if (convert_hostname_port_to_device_address(fcfg->msgcfg.mqtt_sn_gateway_host,
                                              fcfg->msgcfg.mqtt_sn_gateway_port,
                                              &mqttSnGatewayNetworkAddress,
                                              "mqtt-sn gateway")) {
    return EXIT_FAILURE;
  }

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

  const client_plugin_device_address
      pluginMqttSnForwarderNetworkAddress(forwarderClientNetworkAddress.bytes, sizeof(device_address));

  client_plugin_config plugin_cfg(fcfg->cncfg.client_network_plugin_path,
                                  fcfg->cncfg.client_network_protocol,
                                  MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH,
                                  MQTT_SN_DEVICE_ADDRESS_LENGTH,
                                  &pluginMqttSnForwarderNetworkAddress);

  MqttSnClientPluginContext clientPluginContext(&plugin_cfg);

  if (ClientNetworkInitialize(&mqttSnForwarder->clientNetwork,
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

  return start();
}
#endif

#ifdef WITH_LINUX_TCP_CLIENT_NETWORK
int32_t MqttSnForwarderTestContainer::start_client_tcp(const forwarder_config *fcfg,
                                                       const MqttSnLogger *logger,
                                                       MqttSnForwarder *mqttSnForwarder) {

  device_address mqttSnGatewayNetworkAddress = {0};
  device_address forwarderClientNetworkAddress = {0};
  device_address forwarderClientNetworkBroadcastAddress = {0};
  MqttSnClientTcpNetwork tcpClientNetworkContext = {0};

  if (convert_hostname_port_to_device_address(fcfg->msgcfg.mqtt_sn_gateway_host,
                                              fcfg->msgcfg.mqtt_sn_gateway_port,
                                              &mqttSnGatewayNetworkAddress,
                                              "mqtt-sn gateway")) {
    return EXIT_FAILURE;
  }

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

  if (ClientNetworkInitialize(&mqttSnForwarder->clientNetwork,
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

  return start();
}
#endif

#ifdef WITH_LINUX_UDP_CLIENT_NETWORK
int32_t MqttSnForwarderTestContainer::start_client_udp(const forwarder_config *fcfg,
                                                       const MqttSnLogger *logger,
                                                       MqttSnForwarder *mqttSnForwarder) {

  device_address forwarderClientNetworkAddress = {0};
  device_address mqttSnGatewayNetworkAddress = {0};
  device_address forwarderClientNetworkBroadcastAddress = {0};
  MqttSnClientUdpNetwork udpClientNetworkContext = {0};

  if (convert_hostname_port_to_device_address(fcfg->msgcfg.mqtt_sn_gateway_host,
                                              fcfg->msgcfg.mqtt_sn_gateway_port,
                                              &mqttSnGatewayNetworkAddress,
                                              "mqtt-sn gateway")) {
    return EXIT_FAILURE;
  }

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

  if (ClientNetworkInitialize(&mqttSnForwarder->clientNetwork,
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

  return start();
}
#endif

#ifdef WITH_LINUX_PLUGIN_NETWORK
int32_t MqttSnForwarderTestContainer::start_gateway_plugin(const forwarder_config *cfg,
                                                           const MqttSnLogger *logger,
                                                           MqttSnForwarder *mqttSnForwarder) {

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

  const gateway_plugin_device_address pluginMqttSnGatewayNetworkAddress(mqttSnGatewayNetworkAddress.bytes,
                                                                        sizeof(device_address));

  const gateway_plugin_device_address pluginForwarderGatewayNetworkAddress(forwarderGatewayNetworkAddress.bytes,
                                                                           sizeof(device_address));

  const gateway_plugin_device_address
      pluginForwarderGatewayNetworkBroadcastAddress(forwarderGatewayNetworkBroadcastAddress.bytes,
                                                    sizeof(device_address));

  gateway_plugin_config plugin_cfg(cfg->gncfg.gateway_network_plugin_path,
                                   cfg->gncfg.gateway_network_protocol,
                                   MQTT_SN_DEVICE_ADDRESS_LENGTH,
                                   MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH,
                                   &pluginMqttSnGatewayNetworkAddress,
                                   &pluginForwarderGatewayNetworkAddress,
                                   &pluginForwarderGatewayNetworkBroadcastAddress);

  MqttSnGatewayPluginContext gatewayPluginContext(&plugin_cfg);

  if (GatewayNetworkInitialize(&mqttSnForwarder->gatewayNetwork,
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

  return start();
}
#endif

#ifdef WITH_LINUX_TCP_GATEWAY_NETWORK
int32_t MqttSnForwarderTestContainer::start_gateway_tcp(const forwarder_config *cfg,
                                                        const MqttSnLogger *logger,
                                                        MqttSnForwarder *mqttSnForwarder) {

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

  if (GatewayNetworkInitialize(&mqttSnForwarder->gatewayNetwork,
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

  return start();
}
#endif

#ifdef WITH_LINUX_UDP_GATEWAY_NETWORK
int32_t MqttSnForwarderTestContainer::start_gateway_udp(const forwarder_config *cfg,
                                                        const MqttSnLogger *logger,
                                                        MqttSnForwarder *mqttSnForwarder) {

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

  if (GatewayNetworkInitialize(&mqttSnForwarder->gatewayNetwork,
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

  return start();
}
#endif
