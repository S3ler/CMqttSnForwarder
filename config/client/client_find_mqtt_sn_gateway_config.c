//
// Created by SomeDude on 28.05.2019.
//

#include "client_find_mqtt_sn_gateway_config.h"
#include "config/common/config_command_helper.h"
#include <string.h>
#include <platform/platform_compatibility.h>
#include <stdlib.h>

/*
#define MQTT_SN_MESSAGE_TYPE_STRING_NEGATIVE_ENUMS_OFFSET 2
static const char *FIND_MQTT_SN_GATEWAY_PATTERN_TYPE_STRING[] = {
    FOREACH_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE(GENERATE_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE_STRING)
};
*/

int32_t client_find_mqtt_sn_gateway_config_init(client_find_mqtt_sn_gateway_config *cfg) {
  memset(cfg, 0, sizeof(*cfg));

  cfg->find_pattern_type = CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_DEFAULT_FIND_PATTERN;
  cfg->advertisement_wait_duration = CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_DEFAULT_ADVERTISEMENT_WAIT_DURATION;
  cfg->search_gw_radius = CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_DEFAULT_SEARCH_GW_RADIUS;

  cfg->gw_id_regex_filter = NULL;
  cfg->gw_add_regex_filter = NULL;

  cfg->gw_id_whitelist_max_len = CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_GW_ID_WHITELIST_LENGTH;
  cfg->gw_id_whitelist_len = 0;

  cfg->gw_add_whitelist_max_len = CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_GW_ADD_WHITELIST_LENGTH;
  cfg->gw_add_whitelist_len = 0;

  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}

void client_find_mqtt_sn_gateway_config_cleanup(client_find_mqtt_sn_gateway_config *cfg) {
  if (cfg->gw_id_regex_filter) {
    //free(cfg->gw_id_regex_filter);
  }
  if (cfg->gw_add_regex_filter) {
    //free(cfg->gw_add_regex_filter);
  }

  if (cfg->gw_id_whitelist_len > 0) {
    for (uint16_t i = 0; i < cfg->gw_id_whitelist_len; i++) {
      //free(cfg->gw_id_whitelist[i]);
    }
  }

  if (cfg->gw_add_whitelist_len > 0) {
    for (uint16_t i = 0; i < cfg->gw_add_whitelist_len; i++) {
      //free(cfg->gw_add_whitelist[i]);
    }
  }
}
int32_t is_client_find_mqtt_sn_gateway_config_command(const char *arg, int *i) {
  if (!strcmp(arg, "-gfp") || !strcmp(arg, "--gateway_find_pattern")) {
    (*i)++;
    return 1;
  } else if (!strcmp(arg, "-awd") || !strcmp(arg, "--advertisement_wait_duration")) {
    (*i)++;
    return 1;
  } else if (!strcmp(arg, "--gw_id_filter")) {
    (*i)++;
    return 1;
  } else if (!strcmp(arg, "--gw_add_filter")) {
    (*i)++;
    return 1;
  } else if (!strcmp(arg, "--gw_id_whitelist")) {
    (*i)++;
    return 1;
  } else if (!strcmp(arg, "--gw_add_whitelist")) {
    (*i)++;
    return 1;
  }
  return 0;
}
int32_t client_find_mqtt_sn_gateway_config_process_args(client_find_mqtt_sn_gateway_config *cfg,
                                                        const MqttSnLogger *logger,
                                                        int argc,
                                                        char **argv) {
  for (int i = 0; i < argc; i++) {
    if (!strcmp(argv[i], "-gfp") || !strcmp(argv[i], "--gateway_find_pattern")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "find pattern");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        if (strcmp(argv[i + 1], "any")) {
          cfg->find_pattern_type = ANY_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE;
        } else if (strcmp(argv[i + 1], "advertisement")) {
          cfg->find_pattern_type = ADVERTISMENT_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE;
        } else if (strcmp(argv[i + 1], "search")) {
          cfg->find_pattern_type = SEARCH_GW_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE;
        } else if (strcmp(argv[i + 1], "connect")) {
          cfg->find_pattern_type = CONNECT_CREDENTIALS_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE;
        } else {
          print_argument_value_not_specified(logger, argv[i], "valid pattern type");
          return MQTT_SN_PARSE_CONFIG_FAILURE;
        }
      }
      i++;
    } else if (!strcmp(argv[i], "-awd") || !strcmp(argv[i], "--advertisement_wait_duration")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "advertisement wait duration");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        parse_timeout(logger, argv[i + 1], &cfg->advertisement_wait_duration);
      }
      i++;
    } else if (!strcmp(argv[i], "--gw_id_filter")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "gw_id filter");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        cfg->gw_id_regex_filter = argv[i + 1];
      }
      i++;
    } else if (!strcmp(argv[i], "--gw_add_filter")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "gw_add filter");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        cfg->gw_add_regex_filter = argv[i + 1];
      }
      i++;
    } else if (!strcmp(argv[i], "--gw_id_whitelist")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "gw_id whitelist");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        if (cfg->gw_id_whitelist_len < cfg->gw_id_whitelist_max_len) {
          cfg->gw_id_whitelist[cfg->gw_id_whitelist_len++] = argv[i + 1];
        } else {
          print_config_list_full(logger, argv[i], cfg->gw_id_whitelist_max_len);
          return MQTT_SN_PARSE_CONFIG_FAILURE;
        }
      }
      i++;
    } else if (!strcmp(argv[i], "--gw_add_whitelist")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "gw_add whitelist");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        if (cfg->gw_add_whitelist_len < cfg->gw_add_whitelist_max_len) {
          cfg->gw_add_whitelist[cfg->gw_add_whitelist_len++] = argv[i + 1];
        } else {
          print_config_list_full(logger, argv[i], cfg->gw_add_whitelist_max_len);
          return MQTT_SN_PARSE_CONFIG_FAILURE;
        }
      }
      i++;
    }
  }
  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
