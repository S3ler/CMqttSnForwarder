//
// Created by SomeDude on 02.08.2019.
//

#include "MqttSnCmdBuffer.h"
#include "mqtt_sn_client_interactive_cmd_result.h"
#include <string.h>
#include <platform/platform_compatibility.h>
#include <config/common/config_command_helper.h>
#include <network/linux/shared/ip/MqttSnIpNetworkHelper.h>
#include <network/shared/ip/IpHelper.h>
#include <client/MqttSnClientLogger.h>
#include <logging/linux/file/FileLogging.h>
#include <logging/linux/stdout/StdoutLogging.h>
#include <logging/linux/filestdout/FileStdoutLogging.h>

void mqtt_sn_cmd_buffer_init(mqtt_sn_interactive_client_cmd_buffer *mqtt_sn_cmd_buffer) {
  memset(mqtt_sn_cmd_buffer, 0, sizeof(*mqtt_sn_cmd_buffer));
  memset(mqtt_sn_cmd_buffer->argv, 0, sizeof(mqtt_sn_cmd_buffer->buffer));
  mqtt_sn_cmd_buffer->argc = 0;
  mqtt_sn_cmd_buffer->argv_0[0] = '\0';
  mqtt_sn_cmd_buffer->argv[mqtt_sn_cmd_buffer->argc++] = mqtt_sn_cmd_buffer->argv_0;
  mqtt_sn_cmd_buffer->status = MQTT_SN_CLIENT_INTERACTIVE_STATUS_GATEWAY_NETWORK_DISCONNECTED;
}
void mqtt_sn_cmd_buffer_reset(mqtt_sn_interactive_client_cmd_buffer *mqtt_sn_cmd_buffer) {
  mqtt_sn_cmd_buffer_init(mqtt_sn_cmd_buffer);
}
int32_t mqtt_sn_cmd_buffer_parse_string(mqtt_sn_interactive_client_cmd_buffer *mqtt_sn_cmd_buffer) {
  mqtt_sn_cmd_buffer->argc = tokenize_command_str(mqtt_sn_cmd_buffer->buffer,
                                                  strlen(mqtt_sn_cmd_buffer->buffer) + 1,
                                                  mqtt_sn_cmd_buffer->argv_0,
                                                  mqtt_sn_cmd_buffer->argv,
                                                  MQTT_SN_COMMAND_BUFFER_MAXIMUM_ARGV_LENGTH);
  if (mqtt_sn_cmd_buffer->argc < 0) {
    return -1;
  }
  return 0;
}
uint8_t mqtt_sn_cmd_buffer_is_exit_command(const char *str) {
  if (strncmp(str, "exit", strlen("exit")) == 0) {
    return 1;
  }
  return 0;
}
uint8_t mqtt_sn_buffer_is_help(const char *str) {
  if (!strcmp(str, "h") || !strcmp(str, "help")) {
    return 1;
  }
  return 0;
}
MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE mqtt_sn_cmd_buffer_parse_command(mqtt_sn_interactive_client_cmd_buffer *c) {
  if (c->argc <= 1) {
    return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_NO_COMMAND;
  }
  if (!strcmp(c->argv[1], "l") || !strcmp(c->argv[1], "logger")) {
    return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_REINIT_LOGGER;
  } else if (!strcmp(c->argv[1], "i") || !strcmp(c->argv[1], "init")) {
    return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_INIT_CLIENT;
  } else if (!strcmp(c->argv[1], "di") || !strcmp(c->argv[1], "deinit")) {
    return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_DEINIT_CLIENT;
  } else if (!strcmp(c->argv[1], "c") || !strcmp(c->argv[1], "connect")) {
    return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_CONNECT_GATEWAY;
  } else if (!strcmp(c->argv[1], "d") || !strcmp(c->argv[1], "disconnect")) {
    return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_DISCONNECT_GATEWAY;
  } else if (!strcmp(c->argv[1], "r") || !strcmp(c->argv[1], "register")) {
    return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_REGISTER_GATEWAY;
  } else if (!strcmp(c->argv[1], "s") || !strcmp(c->argv[1], "subscribe")) {
    return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_SUBSCRIBE_GATEWAY;
  } else if (!strcmp(c->argv[1], "u") || !strcmp(c->argv[1], "unsubscribe")) {
    return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_UNSUBSCRIBE_GATEWAY;
  } else if (!strcmp(c->argv[1], "s") || !strcmp(c->argv[1], "sleep")) {
    return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_SLEEP_GATEWAY;
  } else if (!strcmp(c->argv[1], "w") || !strcmp(c->argv[1], "wakeup")) {
    return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_WAKEUP_GATEWAY;
  }
  return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_NO_COMMAND;
}
int32_t mqtt_sn_cmd_buffer_check_status(mqtt_sn_interactive_client_cmd_buffer *c,
                                        MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE tk) {
  if (tk == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_INIT_CLIENT) {
    if (c->status == MQTT_SN_CLIENT_INTERACTIVE_STATUS_GATEWAY_NETWORK_DISCONNECTED) {
      return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_RETURN_VALUE_VALID_STATE;
    }
    return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_RETURN_VALUE_INVALID_STATE;
  } else if (tk == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_DEINIT_CLIENT) {
    if (c->status == MQTT_SN_CLIENT_INTERACTIVE_STATUS_GATEWAY_NETWORK_CONNECTED) {
      return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_RETURN_VALUE_VALID_STATE;
    }
    return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_RETURN_VALUE_INVALID_STATE;
  } else if (tk == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_CONNECT_GATEWAY) {
    if (c->status == MQTT_SN_CLIENT_INTERACTIVE_STATUS_MQTT_SN_GATEWAY_DISCONNECTED) {
      return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_RETURN_VALUE_VALID_STATE;
    }
    return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_RETURN_VALUE_INVALID_STATE;
  } else if (tk == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_DISCONNECT_GATEWAY) {
    if (c->status == MQTT_SN_CLIENT_INTERACTIVE_STATUS_MQTT_SN_GATEWAY_CONNECTED) {
      return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_RETURN_VALUE_VALID_STATE;
    }
    return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_RETURN_VALUE_INVALID_STATE;
  } else if (tk == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_REGISTER_GATEWAY) {
    if (c->status == MQTT_SN_CLIENT_INTERACTIVE_STATUS_MQTT_SN_GATEWAY_CONNECTED) {
      return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_RETURN_VALUE_VALID_STATE;
    }
    return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_RETURN_VALUE_INVALID_STATE;
  } else if (tk == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_SUBSCRIBE_GATEWAY) {
    if (c->status == MQTT_SN_CLIENT_INTERACTIVE_STATUS_MQTT_SN_GATEWAY_CONNECTED) {
      return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_RETURN_VALUE_VALID_STATE;
    }
    return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_RETURN_VALUE_INVALID_STATE;
  } else if (tk == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_UNSUBSCRIBE_GATEWAY) {
    if (c->status == MQTT_SN_CLIENT_INTERACTIVE_STATUS_MQTT_SN_GATEWAY_CONNECTED) {
      return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_RETURN_VALUE_VALID_STATE;
    }
    return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_RETURN_VALUE_INVALID_STATE;
  } else if (tk == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_SLEEP_GATEWAY) {
    if (c->status == MQTT_SN_CLIENT_INTERACTIVE_STATUS_MQTT_SN_GATEWAY_CONNECTED) {
      return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_RETURN_VALUE_VALID_STATE;
    }
    return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_RETURN_VALUE_INVALID_STATE;
  } else if (tk == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_WAKEUP_GATEWAY) {
    if (c->status == MQTT_SN_CLIENT_INTERACTIVE_STATUS_MQTT_SN_GATEWAY_CONNECTED) {
      return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_RETURN_VALUE_VALID_STATE;
    }
    return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_RETURN_VALUE_INVALID_STATE;
  }
  return MQTT_SN_CLIENT_INTERACTIVE_COMMAND_RETURN_VALUE_NO_COMMAND;
}

