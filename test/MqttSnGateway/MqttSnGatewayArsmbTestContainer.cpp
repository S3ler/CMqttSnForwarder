//
// Created by SomeDude on 09.07.2019.
//

#include "MqttSnGatewayArsmbTestContainer.h"
#include <config/common/config_command_helper.h>
#include <config/common/starter/starter_helper.h>
#include <gateway/config/gateway_config.h>
#include <gateway/starter/linux/gateway_starter.h>
#include <logging/linux/file/FileLogging.h>
#include <logging/linux/filestdout/FileStdoutLogging.h>
#include <logging/linux/stderr/StderrLogging.h>
#include <logging/linux/stdout/StdoutLogging.h>
#include <network/linux/client/ip/tcp/MqttSnClientTcpNetwork.h>
#include <network/linux/client/plugin/MqttSnClientPluginNetwork.h>
#include <network/linux/client/plugin/client_network_plugin_interface.h>
#include <network/linux/shared/ip/MqttSnIpNetworkHelper.h>
#include <network/shared/ip/IpHelper.h>
#include <platform/platform_compatibility.h>
#include <vector>

//MqttSnGatewayArsmbTestContainer::MqttSnGatewayArsmbTestContainer(const string &identifier, const string &cmd)
//    : MqttSnGatewayTestContainerInterface(identifier, cmd) {}
MqttSnGatewayArsmbTestContainer::MqttSnGatewayArsmbTestContainer(const MqttSnGatewayTestContainerConfiguration &configuration)
    : MqttSnGatewayTestContainerInterface(configuration) {
  string gw_identifier = "ArmsbMqttSnGateway";
  string gw_cmd = "-db --log_identifier MqttSnGateway -lfp MqttSnGateway.log -cp 10000 --gateway_id 1";
  identifier = gw_identifier;
  cmd = gw_cmd;
}
int32_t MqttSnGatewayArsmbTestContainer::initialize() {
  if (isRunning()) {
    return EXIT_FAILURE;
  }

  MqttSnLogger cfg_logger = {0};
  if (MqttSnLoggerInit(&cfg_logger, LOG_LEVEL_VERBOSE, stderr_log_init)) {
    return EXIT_FAILURE;
  }

  if (mqtt_sn_gateway__config_init(&gateway_config) < 0) {
    return EXIT_FAILURE;
  }

  std::copy(cmd.begin(), cmd.end(), std::back_inserter(line_str));
  line_str.push_back('\0');
  int32_t rc = process_config_cmd_str(&cfg_logger,
                                      line_str.data(),
                                      line_str.size(),
                                      identifier.data(),
                                      &gateway_config,
                                      mqtt_sn_gateway__config_file_process_command_callback,
                                      argv,
                                      &argc,
                                      argv_max_len);

  if (rc) {
    mqtt_sn_gateway__config_cleanup(&gateway_config);
    MqttSnLoggerDeinit(&cfg_logger);
    return EXIT_FAILURE;
  }

  if (start_logger(&gateway_config.mslcfg, &gw_logger) < 0) {
    log_str(&cfg_logger, "Could not initialize mqtt sn logger\n");
    MqttSnLoggerDeinit(&cfg_logger);
    return EXIT_FAILURE;
  }

  MqttSnLoggerDeinit(&cfg_logger);
  return EXIT_SUCCESS;
}
int32_t MqttSnGatewayArsmbTestContainer::start_logger(const mqtt_sn_logger_config *cfg, MqttSnLogger *logger) {
  if (!strcmp(cfg->log_target, "console")) {
    if (cfg->log_filepath != NULL) {
      if (MqttSnLoggerInitFile(logger,
                               cfg->log_lvl,
                               cfg->log_filepath,
                               file_stdout_log_init,
                               &file_stdout_logging_context_) < 0) {
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
int32_t MqttSnGatewayArsmbTestContainer::start() {
  if (initialize() != 0) {
    return -1;
  }
  /*
  const mqtt_sn_gateway__config *fcfg = &gateway_config;
  MqttSnGateway *mqtt_sn_gateway = &gateway;
  const MqttSnLogger *logger = &gw_&gw_logger;
  */
  if (clientNetworkContext == NULL) {
#ifdef WITH_LINUX_PLUGIN_NETWORK
    /*
    if (gateway_config.cncfg.client_network_plugin_path != NULL) {
      return start_mqtt_sn_gateway_plugin(&gateway_config, &gw_logger, &gateway);
    }
     */
#endif
#ifdef WITH_LINUX_TCP_CLIENT_NETWORK
    if (!strcmp(gateway_config.cncfg.client_network_protocol, "tcp")) {
      return start_mqtt_sn_gateway_tcp(&gateway_config, &gw_logger, &gateway);
    }
#endif
#ifdef WITH_LINUX_UDP_CLIENT_NETWORK
    if (!strcmp(gateway_config.cncfg.client_network_protocol, "udp")) {
      return start_mqtt_sn_gateway_client_udp(&gateway_config, &gw_logger, &gateway);
    }
#endif
    log_str(&gw_logger, PSTR("Error init client network unknown protocol:"));
    log_str(&gw_logger, gateway_config.cncfg.client_network_protocol);
    log_str(&gw_logger, PSTR("\n"));
    MqttSnLoggerDeinit(&gateway.logger);
    return EXIT_FAILURE;
  }

  gateway.clientNetworkSendTimeout = gateway_config.cncfg.client_network_send_timeout;
  gateway.clientNetworkReceiveTimeout = gateway_config.cncfg.client_network_receive_timeout;

#ifdef WITH_LOGGING
  print_program_started(&gateway.logger, &gateway_config.msvcfg, gateway_config.executable_name);
#endif

  // FIXME MqttClient
  if (MqttSnGatewayInitialize(&gateway,
                              &gw_logger,
                              NULL,
                              clientNetworkContext,
                              &gateway_config.gacfg,
                              &gateway_config.gccccfg) < 0) {

    MqttSnGatewayDeinitialize(&gateway);
    MqttSnLoggerDeinit(&gateway.logger);
    return EXIT_FAILURE;
  }

  if (MqttSnGatewayConnect(&gateway) < 0) {
    MqttSnGatewayDisconnect(&gateway);
#ifdef WITH_LOGGING
    print_program_terminated(&gateway.logger, &gateway_config.msvcfg, gateway_config.executable_name);
#endif
    MqttSnGatewayDeinitialize(&gateway);
    MqttSnLoggerDeinit(&gateway.logger);
    running = false;
    return EXIT_FAILURE;
  }

  runner = thread(&MqttSnGatewayArsmbTestContainer::loop, this);
  runner.detach();

  return EXIT_SUCCESS;
}
void MqttSnGatewayArsmbTestContainer::stop() {
  stopped = true;
}
bool MqttSnGatewayArsmbTestContainer::isStopped() {
  return stopped;
}
bool MqttSnGatewayArsmbTestContainer::isRunning() {
  return running;
}
void MqttSnGatewayArsmbTestContainer::loop() {
  stopped = false;
  running = true;

  while ((MqttSnGatewayLoop(&gateway) >= 0) & !stopped) {}

  MqttSnGatewayDisconnect(&gateway);
#ifdef WITH_LOGGING
  print_program_terminated(&gateway.logger, &gateway_config.msvcfg, gateway_config.executable_name);
#endif
  MqttSnGatewayDeinitialize(&gateway);
  MqttSnLoggerDeinit(&gateway.logger);
  mqtt_sn_gateway__config_cleanup(&gateway_config);
  running = false;
}

#ifdef WITH_LINUX_PLUGIN_NETWORK
/*
int MqttSnGatewayArsmbTestContainer::start_mqtt_sn_gateway_plugin(const mqtt_sn_gateway__config *fcfg,
                                                                  const MqttSnLogger *logger,
                                                                  MqttSnGateway *mqtt_sn_gateway) {
  // TODO adept - do not use atm
*/
  /*
  if (convert_hostname_port_to_device_address(fcfg->msgcfg.mqtt_sn_gateway_host,
                                              fcfg->msgcfg.mqtt_sn_gateway_port,
                                              &mqttSnGatewayNetworkAddress,
                                              "mqtt-sn gateway")) {
    return EXIT_FAILURE;
  }
  */
/*
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

client_plugin_device_address
    pluginMqttSnForwarderNetworkAddress(forwarderClientNetworkAddress.bytes, sizeof(device_address));

client_plugin_config plugin_cfg(fcfg->cncfg.client_network_plugin_path,
                                fcfg->cncfg.client_network_protocol,
                                MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH,
                                MQTT_SN_DEVICE_ADDRESS_LENGTH,
                                &pluginMqttSnForwarderNetworkAddress);

MqttSnClientPluginContext clientPluginContext(&plugin_cfg);

if (ClientNetworkInitialize(&mqtt_sn_gateway->clientNetwork,
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
*/
#endif

#ifdef WITH_LINUX_TCP_CLIENT_NETWORK
int MqttSnGatewayArsmbTestContainer::start_mqtt_sn_gateway_tcp(const mqtt_sn_gateway__config *fcfg,
                                                               const MqttSnLogger *logger,
                                                               MqttSnGateway *mqtt_sn_gateway) {

  MqttSnClientTcpNetwork tcpClientNetworkContext = {0};

  /*
  if (convert_hostname_port_to_device_address(fcfg->msgcfg.mqtt_sn_gateway_host,
                                              fcfg->msgcfg.mqtt_sn_gateway_port,
                                              &mqttSnGatewayNetworkAddress,
                                              "mqtt-sn gateway")) {
    return EXIT_FAILURE;
  }
  */

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

  if (ClientNetworkInitialize(&mqtt_sn_gateway->clientNetwork,
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
int MqttSnGatewayArsmbTestContainer::start_mqtt_sn_gateway_client_udp(const mqtt_sn_gateway__config *fcfg,
                                                                      const MqttSnLogger *logger,
                                                                      MqttSnGateway *mqtt_sn_gateway) {

  udpClientNetworkContext = shared_ptr<MqttSnClientUdpNetwork>(new MqttSnClientUdpNetwork());

  /*
  if (convert_hostname_port_to_device_address(fcfg->msgcfg.mqtt_sn_gateway_host,
                                              fcfg->msgcfg.mqtt_sn_gateway_port,
                                              &mqttSnGatewayNetworkAddress,
                                              "mqtt-sn gateway")) {
    return EXIT_FAILURE;
  }
  */

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

  if (ClientNetworkInitialize(&mqtt_sn_gateway->clientNetwork,
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
