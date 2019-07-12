//
// Created by SomeDude on 09.07.2019.
//

#include <config/starter/starter_helper.h>
#include <client/pub/config/publish_client_config.h>
#include <client/pub/linux/starter/publish_client_starter.h>
#include <logging/linux/stdout/StdoutLogging.h>
#include <config/common/config_command_helper.h>
#include <string.h>
#include <network/linux/shared/ip/MqttSnIpNetworkHelper.h>
#include <network/shared/ip/IpHelper.h>
#include <network/linux/gateway/plugin/gateway_network_plugin_interface.h>
#include <network/linux/gateway/plugin/MqttSnGatewayPluginNetwork.h>
#include <network/linux/gateway/ip/udp/MqttSnGatewayUdpNetwork.h>
#include <network/linux/gateway/ip/tcp/MqttSnGatewayTcpNetwork.h>
#include <client/MqttSnClientLogger.h>
#include "MqttSnClientTestContainer.h"

using std::shared_ptr;

MqttSnClientTestContainer::MqttSnClientTestContainer(const string &identifier, const string &cmd) : identifier(
    identifier), cmd(cmd) {}
int32_t MqttSnClientTestContainer::initialize() {
  if (isRunning()) {
    return EXIT_FAILURE;
  }
  if (MqttSnLoggerInit(&client_logger, LOG_LEVEL_VERBOSE, stdout_log_init)) {
    return EXIT_FAILURE;
  }

  if (publish_client_config_init(&client_cfg) < 0) {
    return EXIT_FAILURE;
  }

  int32_t rc = process_config_file_line(&client_logger,
                                        cmd.data(),
                                        cmd.length() + 1,
                                        identifier.data(),
                                        &client_cfg,
                                        publish_client_config_file_process_command_callback);

  if (rc) {
    publish_client_config_cleanup(&client_cfg);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
int32_t MqttSnClientTestContainer::start() {
  /*
  const publish_client_config *cfg = &client_cfg;
  const MqttSnLogger *logger = &client_logger;
  MqttSnClient *publish_client = &client;
  */
  if (gatewayNetworkContext == NULL) {
#ifdef WITH_LINUX_PLUGIN_NETWORK
    if (client_cfg.gncfg.gateway_network_plugin_path != NULL) {
      return start_publish_client_plugin(&client_cfg, &client, &client_logger);
    }
#endif
#ifdef WITH_LINUX_UDP_GATEWAY_NETWORK
    if (!strcmp(client_cfg.gncfg.gateway_network_protocol, "udp")) {
      return start_publish_client_udp(&client_cfg, &client, &client_logger);
    }
#endif
#ifdef WITH_LINUX_TCP_GATEWAY_NETWORK
    if (!strcmp(client_cfg.gncfg.gateway_network_protocol, "tcp")) {
      return start_publish_client_tcp(&client_cfg, &client, &client_logger);
    }
#endif
    log_str(&client_logger, "Error init gateway network unknown protocol: ");
    log_str(&client_logger, client_cfg.gncfg.gateway_network_protocol);
    log_str(&client_logger, "\n");
    return EXIT_FAILURE;
  }

  client.gatewayNetworkSendTimeout = client_cfg.gncfg.gateway_network_send_timeout;
  client.gatewayNetworkReceiveTimeout = client_cfg.gncfg.gateway_network_receive_timeout;
#ifdef WITH_LOGGING
  print_program_started(&client_logger, &client_cfg.msvcfg, client_cfg.executable_name);
#endif
  if (MqttSnClientInit(&client, client_cfg.mslcfg.log_lvl, gatewayNetworkContext) < 0) {
    MqttSnClientDeinit(&client);
    return EXIT_FAILURE;
  }

  runner = thread(&MqttSnClientTestContainer::loop, this);
  runner.detach();

  return EXIT_SUCCESS;
}
void MqttSnClientTestContainer::stop() {
  stopped = true;
}
bool MqttSnClientTestContainer::isStopped() {
  return stopped;
}
bool MqttSnClientTestContainer::isRunning() {
  return running;
}
void MqttSnClientTestContainer::loop() {
  stopped = false;
  running = false;

  /*
  const publish_client_config *cfg = &client_cfg;
  MqttSnClient *publish_client = &client;
  */
  MQTT_SN_CLIENT_RETURN_CODE connect_rc;
  MqttSnClientTimeoutOffset(&client, client_cfg.cccfg.connect_timeout_offset);
  if ((connect_rc = MqttSnClientConnect(&client,
                                        client.gatewayNetwork.mqtt_sn_gateway_address,
                                        client_cfg.cccfg.client_connect_timeout_ms,
                                        client_cfg.cccfg.clean_session,
                                        client_cfg.cccfg.client_id,
                                        client_cfg.cccfg.connect_duration)) != MQTT_SN_CLIENT_RETURN_SUCCESS) {

    log_mqtt_sn_client(&client.logger, connect_rc);
    // TODO log error here
#ifdef WITH_LOGGING
    print_program_terminated(&client.logger, &client_cfg.msvcfg, client_cfg.executable_name);
#endif
    MqttSnClientDeinit(&client);
    return;
  }

#ifdef WITH_LOGGING
  print_program_started(&client.logger, &client_cfg.msvcfg, client_cfg.executable_name);
#endif

  while ((MqttSnClientLoop(&client) >= 0 && client.connected) & !stopped) {}

#ifdef WITH_LOGGING
  print_program_terminated(&client.logger, &client_cfg.msvcfg, client_cfg.executable_name);
#endif
  MqttSnClientDeinit(&client);

  publish_client_config_cleanup(&client_cfg);

  running = false;
}
#ifdef WITH_LINUX_PLUGIN_NETWORK
int32_t MqttSnClientTestContainer::start_publish_client_plugin(const publish_client_config *cfg,
                                                               MqttSnClient *publish_client,
                                                               const MqttSnLogger *logger) {
  // TODO adept - do not use atm

  mqttSnGatewayNetworkAddress = {0};
  forwarderGatewayNetworkAddress = {0};
  forwarderGatewayNetworkBroadcastAddress = {0};

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

  gatewayPluginContext = shared_ptr<MqttSnGatewayPluginContext>(new MqttSnGatewayPluginContext(&plugin_cfg));

  if (GatewayNetworkInitialize(&publish_client->gatewayNetwork,
                               MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH,
                               &mqttSnGatewayNetworkAddress,
                               &forwarderGatewayNetworkAddress,
                               &forwarderGatewayNetworkBroadcastAddress,
                               gatewayPluginContext.get(),
                               GatewayLinuxPluginInitialize)) {
    log_str(logger, "Error init gateway network\n");
    return EXIT_FAILURE;
  }
  gatewayNetworkContext = gatewayPluginContext.get();

  return start();
}
#endif
#ifdef WITH_LINUX_UDP_GATEWAY_NETWORK
int32_t MqttSnClientTestContainer::start_publish_client_udp(const publish_client_config *cfg,
                                                            MqttSnClient *publish_client,
                                                            const MqttSnLogger *logger) {

  mqttSnGatewayNetworkAddress = {0};
  forwarderGatewayNetworkAddress = {0};
  forwarderGatewayNetworkBroadcastAddress = {0};
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

  if (GatewayNetworkInitialize(&publish_client->gatewayNetwork,
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
#ifdef WITH_LINUX_TCP_GATEWAY_NETWORK
int32_t MqttSnClientTestContainer::start_publish_client_tcp(const publish_client_config *cfg,
                                                            MqttSnClient *publish_client,
                                                            const MqttSnLogger *logger) {
  mqttSnGatewayNetworkAddress = {0};
  forwarderGatewayNetworkAddress = {0};
  forwarderGatewayNetworkBroadcastAddress = {0};
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

  if (GatewayNetworkInitialize(&publish_client->gatewayNetwork,
                               MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH,
                               &mqttSnGatewayNetworkAddress,
                               &forwarderGatewayNetworkAddress,
                               &forwarderGatewayNetworkBroadcastAddress,
                               tcpGatewayNetworkContext.get(),
                               GatewayLinuxTcpInitialize)) {
    log_str(logger, "Error init client network\n");
    return EXIT_FAILURE;
  }
  gatewayNetworkContext = tcpGatewayNetworkContext.get();

  return start();
}
#endif