int32_t mqtt_sn_cmd_buffer_change_status(mqtt_sn_interactive_client_cmd_buffer *cmd_buffer,
                                         MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE tk,
                                         MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE result) {

}

mqtt_sn_client_interactive_cmd_result mqtt_sn_cmd_buffer_execute_command(mqtt_sn_interactive_client *client,
                                                                         MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE tk,
                                                                         const interactive_client_config *cfg,
                                                                         MqttSnLogger *logger) {
  MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE action_result;
  uint64_t start_t_s = 0, start_t_ns = 0;
  uint64_t end_t_s = 0, end_t_ns = 0;
  if (tk == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_REINIT_LOGGER) {
    if (get_timestamp_ns(&start_t_s, &start_t_ns) < 0) {
      return mqtt_sn_client_interactive_cmd_result_error_only(tk);
    }
    action_result = MqttSnClientInteractiveReinitLogger(client, tk, cfg, logger);
    if (get_timestamp_ns(&end_t_s, &end_t_ns) < 0) {
      return mqtt_sn_client_interactive_cmd_result_error_only(tk);
    }
  }
  if (tk == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_INIT_CLIENT) {
    if (get_timestamp_ns(&start_t_s, &start_t_ns) < 0) {
      return mqtt_sn_client_interactive_cmd_result_error_only(tk);
    }
    action_result = MqttSnClientInteractiveInit(client, tk, cfg, logger);
    if (get_timestamp_ns(&end_t_s, &end_t_ns) < 0) {
      return mqtt_sn_client_interactive_cmd_result_error_only(tk);
    }
  }
  if (tk == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_DEINIT_CLIENT) {
    if (get_timestamp_ns(&start_t_s, &start_t_ns) < 0) {
      return mqtt_sn_client_interactive_cmd_result_error_only(tk);
    }
    action_result = MqttSnClientInteractiveDeinit(client, tk, cfg, logger);
    if (get_timestamp_ns(&end_t_s, &end_t_ns) < 0) {
      return mqtt_sn_client_interactive_cmd_result_error_only(tk);
    }
  }
  if (tk == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_CONNECT_GATEWAY) {
    if (cfg->cfmsgcfg.find_pattern_type == INVALID_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE) {
      return mqtt_sn_client_interactive_cmd_result_error_only(tk);
    }
    if (cfg->cfmsgcfg.find_pattern_type == SEARCH_GW_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE) {
      if (get_timestamp_ns(&start_t_s, &start_t_ns) < 0) {
        return mqtt_sn_client_interactive_cmd_result_error_only(tk);
      }
      action_result = MqttSnClientInteractiveSearchGwAndConnect(client, tk, cfg, logger);
      if (get_timestamp_ns(&end_t_s, &end_t_ns) < 0) {
        return mqtt_sn_client_interactive_cmd_result_error_only(tk);
      }
    }
    if (cfg->cfmsgcfg.find_pattern_type == ANY_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE ||
        cfg->cfmsgcfg.find_pattern_type == CONNECT_CREDENTIALS_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE) {
      if (get_timestamp_ns(&start_t_s, &start_t_ns) < 0) {
        return mqtt_sn_client_interactive_cmd_result_error_only(tk);
      }
      action_result = MqttSnClientInteractiveDirectConnect(client, tk, cfg, logger);
      if (get_timestamp_ns(&end_t_s, &end_t_ns) < 0) {
        return mqtt_sn_client_interactive_cmd_result_error_only(tk);
      }
    }
    if (cfg->cfmsgcfg.find_pattern_type == ADVERTISMENT_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE) {
      if (get_timestamp_ns(&start_t_s, &start_t_ns) < 0) {
        return mqtt_sn_client_interactive_cmd_result_error_only(tk);
      }
      action_result = MqttSnClientInteractiveAdvertiseAndDirectConnect(client, tk, cfg, logger);
      if (get_timestamp_ns(&end_t_s, &end_t_ns) < 0) {
        return mqtt_sn_client_interactive_cmd_result_error_only(tk);
      }
    }
  }

  if (tk == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_DIRECT_CONNECT_GATEWAY) {
    if (get_timestamp_ns(&start_t_s, &start_t_ns) < 0) {
      return mqtt_sn_client_interactive_cmd_result_error_only(tk);
    }
    action_result = MqttSnClientInteractiveDirectConnect(client, tk, cfg, logger);
    if (get_timestamp_ns(&end_t_s, &end_t_ns) < 0) {
      return mqtt_sn_client_interactive_cmd_result_error_only(tk);
    }
  }
  if (tk == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_SEARCHGW_AND_CONNECT_GATEWAY) {
    if (get_timestamp_ns(&start_t_s, &start_t_ns) < 0) {
      return mqtt_sn_client_interactive_cmd_result_error_only(tk);
    }
    action_result = MqttSnClientInteractiveSearchGwAndConnect(client, tk, cfg, logger);
    if (get_timestamp_ns(&end_t_s, &end_t_ns) < 0) {
      return mqtt_sn_client_interactive_cmd_result_error_only(tk);
    }
  }
  if (tk == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_ADVERTISE_AND_CONNECT_GATEWAY) {
    if (get_timestamp_ns(&start_t_s, &start_t_ns) < 0) {
      return mqtt_sn_client_interactive_cmd_result_error_only(tk);
    }
    action_result = MqttSnClientInteractiveAdvertiseAndDirectConnect(client, tk, cfg, logger);
    if (get_timestamp_ns(&end_t_s, &end_t_ns) < 0) {
      return mqtt_sn_client_interactive_cmd_result_error_only(tk);
    }
  }
  if (tk == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_DISCONNECT_GATEWAY) {
    if (get_timestamp_ns(&start_t_s, &start_t_ns) < 0) {
      return mqtt_sn_client_interactive_cmd_result_error_only(tk);
    }
    action_result = MqttSnClientInteractiveDisconnect(client, tk, cfg, logger);
    if (get_timestamp_ns(&end_t_s, &end_t_ns) < 0) {
      return mqtt_sn_client_interactive_cmd_result_error_only(tk);
    }
  }
  if (tk == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_REGISTER_GATEWAY) {
    if (get_timestamp_ns(&start_t_s, &start_t_ns) < 0) {
      return mqtt_sn_client_interactive_cmd_result_error_only(tk);
    }
    action_result = MqttSnClientInteractiveRegister(client, tk, cfg, logger);
    if (get_timestamp_ns(&end_t_s, &end_t_ns) < 0) {
      return mqtt_sn_client_interactive_cmd_result_error_only(tk);
    }
  }
  if (tk == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_SUBSCRIBE_GATEWAY) {
    if (get_timestamp_ns(&start_t_s, &start_t_ns) < 0) {
      return mqtt_sn_client_interactive_cmd_result_error_only(tk);
    }
    action_result = MqttSnClientInteractiveSubscribe(client, tk, cfg, logger);
    if (get_timestamp_ns(&end_t_s, &end_t_ns) < 0) {
      return mqtt_sn_client_interactive_cmd_result_error_only(tk);
    }
  }
  if (tk == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_UNSUBSCRIBE_GATEWAY) {
    if (get_timestamp_ns(&start_t_s, &start_t_ns) < 0) {
      return mqtt_sn_client_interactive_cmd_result_error_only(tk);
    }
    action_result = MqttSnClientInteractiveUnsubscribe(client, tk, cfg, logger);
    if (get_timestamp_ns(&end_t_s, &end_t_ns) < 0) {
      return mqtt_sn_client_interactive_cmd_result_error_only(tk);
    }
  }
  if (tk == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_GATEWAY) {
    if (get_timestamp_ns(&start_t_s, &start_t_ns) < 0) {
      return mqtt_sn_client_interactive_cmd_result_error_only(tk);
    }
    action_result = MqttSnClientInteractivePublish(client, tk, cfg, logger);
    if (get_timestamp_ns(&end_t_s, &end_t_ns) < 0) {
      return mqtt_sn_client_interactive_cmd_result_error_only(tk);
    }
  }
  if (tk == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_SLEEP_GATEWAY) {
    if (get_timestamp_ns(&start_t_s, &start_t_ns) < 0) {
      return mqtt_sn_client_interactive_cmd_result_error_only(tk);
    }
    action_result = MqttSnClientInteractiveSleep(client, tk, cfg, logger);
    if (get_timestamp_ns(&end_t_s, &end_t_ns) < 0) {
      return mqtt_sn_client_interactive_cmd_result_error_only(tk);
    }
  }
  if (tk == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_WAKEUP_GATEWAY) {
    if (get_timestamp_ns(&start_t_s, &start_t_ns) < 0) {
      return mqtt_sn_client_interactive_cmd_result_error_only(tk);
    }
    action_result = MqttSnClientInteractiveWakeup(client, tk, cfg, logger);
    if (get_timestamp_ns(&end_t_s, &end_t_ns) < 0) {
      return mqtt_sn_client_interactive_cmd_result_error_only(tk);
    }
  }

  mqtt_sn_client_interactive_cmd_result result = {0};

  char *identifier = NULL;
  if (logger->log_identifier) {
    identifier = logger->log_identifier;
  } else {
    identifier = cfg->cccfg.client_id;
  }
  mqtt_sn_client_interactive_cmd_result_init(&result,
                                             identifier,
                                             client->action_number++,
                                             tk,
                                             action_result,
                                             start_t_s,
                                             start_t_ns,
                                             end_t_s,
                                             end_t_ns);

  return result;
}

