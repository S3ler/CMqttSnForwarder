//
// Created by SomeDude on 30.05.2019.
//

#include <platform/platform_compatibility.h>
#include "gateway_advertisment_config.h"
#include "config/common/config_command_helper.h"

int32_t gateway_advertisement_config_init(gateway_advertise_config *cfg) {
  memset(cfg, 0, sizeof(*cfg));

  cfg->advertisement_duration = MQTT_SN_GATEWAY_ADVERTISEMENT_DEFAULT_DURATION;
  cfg->advertisement_radius = MQTT_SN_GATEWAY_ADVERTISEMENT_DEFAULT_RADIUS;
  cfg->gateway_id = MQTT_SN_GATEWAY_ADVERTISEMENT_DEFAULT_GW_ID;
  cfg->missing_advertise_count = MQTT_SN_GATEWAY_ADVERTISEMENT_MISSING_ADVERTISE_COUNT;
  cfg->gateway_ping_monitoring = MQTT_SN_GATEWAY_ADVERTISEMENT_GATEWAY_PING_MONITORING;
  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
void gateway_advertisement_config_cleanup(gateway_advertise_config *cfg) {
  // nothing to do here
}
int32_t is_gateway_advertisement_config_command(const char *arg, int *i) {
  if (!strcmp(arg, "-ad") || !strcmp(arg, "--advertisement_duration")) {
    (*i)++;
    return 1;
  } else if (!strcmp(arg, "-ar") || !strcmp(arg, "--advertisement_radius")) {
    (*i)++;
    return 1;
  } else if (!strcmp(arg, "-gi") || !strcmp(arg, "--gateway_id")) {
    (*i)++;
    return 1;
  }
  return 0;
}
int32_t gateway_advertisement_config_process_args(gateway_advertise_config *cfg,
                                                  const MqttSnLogger *logger,
                                                  int argc,
                                                  char **argv) {
  int32_t parsed_args = 0;
  for (int i = 0; i < argc; i++) {
    if (!strcmp(argv[i], "-ad") || !strcmp(argv[i], "--advertisement_duration")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "advertisement duration");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        parse_duration(logger, argv[i + 1], &cfg->advertisement_duration);
      }
      i++;
      parsed_args += 2;
    } else if (!strcmp(argv[i], "-ar") || !strcmp(argv[i], "--advertisement_radius")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "advertisement radius");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        parse_radius(logger, argv[i + 1], &cfg->advertisement_radius);
      }
      i++;
      parsed_args += 2;
    } else if (!strcmp(argv[i], "-gi") || !strcmp(argv[i], "--gateway_id")) {
      if (i == argc - 1) {
        print_argument_value_not_specified(logger, argv[i], "gateway id");
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      } else {
        parse_gateway_id(logger, argv[i + 1], &cfg->gateway_id);
      }
      i++;
      parsed_args += 2;
    }
  }
  return parsed_args;
}
void gateway_advertisement_config_print_usage_short(const MqttSnLogger *logger, const char *indent) {
  if (indent) {
    log_str(logger, indent);
  }
  log_str(logger, PSTR("[-ad --advertisement_duration] [-ar --advertisement_radius] [-gi --gateway_id]\n"));
}
void gateway_advertisement_config_print_usage_long(const MqttSnLogger *logger) {
  log_str(logger, PSTR(" -ad : specify the advertisement duration in s. Defaults to "));
  log_int32(logger, MQTT_SN_GATEWAY_ADVERTISEMENT_DEFAULT_DURATION);
  log_str(logger, PSTR(" s.\n"));

  log_str(logger, PSTR(" -ar : specify the advertisement radius between 0 and 255. Defaults to "));
  log_uint8(logger, MQTT_SN_GATEWAY_ADVERTISEMENT_DEFAULT_RADIUS);
  log_str(logger, PSTR(".\n"));

  log_str(logger, PSTR(" -gi : specify the gateway id (GwId) between 0 and 255. Defaults to "));
  log_uint8(logger, MQTT_SN_GATEWAY_ADVERTISEMENT_DEFAULT_GW_ID);
  log_str(logger, PSTR(".\n"));
}
