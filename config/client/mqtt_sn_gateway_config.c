//
// Created by SomeDude on 27.05.2019.
//

#include "mqtt_sn_gateway_config.h"
#include <string.h>
#include <stdlib.h>
#include <config/common/config_command_helper.h>
#include <platform/platform_compatibility.h>

int32_t mqtt_sn_gateway_config_init(mqtt_sn_gateway_config *msgcfg) {
  memset(msgcfg, 0, sizeof(*msgcfg));
  msgcfg->protocol_id = DEFAULT_MQTT_SN_PROTOCOL_ID;
  memcpy(msgcfg->default_mqtt_sn_gateway_host, DEFAULT_MQTT_SN_GATEWAY_HOST, sizeof(DEFAULT_MQTT_SN_GATEWAY_HOST));
  msgcfg->mqtt_sn_gateway_host = msgcfg->default_mqtt_sn_gateway_host;
  msgcfg->mqtt_sn_gateway_port = DEFAULT_MQTT_SN_GATEWAY_PORT;
  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
void mqtt_sn_gateway_config_cleanup(mqtt_sn_gateway_config *msgcfg) {
  if (msgcfg->mqtt_sn_gateway_host != msgcfg->default_mqtt_sn_gateway_host) {
    free(msgcfg->mqtt_sn_gateway_host);
  }
}

int32_t is_mqtt_sn_gateway_config_command(const char *arg, int *i) {
    if (!strcmp(arg, "-h") || !strcmp(arg, "--mqtt_sn_gateway_host")) {
      (*i)++;
      return 1;
    } else if (!strcmp(arg, "-p") || !strcmp(arg, "--mqtt_sn_gateway_port")) {
      (*i)++;
      return 1;
    } else if (!strcmp(arg, "-L") || !strcmp(arg, "--url")) {
      (*i)++;
      return 1;
    }
  return 0;
}

int32_t mqtt_sn_gateway_config_process_args(mqtt_sn_gateway_config *msgcfg,
                                            const MqttSnLogger *logger,
                                            int argc,
                                            char **argv) {
  int32_t parsed_args = 0;
  for (int i = 0; i < argc; i++) {
    if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--mqtt_sn_gateway_host")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "host");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        if (msgcfg->mqtt_sn_gateway_host != msgcfg->default_mqtt_sn_gateway_host) {
          free(msgcfg->mqtt_sn_gateway_host);
        }
        msgcfg->mqtt_sn_gateway_host = strdup(argv[i + 1]);
      }
      i++;
      parsed_args += 2;
    } else if (!strcmp(argv[i], "-p") || !strcmp(argv[i], "--mqtt_sn_gateway_port")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "port");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        if (parse_port(logger, argv[i + 1], &msgcfg->mqtt_sn_gateway_port)) {
          return MQTT_SN_PARSE_CONFIG_FAILURE;
        }
      }
      i++;
      parsed_args += 2;
    } else if (!strcmp(argv[i], "-L") || !strcmp(argv[i], "--url")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "URL");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        char *url = argv[i + 1];

        if (!strncasecmp(url, "mqttsn://", 7 + 2)) {
          url += 7;
          msgcfg->mqtt_sn_gateway_port = 8888;
        } else {
          print_unsupported_url_scheme(logger);
          return MQTT_SN_PARSE_CONFIG_FAILURE;
        }

        if (msgcfg->mqtt_sn_gateway_host != msgcfg->default_mqtt_sn_gateway_host) {
          free(msgcfg->mqtt_sn_gateway_host);
        }

        msgcfg->mqtt_sn_gateway_host = url + 2;
        char *tmp = strchr(url, ':');
        if (tmp) {
          *tmp++ = 0;
          if (parse_port(logger, tmp, &msgcfg->mqtt_sn_gateway_port)) {
            return MQTT_SN_PARSE_CONFIG_FAILURE;
          }
        }
        msgcfg->mqtt_sn_gateway_host = strdup(msgcfg->mqtt_sn_gateway_host);
      }
      i++;
      parsed_args += 2;
    }
  }
  return parsed_args;
}
void mqtt_sn_gateway_config_print_usage_short(const MqttSnLogger *logger, const char *indent) {
  if(indent) {
    log_str(logger, indent);
  }
  log_str(logger, PSTR("{[[-h mqtt_sn_gateway_host] [-p mqtt_sn_gateway_port]] | -L URL}\n"));
}
void mqtt_sn_gateway_config_print_usage_long(const MqttSnLogger *logger) {
  log_str(logger, PSTR(" -h : mqtt-sn gateway host to connect to. Defaults to "));
  log_str(logger, DEFAULT_MQTT_SN_GATEWAY_HOST);
  log_str(logger, PSTR(".\n"));

  log_str(logger, PSTR(" -p : mqtt-sn gateway network port to connect to. Defaults to "));
  log_int32(logger, DEFAULT_MQTT_SN_GATEWAY_PORT);
  log_str(logger, PSTR(" for plain MQTT-SN.\n"));

  log_str(logger, PSTR(" -L : specify hostname, port as a URL in the form:\n"));
  log_str(logger, PSTR("      mqttsn(s)://host[:port]\n"));
  log_str(logger, PSTR("      mqttsn(s) uses the same protocol as the gateway network.\n"));
}