int32_t interactive_client_udp_buffer_start(const MqttSnLogger *logger,
                                            const mqtt_sn_gateway_config *msngcfg,
                                            const gateway_network_config *gncfg,
                                            MqttSnGatewayNetworkInterface *gatewayNetwork,
                                            interactive_client_udp_buffer *gatewayNetworkContext,
                                            device_address *mqtt_sn_gateway_address) {
  interactive_client_udp_buffer_init(gatewayNetworkContext);

  if (convert_hostname_port_to_device_address(msngcfg->mqtt_sn_gateway_host,
                                              msngcfg->mqtt_sn_gateway_port,
                                              &gatewayNetworkContext->mqttSnGatewayNetworkAddress,
                                              "mqtt-sn gateway")) {
    return EXIT_FAILURE;
  }
  (*mqtt_sn_gateway_address) = gatewayNetworkContext->mqttSnGatewayNetworkAddress;

  if (convert_hostname_port_to_device_address(gncfg->gateway_network_bind_address,
                                              gncfg->gateway_network_bind_port,
                                              &gatewayNetworkContext->forwarderGatewayNetworkAddress,
                                              "gateway unicast")) {
    return EXIT_FAILURE;
  }
  if (convert_string_ip_port_to_device_address(logger,
                                               gncfg->gateway_network_broadcast_address,
                                               gncfg->gateway_network_broadcast_bind_port,
                                               &gatewayNetworkContext->forwarderGatewayNetworkBroadcastAddress,
                                               "gateway broadcast")) {
    return EXIT_FAILURE;
  }

  if (GatewayNetworkInitialize(gatewayNetwork,
                               MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH,
                               mqtt_sn_gateway_address,
                               &gatewayNetworkContext->forwarderGatewayNetworkAddress,
                               &gatewayNetworkContext->forwarderGatewayNetworkBroadcastAddress,
                               &gatewayNetworkContext->udpGatewayNetworkContext,
                               GatewayLinuxUdpInitialize)) {
    log_str(logger, "Error init gateway network\n");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
void interactive_client_udp_buffer_init(interactive_client_udp_buffer *cfg) {
  memset(cfg, 0, sizeof(*cfg));
}
int32_t mqtt_sn_client_interactive_start_logger(mqtt_sn_interactive_client *client,
                                                const mqtt_sn_logger_config *cfg,
                                                MqttSnLogger *logger) {
  if (cfg->log_identifier) {
    logger->log_identifier = cfg->log_identifier;
  }

  if (!strcmp(cfg->log_target, "console")) {
    if (MqttSnLoggerInit(logger, cfg->log_lvl, stdout_log_init) < 0) {
      return -1;
    }
  } else if (!strcmp(cfg->log_target, "file")) {
    if (MqttSnLoggerInitFile(logger, cfg->log_lvl, cfg->log_file_path, file_log_init, &client->file_log_context) < 0) {
      return -1;
    }
  } else if (!strcmp(cfg->log_target, "consolefile")) {
    if (cfg->log_file_path == NULL) {
      return -1;
    }
    if (MqttSnLoggerInitFile(logger,
                             cfg->log_lvl,
                             cfg->log_file_path,
                             file_stdout_log_init,
                             &client->fileconsole_log_context) < 0) {
      return -1;
    }
  }

  return 0;
}
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE MqttSnClientInteractiveReinitLogger(mqtt_sn_interactive_client *client,
                                                                                  MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE tk,
                                                                                  const interactive_client_config *cfg,
                                                                                  MqttSnLogger *logger) {
  MqttSnLoggerDeinit(logger);
  if (mqtt_sn_client_interactive_start_logger(client, &cfg->mslcfg, logger) < 0) {
    return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
  }
  return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS;
}
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE MqttSnClientInteractiveInit(mqtt_sn_interactive_client *client,
                                                                          MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE tk,
                                                                          const interactive_client_config *cfg,
                                                                          const MqttSnLogger *logger) {
  if (client->client.gatewayNetworkContext != NULL) {
    return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
  }
#ifdef WITH_LINUX_UDP_GATEWAY_NETWORK

  if (!strcmp(cfg->gncfg.gateway_network_protocol, "udp")) {
    if (interactive_client_udp_buffer_start(logger,
                                            &cfg->msgcfg,
                                            &cfg->gncfg,
                                            &client->client.gatewayNetwork,
                                            &client->udp_network_buffer,
                                            &client->mqtt_sn_gateway_address)) {
      return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
    }
    if (MqttSnClientInit(&client->client, logger, &client->udp_network_buffer.udpGatewayNetworkContext) < 0) {
      return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
    }
    return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS;
  }
#endif
  log_str(logger, "Error init gateway network unknown protocol: ");
  log_str(logger, cfg->gncfg.gateway_network_protocol);
  log_str(logger, "\n");
  return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
}
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE MqttSnClientInteractiveDeinit(mqtt_sn_interactive_client *client,
                                                                            MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE tk,
                                                                            const interactive_client_config *cfg,
                                                                            const MqttSnLogger *logger) {
  if (client->client.gatewayNetworkContext == NULL) {
    return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
  }
  MqttSnClientDeinit(&client->client);
  GatewayNetworkDeinitialize(&client->client.gatewayNetwork, client->client.gatewayNetworkContext);
  client->client.gatewayNetworkContext = NULL;
  return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS;
}

MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE MqttSnClientInteractiveSearchGwAndConnect(mqtt_sn_interactive_client *client,
                                                                                        MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE tk,
                                                                                        const interactive_client_config *cfg,
                                                                                        const MqttSnLogger *logger) {

  MqttSnClientReceivedGwInfo received_gw_info = {0};
  MQTT_SN_CLIENT_RETURN_CODE gw_info_rc;
  if ((gw_info_rc = MqttSnClientSearchGw(&client->client,
                                         cfg->cfmsgcfg.search_gateway_wait_timeout_ms,
                                         cfg->cfmsgcfg.search_gateway_radius,
                                         &received_gw_info)) != MQTT_SN_CLIENT_RETURN_SUCCESS) {
    log_mqtt_sn_client(&client->client.logger, gw_info_rc);
    if (gw_info_rc == MQTT_SN_CLIENT_RETURN_TIMEOUT) {
      return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_TIMEOUT;
    }
    return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
  }

  if (received_gw_info.received_gw_info.gwAddLen > 0) {
    client->mqtt_sn_gateway_address = received_gw_info.from;
    int32_t addr_diff_len = sizeof(device_address) - received_gw_info.received_gw_info.gwAddLen;
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
  if ((connect_rc = MqttSnClientDirectConnect(&client->client,
                                              &client->mqtt_sn_gateway_address,
                                              cfg->cccfg.client_connect_timeout_ms,
                                              cfg->cccfg.clean_session,
                                              cfg->cccfg.client_id,
                                              cfg->cccfg.connect_duration)) != MQTT_SN_CLIENT_RETURN_SUCCESS) {
    log_mqtt_sn_client(&client->client.logger, connect_rc);
    if (connect_rc == MQTT_SN_CLIENT_RETURN_TIMEOUT) {
      return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_TIMEOUT;
    }
    return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
  }
  return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS;
}
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE MqttSnClientInteractiveDirectConnect(mqtt_sn_interactive_client *client,
                                                                                   MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE tk,
                                                                                   const interactive_client_config *cfg,
                                                                                   const MqttSnLogger *logger) {
  MQTT_SN_CLIENT_RETURN_CODE connect_rc;
  if ((connect_rc = MqttSnClientDirectConnect(&client->client,
                                              &client->mqtt_sn_gateway_address,
                                              cfg->cccfg.client_connect_timeout_ms,
                                              cfg->cccfg.clean_session,
                                              cfg->cccfg.client_id,
                                              cfg->cccfg.connect_duration)) != MQTT_SN_CLIENT_RETURN_SUCCESS) {
    log_mqtt_sn_client(&client->client.logger, connect_rc);
    if (connect_rc == MQTT_SN_CLIENT_RETURN_TIMEOUT) {
      return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_TIMEOUT;
    }
    return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
  }
  return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS;
}
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE MqttSnClientInteractiveAdvertiseAndDirectConnect(
    mqtt_sn_interactive_client *client,
    MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE tk,
    const interactive_client_config *cfg,
    const MqttSnLogger *logger) {
  MqttSnClientReceivedAdvertise received_advertise = {0};
  MQTT_SN_CLIENT_RETURN_CODE advertise_rc;
  if ((advertise_rc = MqttSnClientAwaitAdvertise(&client->client,
                                                 cfg->cfmsgcfg.advertise_wait_timeout_s,
                                                 &received_advertise)) != MQTT_SN_CLIENT_RETURN_SUCCESS) {
    log_mqtt_sn_client(&client->client.logger, advertise_rc);
    if (advertise_rc == MQTT_SN_CLIENT_RETURN_TIMEOUT) {
      return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_TIMEOUT;
    }
    return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
  }
  client->mqtt_sn_gateway_address = received_advertise.from;
  client->client.default_signal_strength = received_advertise.signal_strength;

  MQTT_SN_CLIENT_RETURN_CODE connect_rc;
  if ((connect_rc = MqttSnClientDirectConnect(&client->client,
                                              client->client.gatewayNetwork.mqtt_sn_gateway_address,
                                              cfg->cccfg.client_connect_timeout_ms,
                                              cfg->cccfg.clean_session,
                                              cfg->cccfg.client_id,
                                              cfg->cccfg.connect_duration)) != MQTT_SN_CLIENT_RETURN_SUCCESS) {
    log_mqtt_sn_client(&client->client.logger, connect_rc);
    if (connect_rc == MQTT_SN_CLIENT_RETURN_TIMEOUT) {
      return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_TIMEOUT;
    }
    return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
  }
  return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS;
}

MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE MqttSnClientInteractiveDisconnect(mqtt_sn_interactive_client *client,
                                                                                MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE tk,
                                                                                const interactive_client_config *cfg,
                                                                                const MqttSnLogger *logger) {
  MQTT_SN_CLIENT_RETURN_CODE disconnect_rc;
  if ((disconnect_rc = MqttSnClientDisconnect(&client->client)) != MQTT_SN_CLIENT_RETURN_SUCCESS) {
    log_mqtt_sn_client(&client->client.logger, disconnect_rc);
    if (disconnect_rc == MQTT_SN_CLIENT_RETURN_TIMEOUT) {
      return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_TIMEOUT;
    }
    return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
  }
  return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS;
}

MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE MqttSnClientInteractivePublish(mqtt_sn_interactive_client *client,
                                                                             MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE tk,
                                                                             const interactive_client_config *cfg,
                                                                             const MqttSnLogger *logger) {
  if (cfg->cpcfg.publish_list_len > 0) {
    return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
  }
  MQTT_SN_CLIENT_RETURN_CODE publish_rc;
  single_client_publish_config publish = cfg->cpcfg.publish_list[0];
  if (publish.topic_id_type == CLIENT_MQTT_SN_PUBLISH_TOPIC_ID_TYPE_TOPICNAME) {
    publish_rc = MqttSnClientPublishTopicName(&client->client,
                                              publish.topic_name,
                                              strlen(publish.topic_name),
                                              publish.retain,
                                              publish.qos,
                                              publish.data,
                                              publish.data_length);
  }
  if (publish.topic_id_type == CLIENT_MQTT_SN_PUBLISH_TOPIC_ID_TYPE_PREDEFINED) {
    if (publish.qos == -1) {
      publish_rc = MqttSnClientPublishPredefinedM1(&client->client,
                                                   publish.topic_id,
                                                   publish.retain,
                                                   publish.data,
                                                   publish.data_length);
    }
    if (publish.qos > 0) {
      publish_rc = MqttSnClientPublishPredefined(&client->client,
                                                 publish.topic_id,
                                                 publish.retain,
                                                 publish.qos,
                                                 publish.data,
                                                 publish.data_length);
    }
  }
  if (publish.topic_id_type == CLIENT_MQTT_SN_PUBLISH_TOPIC_ID_TYPE_SHORT) {
    publish_rc = MqttSnClientPublishTopicId(&client->client,
                                            publish.topic_id,
                                            publish.retain,
                                            publish.qos,
                                            publish.data,
                                            publish.data_length);
  }
  if (publish_rc != MQTT_SN_CLIENT_RETURN_SUCCESS) {
    log_mqtt_sn_client(&client->client.logger, publish_rc);
    if (publish_rc == MQTT_SN_CLIENT_RETURN_TIMEOUT) {
      return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_TIMEOUT;
    }
    return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
  }
  return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS;
}

MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE MqttSnClientInteractiveSubscribe(mqtt_sn_interactive_client *client,
                                                                               MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE tk,
                                                                               const interactive_client_config *cfg,
                                                                               const MqttSnLogger *logger) {
  if (cfg->cscfg.subscription_len > 0) {
    return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
  }
  MQTT_SN_CLIENT_RETURN_CODE subscribe_rc;
  client_subscription_config subscribe = cfg->cscfg.subscription_list[0];

  if (subscribe.topic_id_type == CLIENT_MQTT_SN_SUBSCRIPTION_TOPIC_ID_TYPE_PREDEFINED) {
    subscribe_rc = MqttSnClientSubscribePredefined(&client->client, subscribe.topic_id);
  }
  if (subscribe.topic_id_type == CLIENT_MQTT_SN_SUBSCRIPTION_TOPIC_ID_TYPE_SHORT) {
    subscribe_rc = MqttSnClientSubscribeShort(&client->client, subscribe.topic_id);
  }
  if (subscribe.topic_id_type == CLIENT_MQTT_SN_SUBSCRIPTION_TOPIC_ID_TYPE_TOPICNAME) {
    subscribe_rc = MqttSnClientSubscribeTopicName(&client->client, subscribe.topic_name);
  }
  if (subscribe_rc != MQTT_SN_CLIENT_RETURN_SUCCESS) {
    log_mqtt_sn_client(&client->client.logger, subscribe_rc);
    if (subscribe_rc == MQTT_SN_CLIENT_RETURN_TIMEOUT) {
      return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_TIMEOUT;
    }
    return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
  }
  return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS;
}

MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE MqttSnClientInteractiveUnsubscribe(mqtt_sn_interactive_client *client,
                                                                                 MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE tk,
                                                                                 const interactive_client_config *cfg,
                                                                                 const MqttSnLogger *logger) {
  // TODO implement me
  return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
}
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE MqttSnClientInteractiveRegister(mqtt_sn_interactive_client *client,
                                                                              MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE tk,
                                                                              const interactive_client_config *cfg,
                                                                              const MqttSnLogger *logger) {
  if (cfg->crcfg.registration_list_len > 0) {
    return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
  }
  MQTT_SN_CLIENT_RETURN_CODE register_rc;
  if ((register_rc = MqttSnClientRegister(&client->client, cfg->crcfg.registration_list[0].topic_name))
      != MQTT_SN_CLIENT_RETURN_SUCCESS) {
    log_mqtt_sn_client(&client->client.logger, register_rc);
    if (register_rc == MQTT_SN_CLIENT_RETURN_TIMEOUT) {
      return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_TIMEOUT;
    }
    return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
  }
  return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS;
}

MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE MqttSnClientInteractiveSleep(mqtt_sn_interactive_client *client,
                                                                           MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE tk,
                                                                           const interactive_client_config *cfg,
                                                                           const MqttSnLogger *logger) {
  MQTT_SN_CLIENT_RETURN_CODE sleep_rc;
  sleep_rc = MqttSnClientSleep(&client->client, cfg->scfg.sleep_duration);
  if (sleep_rc != MQTT_SN_CLIENT_RETURN_SUCCESS) {
    log_mqtt_sn_client(&client->client.logger, sleep_rc);
    if (sleep_rc == MQTT_SN_CLIENT_RETURN_TIMEOUT) {
      return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_TIMEOUT;
    }
    return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
  }
  return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS;
}
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE MqttSnClientInteractiveWakeup(mqtt_sn_interactive_client *client,
                                                                            MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE tk,
                                                                            const interactive_client_config *cfg,
                                                                            const MqttSnLogger *logger) {
  MQTT_SN_CLIENT_RETURN_CODE wakeup_rc;
  wakeup_rc = MqttSnClientWakeup(&client->client);
  if (wakeup_rc != MQTT_SN_CLIENT_RETURN_SUCCESS) {
    log_mqtt_sn_client(&client->client.logger, wakeup_rc);
    if (wakeup_rc == MQTT_SN_CLIENT_RETURN_TIMEOUT) {
      return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_TIMEOUT;
    }
    return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
  }
  return MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS;
}

void mqtt_sn_cmd_buffer_print_current_status(const MqttSnLogger *logger,
                                             const mqtt_sn_interactive_client_cmd_buffer *mqtt_sn_cmd_buffer) {
  switch (mqtt_sn_cmd_buffer->status) {
    case MQTT_SN_CLIENT_INTERACTIVE_STATUS_GATEWAY_NETWORK_DISCONNECTED:log_str(logger, ">");
      break;
    case MQTT_SN_CLIENT_INTERACTIVE_STATUS_GATEWAY_NETWORK_CONNECTED:log_str(logger, ">>");
      break;
    case MQTT_SN_CLIENT_INTERACTIVE_STATUS_MQTT_SN_GATEWAY_DISCONNECTED:log_str(logger, ">>>");
      break;
    case MQTT_SN_CLIENT_INTERACTIVE_STATUS_MQTT_SN_GATEWAY_CONNECTED:log_str(logger, ">>>>");
      break;
    default:log_str(logger, "");
      break;
  }
}
void mqtt_sn_cmd_buffer_print_usage_long(const MqttSnLogger *logger) {
  log_str(logger, PSTR(" commands use the give configuration see: --help\n"));
  log_str(logger, PSTR(" [h | help] : shows this help usage.\n"));
  log_str(logger, PSTR(" [l | logger] : reinitializes the logger.\n"));
  log_str(logger, PSTR(" [i | init] : initializes the network and client.\n"));
  log_str(logger, PSTR(" [di | deinit] : deinitializes the network and client.\n"));
  log_str(logger, PSTR(" [c | connect] : connect the client to the gateway.\n"));
  log_str(logger, PSTR(" [d | disconnect] : disconnect the client from the gateway.\n"));
  log_str(logger, PSTR(" [r | register] : registers a topic.\n"));
  log_str(logger, PSTR(" [s | subscribe] : subscribes to a topic.\n"));
  log_str(logger, PSTR(" [u | unsubscribe] : unsubscribe from a topic.\n"));
  log_str(logger, PSTR(" [s | sleep] : sets the client to sleep.\n"));
  log_str(logger, PSTR(" [w | wakeup] : wakes the client up.\n"));
  log_str(logger, PSTR(" --help : shows configuration commands see: \n"));
  log_flush(logger);
}
void mqtt_sn_interactive_client_init(mqtt_sn_interactive_client *interactive_client) {
  memset(interactive_client, 0, sizeof(*interactive_client));
}
