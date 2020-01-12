//
// Created by SomeDude on 07.04.2019.
//

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
#include <config/common/starter/starter_helper.h>
#include <network/linux/gateway/ip/udp/MqttSnLinuxGatewayUdpNetworkContainer.h>
#include <network/linux/client/ip/udp/MqttSnLinuxClientUdpNetworkContainer.h>
#include <network/linux/gateway/ip/tcp/MqttSnLinuxGatewayTcpNetworkContainer.h>
#include <network/linux/client/ip/tcp/MqttSnLinuxClientTcpNetworkContainer.h>
/*
#ifdef WITH_LINUX_PLUGIN_NETWORK
int start_gateway_plugin(const forwarder_config *cfg, const MqttSnLogger *logger, MqttSnForwarder *mqttSnForwarder,
                         void *gatewayNetworkContext, void *clientNetworkContext) {
    device_address mqttSnGatewayNetworkAddress             = { 0 };
    device_address forwarderGatewayNetworkAddress          = { 0 };
    device_address forwarderGatewayNetworkBroadcastAddress = { 0 };

    if (convert_hostname_port_to_device_address(cfg->msgcfg.mqtt_sn_gateway_host, cfg->msgcfg.mqtt_sn_gateway_port,
                                                &mqttSnGatewayNetworkAddress, "mqtt-sn gateway")) {
        return EXIT_FAILURE;
    }
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

    if (GatewayNetworkInitialize(&mqttSnForwarder->gatewayNetwork, MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH, &mqttSnGatewayNetworkAddress,
                                 &forwarderGatewayNetworkAddress, &forwarderGatewayNetworkBroadcastAddress, &gatewayPluginContext,
                                 GatewayLinuxPluginInitialize)) {
        log_str(logger, "Error init gateway network\n");
        return EXIT_FAILURE;
    }
    gatewayNetworkContext = &gatewayPluginContext;

    return start_forwarder(cfg, logger, mqttSnForwarder, gatewayNetworkContext, clientNetworkContext);
}
#endif

#ifdef WITH_LINUX_TCP_GATEWAY_NETWORK
int start_gateway_tcp(const forwarder_config *cfg, const MqttSnLogger *logger, MqttSnForwarder *mqttSnForwarder,
                      void *gatewayNetworkContext, void *clientNetworkContext) {
    device_address          mqttSnGatewayNetworkAddress             = { 0 };
    device_address          forwarderGatewayNetworkAddress          = { 0 };
    device_address          forwarderGatewayNetworkBroadcastAddress = { 0 };
    MqttSnGatewayTcpNetwork tcpGatewayNetworkContext                = { 0 };

    if (convert_hostname_port_to_device_address(cfg->msgcfg.mqtt_sn_gateway_host, cfg->msgcfg.mqtt_sn_gateway_port,
                                                &mqttSnGatewayNetworkAddress, "mqtt-sn gateway")) {
        return EXIT_FAILURE;
    }
    if (convert_hostname_port_to_device_address(cfg->gncfg.gateway_network_bind_address, cfg->gncfg.gateway_network_bind_port,
                                                &forwarderGatewayNetworkAddress, "gateway")) {
        return EXIT_FAILURE;
    }
    if (convert_string_ip_port_to_device_address(logger, cfg->gncfg.gateway_network_broadcast_address,
                                                 cfg->gncfg.gateway_network_broadcast_bind_port, &forwarderGatewayNetworkBroadcastAddress,
                                                 "gateway broadcast")) {
        return EXIT_FAILURE;
    }

    if (GatewayNetworkInitialize(&mqttSnForwarder->gatewayNetwork, MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH, &mqttSnGatewayNetworkAddress,
                                 &forwarderGatewayNetworkAddress, &forwarderGatewayNetworkBroadcastAddress, &tcpGatewayNetworkContext,
                                 GatewayLinuxTcpInitialize)) {
        log_str(logger, "Error init client network\n");
        return EXIT_FAILURE;
    }
    gatewayNetworkContext = &tcpGatewayNetworkContext;

    return start_forwarder(cfg, logger, mqttSnForwarder, gatewayNetworkContext, clientNetworkContext);
}
#endif

#ifdef WITH_LINUX_UDP_GATEWAY_NETWORK
int start_gateway_udp(const forwarder_config *cfg, const MqttSnLogger *logger, MqttSnForwarder *mqttSnForwarder,
                      void *gatewayNetworkContext, void *clientNetworkContext) {
    device_address          mqttSnGatewayNetworkAddress             = { 0 };
    device_address          forwarderGatewayNetworkAddress          = { 0 };
    device_address          forwarderGatewayNetworkBroadcastAddress = { 0 };
    MqttSnGatewayUdpNetwork udpGatewayNetworkContext                = { 0 };

    if (convert_hostname_port_to_device_address(cfg->msgcfg.mqtt_sn_gateway_host, cfg->msgcfg.mqtt_sn_gateway_port,
                                                &mqttSnGatewayNetworkAddress, "mqtt-sn gateway")) {
        return EXIT_FAILURE;
    }
    if (convert_hostname_port_to_device_address(cfg->gncfg.gateway_network_bind_address, cfg->gncfg.gateway_network_bind_port,
                                                &forwarderGatewayNetworkAddress, "gateway unicast")) {
        return EXIT_FAILURE;
    }
    if (convert_string_ip_port_to_device_address(logger, cfg->gncfg.gateway_network_broadcast_address,
                                                 cfg->gncfg.gateway_network_broadcast_bind_port, &forwarderGatewayNetworkBroadcastAddress,
                                                 "gateway broadcast")) {
        return EXIT_FAILURE;
    }

    if (GatewayNetworkInitialize(&mqttSnForwarder->gatewayNetwork, MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH, &mqttSnGatewayNetworkAddress,
                                 &forwarderGatewayNetworkAddress, &forwarderGatewayNetworkBroadcastAddress, &udpGatewayNetworkContext,
                                 GatewayLinuxUdpInitialize)) {
        log_str(logger, "Error init gateway network\n");
        return EXIT_FAILURE;
    }
    gatewayNetworkContext = &udpGatewayNetworkContext;

    return start_forwarder(cfg, logger, mqttSnForwarder, gatewayNetworkContext, clientNetworkContext);
}
#endif

#ifdef WITH_LINUX_PLUGIN_NETWORK
int start_client_plugin(const forwarder_config *fcfg, const MqttSnLogger *logger, MqttSnForwarder *mqttSnForwarder,
                        void *gatewayNetworkContext, void *clientNetworkContext) {
    device_address mqttSnGatewayNetworkAddress            = { 0 };
    device_address forwarderClientNetworkAddress          = { 0 };
    device_address forwarderClientNetworkBroadcastAddress = { 0 };

    if (convert_hostname_port_to_device_address(fcfg->msgcfg.mqtt_sn_gateway_host, fcfg->msgcfg.mqtt_sn_gateway_port,
                                                &mqttSnGatewayNetworkAddress, "mqtt-sn gateway")) {
        return EXIT_FAILURE;
    }

    if (convert_hostname_port_to_device_address(fcfg->cncfg.client_network_bind_address, fcfg->cncfg.client_network_bind_port,
                                                &forwarderClientNetworkAddress, "client")) {
        return EXIT_FAILURE;
    }
    if (convert_string_ip_port_to_device_address(logger, fcfg->cncfg.client_network_broadcast_address,
                                                 fcfg->cncfg.client_network_broadcast_bind_port, &forwarderClientNetworkBroadcastAddress,
                                                 "client broadcast")) {
        return EXIT_FAILURE;
    }

    const client_plugin_device_address pluginMqttSnForwarderNetworkAddress = { .bytes  = forwarderClientNetworkAddress.bytes,
                                                                               .length = sizeof(device_address) };

    client_plugin_config plugin_cfg = { .plugin_path                         = fcfg->cncfg.client_network_plugin_path,
                                        .protocol                            = fcfg->cncfg.client_network_protocol,
                                        .forwarder_client_network_address    = &pluginMqttSnForwarderNetworkAddress,
                                        .client_plugin_device_address_length = MQTT_SN_DEVICE_ADDRESS_LENGTH,
                                        .forwarder_maximum_message_length    = MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH };

    MqttSnClientPluginContext clientPluginContext = { .dl_handle                 = NULL,
                                                      .plugin_network_initialize = NULL,
                                                      .plugin_network_disconnect = NULL,
                                                      .plugin_network_connect    = NULL,
                                                      .plugin_network_receive    = NULL,
                                                      .plugin_network_send       = NULL,
                                                      .plugin_context            = NULL,
                                                      .plugin_cfg                = &plugin_cfg };

    if (ClientNetworkInitialize(&mqttSnForwarder->clientNetwork, MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH, &mqttSnGatewayNetworkAddress,
                                &forwarderClientNetworkAddress, &forwarderClientNetworkBroadcastAddress, &clientPluginContext,
                                ClientLinuxPluginInitialize)) {
        log_str(logger, "Error init client network\n");
        return EXIT_FAILURE;
    }
    clientNetworkContext = &clientPluginContext;

    return start_forwarder(fcfg, logger, mqttSnForwarder, gatewayNetworkContext, clientNetworkContext);
}
#endif

#ifdef WITH_LINUX_TCP_CLIENT_NETWORK
int start_client_tcp(const forwarder_config *fcfg, const MqttSnLogger *logger, MqttSnForwarder *mqttSnForwarder,
                     void *gatewayNetworkContext, void *clientNetworkContext) {
    device_address         mqttSnGatewayNetworkAddress            = { 0 };
    device_address         forwarderClientNetworkAddress          = { 0 };
    device_address         forwarderClientNetworkBroadcastAddress = { 0 };
    MqttSnClientTcpNetwork tcpClientNetworkContext                = { 0 };

    if (convert_hostname_port_to_device_address(fcfg->msgcfg.mqtt_sn_gateway_host, fcfg->msgcfg.mqtt_sn_gateway_port,
                                                &mqttSnGatewayNetworkAddress, "mqtt-sn gateway")) {
        return EXIT_FAILURE;
    }

    if (convert_hostname_port_to_device_address(fcfg->cncfg.client_network_bind_address, fcfg->cncfg.client_network_bind_port,
                                                &forwarderClientNetworkAddress, "client")) {
        return EXIT_FAILURE;
    }
    if (convert_string_ip_port_to_device_address(logger, fcfg->cncfg.client_network_broadcast_address,
                                                 fcfg->cncfg.client_network_broadcast_bind_port, &forwarderClientNetworkBroadcastAddress,
                                                 "client broadcast")) {
        return EXIT_FAILURE;
    }

    if (ClientNetworkInitialize(&mqttSnForwarder->clientNetwork, MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH, &mqttSnGatewayNetworkAddress,
                                &forwarderClientNetworkAddress, &forwarderClientNetworkBroadcastAddress, &tcpClientNetworkContext,
                                ClientLinuxTcpInitialize)) {
        log_str(logger, "Error init client network\n");
        return EXIT_FAILURE;
    }
    clientNetworkContext = &tcpClientNetworkContext;

    return start_forwarder(fcfg, logger, mqttSnForwarder, gatewayNetworkContext, clientNetworkContext);
}
#endif

#ifdef WITH_LINUX_UDP_CLIENT_NETWORK
int start_client_udp(const forwarder_config *fcfg, const MqttSnLogger *logger, MqttSnForwarder *mqttSnForwarder,
                     void *gatewayNetworkContext, void *clientNetworkContext) {
    device_address         forwarderClientNetworkAddress          = { 0 };
    device_address         mqttSnGatewayNetworkAddress            = { 0 };
    device_address         forwarderClientNetworkBroadcastAddress = { 0 };
    MqttSnClientUdpNetwork udpClientNetworkContext                = { 0 };

    if (convert_hostname_port_to_device_address(fcfg->msgcfg.mqtt_sn_gateway_host, fcfg->msgcfg.mqtt_sn_gateway_port,
                                                &mqttSnGatewayNetworkAddress, "mqtt-sn gateway")) {
        return EXIT_FAILURE;
    }

    if (convert_hostname_port_to_device_address(fcfg->cncfg.client_network_bind_address, fcfg->cncfg.client_network_bind_port,
                                                &forwarderClientNetworkAddress, "client unicast")) {
        return EXIT_FAILURE;
    }
    if (convert_string_ip_port_to_device_address(logger, fcfg->cncfg.client_network_broadcast_address,
                                                 fcfg->cncfg.client_network_broadcast_bind_port, &forwarderClientNetworkBroadcastAddress,
                                                 "client broadcast")) {
        return EXIT_FAILURE;
    }

    if (ClientNetworkInitialize(&mqttSnForwarder->clientNetwork, MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH, &mqttSnGatewayNetworkAddress,
                                &forwarderClientNetworkAddress, &forwarderClientNetworkBroadcastAddress, &udpClientNetworkContext,
                                ClientLinuxUdpInitialize)) {
        log_str(logger, "Error init client network\n");
        return EXIT_FAILURE;
    }
    clientNetworkContext = &udpClientNetworkContext;

    return start_forwarder(fcfg, logger, mqttSnForwarder, gatewayNetworkContext, clientNetworkContext);
}
#endif


*/

