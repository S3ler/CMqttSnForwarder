//
// Created by SomeDude on 31.05.2019.
//

#include "publish_client_starter.h"
#include <stddef.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include <network/linux/shared/ip/MqttSnIpNetworkHelper.h>
#include <network/shared/ip/IpHelper.h>
#include <network/linux/gateway/plugin/gateway_network_plugin_interface.h>
#include <network/linux/gateway/plugin/MqttSnGatewayPluginNetwork.h>
#include <network/linux/gateway/ip/tcp/MqttSnGatewayTcpNetwork.h>
#include <stdlib.h>
#include <logging/linux/stdout/StdoutLogging.h>
#include <platform/platform_compatibility.h>
#include <parser/logging/MqttSnAdvertiseMessageLogging.h>
#include <parser/logging/MqttSnGwInfoMessageLogging.h>
#include <config/common/starter/starter_helper.h>

static volatile sig_atomic_t keep_running = 1;

static void sig_handler(int _) {
    (void)_;
    keep_running = 0;
}

int32_t find_gateway_monitor_log_received_advertise(const MqttSnLogger *logger, const int32_t timeout_left,
                                                    const MqttSnReceivedAdvertise *received_advertise) {
    log_msg_start(logger);
    log_str(logger, PSTR("timeout left: "));
    log_int32(logger, timeout_left);
    log_str(logger, PSTR(" advertise from "));
    log_device_address(logger, &received_advertise->from);
    log_str(logger, PSTR(" "));
    log_advertise_message(logger, received_advertise->advertise.gwId, received_advertise->advertise.duration);
    log_str(logger, PSTR("\n"));
    return log_status(logger);
}
int32_t find_gateway_monitor_log_received_received_gwinfo(const MqttSnLogger *logger, const int32_t timeout_left,
                                                          const MqttSnReceivedGwInfo *received_gwinfo) {
    log_msg_start(logger);
    log_str(logger, PSTR("timeout left: "));
    log_int32(logger, timeout_left);
    log_str(logger, PSTR(" gwinfo from "));
    log_device_address(logger, &received_gwinfo->from);
    log_str(logger, PSTR(" "));
    log_gwinfo_message(logger, received_gwinfo->gw_info.gwId, &received_gwinfo->gw_info.gwAdd, received_gwinfo->gw_info.gwAddLen);
    log_flush(logger);
    return log_status(logger);
}

int32_t find_gateway_monitor_adv_cb(const MqttSnFindGatewayMonitor *client, const int32_t timeout_left,
                                    const MqttSnReceivedAdvertise *received_advertise, void *context) {
    int32_t log_rc = find_gateway_monitor_log_received_advertise(client->logger, timeout_left, received_advertise);
    if (log_rc < 0) {
        return -1;
    }
    return 0;
}
int32_t find_gateway_monitor_gwinfo_cb(const MqttSnFindGatewayMonitor *client, const int32_t timeout_left,
                                       const MqttSnReceivedGwInfo *received_gwinfo, void *context) {
    int32_t log_rc = find_gateway_monitor_log_received_received_gwinfo(client->logger, timeout_left, received_gwinfo);
    if (log_rc < 0) {
        return -1;
    }
    return 0;
}
void *publish_client_thread_function(void *ptr) {
    MqttSnFindGatewayMonitor *          client = ((FindGatewayMonitor_cfg_ptr *)ptr)->c_ptr;
    const find_gateway_monitor_config *cfg    = ((FindGatewayMonitor_cfg_ptr *)ptr)->cfg_ptr;
    if (cfg->cfmqsgcfg.find_pattern_type == ADVERTISMENT_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE) {
        MqttSnFindGatewayAwaitAdvertise(client, cfg->cfmqsgcfg.advertise_wait_timeout_s, NULL, find_gateway_monitor_adv_cb);
    } else if (cfg->cfmqsgcfg.find_pattern_type == ANY_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE ||
               cfg->cfmqsgcfg.find_pattern_type == SEARCH_GW_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE) {
        MqttSnFindGatewaySearchGw(client, cfg->cfmqsgcfg.search_gateway_wait_timeout_ms, cfg->cfmqsgcfg.search_gateway_radius, NULL,
                                  find_gateway_monitor_gwinfo_cb, find_gateway_monitor_adv_cb);
    } else if (cfg->cfmqsgcfg.find_pattern_type == CONNECT_CREDENTIALS_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE) {
        log_str(client->logger, PSTR("Cannot process [-gfp --gateway_find_pattern] connect\n."));
    } else {
        log_str(client->logger, PSTR("No valid [-gfp --gateway_find_pattern]\n."));
    }
    return (void *)EXIT_SUCCESS;
}

