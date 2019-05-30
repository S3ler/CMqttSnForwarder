//
// Created by SomeDude on 28.05.2019.
//

#ifndef CMQTTSNFORWARDER_CONFIG_MQTT_SN_VERSION_CONFIG_H_
#define CMQTTSNFORWARDER_CONFIG_MQTT_SN_VERSION_CONFIG_H_

#include <stdint.h>
#include <logging/MqttSnLoggingInterface.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef VERSION
#define VERSION "N/D"
#endif
#ifndef MAJOR
#define MAJOR 0
#endif
#ifndef MINOR
#define MINOR 0
#endif
#ifndef TWEAK
#define TWEAK 0
#endif
#ifndef CMAKE_BUILD_TIMESTAMP
#define CMAKE_BUILD_TIMESTAMP "N/D"
#endif

typedef struct mqtt_sn_version_config_ {
  char version[sizeof(VERSION)];
  int32_t major;
  int32_t minor;
  uint64_t tweak;
  char build_date[sizeof(CMAKE_BUILD_TIMESTAMP)+1];
} mqtt_sn_version_config;

int32_t mqtt_sn_version_config_init(mqtt_sn_version_config* cfg);
void mqtt_sn_version_config_cleanup(mqtt_sn_version_config* cfg);
void mqtt_sn_version_config_print_usage_short(const MqttSnLogger *logger);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_CONFIG_MQTT_SN_VERSION_CONFIG_H_
