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
  cfg->advertise_wait_timeout_s = CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_DEFAULT_ADVERTISEMENT_WAIT_TIMEOUT;

  cfg->search_gateway_wait_timeout_ms = CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_DEFAULT_SEARCH_GW_WAIT_TIMEOUT;
  cfg->search_gateway_radius = CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_DEFAULT_SEARCH_GW_RADIUS;

#ifdef WITH_FIND_MQTT_SN_GATEWAY_CONFIG_REGEX_FILTER
  cfg->gw_id_regex_filter = NULL;
  cfg->gw_add_regex_filter = NULL;
#endif
#ifdef WITH_FIND_MQTT_SN_GATEWAY_CONFIG_WHITELIST_FILTER
  cfg->gw_id_whitelist_max_len = CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_GW_ID_WHITELIST_LENGTH;
  cfg->gw_id_whitelist_len = 0;

  cfg->gw_add_whitelist_max_len = CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_GW_ADD_WHITELIST_LENGTH;
  cfg->gw_add_whitelist_len = 0;
#endif
#ifdef WITH_FIND_MQTT_SN_GATEWAY_CONFIG_BLACKLIST_FILTER
  // FEATURE implement blacklist filter configuration
#endif

  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}

void client_find_mqtt_sn_gateway_config_cleanup(client_find_mqtt_sn_gateway_config *cfg) {
  // nothing to do yet
}

int32_t is_client_find_mqtt_sn_gateway_config_command(const char *arg, int *i) {
  if (!strcmp(arg, "-gfp") || !strcmp(arg, "--gateway_find_pattern")) {
    (*i)++;
    return 1;
  } else if (!strcmp(arg, "-awt") || !strcmp(arg, "--advertise_wait_timeout")) {
    (*i)++;
    return 1;
  } else if (!strcmp(arg, "-sgt") || !strcmp(arg, "--search_gateway_timeout")) {
    (*i)++;
    return 1;
  } else if (!strcmp(arg, "-sgr") || !strcmp(arg, "--search_gateway_radius")) {
    (*i)++;
    return 1;
  }
#ifdef WITH_FIND_MQTT_SN_GATEWAY_CONFIG_REGEX_FILTER
  else if (!strcmp(arg, "--gw_id_filter")) {
    (*i)++;
    return 1;
  } else if (!strcmp(arg, "--gw_add_filter")) {
    (*i)++;
    return 1;
  }
#endif
#ifdef WITH_FIND_MQTT_SN_GATEWAY_CONFIG_WHITELIST_FILTER
  else if (!strcmp(arg, "--gw_id_whitelist")) {
    (*i)++;
    return 1;
  } else if (!strcmp(arg, "--gw_add_whitelist")) {
    (*i)++;
    return 1;
  }
#endif
#ifdef WITH_FIND_MQTT_SN_GATEWAY_CONFIG_BLACKLIST_FILTER
  else if (!strcmp(arg, "--gw_id__blacklist")) {
    (*i)++;
    return 1;
  } else if (!strcmp(arg, "--gw_add_blacklist")) {
    (*i)++;
    return 1;
  }
#endif

  return 0;
}
int32_t client_find_mqtt_sn_gateway_config_process_args(client_find_mqtt_sn_gateway_config *cfg,
                                                        const MqttSnLogger *logger,
                                                        int argc,
                                                        char **argv) {
  int32_t parsed_args = 0;
  for (int i = 0; i < argc; i++) {
    if (!strcmp(argv[i], "-gfp") || !strcmp(argv[i], "--gateway_find_pattern")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "find pattern");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        if (!strcmp(argv[i + 1], "any")) {
          cfg->find_pattern_type = ANY_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE;
        } else if (!strcmp(argv[i + 1], "advertise")) {
          cfg->find_pattern_type = ADVERTISMENT_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE;
        } else if (!strcmp(argv[i + 1], "search")) {
          cfg->find_pattern_type = SEARCH_GW_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE;
        } else if (!strcmp(argv[i + 1], "connect")) {
          cfg->find_pattern_type = CONNECT_CREDENTIALS_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE;
        } else {
          print_argument_value_not_specified(logger, argv[i], "valid pattern type");
          return MQTT_SN_PARSE_CONFIG_FAILURE;
        }
      }
      i++;
      parsed_args += 2;
    } else if (!strcmp(argv[i], "-awt") || !strcmp(argv[i], "--advertise_wait_timeout")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "advertise wait timeout");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        parse_timeout(logger, argv[i + 1], &cfg->advertise_wait_timeout_s);
      }
      i++;
      parsed_args += 2;
    } else if (!strcmp(argv[i], "-sgt") || !strcmp(argv[i], "--search_gateway_timeout")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "search gateway timeout");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        parse_timeout(logger, argv[i + 1], &cfg->search_gateway_wait_timeout_ms);
      }
      i++;
      parsed_args += 2;
    } else if (!strcmp(argv[i], "-sgr") || !strcmp(argv[i], "--search_gateway_radius")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "search gateway radius");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        parse_radius(logger, argv[i + 1], &cfg->search_gateway_radius);
      }
      i++;
      parsed_args += 2;
    }
