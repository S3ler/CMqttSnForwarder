//
// Created by SomeDude on 09.07.2019.
//

#include "MqttSnForwarderTestContainer.h"
#include <config/common/config_command_helper.h>
#include <config/common/starter/starter_helper.h>
#include <forwarder/config/forwarder_config.h>
#include <logging/linux/file/FileLogging.h>
#include <logging/linux/filestdout/FileStdoutLogging.h>
#include <logging/linux/stderr/StderrLogging.h>
#include <logging/linux/stdout/StdoutLogging.h>
#include <network/linux/client/ip/tcp/MqttSnClientTcpNetwork.h>
#include <network/linux/client/plugin/MqttSnClientPluginNetwork.h>
#include <network/linux/client/plugin/client_network_plugin_interface.h>
#include <network/linux/gateway/ip/tcp/MqttSnGatewayTcpNetwork.h>
#include <network/linux/gateway/plugin/MqttSnGatewayPluginNetwork.h>
#include <network/linux/gateway/plugin/gateway_network_plugin_interface.h>
#include <network/linux/shared/ip/MqttSnIpNetworkHelper.h>
#include <network/shared/ip/IpHelper.h>
#include <string.h>

MqttSnForwarderTestContainer::MqttSnForwarderTestContainer(const string &identifier, const string &cmd)
    : identifier(identifier), cmd(cmd) {}

int32_t MqttSnForwarderTestContainer::initialize() {
  if (isRunning()) {
    return -1;
  }

  MqttSnLogger cfg_logger = {0};
  if (MqttSnLoggerInit(&cfg_logger, LOG_LEVEL_VERBOSE, stderr_log_init)) {
    return EXIT_FAILURE;
  }

  if (forwarder_config_init(&fw_cfg) < 0) {
    return EXIT_FAILURE;
  }

  std::copy(cmd.begin(), cmd.end(), std::back_inserter(line_str));
  int32_t rc = process_config_cmd_str(&cfg_logger,
                                      line_str.data(),
                                      line_str.size() + 1,
                                      identifier.data(),
                                      &fw_cfg,
                                      forwarder_config_file_process_command_callback,
                                      argv,
                                      &argc,
                                      argv_max_len);

  if (rc) {
    forwarder_config_cleanup(&fw_cfg);
    MqttSnLoggerDeinit(&cfg_logger);
    return EXIT_FAILURE;
  }

  if (start_logger(&fw_cfg.mslcfg, &fw_logger) < 0) {
    log_str(&cfg_logger, "Could not initialize mqtt sn logger\n");
    MqttSnLoggerDeinit(&cfg_logger);
    return EXIT_FAILURE;
  }

  MqttSnLoggerDeinit(&cfg_logger);
  return EXIT_SUCCESS;
}
int32_t MqttSnForwarderTestContainer::start_logger(const mqtt_sn_logger_config *cfg, MqttSnLogger *logger) {
  if (!strcmp(cfg->log_target, "console")) {
    if (cfg->log_filepath != NULL) {
      if (MqttSnLoggerInitFile(logger, cfg->log_lvl, cfg->log_filepath, file_stdout_log_init, &file_stdout_logging_context_) < 0) {
        return -1;
      }
    } else {
      if (MqttSnLoggerInit(logger, cfg->log_lvl, stdout_log_init) < 0) {
        return -1;
      }
    }
  } else if (!strcmp(cfg->log_target, "file")) {
    if (MqttSnLoggerInitFile(logger, cfg->log_lvl, cfg->log_filepath, file_log_init, &file_logging_context_) < 0) {
      return -1;
    }
  }
  if (cfg->log_identifier) {
    logger->log_identifier = cfg->log_identifier;
  }
  return 0;
}
int32_t MqttSnForwarderTestContainer::start() {

  if ((gatewayNetworkContext == NULL && clientNetworkContext != NULL)
      || (gatewayNetworkContext == NULL && clientNetworkContext == NULL)) {

#ifdef WITH_LINUX_PLUGIN_NETWORK
    if (fw_cfg.gncfg.gateway_network_plugin_path != NULL) {
      return start_gateway_plugin(&fw_cfg, &fw_logger, &forwarder);
    }
#endif
#ifdef WITH_LINUX_UDP_GATEWAY_NETWORK
    if (!strcmp(fw_cfg.gncfg.gateway_network_protocol, "udp")) {
      return start_gateway_udp(&fw_cfg, &fw_logger, &forwarder);
    }
#endif
#ifdef WITH_LINUX_TCP_GATEWAY_NETWORK
    if (!strcmp(fw_cfg.gncfg.gateway_network_protocol, "tcp")) {
      return start_gateway_tcp(&fw_cfg, &fw_logger, &forwarder);
    }
#endif
    log_str(&fw_logger, "Error init gateway network unknown protocol: ");
    log_str(&fw_logger, fw_cfg.gncfg.gateway_network_protocol);
    log_str(&fw_logger, "\n");
  }

  if (gatewayNetworkContext != NULL && clientNetworkContext == NULL) {
#ifdef WITH_LINUX_PLUGIN_NETWORK
    if (fw_cfg.cncfg.client_network_plugin_path != NULL) {
      return start_client_plugin(&fw_cfg, &fw_logger, &forwarder);
    }
#endif
#ifdef WITH_LINUX_UDP_CLIENT_NETWORK
    if (!strcmp(fw_cfg.cncfg.client_network_protocol, "udp")) {
      return start_client_udp(&fw_cfg, &fw_logger, &forwarder);
    }
#endif
#ifdef WITH_LINUX_TCP_CLIENT_NETWORK
    if (!strcmp(fw_cfg.cncfg.client_network_protocol, "tcp")) {
      return start_client_tcp(&fw_cfg, &fw_logger, &forwarder);
    }
#endif
    log_str(&fw_logger, "Error init client network unknown protocol: ");
    log_str(&fw_logger, fw_cfg.cncfg.client_network_protocol);
    log_str(&fw_logger, "\n");
    return EXIT_FAILURE;
  }

  forwarder.clientNetworkSendTimeout = fw_cfg.cncfg.client_network_send_timeout;
  forwarder.clientNetworkReceiveTimeout = fw_cfg.cncfg.client_network_receive_timeout;
  forwarder.gatewayNetworkSendTimeout = fw_cfg.gncfg.gateway_network_send_timeout;
  forwarder.gatewayNetworkReceiveTimeout = fw_cfg.gncfg.gateway_network_receive_timeout;

#ifdef WITH_LOGGING
  print_program_started(&fw_logger, &fw_cfg.msvcfg, fw_cfg.executable_name);
#endif

  if (MqttSnForwarderInit(&forwarder, &fw_logger, &forwarder.mqtt_sn_gateway_address) != 0) {
    MqttSnForwarderDeinit(&forwarder);
    MqttSnLoggerDeinit(&fw_logger);
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
  // TODO adept - do not use atm
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

  const client_plugin_device_address pluginMqttSnForwarderNetworkAddress(forwarderClientNetworkAddress.bytes,
                                                                         sizeof(device_address));

  client_plugin_config plugin_cfg(fcfg->cncfg.client_network_plugin_path,
                                  fcfg->cncfg.client_network_protocol,
                                  MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH,
                                  MQTT_SN_DEVICE_ADDRESS_LENGTH,
                                  &pluginMqttSnForwarderNetworkAddress);

  clientPluginContext = shared_ptr<MqttSnClientPluginContext>(new MqttSnClientPluginContext(&plugin_cfg));

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

  tcpClientNetworkContext = shared_ptr<MqttSnClientTcpNetwork>(new MqttSnClientTcpNetwork());

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

  udpClientNetworkContext = shared_ptr<MqttSnClientUdpNetwork>(new MqttSnClientUdpNetwork());

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
                              udpClientNetworkContext.get(),
                              ClientLinuxUdpInitialize)) {
    log_str(logger, "Error init client network\n");
    return EXIT_FAILURE;
  }

  clientNetworkContext = udpClientNetworkContext.get();

  return start();
}
#endif

