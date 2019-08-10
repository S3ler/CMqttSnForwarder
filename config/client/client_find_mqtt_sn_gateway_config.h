//
// Created by SomeDude on 28.05.2019.
//

#ifndef CMQTTSNFORWARDER_CONFIG_CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_H_
#define CMQTTSNFORWARDER_CONFIG_CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_H_

#include <stdint.h>
#include <logging/MqttSnLoggingInterface.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FOREACH_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE(FIND_MQTT_SN_GATEWAY_PATTERN_TYPE) \
        FIND_MQTT_SN_GATEWAY_PATTERN_TYPE(INVALID_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE , -1)   \
        FIND_MQTT_SN_GATEWAY_PATTERN_TYPE(ANY_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE , 0x00)   \
        FIND_MQTT_SN_GATEWAY_PATTERN_TYPE(ADVERTISMENT_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE, 0x01)   \
        FIND_MQTT_SN_GATEWAY_PATTERN_TYPE(SEARCH_GW_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE, 0x02)   \
        FIND_MQTT_SN_GATEWAY_PATTERN_TYPE(CONNECT_CREDENTIALS_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE, 0x03)   \

#define GENERATE_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE_ENUM(FIND_MQTT_SN_GATEWAY_PATTERN_TYPE, CODE) FIND_MQTT_SN_GATEWAY_PATTERN_TYPE = CODE,
#define GENERATE_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE_STRING(STRING, NUMBER) #STRING,

typedef enum CONFIG_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE_ {
  FOREACH_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE(GENERATE_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE_ENUM)
} CONFIG_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE;

#ifndef CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_DEFAULT_ADVERTISEMENT_WAIT_TIMEOUT
#define CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_DEFAULT_ADVERTISEMENT_WAIT_TIMEOUT 60
#endif

#ifndef CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_DEFAULT_FIND_PATTERN
#define CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_DEFAULT_FIND_PATTERN ANY_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE
#endif

#ifndef CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_DEFAULT_SEARCH_GW_WAIT_TIMEOUT
#define CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_DEFAULT_SEARCH_GW_WAIT_TIMEOUT 60
#endif

#ifndef CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_DEFAULT_SEARCH_GW_RADIUS
#define CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_DEFAULT_SEARCH_GW_RADIUS 1
#endif

#ifndef CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_GW_ID_WHITELIST_LENGTH
#define CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_GW_ID_WHITELIST_LENGTH 5
#endif

#ifndef CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_GW_ADD_WHITELIST_LENGTH
#define CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_GW_ADD_WHITELIST_LENGTH 5
#endif

#ifndef CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_GW_ID_BLACKLIST_LENGTH
#define CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_GW_ID_BLACKLIST_LENGTH 5
#endif

#ifndef CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_GW_ADD_BLACKLIST_LENGTH
#define CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_GW_ADD_BLACKLIST_LENGTH 5
#endif

typedef struct client_find_mqtt_sn_gateway_config_ {
  CONFIG_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE find_pattern_type;
  int32_t advertise_wait_timeout_s;

  int32_t search_gateway_wait_timeout_ms;
  uint8_t search_gateway_radius;

#ifdef WITH_FIND_MQTT_SN_GATEWAY_CONFIG_REGEX_FILTER
  char *gw_id_regex_filter;
  char *gw_add_regex_filter;
#endif
#ifdef WITH_FIND_MQTT_SN_GATEWAY_CONFIG_WHITELIST_FILTER
  uint16_t gw_id_whitelist_max_len;
  char *gw_id_whitelist[CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_GW_ID_WHITELIST_LENGTH];
  uint16_t gw_id_whitelist_len;

  uint16_t gw_add_whitelist_max_len;
  char *gw_add_whitelist[CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_GW_ADD_WHITELIST_LENGTH];
  uint16_t gw_add_whitelist_len;
#endif
#ifdef WITH_FIND_MQTT_SN_GATEWAY_CONFIG_BLACKLIST_FILTER
  uint16_t gw_id_blacklist_max_len;
  char *gw_id_blacklist[CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_GW_ID_BLACKLIST_LENGTH];
  uint16_t gw_id_blacklist_len;

  uint16_t gw_add_blacklist_max_len;
  char *gw_add_blacklist[CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_GW_ADD_BLACKLIST_LENGTH];
  uint16_t gw_add_blacklist_len;
#endif
} client_find_mqtt_sn_gateway_config;

int32_t client_find_mqtt_sn_gateway_config_init(client_find_mqtt_sn_gateway_config *cfg);
void client_find_mqtt_sn_gateway_config_cleanup(client_find_mqtt_sn_gateway_config *cfg);

int32_t is_client_find_mqtt_sn_gateway_config_command(const char *arg, int *i);
int32_t client_find_mqtt_sn_gateway_config_process_args(client_find_mqtt_sn_gateway_config *cfg,
                                                        const MqttSnLogger *logger,
                                                        int argc,
                                                        char *argv[]);

void client_find_mqtt_sn_gateway_config_print_usage_short(const MqttSnLogger *logger, const char *indent);
void client_find_mqtt_sn_gateway_config_print_usage_long(const MqttSnLogger *logger);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_CONFIG_CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_H_