static volatile sig_atomic_t keep_running = 1;

static void sig_handler(int _) {
    (void)_;
    keep_running = 0;
}

void *forwarder_inc_c(void *mqttSnForwarderFcfgPtr_ptr) {
    MqttSnForwarder_fcfg_ptr *mqttSnForwarderFcfgPtr = (MqttSnForwarder_fcfg_ptr *)mqttSnForwarderFcfgPtr_ptr;
    MqttSnForwarder *         mqttSnForwarder        = mqttSnForwarderFcfgPtr->mqttSnForwarder_ptr;

    while ((MqttSnForwarderLoop(mqttSnForwarder) == 0) & keep_running) {
    }

#ifdef WITH_LOGGING
    const forwarder_config *fcfg = mqttSnForwarderFcfgPtr->fcfg_ptr;
    print_program_terminated(&mqttSnForwarder->logger, &fcfg->msvcfg, fcfg->executable_name);
#endif

    MqttSnForwarderDeinit(mqttSnForwarder);
    return (void *)EXIT_SUCCESS;
}

int start_forwarder(const forwarder_config *fcfg, const MqttSnLogger *logger, MqttSnForwarder *mqttSnForwarder) {
#if defined(WITH_LINUX_GATEWAY_PLUGIN_NETWORK) || defined(WITH_LINUX_PLUGIN_NETWORK)
    //TODO
#endif
#ifdef WITH_LINUX_TCP_GATEWAY_NETWORK
    MqttSnLinuxGatewayTcpNetworkContainer tcpGatewayNetworkContainer;
    if (!strcmp(fcfg->gncfg.gateway_network_protocol, "tcp")) {
        if (MqttSnLinuxGatewayTcpNetworkContainerStart(logger, fcfg->msgcfg.mqtt_sn_gateway_host, fcfg->msgcfg.mqtt_sn_gateway_port,
                                                       fcfg->gncfg.gateway_network_bind_address, fcfg->gncfg.gateway_network_bind_port,
                                                       fcfg->gncfg.gateway_network_broadcast_address,
                                                       fcfg->gncfg.gateway_network_broadcast_bind_port, &mqttSnForwarder->gatewayNetwork,
                                                       &tcpGatewayNetworkContainer, &mqttSnForwarder->mqtt_sn_gateway_address)) {
            log_str(logger, "Error init udp gateway network\n");
            return EXIT_FAILURE;
        }
    }
#endif
#ifdef WITH_LINUX_UDP_GATEWAY_NETWORK
    MqttSnLinuxGatewayUdpNetworkContainer udpGatewayNetworkContainer;
    if (!strcmp(fcfg->gncfg.gateway_network_protocol, "udp")) {
        if (MqttSnLinuxGatewayUdpNetworkContainerStart(logger, fcfg->msgcfg.mqtt_sn_gateway_host, fcfg->msgcfg.mqtt_sn_gateway_port,
                                                       fcfg->gncfg.gateway_network_bind_address, fcfg->gncfg.gateway_network_bind_port,
                                                       fcfg->gncfg.gateway_network_broadcast_address,
                                                       fcfg->gncfg.gateway_network_broadcast_bind_port, &mqttSnForwarder->gatewayNetwork,
                                                       &udpGatewayNetworkContainer, &mqttSnForwarder->mqtt_sn_gateway_address)) {
            log_str(logger, "Error init udp gateway network\n");
            return EXIT_FAILURE;
        }
    }
#endif

#ifdef WITH_LINUX_TCP_CLIENT_NETWORK
    MqttSnLinuxClientTcpNetworkContainer tcpClientNetworkContainer;
    device_address                       tcp_client_network_mqtt_sn_gateway_address;
    if (!strcmp(fcfg->cncfg.client_network_protocol, "tcp")) {
        if (MqttSnLinuxClientTcpNetworkContainerStart(logger, fcfg->msgcfg.mqtt_sn_gateway_host, fcfg->msgcfg.mqtt_sn_gateway_port,
                                                      fcfg->cncfg.client_network_bind_address, fcfg->cncfg.client_network_bind_port,
                                                      fcfg->cncfg.client_network_broadcast_address,
                                                      fcfg->cncfg.client_network_broadcast_bind_port, &mqttSnForwarder->clientNetwork,
                                                      &tcpClientNetworkContainer, &tcp_client_network_mqtt_sn_gateway_address)) {
            log_str(logger, "Error init udp client network\n");
            return EXIT_FAILURE;
        }
    }
#endif
#ifdef WITH_LINUX_UDP_CLIENT_NETWORK
    MqttSnLinuxClientUdpNetworkContainer udpClientNetworkContainer;
    device_address                       udp_client_network_mqtt_sn_gateway_address;
    if (!strcmp(fcfg->cncfg.client_network_protocol, "udp")) {
        if (MqttSnLinuxClientUdpNetworkContainerStart(logger, fcfg->msgcfg.mqtt_sn_gateway_host, fcfg->msgcfg.mqtt_sn_gateway_port,
                                                      fcfg->cncfg.client_network_bind_address, fcfg->cncfg.client_network_bind_port,
                                                      fcfg->cncfg.client_network_broadcast_address,
                                                      fcfg->cncfg.client_network_broadcast_bind_port, &mqttSnForwarder->clientNetwork,
                                                      &udpClientNetworkContainer, &udp_client_network_mqtt_sn_gateway_address)) {
            log_str(logger, "Error init udp client network\n");
            return EXIT_FAILURE;
        }
    }
#endif

#if defined(WITH_LINUX_CLIENT_PLUGIN_NETWORK) || defined(WITH_LINUX_PLUGIN_NETWORK)
    //TODO
#endif

#ifdef WITH_LOGGING
    print_program_started(logger, &fcfg->msvcfg, fcfg->executable_name);
#endif

#ifdef WITH_MQTT_SN_CLIENT_NETWORK_WHITELIST_CONFIG
    if (fcfg->cncfg.addr_whitelist_len) {
        for (int i = 0; i < fcfg->cncfg.addr_whitelist_len; ++i) {
            MqttSnForwarderAddClientNetworkWhitelistAddr(mqttSnForwarder, &fcfg->cncfg.addr_whitelist[i]);
        }
    }
#endif
#ifdef WITH_MQTT_SN_CLIENT_NETWORK_BLACKLIST_CONFIG
    if (fcfg->cncfg.addr_blacklist_len) {
        for (int i = 0; i < fcfg->cncfg.addr_blacklist_len; ++i) {
            MqttSnForwarderAddClientNetworkBlacklistAddr(mqttSnForwarder, &fcfg->cncfg.addr_blacklist[i]);
        }
    }
#endif
#ifdef WITH_MQTT_SN_GATEWAY_NETWORK_WHITELIST_CONFIG
    if (fcfg->gncfg.gw_id_whitelist_len) {
        for (int i = 0; i < fcfg->gncfg.gw_id_whitelist_len; ++i) {
            MqttSnForwarderAddGatewayNetworkWhitelistGwId(mqttSnForwarder, fcfg->gncfg.gw_id_whitelist[i]);
        }
    }
    if (fcfg->gncfg.gw_addr_whitelist_len) {
        for (int i = 0; i < fcfg->gncfg.gw_addr_whitelist_len; ++i) {
            MqttSnForwarderAddGatewayNetworkWhitelistAddr(mqttSnForwarder, &fcfg->gncfg.gw_addr_whitelist[i]);
        }
    }
#endif
#ifdef WITH_MQTT_SN_GATEWAY_NETWORK_BLACKLIST_CONFIG
    if (fcfg->gncfg.gw_id_blacklist_len) {
        for (int i = 0; i < fcfg->gncfg.gw_id_blacklist_len; ++i) {
            MqttSnForwarderAddGatewayNetworkBlacklistGwId(mqttSnForwarder, fcfg->gncfg.gw_id_blacklist[i]);
        }
    }

    if (fcfg->gncfg.gw_addr_blacklist_len) {
        for (int i = 0; i < fcfg->gncfg.gw_addr_blacklist_len; ++i) {
            MqttSnForwarderAddGatewayNetworkBlacklistAddr(mqttSnForwarder, &fcfg->gncfg.gw_addr_blacklist[i]);
        }
    }
#endif

    mqttSnForwarder->clientNetworkSendTimeout     = fcfg->cncfg.client_network_send_timeout;
    mqttSnForwarder->clientNetworkReceiveTimeout  = fcfg->cncfg.client_network_receive_timeout;
    mqttSnForwarder->gatewayNetworkSendTimeout    = fcfg->gncfg.gateway_network_send_timeout;
    mqttSnForwarder->gatewayNetworkReceiveTimeout = fcfg->gncfg.gateway_network_receive_timeout;

    MqttSnForwarderSetPinConnection(mqttSnForwarder, fcfg->pin_connection);
    if (fcfg->pin_connection && fcfg->connection_state == DISCONNECTED_CONFIG_MQTT_SN_FORWARDER_START_CONNECTION_STATE) {
        log_str(logger, "Error: mqtt-sn gateway cannot start in disconnected state with pinned gateway connection.\n");
    }

    if (fcfg->connection_state == DISCONNECTED_CONFIG_MQTT_SN_FORWARDER_START_CONNECTION_STATE) {
#if defined(WITH_ADVERTISE_GATEWAY_DISCOVERED_LIST) || defined(WITH_GWINFO_GATEWAY_DISCOVERED_LIST)
        if (MqttSnForwarderInit(mqttSnForwarder, logger, NULL) != 0) {
            MqttSnForwarderDeinit(mqttSnForwarder);
            return EXIT_FAILURE;
        }
#else
        log_str(logger, "Error: MQTT-SN Gateway cannot start in disconnected state without discovered gateway list.\n");
#endif
    } else {
        if (MqttSnForwarderInit(mqttSnForwarder, logger, &mqttSnForwarder->mqtt_sn_gateway_address) != 0) {
            MqttSnForwarderDeinit(mqttSnForwarder);
            return EXIT_FAILURE;
        }
    }

    signal(SIGINT, sig_handler);
    pthread_t                mqttSnForwarder_thread;
    MqttSnForwarder_fcfg_ptr mqttSnForwarderFcfgPtr = { .mqttSnForwarder_ptr = mqttSnForwarder, .fcfg_ptr = fcfg };
    if (pthread_create(&mqttSnForwarder_thread, NULL, forwarder_inc_c, &mqttSnForwarderFcfgPtr)) {
        log_str(logger, "Error creating thread\n");
        return EXIT_FAILURE;
    }

    int rc = EXIT_SUCCESS;

    if (pthread_join(mqttSnForwarder_thread, (void *)&rc)) {
        log_str(logger, "Error joining thread\n");
        return EXIT_FAILURE;
    }

    GatewayNetworkDeinitialize(&mqttSnForwarder->gatewayNetwork, mqttSnForwarder->gatewayNetwork.context);
    ClientNetworkDeinitialize(&mqttSnForwarder->clientNetwork, mqttSnForwarder->clientNetwork.context);

    return rc;
}