#ifdef WITH_LINUX_PLUGIN_NETWORK
int32_t MqttSnForwarderTestContainer::start_gateway_plugin(const forwarder_config *cfg,
                                                           const MqttSnLogger *logger,
                                                           MqttSnForwarder *mqttSnForwarder) {
  // TODO adept - do not use atm
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

  pluginMqttSnGatewayNetworkAddress = shared_ptr<gateway_plugin_device_address>(
      new gateway_plugin_device_address(
          mqttSnGatewayNetworkAddress.bytes,
          sizeof(device_address)));

  pluginForwarderGatewayNetworkAddress = shared_ptr<gateway_plugin_device_address>(
      new gateway_plugin_device_address(
          forwarderGatewayNetworkAddress.bytes,
          sizeof(device_address)));

  pluginForwarderGatewayNetworkBroadcastAddress = shared_ptr<gateway_plugin_device_address>(
      new gateway_plugin_device_address(
          forwarderGatewayNetworkBroadcastAddress.bytes,
          sizeof(device_address)));

  plugin_cfg = shared_ptr<gateway_plugin_config>(
      new gateway_plugin_config(cfg->gncfg.gateway_network_plugin_path,
                                cfg->gncfg.gateway_network_protocol,
                                MQTT_SN_DEVICE_ADDRESS_LENGTH,
                                MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH,
                                pluginMqttSnGatewayNetworkAddress.get(),
                                pluginForwarderGatewayNetworkAddress.get(),
                                pluginForwarderGatewayNetworkBroadcastAddress.get()));

  gatewayPluginContext = shared_ptr<MqttSnGatewayPluginContext>(new MqttSnGatewayPluginContext(plugin_cfg.get()));

  if (GatewayNetworkInitialize(&mqttSnForwarder->gatewayNetwork,
                               MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH,
                               &mqttSnGatewayNetworkAddress,
                               &forwarderGatewayNetworkAddress,
                               &forwarderGatewayNetworkBroadcastAddress,
                               gatewayPluginContext.get(),
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

  tcpGatewayNetworkContext = shared_ptr<MqttSnGatewayTcpNetwork>(new MqttSnGatewayTcpNetwork());

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

  udpGatewayNetworkContext = shared_ptr<MqttSnGatewayUdpNetwork>(new MqttSnGatewayUdpNetwork());

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
                               udpGatewayNetworkContext.get(),
                               GatewayLinuxUdpInitialize)) {
    log_str(logger, "Error init gateway network\n");
    return EXIT_FAILURE;
  }
  gatewayNetworkContext = udpGatewayNetworkContext.get();

  return start();
}
#endif