int32_t start_find_gateway_monitor(const find_gateway_monitor_config *cfg, MqttSnFindGatewayMonitor *client, const MqttSnLogger *logger,
                                   MqttSnGatewayNetworkInterface *gatewayNetwork, void *gatewayNetworkContext) {
    if (gatewayNetworkContext == NULL) {
#ifdef WITH_LINUX_PLUGIN_NETWORK
        if (cfg->gncfg.gateway_network_plugin_path != NULL) {
            return start_find_gateway_client_plugin(cfg, client, logger, gatewayNetwork, gatewayNetworkContext);
        }
#endif
#ifdef WITH_LINUX_UDP_GATEWAY_NETWORK
        if (!strcmp(cfg->gncfg.gateway_network_protocol, "udp")) {
            return start_find_gateway_client_udp(cfg, client, logger, gatewayNetwork, gatewayNetworkContext);
        }
#endif
#ifdef WITH_LINUX_TCP_GATEWAY_NETWORK
        if (!strcmp(cfg->gncfg.gateway_network_protocol, "tcp")) {
            return start_find_gateway_client_tcp(cfg, client, logger, gatewayNetwork, gatewayNetworkContext);
        }
#endif
        log_str(logger, "Error init gateway network unknown protocol: ");
        log_str(logger, cfg->gncfg.gateway_network_protocol);
        log_str(logger, "\n");
        return EXIT_FAILURE;
    }

    MqttSnLogger client_logger = { 0 };
    if (MqttSnLoggerInit(&client_logger, cfg->mslcfg.log_lvl, stdout_log_init) != 0) {
        MqttSnLoggerDeinit(&client_logger);
        return EXIT_FAILURE;
    }

#ifdef WITH_LOGGING
    print_program_started(&client_logger, &cfg->msvcfg, cfg->executable_name);
#endif
    gatewayNetwork->logger = &client_logger;
    if (GatewayNetworkConnect(gatewayNetwork, gatewayNetworkContext) < 0) {
        GatewayNetworkDeinitialize(gatewayNetwork, gatewayNetworkContext);
        return EXIT_FAILURE;
    }

    if (MqttSnFindGatewayClientInit(client, &client_logger, gatewayNetwork, gatewayNetworkContext, cfg->gncfg.gateway_network_send_timeout,
                                    cfg->gncfg.gateway_network_receive_timeout) < 0) {
        MqttSnFindGatewayClientDeinit(client);
        return EXIT_FAILURE;
    }

    signal(SIGINT, sig_handler);
    pthread_t                  thread;
    FindGatewayMonitor_cfg_ptr ptr = { .c_ptr = client, .cfg_ptr = cfg };
    if (pthread_create(&thread, NULL, publish_client_thread_function, &ptr)) {
        log_str(logger, "Error creating thread\n");
        return EXIT_FAILURE;
    }

    int rc;
    if (pthread_join(thread, (void *)&rc)) {
        log_str(logger, "Error joining thread\n");
        return EXIT_FAILURE;
    }

#ifdef WITH_LOGGING
    print_program_terminated(&client_logger, &cfg->msvcfg, cfg->executable_name);
#endif
    MqttSnFindGatewayClientDeinit(client);

    GatewayNetworkDisconnect(gatewayNetwork, gatewayNetworkContext);
    MqttSnLoggerDeinit(&client_logger);

    return rc;
}
#ifdef WITH_LINUX_PLUGIN_NETWORK
int32_t start_find_gateway_client_plugin(const find_gateway_monitor_config *cfg, MqttSnFindGatewayMonitor *publish_client,
                                         const MqttSnLogger *logger, MqttSnGatewayNetworkInterface *gatewayNetwork,
                                         void *gatewayNetworkContext) {
    device_address mqttSnGatewayNetworkAddress             = { 0 };
    device_address forwarderGatewayNetworkAddress          = { 0 };
    device_address forwarderGatewayNetworkBroadcastAddress = { 0 };

    /*
  if (convert_hostname_port_to_device_address(cfg->msgcfg.mqtt_sn_gateway_host,
                                              cfg->msgcfg.mqtt_sn_gateway_port,
                                              &mqttSnGatewayNetworkAddress,
                                              "mqtt-sn gateway")) {
    return EXIT_FAILURE;
  }
  */
    if (convert_hostname_port_to_device_address(cfg->gncfg.gateway_network_bind_address, cfg->gncfg.gateway_network_bind_port,
                                                &forwarderGatewayNetworkAddress, "gateway")) {
        return EXIT_FAILURE;
    }
    if (convert_string_ip_port_to_device_address(logger, cfg->gncfg.gateway_network_broadcast_address,
                                                 cfg->gncfg.gateway_network_broadcast_bind_port, &forwarderGatewayNetworkBroadcastAddress,
                                                 "gateway broadcast")) {
        return EXIT_FAILURE;
    }

    const gateway_plugin_device_address pluginMqttSnGatewayNetworkAddress = { .bytes  = mqttSnGatewayNetworkAddress.bytes,
                                                                              .length = sizeof(device_address) };

    const gateway_plugin_device_address pluginForwarderGatewayNetworkAddress = { .bytes  = forwarderGatewayNetworkAddress.bytes,
                                                                                 .length = sizeof(device_address) };

    const gateway_plugin_device_address pluginForwarderGatewayNetworkBroadcastAddress = { .bytes =
                                                                                            forwarderGatewayNetworkBroadcastAddress.bytes,
                                                                                          .length = sizeof(device_address) };

    gateway_plugin_config plugin_cfg = { .plugin_path                                 = cfg->gncfg.gateway_network_plugin_path,
                                         .protocol                                    = cfg->gncfg.gateway_network_protocol,
                                         .mqtt_sn_gateway_network_address             = &pluginMqttSnGatewayNetworkAddress,
                                         .forwarder_gateway_network_address           = &pluginForwarderGatewayNetworkAddress,
                                         .forwarder_gateway_network_broadcast_address = &pluginForwarderGatewayNetworkBroadcastAddress,
                                         .gateway_plugin_device_address_length        = MQTT_SN_DEVICE_ADDRESS_LENGTH,
                                         .forwarder_maximum_message_length            = MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH };

    MqttSnGatewayPluginContext gatewayPluginContext = { .dl_handle                 = NULL,
                                                        .plugin_network_initialize = NULL,
                                                        .plugin_network_disconnect = NULL,
                                                        .plugin_network_connect    = NULL,
                                                        .plugin_network_receive    = NULL,
                                                        .plugin_network_send       = NULL,
                                                        .plugin_context            = NULL,
                                                        .plugin_cfg                = &plugin_cfg };

    if (GatewayNetworkInitialize(gatewayNetwork, MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH, &mqttSnGatewayNetworkAddress,
                                 &forwarderGatewayNetworkAddress, &forwarderGatewayNetworkBroadcastAddress, &gatewayPluginContext,
                                 GatewayLinuxPluginInitialize)) {
        log_str(logger, "Error init gateway network\n");
        return EXIT_FAILURE;
    }
    gatewayNetworkContext = &gatewayPluginContext;

    return start_find_gateway_monitor(cfg, publish_client, logger, gatewayNetwork, gatewayNetworkContext);
}
#endif
#ifdef WITH_LINUX_UDP_GATEWAY_NETWORK
int32_t start_find_gateway_client_udp(const find_gateway_monitor_config *cfg, MqttSnFindGatewayMonitor *publish_client,
                                      const MqttSnLogger *logger, MqttSnGatewayNetworkInterface *gatewayNetwork,
                                      void *gatewayNetworkContext) {
    device_address          mqttSnGatewayNetworkAddress             = { 0 };
    device_address          forwarderGatewayNetworkAddress          = { 0 };
    device_address          forwarderGatewayNetworkBroadcastAddress = { 0 };
    MqttSnGatewayUdpNetwork udpGatewayNetworkContext                = { 0 };

    /*
  if (convert_hostname_port_to_device_address(cfg->msgcfg.mqtt_sn_gateway_host,
                                              cfg->msgcfg.mqtt_sn_gateway_port,
                                              &mqttSnGatewayNetworkAddress,
                                              "mqtt-sn gateway")) {
    return EXIT_FAILURE;
  }
  */
    if (convert_hostname_port_to_device_address(cfg->gncfg.gateway_network_bind_address, cfg->gncfg.gateway_network_bind_port,
                                                &forwarderGatewayNetworkAddress, "gateway unicast")) {
        return EXIT_FAILURE;
    }
    if (convert_string_ip_port_to_device_address(logger, cfg->gncfg.gateway_network_broadcast_address,
                                                 cfg->gncfg.gateway_network_broadcast_bind_port, &forwarderGatewayNetworkBroadcastAddress,
                                                 "gateway broadcast")) {
        return EXIT_FAILURE;
    }

    if (GatewayNetworkInitialize(gatewayNetwork, MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH, &mqttSnGatewayNetworkAddress,
                                 &forwarderGatewayNetworkAddress, &forwarderGatewayNetworkBroadcastAddress, &udpGatewayNetworkContext,
                                 GatewayLinuxUdpInitialize)) {
        log_str(logger, "Error init gateway network\n");
        return EXIT_FAILURE;
    }
    gatewayNetworkContext = &udpGatewayNetworkContext;

    return start_find_gateway_monitor(cfg, publish_client, logger, gatewayNetwork, gatewayNetworkContext);
}
#endif
#ifdef WITH_LINUX_TCP_GATEWAY_NETWORK
int32_t start_find_gateway_client_tcp(const find_gateway_monitor_config *cfg, MqttSnFindGatewayMonitor *publish_client,
                                      const MqttSnLogger *logger, MqttSnGatewayNetworkInterface *gatewayNetwork,
                                      void *gatewayNetworkContext) {
    device_address          mqttSnGatewayNetworkAddress             = { 0 };
    device_address          forwarderGatewayNetworkAddress          = { 0 };
    device_address          forwarderGatewayNetworkBroadcastAddress = { 0 };
    MqttSnGatewayTcpNetwork tcpGatewayNetworkContext                = { 0 };

    /*
  if (convert_hostname_port_to_device_address(cfg->msgcfg.mqtt_sn_gateway_host,
                                              cfg->msgcfg.mqtt_sn_gateway_port,
                                              &mqttSnGatewayNetworkAddress,
                                              "mqtt-sn gateway")) {
    return EXIT_FAILURE;
  }
  */
    if (convert_hostname_port_to_device_address(cfg->gncfg.gateway_network_bind_address, cfg->gncfg.gateway_network_bind_port,
                                                &forwarderGatewayNetworkAddress, "gateway")) {
        return EXIT_FAILURE;
    }
    if (convert_string_ip_port_to_device_address(logger, cfg->gncfg.gateway_network_broadcast_address,
                                                 cfg->gncfg.gateway_network_broadcast_bind_port, &forwarderGatewayNetworkBroadcastAddress,
                                                 "gateway broadcast")) {
        return EXIT_FAILURE;
    }

    if (GatewayNetworkInitialize(gatewayNetwork, MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH, &mqttSnGatewayNetworkAddress,
                                 &forwarderGatewayNetworkAddress, &forwarderGatewayNetworkBroadcastAddress, &tcpGatewayNetworkContext,
                                 GatewayLinuxTcpInitialize)) {
        log_str(logger, "Error init client network\n");
        return EXIT_FAILURE;
    }
    gatewayNetworkContext = &tcpGatewayNetworkContext;

    return start_find_gateway_monitor(cfg, publish_client, logger, gatewayNetwork, gatewayNetworkContext);
}
#endif
