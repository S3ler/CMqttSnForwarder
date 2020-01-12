//
// Created by SomeDude on 02.08.2019.
//

#include "mqtt_sn_client_interactive.h"
#include "mqtt_sn_client_interactive_cmd_result.h"
#include <string.h>
#include <platform/platform_compatibility.h>
#include <config/common/config_command_helper.h>
#include <network/shared/ip/IpHelper.h>
#include <client/MqttSnClientLogger.h>
#include <logging/linux/file/FileLogging.h>
#include <logging/linux/stdout/StdoutLogging.h>
#include <logging/linux/filestdout/FileStdoutLogging.h>
#include <parser/logging/MqttSnPublishMessageLogging.h>
#if defined(WITH_LINUX_UDP_GATEWAY_NETWORK) || defined(WITH_LINUX_TCP_GATEWAY_NETWORK)
#include <network/linux/shared/ip/MqttSnIpNetworkHelper.h>
#endif
single_client_publish_config *interactive_client_publish_callback_context = NULL;
MQTT_SN_CLIENT_RETURN_CODE    interactive_client_publish_callback_rc;

int32_t interactive_client_publish_callback(const MqttSnClient *client, const MqttSnPublish *publish);

int32_t mqtt_sn_client_interactive_start_logger(mqtt_sn_interactive_client *client, const mqtt_sn_logger_config *cfg,
                                                MqttSnLogger *logger) {
#ifdef MQTT_SN_ARDUINO
    return 0;
#else
    if (cfg->log_identifier) {
        logger->log_identifier = cfg->log_identifier;
    }

    if (!strcmp(cfg->log_target, "console")) {
        if (MqttSnLoggerInit(logger, cfg->log_lvl, stdout_log_init) < 0) {
            return -1;
        }
    } else if (!strcmp(cfg->log_target, "file")) {
        if (MqttSnLoggerInitFile(logger, cfg->log_lvl, cfg->log_filepath, file_log_init, &client->file_log_context) < 0) {
            return -1;
        }
    } else if (!strcmp(cfg->log_target, "consolefile")) {
        if (cfg->log_filepath == NULL) {
            return -1;
        }
        if (MqttSnLoggerInitFile(logger, cfg->log_lvl, cfg->log_filepath, file_stdout_log_init, &client->fileconsole_log_context) < 0) {
            return -1;
        }
    }
#endif

    return 0;
}
void mqtt_sn_interactive_client_init(mqtt_sn_interactive_client *interactive_client) {
    memset(interactive_client, 0, sizeof(*interactive_client));
    interactive_client_publish_callback_context = NULL;
    interactive_client_publish_callback_rc      = MQTT_SN_CLIENT_RETURN_ERROR;
}
void mqtt_sn_client_interactive_print_usage(const MqttSnLogger *logger) {
    log_str(logger, PSTR(MQTT_SN_INTERACTIVE_CLIENT_EXECUTABLE_NAME));
    log_str(logger, PSTR(" is a interactive mqtt-sn client for testing and measuring mqtt-sn gateways.\n"));
    mqtt_sn_version_config_print_usage_short(logger);
    mqtt_sn_client_interactive_print_usage_short(logger);
    log_flush(logger);
    mqtt_sn_client_interactive_print_usage_long(logger);
    log_flush(logger);
}
void mqtt_sn_client_interactive_print_usage_short(const MqttSnLogger *logger) {
    const char *indent = "                     ";
    log_str(logger, PSTR(MQTT_SN_INTERACTIVE_CLIENT_EXECUTABLE_NAME));
    log_str(logger, PSTR(":"));
    log_str(logger, PSTR(" [h | help] [l | logger] [i | init] [di | deinit] [c | connect] [d | disconnect] [r | register] \n"));
    log_str(logger, indent);
    log_str(logger, PSTR(" [s | subscribe] [u | unsubscribe] [sl | sleep] [w | wakeup] [rc | reconnect] [pi | ping]\n"));
}
void mqtt_sn_client_interactive_print_usage_long(const MqttSnLogger *logger) {
    log_str(logger, PSTR(" [h | help] : shows this help usage.\n"));
    log_str(logger, PSTR(" [l | logger] : reinitializes the logger.\n"));
    log_str(logger, PSTR(" [i | init] : initializes the network and client.\n"));
    log_str(logger, PSTR(" [di | deinit] : deinitializes the network and client.\n"));
    log_str(logger, PSTR(" [c | connect] : (auto)connect the client to the gateway.\n"));
    log_str(logger, PSTR(" [dc | directconnect] : direct connect the client to the gateway.\n"));
    log_str(logger, PSTR(" [sc | searchconnect] : connect the client to the gateway by search gateway.\n"));
    log_str(logger, PSTR(" [ac | advertiseconnect] : connect the client to the gateway by advertise.\n"));
    log_str(logger, PSTR(" [d | disconnect] : disconnect the client from the gateway.\n"));
    log_str(logger, PSTR(" [r | register] : register topic names.\n"));
    log_str(logger, PSTR(" [s | subscribe] : subscribe to topics.\n"));
    log_str(logger, PSTR(" [u | unsubscribe] : unsubscribe from topics.\n"));
    log_str(logger, PSTR(" [p | publish] : sends publishes.\n"));
    log_str(logger, PSTR(" [prp | publishreceivepublish] : publishes and receives the publish data.\n"));
    log_str(logger, PSTR(" [sl | sleep] : sets the client to sleep.\n"));
    log_str(logger, PSTR(" [w | wakeup] : wakes the client up.\n"));
    log_str(logger, PSTR(" [rc | reconnect] : reconnects the client up.\n"));
    log_str(logger, PSTR(" [pi | ping] : pings the gateway.\n"));
    log_str(logger, PSTR(" --help : shows mqtt-sn client configuration commands.\n"));
}
uint8_t mqtt_sn_client_interactive_is_exit(const char *str) {
    if (strncmp(str, "exit", strlen("exit")) == 0) {
        return 1;
    }
    return 0;
}
uint8_t mqtt_sn_client_interactive_is_help(const char *str) {
    if (!strcmp(str, "h") || !strcmp(str, "help")) {
        return 1;
    }
    return 0;
}
MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE mqtt_sn_client_interactive_parse_action(const char *buffer, int32_t buffer_length) {
    if (buffer_length <= 0) {
        return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_NO_COMMAND;
    }
    if (!strcmp(buffer, "l") || !strcmp(buffer, "logger")) {
        return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_REINIT_LOGGER;
    } else if (!strcmp(buffer, "i") || !strcmp(buffer, "init")) {
        return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_INIT_CLIENT;
    } else if (!strcmp(buffer, "di") || !strcmp(buffer, "deinit")) {
        return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_DEINIT_CLIENT;
    } else if (!strcmp(buffer, "c") || !strcmp(buffer, "connect")) {
        return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_CONNECT_GATEWAY;
    } else if (!strcmp(buffer, "dc") || !strcmp(buffer, "directconnect")) {
        return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_DIRECT_CONNECT_GATEWAY;
    } else if (!strcmp(buffer, "sc") || !strcmp(buffer, "searchconnect")) {
        return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_SEARCHGW_AND_CONNECT_GATEWAY;
    } else if (!strcmp(buffer, "ac") || !strcmp(buffer, "advertiseconnect")) {
        return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_ADVERTISE_AND_CONNECT_GATEWAY;
    } else if (!strcmp(buffer, "d") || !strcmp(buffer, "disconnect")) {
        return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_DISCONNECT_GATEWAY;
    } else if (!strcmp(buffer, "r") || !strcmp(buffer, "register")) {
        return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_REGISTER_GATEWAY;
    } else if (!strcmp(buffer, "s") || !strcmp(buffer, "subscribe")) {
        return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_SUBSCRIBE_GATEWAY;
    } else if (!strcmp(buffer, "u") || !strcmp(buffer, "unsubscribe")) {
        return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_UNSUBSCRIBE_GATEWAY;
    } else if (!strcmp(buffer, "p") || !strcmp(buffer, "publish")) {
        return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_GATEWAY;
    } else if (!strcmp(buffer, "prp") || !strcmp(buffer, "publishreceivepublish")) {
        return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_RECEIVE_GATEWAY;
    } else if (!strcmp(buffer, "sl") || !strcmp(buffer, "sleep")) {
        return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_SLEEP_GATEWAY;
    } else if (!strcmp(buffer, "w") || !strcmp(buffer, "wakeup")) {
        return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_WAKEUP_GATEWAY;
    } else if (!strcmp(buffer, "rc") || !strcmp(buffer, "reconnect")) {
        return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_RECONNECT_GATEWAY;
    } else if (!strcmp(buffer, "pi") || !strcmp(buffer, "ping")) {
        return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PING_GATEWAY;
    }
    return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_NO_COMMAND;
}
mqtt_sn_client_interactive_cmd_result mqtt_sn_cmd_buffer_execute_command(mqtt_sn_interactive_client *               client,
                                                                         MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE     tk,
                                                                         const interactive_client_config_container *cfg,
                                                                         MqttSnLogger *                             logger) {
    MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE action_result;

    uint64_t start_t_s = 0, start_t_ns = 0;
    uint64_t end_t_s = 0, end_t_ns = 0;

    if (tk == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_REINIT_LOGGER) {
        if (get_timestamp_ns(&start_t_s, &start_t_ns) < 0) {
            return mqtt_sn_client_interactive_cmd_result_error_only(tk);
        }
        action_result = MqttSnClientInteractiveReinitLogger(client, cfg, logger);
        if (get_timestamp_ns(&end_t_s, &end_t_ns) < 0) {
            return mqtt_sn_client_interactive_cmd_result_error_only(tk);
        }
    } else if (tk == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_INIT_CLIENT) {
        if (get_timestamp_ns(&start_t_s, &start_t_ns) < 0) {
            return mqtt_sn_client_interactive_cmd_result_error_only(tk);
        }
        action_result = MqttSnClientInteractiveInit(client, cfg, logger);
        if (get_timestamp_ns(&end_t_s, &end_t_ns) < 0) {
            return mqtt_sn_client_interactive_cmd_result_error_only(tk);
        }
    } else if (tk == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_DEINIT_CLIENT) {
        if (get_timestamp_ns(&start_t_s, &start_t_ns) < 0) {
            return mqtt_sn_client_interactive_cmd_result_error_only(tk);
        }
        action_result = MqttSnClientInteractiveDeinit(client);
        if (get_timestamp_ns(&end_t_s, &end_t_ns) < 0) {
            return mqtt_sn_client_interactive_cmd_result_error_only(tk);
        }
    } else if (tk == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_CONNECT_GATEWAY ||
               tk == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_AUTO_CONNECT_GATEWAY) {
        MQTT_SN_CLIENT_FIND_GW_PATTERN find_gw_pattern;
        int32_t                        find_gateway_wait_ms;
        if (cfg->cfmsgcfg.find_pattern_type == ADVERTISMENT_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE) {
            find_gw_pattern = MQTT_SN_CLIENT_FIND_GW_PATTERN_ADVERTISE;
            if (cfg->cfmsgcfg.advertise_wait_timeout_s > 0) {
                find_gateway_wait_ms = cfg->cfmsgcfg.advertise_wait_timeout_s * 1000;
            } else {
                find_gateway_wait_ms = cfg->cfmsgcfg.advertise_wait_timeout_s;
            }
        } else if (cfg->cfmsgcfg.find_pattern_type == SEARCH_GW_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE) {
            find_gw_pattern      = MQTT_SN_CLIENT_FIND_GW_PATTERN_SEARCHGW;
            find_gateway_wait_ms = cfg->cfmsgcfg.search_gateway_wait_timeout_ms;
        } else if (cfg->cfmsgcfg.find_pattern_type == CONNECT_CREDENTIALS_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE) {
            find_gw_pattern      = MQTT_SN_CLIENT_FIND_GW_PATTERN_DIRECT;
            find_gateway_wait_ms = cfg->cfmsgcfg.search_gateway_wait_timeout_ms;
        } else if (cfg->cfmsgcfg.find_pattern_type == ANY_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE) {
            find_gw_pattern      = MQTT_SN_CLIENT_FIND_GW_PATTERN_DIRECT;
            find_gateway_wait_ms = cfg->cfmsgcfg.search_gateway_wait_timeout_ms;
        } else {
            return mqtt_sn_client_interactive_cmd_result_error_only(tk);
        }

        if (get_timestamp_ns(&start_t_s, &start_t_ns) < 0) {
            return mqtt_sn_client_interactive_cmd_result_error_only(tk);
        }
        action_result = MqttSnClientInteractiveAutoConnect(client, cfg, find_gw_pattern, find_gateway_wait_ms, logger);
        if (get_timestamp_ns(&end_t_s, &end_t_ns) < 0) {
            return mqtt_sn_client_interactive_cmd_result_error_only(tk);
        }
    } else if (tk == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_DIRECT_CONNECT_GATEWAY) {
        if (get_timestamp_ns(&start_t_s, &start_t_ns) < 0) {
            return mqtt_sn_client_interactive_cmd_result_error_only(tk);
        }
        action_result = MqttSnClientInteractiveDirectConnect(client, cfg, logger);
        if (get_timestamp_ns(&end_t_s, &end_t_ns) < 0) {
            return mqtt_sn_client_interactive_cmd_result_error_only(tk);
        }
    } else if (tk == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_SEARCHGW_AND_CONNECT_GATEWAY) {
        if (get_timestamp_ns(&start_t_s, &start_t_ns) < 0) {
            return mqtt_sn_client_interactive_cmd_result_error_only(tk);
        }
        action_result = MqttSnClientInteractiveSearchGwAndConnect(client, cfg);
        if (get_timestamp_ns(&end_t_s, &end_t_ns) < 0) {
            return mqtt_sn_client_interactive_cmd_result_error_only(tk);
        }
    } else if (tk == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_ADVERTISE_AND_CONNECT_GATEWAY) {
        if (get_timestamp_ns(&start_t_s, &start_t_ns) < 0) {
            return mqtt_sn_client_interactive_cmd_result_error_only(tk);
        }
        action_result = MqttSnClientInteractiveAdvertiseAndDirectConnect(client, cfg);
        if (get_timestamp_ns(&end_t_s, &end_t_ns) < 0) {
            return mqtt_sn_client_interactive_cmd_result_error_only(tk);
        }
    } else if (tk == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_DISCONNECT_GATEWAY) {
        if (get_timestamp_ns(&start_t_s, &start_t_ns) < 0) {
            return mqtt_sn_client_interactive_cmd_result_error_only(tk);
        }
        action_result = MqttSnClientInteractiveDisconnect(client);
        if (get_timestamp_ns(&end_t_s, &end_t_ns) < 0) {
            return mqtt_sn_client_interactive_cmd_result_error_only(tk);
        }
    } else if (tk == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_REGISTER_GATEWAY) {
        if (get_timestamp_ns(&start_t_s, &start_t_ns) < 0) {
            return mqtt_sn_client_interactive_cmd_result_error_only(tk);
        }
        action_result = MqttSnClientInteractiveRegister(client, cfg);
        if (get_timestamp_ns(&end_t_s, &end_t_ns) < 0) {
            return mqtt_sn_client_interactive_cmd_result_error_only(tk);
        }
    } else if (tk == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_SUBSCRIBE_GATEWAY) {
        if (get_timestamp_ns(&start_t_s, &start_t_ns) < 0) {
            return mqtt_sn_client_interactive_cmd_result_error_only(tk);
        }
        action_result = MqttSnClientInteractiveSubscribe(client, cfg);
        if (get_timestamp_ns(&end_t_s, &end_t_ns) < 0) {
            return mqtt_sn_client_interactive_cmd_result_error_only(tk);
        }
        if (cfg->cscfg.subscription_len == 1) {
            tk = get_single_subscribe_mqtt_sn_client_interactive_action_type(&cfg->cscfg.subscription_list[0]);
        }
    } else if (tk == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_UNSUBSCRIBE_GATEWAY) {
        if (get_timestamp_ns(&start_t_s, &start_t_ns) < 0) {
            return mqtt_sn_client_interactive_cmd_result_error_only(tk);
        }
        action_result = MqttSnClientInteractiveUnsubscribe(client, cfg);
        if (get_timestamp_ns(&end_t_s, &end_t_ns) < 0) {
            return mqtt_sn_client_interactive_cmd_result_error_only(tk);
        }
    } else if (tk == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_GATEWAY) {
        if (get_timestamp_ns(&start_t_s, &start_t_ns) < 0) {
            return mqtt_sn_client_interactive_cmd_result_error_only(tk);
        }
        action_result = MqttSnClientInteractivePublish(client, cfg);
        if (get_timestamp_ns(&end_t_s, &end_t_ns) < 0) {
            return mqtt_sn_client_interactive_cmd_result_error_only(tk);
        }
        if (cfg->cpcfg.publish_list_len == 1) {
            // when it is only one publish we can determine and return the exact type
            tk = get_single_publish_mqtt_sn_client_interactive_action_type(&cfg->cpcfg.publish_list[0], &client->client);
        }
    } else if (tk == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_RECEIVE_GATEWAY) {
        if (get_timestamp_ns(&start_t_s, &start_t_ns) < 0) {
            return mqtt_sn_client_interactive_cmd_result_error_only(tk);
        }
        action_result = MqttSnClientInteractivePublishReceivePublish(client, cfg);
        if (get_timestamp_ns(&end_t_s, &end_t_ns) < 0) {
            return mqtt_sn_client_interactive_cmd_result_error_only(tk);
        }
        if (cfg->cpcfg.publish_list_len == 1) {
            // when it is only one publish we can determine and return the exact type
            tk = get_single_publish_receive_publish_mqtt_sn_client_interactive_action_type(&cfg->cpcfg.publish_list[0]);
        }
    } else if (tk == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_SLEEP_GATEWAY) {
        if (get_timestamp_ns(&start_t_s, &start_t_ns) < 0) {
            return mqtt_sn_client_interactive_cmd_result_error_only(tk);
        }
        action_result = MqttSnClientInteractiveSleep(client, cfg);
        if (get_timestamp_ns(&end_t_s, &end_t_ns) < 0) {
            return mqtt_sn_client_interactive_cmd_result_error_only(tk);
        }
    } else if (tk == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_WAKEUP_GATEWAY) {
        if (get_timestamp_ns(&start_t_s, &start_t_ns) < 0) {
            return mqtt_sn_client_interactive_cmd_result_error_only(tk);
        }
        action_result = MqttSnClientInteractiveWakeup(client);
        if (get_timestamp_ns(&end_t_s, &end_t_ns) < 0) {
            return mqtt_sn_client_interactive_cmd_result_error_only(tk);
        }
    } else if (tk == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_RECONNECT_GATEWAY) {
        if (get_timestamp_ns(&start_t_s, &start_t_ns) < 0) {
            return mqtt_sn_client_interactive_cmd_result_error_only(tk);
        }
        action_result = MqttSnClientInteractiveReconnect(client);
        if (get_timestamp_ns(&end_t_s, &end_t_ns) < 0) {
            return mqtt_sn_client_interactive_cmd_result_error_only(tk);
        }
    } else if (tk == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PING_GATEWAY) {
        if (get_timestamp_ns(&start_t_s, &start_t_ns) < 0) {
            return mqtt_sn_client_interactive_cmd_result_error_only(tk);
        }
        action_result = MqttSnClientInteractivePing(client);
        if (get_timestamp_ns(&end_t_s, &end_t_ns) < 0) {
            return mqtt_sn_client_interactive_cmd_result_error_only(tk);
        }
    } else {
        if (get_timestamp_ns(&start_t_s, &start_t_ns) < 0) {
            return mqtt_sn_client_interactive_cmd_result_error_only(tk);
        }
        action_result = MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_UNKNOWNACTION;
        if (get_timestamp_ns(&end_t_s, &end_t_ns) < 0) {
            return mqtt_sn_client_interactive_cmd_result_error_only(tk);
        }
    }

    mqtt_sn_client_interactive_cmd_result result = { { 0 } };

    char *identifier = NULL;
    if (logger->log_identifier) {
        identifier = logger->log_identifier;
    } else {
        identifier = cfg->cccfg.client_id;
    }
    mqtt_sn_client_interactive_cmd_result_init(&result, identifier, client->action_number++, tk, action_result, start_t_s, start_t_ns,
                                               end_t_s, end_t_ns);

    return result;
}
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE
MqttSnClientInteractiveReinitLogger(mqtt_sn_interactive_client *client, const interactive_client_config_container *cfg,
                                    MqttSnLogger *logger) {
#ifdef MQTT_SN_ARDUINO
    return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
#endif
    MqttSnLoggerDeinit(logger);
    if (mqtt_sn_client_interactive_start_logger(client, &cfg->mslcfg, logger) < 0) {
        return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
    }
    return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS;
}
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE
MqttSnClientInteractiveInit(mqtt_sn_interactive_client *client, const interactive_client_config_container *cfg,
                            const MqttSnLogger *logger) {
    if (client->client.gatewayNetwork.status != MQTT_SN_GATEWAY_NETWORK_INTERFACE_STATUS_DEINITIALIZED) {
        return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
    }
    int32_t network_init_rc   = 2;
    uint8_t is_config_success = 1;

#ifdef WITH_LINUX_UDP_GATEWAY_NETWORK
    if (!strcmp(cfg->gncfg.gateway_network_protocol, "udp")) {
        if ((network_init_rc = MqttSnLinuxGatewayUdpNetworkContainerStart(
               logger, cfg->msgcfg.mqtt_sn_gateway_host, cfg->msgcfg.mqtt_sn_gateway_port, cfg->gncfg.gateway_network_bind_address,
               cfg->gncfg.gateway_network_bind_port, cfg->gncfg.gateway_network_broadcast_address,
               cfg->gncfg.gateway_network_broadcast_bind_port, &client->client.gatewayNetwork, &client->udpGatewayNetworkContainer,
               &client->client.mqtt_sn_gateway_address))) {
            return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
        }

        if (is_config_success) {
            if (MqttSnClientSetMqttSnBroadcastAddress(&client->client,
                                                      &client->udpGatewayNetworkContainer.forwarderGatewayNetworkBroadcastAddress) !=
                MQTT_SN_CLIENT_RETURN_SUCCESS) {
                is_config_success = 0;
            }
        }
    }
#endif
#ifdef WITH_LINUX_TCP_GATEWAY_NETWORK
    if (!strcmp(cfg->gncfg.gateway_network_protocol, "tcp")) {
        if ((network_init_rc = MqttSnLinuxGatewayTcpNetworkContainerStart(
               logger, cfg->msgcfg.mqtt_sn_gateway_host, cfg->msgcfg.mqtt_sn_gateway_port, cfg->gncfg.gateway_network_bind_address,
               cfg->gncfg.gateway_network_bind_port, cfg->gncfg.gateway_network_broadcast_address,
               cfg->gncfg.gateway_network_broadcast_bind_port, &client->client.gatewayNetwork, &client->tcpGatewayNetworkContainer,
               &client->client.mqtt_sn_gateway_address))) {
            return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
        }

        if (is_config_success) {
            if (MqttSnClientSetMqttSnBroadcastAddress(&client->client,
                                                      &client->tcpGatewayNetworkContainer.forwarderGatewayNetworkBroadcastAddress) !=
                MQTT_SN_CLIENT_RETURN_SUCCESS) {
                is_config_success = 0;
            }
        }
    }
#endif
#ifdef WITH_ARDUINO_GATEWAY_NETWORK_UDP
    if (!strcmp(cfg->gncfg.gateway_network_protocol, "udp")) {
        if ((network_init_rc = MqttSnArduinoGatewayUdpNetworkContainerStart(
               logger, cfg->msgcfg.mqtt_sn_gateway_host, cfg->msgcfg.mqtt_sn_gateway_port, cfg->gncfg.gateway_network_bind_address,
               cfg->gncfg.gateway_network_bind_port, cfg->gncfg.gateway_network_broadcast_address,
               cfg->gncfg.gateway_network_broadcast_bind_port, &client->client.gatewayNetwork, &client->udpGatewayNetworkContainer,
               &client->client.mqtt_sn_gateway_address))) {
            return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
        }

        if (is_config_success) {
            if (MqttSnClientSetMqttSnBroadcastAddress(&client->client,
                                                      &client->udpGatewayNetworkContainer.forwarderGatewayNetworkBroadcastAddress) !=
                MQTT_SN_CLIENT_RETURN_SUCCESS) {
                is_config_success = 0;
            }
        }
    }
#endif
    if (network_init_rc == EXIT_FAILURE) {
        return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
    }
    if (network_init_rc == 2) {
        log_str(logger, "Error init unknown gateway network  protocol: ");
        log_str(logger, cfg->gncfg.gateway_network_protocol);
        log_str(logger, "\n");
        return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
    }
    if (MqttSnClientInit(&client->client, logger) != MQTT_SN_CLIENT_CONNECTION_STATUS_INITALIZED) {
        return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
    }
    // configure client with the given values from the cfg
    if (is_config_success) {
        if (MqttSnClientSetPublishCallback(&client->client, interactive_client_publish_callback) != MQTT_SN_CLIENT_RETURN_SUCCESS) {
            is_config_success = 0;
        }
    }

    // client_network_parameter_config
    MqttSnClientSetTimeoutOffset(&client->client, cfg->cccfg.connection_timeout_offset);
    if (is_config_success) {
        if (MqttSnClientSetRetryTimeout(&client->client, cfg->cnpcfg.retry_timeout_ms) != MQTT_SN_CLIENT_RETURN_SUCCESS) {
            is_config_success = 0;
        }
    }
    MqttSnClientSetRetryCount(&client->client, cfg->cnpcfg.retry_count);
    MqttSnClientSetCongestionDelay(&client->client, cfg->cnpcfg.congestion_delay_s);
    MqttSnClientSetDefaultSignalStrength(&client->client, cfg->cnpcfg.default_signal_strength);

    // client_search_gw_reply_config
    if (is_config_success) {
        if (cfg->csrcfg.search_gw_reply_enabled) {
            MqttSnClientEnableSearchGwReply(&client->client);
        } else {
            MqttSnClientDisableSearchGwReply(&client->client);
        }
    }
    MqttSnClientSearchGwGwInfoReplyDelay(&client->client, cfg->csrcfg.gw_info_delay_ms);

#if defined(WITH_MQTT_SN_CLIENT_AUTO_RECONNECT)
    if (is_config_success) {
        if (cfg->cccfg.auto_reconnect) {
            MqttSnClientEnableAutoReconnect(&client->client);
        } else {
            MqttSnClientDisableAutoReconnect(&client->client);
        }
    }
#endif
#ifdef WITH_MQTT_SN_CLIENT_WHITELIST
    if (is_config_success) {
        MqttSnClientClearWhitelistGwId(&client->client);
        MqttSnClientClearWhitelistNetworkAddress(&client->client);
    }
    if (is_config_success) {
        for (int32_t i = 0; i < cfg->gncfg.gw_id_whitelist_len; i++) {
            if (MqttSnClientAddWhitelistGwId(&client->client, cfg->gncfg.gw_id_whitelist[i]) != MQTT_SN_CLIENT_RETURN_SUCCESS) {
                is_config_success = 0;
                break;
            }
        }
    }
    if (is_config_success) {
        for (int32_t i = 0; i < cfg->gncfg.gw_addr_whitelist_len; i++) {
            if (MqttSnClientAddWhitelistNetworkAddress(&client->client, cfg->gncfg.gw_addr_whitelist[i]) != MQTT_SN_CLIENT_RETURN_SUCCESS) {
                is_config_success = 0;
                break;
            }
        }
    }
#endif
#ifdef WITH_MQTT_SN_CLIENT_BLACKLIST
    if (is_config_success) {
        MqttSnClientClearBlacklistGwId(&client->client);
        MqttSnClientClearBlacklistNetworkAddress(&client->client);
    }
    if (is_config_success) {
        for (int32_t i = 0; i < cfg->gncfg.gw_id_blacklist_len; i++) {
            if (MqttSnClientAddBlacklistGwId(&client->client, cfg->gncfg.gw_id_blacklist[i]) != MQTT_SN_CLIENT_RETURN_SUCCESS) {
                is_config_success = 0;
                break;
            }
        }
    }
    if (is_config_success) {
        for (int32_t i = 0; i < cfg->gncfg.gw_addr_blacklist_len; i++) {
            if (MqttSnClientAddBlacklistNetworkAddress(&client->client, cfg->gncfg.gw_addr_blacklist[i]) != MQTT_SN_CLIENT_RETURN_SUCCESS) {
                is_config_success = 0;
                break;
            }
        }
    }
#endif

    if (!is_config_success) {
        MqttSnClientInteractiveDeinit(client);
    }
    return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS;
}
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE MqttSnClientInteractiveDeinit(mqtt_sn_interactive_client *client) {
    if (client->client.gatewayNetwork.status == MQTT_SN_GATEWAY_NETWORK_INTERFACE_STATUS_DEINITIALIZED) {
        return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
    }
    if (MqttSnClientDeinit(&client->client) != MQTT_SN_CLIENT_CONNECTION_STATUS_UNINITALIZED) {
        return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
    }
    if (GatewayNetworkDeinitialize(&client->client.gatewayNetwork, client->client.gatewayNetwork.context) < 0) {
        return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
    }
    client->client.gatewayNetwork.context = NULL;
    return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS;
}

MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE MqttSnClientInteractiveAutoConnect(mqtt_sn_interactive_client *               client,
                                                                                 const interactive_client_config_container *cfg,
                                                                                 const MQTT_SN_CLIENT_FIND_GW_PATTERN find_gateway_pattern,
                                                                                 const int32_t       find_gateway_timeout_ms,
                                                                                 const MqttSnLogger *logger) {
    MQTT_SN_CLIENT_RETURN_CODE connect_rc;

    device_address mqtt_sn_gateway_address = { 0 };
#ifdef WITH_ARDUINO_GATEWAY_NETWORK_UDP
    if (convert_hostname_port_to_device_address(logger, cfg->msgcfg.mqtt_sn_gateway_host, cfg->msgcfg.mqtt_sn_gateway_port,
                                                &mqtt_sn_gateway_address, "mqtt-sn gateway")) {
        return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
    }
#else
    if (convert_hostname_port_to_device_address(cfg->msgcfg.mqtt_sn_gateway_host, cfg->msgcfg.mqtt_sn_gateway_port,
                                                &mqtt_sn_gateway_address, "mqtt-sn gateway")) {
        return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
    }
#endif

#if defined(WITH_LINUX_UDP_GATEWAY_NETWORK) || defined(WITH_LINUX_TCP_GATEWAY_NETWORK)
    if (client->client.gatewayNetwork.context == &client->udpGatewayNetworkContainer.udpGatewayNetworkContext ||
        client->client.gatewayNetwork.context == &client->tcpGatewayNetworkContainer.tcpGatewayNetworkContext) {
        // check if first four bytes of device address are zero => then it is localhost
        if (is_device_address_ipv4_zeroed(&mqtt_sn_gateway_address)) {
            set_device_address_ipv4_localhost(&mqtt_sn_gateway_address);
        }
    }
#endif

    if ((connect_rc = MqttSnClientAutoConnect(
           &client->client, find_gateway_pattern, find_gateway_timeout_ms, &mqtt_sn_gateway_address, cfg->cccfg.clean_session,
           cfg->cccfg.client_id, cfg->cccfg.connect_duration, cfg->cccfg.will_topic, cfg->cccfg.will_qos, cfg->cccfg.will_retain,
           (const uint8_t *)cfg->cccfg.will_msg, strlen(cfg->cccfg.will_msg))) != MQTT_SN_CLIENT_RETURN_SUCCESS) {
        if (connect_rc == MQTT_SN_CLIENT_RETURN_TIMEOUT) {
            return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_TIMEOUT;
        }
        return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
    }
    return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS;
}
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE MqttSnClientInteractiveSearchGwAndConnect(mqtt_sn_interactive_client *               client,
                                                                                        const interactive_client_config_container *cfg) {
    MqttSnClientReceivedGwInfo received_gw_info = { { 0 } };
    MQTT_SN_CLIENT_RETURN_CODE gw_info_rc;
    if ((gw_info_rc = MqttSnClientSearchGw(&client->client, cfg->cfmsgcfg.search_gateway_wait_timeout_ms,
                                           cfg->cfmsgcfg.search_gateway_radius, &received_gw_info)) != MQTT_SN_CLIENT_RETURN_SUCCESS) {
        if (gw_info_rc == MQTT_SN_CLIENT_RETURN_TIMEOUT) {
            return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_TIMEOUT;
        }
        return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
    }

    if (received_gw_info.received_gw_info.gwAddLen > 0) {
        client->mqtt_sn_gateway_address = received_gw_info.from;
        int32_t addr_diff_len           = sizeof(device_address) - received_gw_info.received_gw_info.gwAddLen;
        for (uint16_t i = 0; i < addr_diff_len; i++) {
            client->mqtt_sn_gateway_address.bytes[i] = 0;
        }
        for (uint16_t i = addr_diff_len; i < sizeof(device_address); i++) {
            client->mqtt_sn_gateway_address.bytes[i] = received_gw_info.received_gw_info.gwAdd.bytes[i - addr_diff_len];
        }
    } else {
        client->mqtt_sn_gateway_address = received_gw_info.from;
    }
    client->client.default_signal_strength = received_gw_info.signal_strength;

    MQTT_SN_CLIENT_RETURN_CODE connect_rc;
    if ((connect_rc =
           MqttSnClientDirectConnect(&client->client, &client->mqtt_sn_gateway_address, cfg->cccfg.clean_session, cfg->cccfg.client_id,
                                     cfg->cccfg.connect_duration, cfg->cccfg.will_topic, cfg->cccfg.will_qos, cfg->cccfg.will_retain,
                                     (const uint8_t *)cfg->cccfg.will_msg, strlen(cfg->cccfg.will_msg))) != MQTT_SN_CLIENT_RETURN_SUCCESS) {
        if (connect_rc == MQTT_SN_CLIENT_RETURN_TIMEOUT) {
            return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_TIMEOUT;
        }
        return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
    }
    return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS;
}
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE MqttSnClientInteractiveDirectConnect(mqtt_sn_interactive_client *               client,
                                                                                   const interactive_client_config_container *cfg,
                                                                                   const MqttSnLogger *                       logger) {
    MQTT_SN_CLIENT_RETURN_CODE connect_rc;
    device_address             mqtt_sn_gateway_address = { 0 };

#ifdef WITH_ARDUINO_GATEWAY_NETWORK_UDP
    if (convert_hostname_port_to_device_address(logger, cfg->msgcfg.mqtt_sn_gateway_host, cfg->msgcfg.mqtt_sn_gateway_port,
                                                &mqtt_sn_gateway_address, "mqtt-sn gateway")) {
        return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
    }
#else
    if (convert_hostname_port_to_device_address(cfg->msgcfg.mqtt_sn_gateway_host, cfg->msgcfg.mqtt_sn_gateway_port,
                                                &mqtt_sn_gateway_address, "mqtt-sn gateway")) {
        return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
    }
#endif

#if defined(WITH_LINUX_UDP_GATEWAY_NETWORK) || defined(WITH_LINUX_TCP_GATEWAY_NETWORK)
    if (client->client.gatewayNetwork.context == &client->udpGatewayNetworkContainer.udpGatewayNetworkContext ||
        client->client.gatewayNetwork.context == &client->tcpGatewayNetworkContainer.tcpGatewayNetworkContext) {
        // check if first four bytes of device address are zero => then it is localhost
        if (is_device_address_ipv4_zeroed(&mqtt_sn_gateway_address)) {
            set_device_address_ipv4_localhost(&mqtt_sn_gateway_address);
        }
    }
#endif

    if ((connect_rc =
           MqttSnClientDirectConnect(&client->client, &mqtt_sn_gateway_address, cfg->cccfg.clean_session, cfg->cccfg.client_id,
                                     cfg->cccfg.connect_duration, cfg->cccfg.will_topic, cfg->cccfg.will_qos, cfg->cccfg.will_retain,
                                     (const uint8_t *)cfg->cccfg.will_msg, strlen(cfg->cccfg.will_msg))) != MQTT_SN_CLIENT_RETURN_SUCCESS) {
        if (connect_rc == MQTT_SN_CLIENT_RETURN_TIMEOUT) {
            return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_TIMEOUT;
        }
        return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
    }
    return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS;
}
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE
MqttSnClientInteractiveAdvertiseAndDirectConnect(mqtt_sn_interactive_client *client, const interactive_client_config_container *cfg) {
    MqttSnClientReceivedAdvertise received_advertise = { { 0 } };
    MQTT_SN_CLIENT_RETURN_CODE    advertise_rc;
    if ((advertise_rc = MqttSnClientAwaitAdvertise(&client->client, cfg->cfmsgcfg.advertise_wait_timeout_s, &received_advertise)) !=
        MQTT_SN_CLIENT_RETURN_SUCCESS) {
        if (advertise_rc == MQTT_SN_CLIENT_RETURN_TIMEOUT) {
            return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_TIMEOUT;
        }
        return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
    }
    client->mqtt_sn_gateway_address        = received_advertise.from;
    client->client.default_signal_strength = received_advertise.signal_strength;

    MQTT_SN_CLIENT_RETURN_CODE connect_rc;
    if ((connect_rc = MqttSnClientDirectConnect(&client->client, &client->client.mqtt_sn_gateway_address, cfg->cccfg.clean_session,
                                                cfg->cccfg.client_id, cfg->cccfg.connect_duration, cfg->cccfg.will_topic,
                                                cfg->cccfg.will_qos, cfg->cccfg.will_retain, (const uint8_t *)cfg->cccfg.will_msg,
                                                strlen(cfg->cccfg.will_msg))) != MQTT_SN_CLIENT_RETURN_SUCCESS) {
        if (connect_rc == MQTT_SN_CLIENT_RETURN_TIMEOUT) {
            return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_TIMEOUT;
        }
        return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
    }
    return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS;
}
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE MqttSnClientInteractiveDisconnect(mqtt_sn_interactive_client *client) {
    MQTT_SN_CLIENT_RETURN_CODE disconnect_rc;
    if ((disconnect_rc = MqttSnClientDisconnect(&client->client)) != MQTT_SN_CLIENT_RETURN_SUCCESS) {
        if (disconnect_rc == MQTT_SN_CLIENT_RETURN_TIMEOUT) {
            return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_TIMEOUT;
        }
        return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
    }
    return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS;
}
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE MqttSnClientInteractivePublish(mqtt_sn_interactive_client *               client,
                                                                             const interactive_client_config_container *cfg) {
    for (uint32_t i = 0; i < cfg->cpcfg.publish_list_len; i++) {
        MQTT_SN_CLIENT_RETURN_CODE   publish_rc;
        single_client_publish_config publish = cfg->cpcfg.publish_list[i];
        if (publish.topic_id_type == CLIENT_MQTT_SN_PUBLISH_TOPIC_ID_TYPE_TOPICNAME) {
            publish_rc = MqttSnClientPublishTopicName(&client->client, publish.topic_name, strlen(publish.topic_name), publish.retain,
                                                      publish.qos, publish.data, publish.data_length);
        } else if (publish.topic_id_type == CLIENT_MQTT_SN_PUBLISH_TOPIC_ID_TYPE_PREDEFINED) {
            if (publish.qos == -1) {
                publish_rc =
                  MqttSnClientPublishPredefinedM1(&client->client, publish.topic_id, publish.retain, publish.data, publish.data_length);
            } else if (publish.qos >= 0) {
                publish_rc = MqttSnClientPublishPredefined(&client->client, publish.topic_id, publish.retain, publish.qos, publish.data,
                                                           publish.data_length);
            } else {
                publish_rc = MQTT_SN_CLIENT_RETURN_ERROR;
            }
        } else if (publish.topic_id_type == CLIENT_MQTT_SN_PUBLISH_TOPIC_ID_TYPE_TOPICID) {
            publish_rc =
              MqttSnClientPublishTopicId(&client->client, publish.topic_id, publish.retain, publish.qos, publish.data, publish.data_length);
        } else if (publish.topic_id_type == CLIENT_MQTT_SN_PUBLISH_TOPIC_ID_TYPE_REGISTRATIONINDEX) {
            publish_rc = MqttSnClientPublishByRegistrationIndex(&client->client, publish.topic_id, publish.retain, publish.qos,
                                                                publish.data, publish.data_length);
        } else {
            publish_rc = MQTT_SN_CLIENT_RETURN_ERROR;
        }
        if (publish_rc != MQTT_SN_CLIENT_RETURN_SUCCESS) {
            if (publish_rc == MQTT_SN_CLIENT_RETURN_TIMEOUT) {
                return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_TIMEOUT;
            } else {
                return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
            }
        }
    }
    return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS;
}
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE MqttSnClientInteractivePublishReceivePublish(mqtt_sn_interactive_client *client,
                                                                                           const interactive_client_config_container *cfg) {
    for (uint32_t i = 0; i < cfg->cpcfg.publish_list_len; i++) {
        MQTT_SN_CLIENT_RETURN_CODE   publish_rc;
        single_client_publish_config publish = cfg->cpcfg.publish_list[i];

        interactive_client_publish_callback_context = NULL;
        interactive_client_publish_callback_rc      = MQTT_SN_CLIENT_RETURN_ERROR;
        // set expected
        interactive_client_publish_callback_context = &publish;

        if (publish.topic_id_type == CLIENT_MQTT_SN_PUBLISH_TOPIC_ID_TYPE_TOPICNAME) {
            publish_rc = MqttSnClientPublishTopicName(&client->client, publish.topic_name, strlen(publish.topic_name), publish.retain,
                                                      publish.qos, publish.data, publish.data_length);
        } else if (publish.topic_id_type == CLIENT_MQTT_SN_PUBLISH_TOPIC_ID_TYPE_PREDEFINED) {
            if (publish.qos == -1) {
                publish_rc =
                  MqttSnClientPublishPredefinedM1(&client->client, publish.topic_id, publish.retain, publish.data, publish.data_length);
            } else if (publish.qos >= 0) {
                publish_rc = MqttSnClientPublishPredefined(&client->client, publish.topic_id, publish.retain, publish.qos, publish.data,
                                                           publish.data_length);
            } else {
                publish_rc = MQTT_SN_CLIENT_RETURN_ERROR;
            }
        } else if (publish.topic_id_type == CLIENT_MQTT_SN_PUBLISH_TOPIC_ID_TYPE_TOPICID) {
            publish_rc =
              MqttSnClientPublishTopicId(&client->client, publish.topic_id, publish.retain, publish.qos, publish.data, publish.data_length);
        } else if (publish.topic_id_type == CLIENT_MQTT_SN_PUBLISH_TOPIC_ID_TYPE_REGISTRATIONINDEX) {
            publish_rc = MqttSnClientPublishByRegistrationIndex(&client->client, publish.topic_id, publish.retain, publish.qos,
                                                                publish.data, publish.data_length);
        } else {
            interactive_client_publish_callback_context = NULL;
            interactive_client_publish_callback_rc      = MQTT_SN_CLIENT_RETURN_ERROR;
            publish_rc                                  = MQTT_SN_CLIENT_RETURN_ERROR;
        }

        if (publish_rc != MQTT_SN_CLIENT_RETURN_SUCCESS) {
            interactive_client_publish_callback_context = NULL;
            interactive_client_publish_callback_rc      = MQTT_SN_CLIENT_RETURN_ERROR;
            if (publish_rc == MQTT_SN_CLIENT_RETURN_TIMEOUT) {
                return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_TIMEOUT;
            } else {
                return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
            }
        }

        uint64_t start_s_ts;
        uint64_t start_ms_ts;
        if (get_timestamp_ms(&start_s_ts, &start_ms_ts) < 0) {
            interactive_client_publish_callback_context = NULL;
            interactive_client_publish_callback_rc      = MQTT_SN_CLIENT_RETURN_ERROR;
            return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
        }

        // wait for client->client.retry_timeout_ms
        while (interactive_client_publish_callback_rc != MQTT_SN_CLIENT_RETURN_SUCCESS) {
            MQTT_SN_CLIENT_LOOP_RETURN_CODE loop_rc = MqttSnClientLoop(&client->client);
            if (loop_rc < 0) {
                publish_rc = MQTT_SN_CLIENT_RETURN_ERROR;
                break;
            }
            uint64_t current_s_ts;
            uint64_t current_ms_ts;
            if (get_timestamp_ms(&current_s_ts, &current_ms_ts) < 0) {
                interactive_client_publish_callback_context = NULL;
                interactive_client_publish_callback_rc      = MQTT_SN_CLIENT_RETURN_ERROR;
                return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
            }
            uint8_t  is_timeout = 0;
            uint64_t diff_s_ts  = current_s_ts - start_s_ts;
            uint64_t diff_ms_ts;
            if (current_ms_ts >= start_ms_ts) {
                diff_ms_ts = current_ms_ts - start_ms_ts;
            } else {
                diff_s_ts -= 1;
                diff_ms_ts = 1000 - (start_ms_ts - current_ms_ts);
            }

            uint64_t total_diff_ms_ts = ((diff_s_ts * (uint64_t)1000) + diff_ms_ts);
            if (client->client.retry_timeout_ms < 0) {
                continue;
            }
            if (total_diff_ms_ts >= (uint64_t)client->client.retry_timeout_ms) {
                is_timeout = 1;
            }
            if (is_timeout) {
                publish_rc = MQTT_SN_CLIENT_RETURN_TIMEOUT;
                break;
            }
        }

        if (interactive_client_publish_callback_rc != MQTT_SN_CLIENT_RETURN_SUCCESS) {
            interactive_client_publish_callback_context = NULL;
            interactive_client_publish_callback_rc      = MQTT_SN_CLIENT_RETURN_ERROR;
            if (publish_rc == MQTT_SN_CLIENT_RETURN_TIMEOUT) {
                return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_TIMEOUT;
            } else {
                return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
            }
        }
    }
    interactive_client_publish_callback_context = NULL;
    interactive_client_publish_callback_rc      = MQTT_SN_CLIENT_RETURN_ERROR;
    return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS;
}
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE MqttSnClientInteractiveSubscribe(mqtt_sn_interactive_client *               client,
                                                                               const interactive_client_config_container *cfg) {
    for (uint16_t i = 0; i < cfg->cscfg.subscription_len; i++) {
        client_subscription_topic subscribe = cfg->cscfg.subscription_list[i];

        MQTT_SN_CLIENT_RETURN_CODE subscribe_rc;
        if (subscribe.topic_id_type == CLIENT_MQTT_SN_SUBSCRIPTION_TOPIC_ID_TYPE_PREDEFINED) {
            subscribe_rc = MqttSnClientSubscribePredefined(&client->client, subscribe.qos, subscribe.topic_id);
        } else if (subscribe.topic_id_type == CLIENT_MQTT_SN_SUBSCRIPTION_TOPIC_ID_TYPE_TOPICID) {
            subscribe_rc = MqttSnClientSubscribeShort(&client->client, subscribe.qos, subscribe.topic_id);
        } else if (subscribe.topic_id_type == CLIENT_MQTT_SN_SUBSCRIPTION_TOPIC_ID_TYPE_TOPICNAME) {
            subscribe_rc =
              MqttSnClientSubscribeTopicName(&client->client, subscribe.qos, subscribe.topic_name, strlen(subscribe.topic_name));
        } else {
            return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
        }
        if (subscribe_rc != MQTT_SN_CLIENT_RETURN_SUCCESS) {
            if (subscribe_rc == MQTT_SN_CLIENT_RETURN_TIMEOUT) {
                return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_TIMEOUT;
            }
            return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
        }
    }

    return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS;
}
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE MqttSnClientInteractiveUnsubscribe(mqtt_sn_interactive_client *               client,
                                                                                 const interactive_client_config_container *cfg) {
    for (uint16_t i = 0; i < cfg->cuscfg.unsubscribe_list_len; i++) {
        client_unsubscription_topic unsubscribe = cfg->cuscfg.unsubscribe_list[i];
        MQTT_SN_CLIENT_RETURN_CODE  unsubscribe_rc;
        if (unsubscribe.topic_id_type == CLIENT_MQTT_SN_UNSUBSCRIPTION_TOPIC_ID_TYPE_TOPICNAME) {
            unsubscribe_rc = MqttSnClientUnsubscribeTopicName(&client->client, unsubscribe.topic_name, strlen(unsubscribe.topic_name));
        } else if (unsubscribe.topic_id_type == CLIENT_MQTT_SN_UNSUBSCRIPTION_TOPIC_ID_TYPE_PREDEFINED) {
            unsubscribe_rc = MqttSnClientUnsubscribePredefined(&client->client, unsubscribe.topic_id);
        } else if (unsubscribe.topic_id_type == CLIENT_MQTT_SN_UNSUBSCRIPTION_TOPIC_ID_TYPE_TOPICID) {
            unsubscribe_rc = MqttSnClientUnsubscribeShort(&client->client, unsubscribe.topic_id);
        } else {
            unsubscribe_rc = MQTT_SN_CLIENT_RETURN_ERROR;
        }
        if (unsubscribe_rc != MQTT_SN_CLIENT_RETURN_SUCCESS) {
            if (unsubscribe_rc == MQTT_SN_CLIENT_RETURN_TIMEOUT) {
                return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_TIMEOUT;
            }
            return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
        }
    }
    return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS;
}
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE MqttSnClientInteractiveRegister(mqtt_sn_interactive_client *               client,
                                                                              const interactive_client_config_container *cfg) {
    for (uint16_t i = 0; i < cfg->crcfg.registration_list_len; i++) {
        MQTT_SN_CLIENT_RETURN_CODE register_rc;
        if ((register_rc = MqttSnClientRegister(&client->client, cfg->crcfg.registration_list[i].topic_name,
                                                strlen(cfg->crcfg.registration_list[i].topic_name))) != MQTT_SN_CLIENT_RETURN_SUCCESS) {
            if (register_rc == MQTT_SN_CLIENT_RETURN_TIMEOUT) {
                return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_TIMEOUT;
            }
            return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
        }
    }
    return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS;
}
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE
MqttSnClientInteractiveSleep(mqtt_sn_interactive_client *client, const interactive_client_config_container *cfg) {
    MQTT_SN_CLIENT_RETURN_CODE sleep_rc;
    sleep_rc = MqttSnClientSleep(&client->client, cfg->scfg.sleep_duration);
    if (sleep_rc != MQTT_SN_CLIENT_RETURN_SUCCESS) {
        if (sleep_rc == MQTT_SN_CLIENT_RETURN_TIMEOUT) {
            return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_TIMEOUT;
        }
        return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
    }
    return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS;
}
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE
MqttSnClientInteractiveWakeup(mqtt_sn_interactive_client *client) {
    MQTT_SN_CLIENT_RETURN_CODE wakeup_rc = MqttSnClientWakeup(&client->client);
    if (wakeup_rc != MQTT_SN_CLIENT_RETURN_SUCCESS) {
        if (wakeup_rc == MQTT_SN_CLIENT_RETURN_TIMEOUT) {
            return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_TIMEOUT;
        }
        return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
    }
    return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS;
}
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE MqttSnClientInteractiveReconnect(mqtt_sn_interactive_client *client) {
    MQTT_SN_CLIENT_RETURN_CODE reconnect_rc = MqttSnClientReconnect(&client->client);
    if (reconnect_rc != MQTT_SN_CLIENT_RETURN_SUCCESS) {
        if (reconnect_rc == MQTT_SN_CLIENT_RETURN_TIMEOUT) {
            return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_TIMEOUT;
        }
        return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
    }
    return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS;
}
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE
MqttSnClientInteractivePing(mqtt_sn_interactive_client *client) {
    MQTT_SN_CLIENT_RETURN_CODE ping_rc = MqttSnClientPing(&client->client);
    if (ping_rc != MQTT_SN_CLIENT_RETURN_SUCCESS) {
        if (ping_rc == MQTT_SN_CLIENT_RETURN_TIMEOUT) {
            return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_TIMEOUT;
        }
        return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
    }
    return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS;
}
MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE
get_single_publish_mqtt_sn_client_interactive_action_type(const single_client_publish_config *publish, const MqttSnClient *client) {
    if (publish->topic_id_type == CLIENT_MQTT_SN_PUBLISH_TOPIC_ID_TYPE_TOPICNAME) {
        if (publish->qos == -1) {
            return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_TOPICNAME_QOSM1_GATEWAY;
        } else if (publish->qos == 0) {
            return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_TOPICNAME_QOS0_GATEWAY;
        } else if (publish->qos == 1) {
            return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_TOPICNAME_QOS1_GATEWAY;
        } else if (publish->qos == 2) {
            return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_TOPICNAME_QOS2_GATEWAY;
        }
    } else if (publish->topic_id_type == CLIENT_MQTT_SN_PUBLISH_TOPIC_ID_TYPE_PREDEFINED) {
        if (publish->qos == -1) {
            if (client->status < MQTT_SN_CLIENT_CONNECTION_STATUS_CONNECTED) {
                return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_UNCONNECTED_PREDEFINED_QOSM1_GATEWAY;
            } else {
                return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_PREDEFINED_QOSM1_GATEWAY;
            }
        } else if (publish->qos == 0) {
            return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_PREDEFINED_QOS0_GATEWAY;
        } else if (publish->qos == 1) {
            return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_PREDEFINED_QOS1_GATEWAY;
        } else if (publish->qos == 2) {
            return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_PREDEFINED_QOS2_GATEWAY;
        }
    } else if (publish->topic_id_type == CLIENT_MQTT_SN_PUBLISH_TOPIC_ID_TYPE_TOPICID) {
        if (publish->qos == -1) {
            return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_TOPICID_QOSM1_GATEWAY;
        } else if (publish->qos == 0) {
            return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_TOPICID_QOS0_GATEWAY;
        } else if (publish->qos == 1) {
            return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_TOPICID_QOS1_GATEWAY;
        } else if (publish->qos == 2) {
            return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_TOPICID_QOS2_GATEWAY;
        }
    } else if (publish->topic_id_type == CLIENT_MQTT_SN_PUBLISH_TOPIC_ID_TYPE_REGISTRATIONINDEX) {
        return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_GATEWAY;
    }
    return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_GATEWAY;
}
MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE
get_single_subscribe_mqtt_sn_client_interactive_action_type(const client_subscription_topic *subscription) {
    if (subscription->topic_id_type == CLIENT_MQTT_SN_SUBSCRIPTION_TOPIC_ID_TYPE_PREDEFINED) {
        return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PREDEFINED_SUBSCRIBE_GATEWAY;
    } else if (subscription->topic_id_type == CLIENT_MQTT_SN_SUBSCRIPTION_TOPIC_ID_TYPE_TOPICID) {
        return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_TOPICID_SUBSCRIBE_GATEWAY;
    } else if (subscription->topic_id_type == CLIENT_MQTT_SN_SUBSCRIPTION_TOPIC_ID_TYPE_TOPICNAME) {
        return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_TOPICNAME_SUBSCRIBE_GATEWAY;
    }
    return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_SUBSCRIBE_GATEWAY;
}
MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE
get_single_publish_receive_publish_mqtt_sn_client_interactive_action_type(const single_client_publish_config *publish) {
    if (publish->topic_id_type == CLIENT_MQTT_SN_PUBLISH_TOPIC_ID_TYPE_TOPICNAME) {
        if (publish->qos == -1) {
            return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_RECEIVE_PUBLISH_TOPICNAME_QOSM1_GATEWAY;
        } else if (publish->qos == 0) {
            return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_RECEIVE_PUBLISH_TOPICNAME_QOS0_GATEWAY;
        } else if (publish->qos == 1) {
            return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_RECEIVE_PUBLISH_TOPICNAME_QOS1_GATEWAY;
        } else if (publish->qos == 2) {
            return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_RECEIVE_PUBLISH_TOPICNAME_QOS2_GATEWAY;
        }
    } else if (publish->topic_id_type == CLIENT_MQTT_SN_PUBLISH_TOPIC_ID_TYPE_PREDEFINED) {
        if (publish->qos == -1) {
            return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_RECEIVE_PUBLISH_PREDEFINED_QOSM1_GATEWAY;
        } else if (publish->qos == 0) {
            return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_RECEIVE_PUBLISH_PREDEFINED_QOS0_GATEWAY;
        } else if (publish->qos == 1) {
            return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_RECEIVE_PUBLISH_PREDEFINED_QOS1_GATEWAY;
        } else if (publish->qos == 2) {
            return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_RECEIVE_PUBLISH_PREDEFINED_QOS2_GATEWAY;
        }
    } else if (publish->topic_id_type == CLIENT_MQTT_SN_PUBLISH_TOPIC_ID_TYPE_TOPICID) {
        if (publish->qos == -1) {
            return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_RECEIVE_PUBLISH_TOPICID_QOSM1_GATEWAY;
        } else if (publish->qos == 0) {
            return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_RECEIVE_PUBLISH_TOPICID_QOS0_GATEWAY;
        } else if (publish->qos == 1) {
            return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_RECEIVE_PUBLISH_TOPICID_QOS1_GATEWAY;
        } else if (publish->qos == 2) {
            return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_RECEIVE_PUBLISH_TOPICID_QOS2_GATEWAY;
        }
    } else if (publish->topic_id_type == CLIENT_MQTT_SN_PUBLISH_TOPIC_ID_TYPE_REGISTRATIONINDEX) {
        return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_RECEIVE_GATEWAY;
    }
    return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_RECEIVE_GATEWAY;
}
int32_t interactive_client_publish_callback(const MqttSnClient *client, const MqttSnPublish *publish) {
    if (interactive_client_publish_callback_context != NULL) {
        if (publish->payload_length == interactive_client_publish_callback_context->data_length) {
            if (memcmp(publish->payload, interactive_client_publish_callback_context->data, publish->payload_length) == 0) {
                interactive_client_publish_callback_rc = MQTT_SN_CLIENT_RETURN_SUCCESS;
                return 0;
            }
        }
    }
    return log_publish_message(&client->logger, publish);
}