void client_find_mqtt_sn_gateway_config_print_usage_short(const MqttSnLogger *logger, const char *indent) {
  if (indent) {
    log_str(logger, indent);
  }
  log_str(logger, PSTR("[-gfp --gateway_find_pattern] [-awd --advertisement_wait_duration]\n"));
  if (indent) {
    log_str(logger, indent);
  }
  log_str(logger, PSTR("[--gw_id_filter] [--gw_add_filter] [--gw_id_whitelist] [--gw_add_whitelist]\n"));
}
void client_find_mqtt_sn_gateway_config_print_usage_long(const MqttSnLogger *logger) {
  log_str(logger, PSTR(" -gfp : specify the mqtt-sn gateway find pattern.\n"));
  log_str(logger, PSTR("        Can be any, advertisement, search, connect. Default is connect.\n"));
  log_str(logger, PSTR(" -awd : specify advertisement wait duration in s. Default is "));
  log_uint64(logger, CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_DEFAULT_ADVERTISEMENT_WAIT_DURATION);
  log_str(logger, PSTR(" s.\n"));
  log_str(logger, PSTR(" --gw_id_filter : specify regex for filtering mqtt-sn gateway id.\n"));
  log_str(logger, PSTR(" --gw_add_filter : specify regex for mqtt-sn gateway address filter.\n"));
  log_str(logger, PSTR(" --gw_id_whitelist : specify whitelist for mqtt-sn gateway ids. Can be used repeatedly."));
  log_str(logger, PSTR(" Maximum entry count is "));
  log_uint64(logger, CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_GW_ID_WHITELIST_LENGTH);
  log_str(logger, PSTR(".\n"));
  log_str(logger, PSTR(" --gw_add_whitelist : specify whitelist for mqtt-sn gateway addresses."));
  log_str(logger, PSTR(" Can be used repeatedly. Maximum entry count is "));
  log_uint64(logger, CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_GW_ADD_WHITELIST_LENGTH);
  log_str(logger, PSTR(".\n"));
}

