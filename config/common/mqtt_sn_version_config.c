//
// Created by SomeDude on 28.05.2019.
//

#include "mqtt_sn_version_config.h"
#include <platform/platform_compatibility.h>

int32_t mqtt_sn_version_config_init(mqtt_sn_version_config *cfg) {
  memset(cfg, 0, sizeof(*cfg));
  memcpy(cfg->version, VERSION, sizeof(VERSION));
  cfg->major = MAJOR;
  cfg->minor = MINOR;
  cfg->tweak = TWEAK;
  memcpy(cfg->build_date, CMAKE_BUILD_TIMESTAMP, sizeof(CMAKE_BUILD_TIMESTAMP));
  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
void mqtt_sn_version_config_cleanup(mqtt_sn_version_config *cfg) {
  // nothing to do here
}
void mqtt_sn_version_config_print_usage_short(const MqttSnLogger *logger) {
  log_str(logger, PSTR("version "));
  log_str(logger, VERSION);
  log_str(logger, PSTR(" is "));
  log_uint64(logger, MAJOR);
  log_str(logger, PSTR("."));
  log_uint64(logger, MINOR);
  log_str(logger, PSTR("."));
  log_uint64(logger, TWEAK);
  log_str(logger, PSTR(" (build date "));
  log_str(logger, CMAKE_BUILD_TIMESTAMP);
  log_str(logger, PSTR(")."));
  log_flush(logger);
}


