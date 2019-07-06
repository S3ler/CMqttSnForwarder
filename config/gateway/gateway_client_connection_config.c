//
// Created by SomeDude on 01.07.2019.
//

#include "gateway_client_connection_config.h"
#include <string.h>
#include <platform/platform_compatibility.h>
#include <config/common/config_command_helper.h>
int32_t gateway_client_connection_config_init(gateway_client_connection_config *cfg) {
  memset(cfg, 0, sizeof(*cfg));

  cfg->client_connection_timeout_check_enabled = MQTT_SN_GATEWAY_CLIENT_CONNECTION_TIMEOUT_DEFAULT_CHECK_ENABLED;
  cfg->client_connection_timeout_check_period = MQTT_SN_GATEWAY_CLIENT_CONNECTION_TIMEOUT_DEFAULT_CHECK_PERIOD;
  cfg->client_connection_timeout_offset = MQTT_SN_GATEWAY_CLIENT_CONNECTION_TIMEOUT_DEFAULT_OFFSET;

  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
void gateway_client_connection_config_cleanup(gateway_client_connection_config *cfg) {
  // nothing to do here
}
int32_t is_gateway_client_connection_config_command(const char *arg, int *i) {
  if (!strcmp(arg, "-cctce") || !strcmp(arg, "--client_connection_timeout_check_enabled")) {
    (*i)++;
    return 1;
  } else if (!strcmp(arg, "-cctcp") || !strcmp(arg, "--client_connection_timeout_check_period")) {
    (*i)++;
    return 1;
  } else if (!strcmp(arg, "-ccto") || !strcmp(arg, "--client_connection_timeout_offset")) {
    (*i)++;
    return 1;
  }
  return 0;
}
int32_t gateway_client_connection_config_process_args(gateway_client_connection_config *cfg,
                                                      const MqttSnLogger *logger,
                                                      int argc,
                                                      char **argv) {
  int32_t parsed_args = 0;
  for (int i = 0; i < argc; i++) {
    if (!strcmp(argv[i], "-cctce") || !strcmp(argv[i], "--client_connection_timeout_check_enabled")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "client connection timeout check enabled");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        if (parse_client_connection_timeout_enabled(logger, argv[i + 1], &cfg->client_connection_timeout_check_enabled)
            == MQTT_SN_PARSE_CONFIG_FAILURE) {
          return MQTT_SN_PARSE_CONFIG_FAILURE;
        }
      }
      i++;
      parsed_args += 2;
    } else if (!strcmp(argv[i], "-cctcp") || !strcmp(argv[i], "--client_connection_timeout_check_period")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "client connection timeout check period");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        if (parse_client_connection_timeout_period(logger, argv[i + 1], &cfg->client_connection_timeout_check_period)
            == MQTT_SN_PARSE_CONFIG_FAILURE) {
          return MQTT_SN_PARSE_CONFIG_FAILURE;
        }
      }
      i++;
      parsed_args += 2;
    } else if (!strcmp(argv[i], "-ccto") || !strcmp(argv[i], "--client_connection_timeout_offset")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "client connection timeout offset");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        if (parse_timeout_offset(logger, argv[i + 1], &cfg->client_connection_timeout_offset)
            == MQTT_SN_PARSE_CONFIG_FAILURE) {
          return MQTT_SN_PARSE_CONFIG_FAILURE;
        }
      }
      i++;
      parsed_args += 2;
    }
  }
  return parsed_args;

}

void gateway_client_connection_config_print_usage_short(const MqttSnLogger *logger, const char *indent) {
  if (indent) {
    log_str(logger, indent);
  }
  log_str(logger,
          PSTR(
              "[-cctce --client_connection_timeout_check_enabled] [-cctcp --client_connection_timeout_check_period] [-ccto --client_connection_timeout_offset]\n"));
}
void gateway_client_connection_config_print_usage_long(const MqttSnLogger *logger) {
  log_str(logger,
          PSTR(" -cctce : enables the client connection timeout check. Can be true or false. Default is true.\n"));
  log_str(logger,
          PSTR(
              " -cctcp : specify the client connection timeout check period in s. Can be 0 to UINT32_MAX. Default is 20.\n"));
  log_str(logger,
          PSTR(
              " -ccto : specify the client connection timeout offset in s. Can be INT32_MIN to INT32_MAX. Default is -20.\n"));
}