#ifdef WITH_FIND_MQTT_SN_GATEWAY_CONFIG_REGEX_FILTER
    else if (!strcmp(argv[i], "--gw_id_filter")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "gw_id filter");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        cfg->gw_id_regex_filter = argv[i + 1];
      }
      i++;
      parsed_args += 2;
    } else if (!strcmp(argv[i], "--gw_add_filter")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "gw_add filter");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        cfg->gw_add_regex_filter = argv[i + 1];
      }
      i++;
      parsed_args += 2;
    }
#endif
#ifdef WITH_FIND_MQTT_SN_GATEWAY_CONFIG_WHITELIST_FILTER
    else if (!strcmp(argv[i], "--gw_id_whitelist")) {
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
      parsed_args += 2;
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
      parsed_args += 2;
    }
#endif
#ifdef WITH_FIND_MQTT_SN_GATEWAY_CONFIG_BLACKLIST_FILTER
    else if (!strcmp(argv[i], "--gw_id_blacklist")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "gw_id blacklist");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        if (cfg->gw_id_blacklist_len < cfg->gw_id_blacklist_max_len) {
          cfg->gw_id_blacklist[cfg->gw_id_blacklist_len++] = argv[i + 1];
        } else {
          print_config_list_full(logger, argv[i], cfg->gw_id_blacklist_max_len);
          return MQTT_SN_PARSE_CONFIG_FAILURE;
        }
      }
      i++;
      parsed_args += 2;
    }
    else if (!strcmp(argv[i], "--gw_add_blacklist")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "gw_add blacklist");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        if (cfg->gw_add_blacklist_len < cfg->gw_add_blacklist_max_len) {
          cfg->gw_add_blacklist[cfg->gw_add_blacklist_len++] = argv[i + 1];
        } else {
          print_config_list_full(logger, argv[i], cfg->gw_add_blacklist_max_len);
          return MQTT_SN_PARSE_CONFIG_FAILURE;
        }
      }
      i++;
      parsed_args += 2;
    }
