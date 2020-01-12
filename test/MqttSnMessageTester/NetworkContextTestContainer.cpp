#include <memory>

#include <memory>

//
// Created by SomeDude on 18.07.2019.
//

#include "NetworkContextTestContainer.h"
#include <string.h>
#include <memory>
#include <network/linux/shared/ip/MqttSnIpNetworkHelper.h>
#include <network/shared/ip/IpHelper.h>
#include <logging/linux/stderr/StderrLogging.h>

using std::shared_ptr;

void *NetworkContextTestContainer::InitializeClientNetwork(MqttSnClientNetworkInterface *clientNetworkInterface,
                                                           const client_network_config *cncfg,
                                                           const client_mqtt_sn_gateway_config *optional_msgcfg) {
  int32_t init_rc = EXIT_FAILURE;

  MqttSnLogger cfg_logger = {0};
  if (MqttSnLoggerInit(&cfg_logger, LOG_LEVEL_VERBOSE, stderr_log_init)) {
    return nullptr;
  }
#ifdef WITH_LINUX_PLUGIN_NETWORK_TODO
  if (cncfg->client_network_plugin_path != NULL) {
    init_rc = init_client_plugin(&cncfg, &cfg_logger, optional_msgcfg, &forwarder);
  } else
#endif
#ifdef WITH_LINUX_UDP_CLIENT_NETWORK
  if (!strcmp(cncfg->client_network_protocol, "udp")) {
    init_rc = init_client_udp(cncfg, optional_msgcfg, clientNetworkInterface, &cfg_logger);
  } else
#endif
#ifdef WITH_LINUX_TCP_CLIENT_NETWORK_TODO
    if (!strcmp(cncfg->client_network_protocol, "tcp")) {
      init_rc = init_client_tcp(&cncfg, &cfg_logger, optional_msgcfg, &forwarder);
    } else
#endif
  {
    log_str(&cfg_logger, "Error init client network unknown protocol: ");
    log_str(&cfg_logger, cncfg->client_network_protocol);
    log_str(&cfg_logger, "\n");
  }
  MqttSnLoggerDeinit(&cfg_logger);
  if (init_rc != EXIT_SUCCESS) {
    return nullptr;
  }
  return gatewayNetworkContext;
}
void *NetworkContextTestContainer::InitializeGatewayNetwork(MqttSnGatewayNetworkInterface *gatewayNetworkInterface,
                                                            const gateway_network_config *gncfg,
                                                            const client_mqtt_sn_gateway_config *msgcfg,
                                                            const client_find_mqtt_sn_gateway_config *cfmsgcfg) {
  int32_t init_rc = EXIT_FAILURE;

  MqttSnLogger cfg_logger = {0};
  if (MqttSnLoggerInit(&cfg_logger, LOG_LEVEL_VERBOSE, stderr_log_init)) {
    return nullptr;
  }
#ifdef WITH_LINUX_PLUGIN_NETWORK_TODO
  if (gncfg->gateway_network_plugin_path != NULL) {
    init_rc = init_gateway_plugin(gncfg, &cfg_logger, msgcfg, cfmsgcfg, gatewayNetworkInterface);
  } else
#endif
#ifdef WITH_LINUX_UDP_GATEWAY_NETWORK
  if (!strcmp(gncfg->gateway_network_protocol, "udp")) {
    init_rc = init_gateway_udp(gncfg, msgcfg, cfmsgcfg, gatewayNetworkInterface, &cfg_logger);
  } else
#endif
#ifdef WITH_LINUX_TCP_GATEWAY_NETWORK_TODO
    if (!strcmp(gncfg->gateway_network_protocol, "tcp")) {
      init_rc = init_gateway_tcp(gncfg, &cfg_logger, msgcfg, cfmsgcfg, gatewayNetworkInterface);
    } else
#endif
  {
    log_str(&cfg_logger, "Error init gateway network unknown protocol: ");
    log_str(&cfg_logger, gncfg->gateway_network_protocol);
    log_str(&cfg_logger, "\n");
  }
  MqttSnLoggerDeinit(&cfg_logger);
  if (init_rc != EXIT_SUCCESS) {
    return nullptr;
  }
  return gatewayNetworkContext;
}
#ifdef WITH_LINUX_UDP_CLIENT_NETWORK
int32_t NetworkContextTestContainer::init_gateway_udp(const gateway_network_config *gncfg,
                                                      const client_mqtt_sn_gateway_config *msgcfg,
                                                      const client_find_mqtt_sn_gateway_config *cfmsgcfg,
                                                      MqttSnGatewayNetworkInterface *gatewayNetworkInterface,
                                                      const MqttSnLogger *logger) {

  if (msgcfg == nullptr && cfmsgcfg == nullptr) {
    return EXIT_FAILURE;
  }
  udpGatewayNetworkContext = std::make_shared<MqttSnGatewayUdpNetwork>();

  if (msgcfg) {
    if (convert_hostname_port_to_device_address(msgcfg->mqtt_sn_gateway_host,
                                                msgcfg->mqtt_sn_gateway_port,
                                                &mqttSnGatewayNetworkAddress,
                                                "mqtt-sn gateway")) {
      return EXIT_FAILURE;
    }
  } else {
    memset(&mqttSnGatewayNetworkAddress, 0, sizeof(device_address));
  }

  if (convert_hostname_port_to_device_address(gncfg->gateway_network_bind_address,
                                              gncfg->gateway_network_bind_port,
                                              &gatewayNetworkAddress,
                                              "gateway unicast")) {
    return EXIT_FAILURE;
  }
  if (convert_string_ip_port_to_device_address(logger,
                                               gncfg->gateway_network_broadcast_address,
                                               gncfg->gateway_network_broadcast_bind_port,
                                               &gatewayNetworkBroadcastAddress,
                                               "gateway broadcast")) {
    return EXIT_FAILURE;
  }

  if (GatewayNetworkInitialize(gatewayNetworkInterface,
                               MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH,
                               &mqttSnGatewayNetworkAddress,
                               &gatewayNetworkAddress,
                               &gatewayNetworkBroadcastAddress,
                               udpGatewayNetworkContext.get(),
                               GatewayLinuxUdpInitialize)) {
    log_str(logger, "Error init gateway network\n");
    return EXIT_FAILURE;
  }

  gatewayNetworkContext = udpGatewayNetworkContext.get();
  return EXIT_SUCCESS;
}
int32_t NetworkContextTestContainer::init_client_udp(const client_network_config *cncfg,
                                                     const client_mqtt_sn_gateway_config *optional_msgcfg,
                                                     MqttSnClientNetworkInterface *clientNetworkInterface,
                                                     const MqttSnLogger *logger) {
  auto udpClientNetwosqrkContext = std::make_shared<MqttSnClientUdpNetwork>();

  if (optional_msgcfg) {
    if (convert_hostname_port_to_device_address(optional_msgcfg->mqtt_sn_gateway_host,
                                                optional_msgcfg->mqtt_sn_gateway_port,
                                                &mqttSnGatewayNetworkAddress,
                                                "mqtt-sn gateway")) {
      return EXIT_FAILURE;
    }
  } else {
    memset(&mqttSnGatewayNetworkAddress, 0, sizeof(device_address));
  }

  if (convert_hostname_port_to_device_address(cncfg->client_network_bind_address,
                                              cncfg->client_network_bind_port,
                                              &clientNetworkAddress,
                                              "client unicast")) {
    return EXIT_FAILURE;
  }
  if (convert_string_ip_port_to_device_address(logger,
                                               cncfg->client_network_broadcast_address,
                                               cncfg->client_network_broadcast_bind_port,
                                               &clientNetworkBroadcastAddress,
                                               "client broadcast")) {
    return EXIT_FAILURE;
  }

  if (ClientNetworkInitialize(clientNetworkInterface,
                              MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH,
                              &mqttSnGatewayNetworkAddress,
                              &clientNetworkAddress,
                              &clientNetworkBroadcastAddress,
                              udpClientNetworkContext.get(),
                              ClientLinuxUdpInitialize)) {
    log_str(logger, "Error init client network\n");
    return EXIT_FAILURE;
  }

  clientNetworkContext = udpClientNetworkContext.get();
  return EXIT_SUCCESS;
}
#endif