#endif
  }
  return parsed_args;
}
void client_find_mqtt_sn_gateway_config_print_usage_short(const MqttSnLogger *logger, const char *indent) {
  if (indent) {
    log_str(logger, indent);
  }
  log_str(logger, PSTR("[-gfp --gateway_find_pattern] [-awt --advertise_wait_timeout] "
                       "[-sgr --search_gateway_radius]\n"));
  if (indent) {
    log_str(logger, indent);
  }
#ifdef WITH_FIND_MQTT_SN_GATEWAY_CONFIG_REGEX_FILTER
  log_str(logger, PSTR("[--gw_id_filter] [--gw_add_filter] "));
#endif
  log_str(logger, PSTR("[--gw_id_whitelist] [--gw_add_whitelist] [--gw_id_blacklist] [--gw_add_blacklist]\n"));
}
void client_find_mqtt_sn_gateway_config_print_usage_long(const MqttSnLogger *logger) {
  log_str(logger, PSTR(" -gfp : specify the mqtt-sn gateway find pattern.\n"));
  log_str(logger, PSTR("        Can be any, advertise, search, connect. Default is "));
  if (CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_DEFAULT_FIND_PATTERN == ADVERTISMENT_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE) {
    log_str(logger, PSTR("advertise"));
  } else if (CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_DEFAULT_FIND_PATTERN == SEARCH_GW_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE) {
    log_str(logger, PSTR("search"));
  } else if (CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_DEFAULT_FIND_PATTERN
      == CONNECT_CREDENTIALS_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE) {
    log_str(logger, PSTR("connect"));
  } else if (CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_DEFAULT_FIND_PATTERN == ANY_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE) {
    log_str(logger, PSTR("any"));
  } else {
    log_str(logger, PSTR("undefined"));
  }
  log_str(logger, PSTR(".\n"));
  log_str(logger, PSTR(" -awt : specify advertise wait timeout in s. Default is "));
  log_uint64(logger, CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_DEFAULT_ADVERTISEMENT_WAIT_TIMEOUT);
  log_str(logger, PSTR(" s.\n"));
  log_str(logger, PSTR(" -sgt : specify SearchGw timeout in s. Default is "));
  log_uint64(logger, CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_DEFAULT_SEARCH_GW_WAIT_TIMEOUT);
  log_str(logger, PSTR(" s.\n"));
  log_str(logger, PSTR(" -sgr : specify radius of SearchGw message between 0 and 255. Default is "));
  log_uint64(logger, CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_DEFAULT_SEARCH_GW_RADIUS);
  log_str(logger, PSTR(".\n"));
#ifdef WITH_FIND_MQTT_SN_GATEWAY_CONFIG_REGEX_FILTER
  log_str(logger, PSTR(" --gw_id_filter : specify regex for filtering mqtt-sn gateway id.\n"));
  log_str(logger, PSTR(" --gw_add_filter : specify regex for mqtt-sn gateway address filter.\n"));
#endif
#ifdef WITH_FIND_MQTT_SN_GATEWAY_CONFIG_WHITELIST_FILTER
  log_str(logger, PSTR(" --gw_id_whitelist : specify whitelist for mqtt-sn gateway ids. Can be used repeatedly."));
  log_str(logger, PSTR(" Maximum entry count is "));
  log_uint64(logger, CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_GW_ID_WHITELIST_LENGTH);
  log_str(logger, PSTR(".\n"));
  log_str(logger, PSTR(" --gw_add_whitelist : specify whitelist for mqtt-sn gateway addresses."));
  log_str(logger, PSTR(" Can be used repeatedly. Maximum entry count is "));
  log_uint64(logger, CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_GW_ADD_WHITELIST_LENGTH);
  log_str(logger, PSTR(".\n"));
#endif
#ifdef WITH_FIND_MQTT_SN_GATEWAY_CONFIG_WHITELIST_FILTER
  log_str(logger, PSTR(" --gw_id_blacklist : specify blaclist for mqtt-sn gateway ids. Can be used repeatedly."));
  log_str(logger, PSTR(" Maximum entry count is "));
  log_uint64(logger, CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_GW_ID_BLACKLIST_LENGTH);
  log_str(logger, PSTR(".\n"));
  log_str(logger, PSTR(" --gw_add_blacklist : specify blacklist for mqtt-sn gateway addresses."));
  log_str(logger, PSTR(" Can be used repeatedly. Maximum entry count is "));
  log_uint64(logger, CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_GW_ADD_BLACKLIST_LENGTH);
  log_str(logger, PSTR(".\n"));
#endif
}

