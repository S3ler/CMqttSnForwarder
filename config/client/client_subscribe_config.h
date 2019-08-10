//
// Created by SomeDude on 28.05.2019.
//

#ifndef CMQTTSNFORWARDER_CONFIG_CLIENT_SUBSCRIBE_CONFIG_H_
#define CMQTTSNFORWARDER_CONFIG_CLIENT_SUBSCRIBE_CONFIG_H_

#include <stdint.h>
#include <logging/MqttSnLoggingInterface.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CLIENT_MQTT_SN_SUBSCRIPTION_TOPIC_ID_TYPE_TOPICNAME 0
#define CLIENT_MQTT_SN_SUBSCRIPTION_TOPIC_ID_TYPE_PREDEFINED 1
#define CLIENT_MQTT_SN_SUBSCRIPTION_TOPIC_ID_TYPE_SHORT 2

#ifndef DEFAULT_CLIENT_MQTT_SN_SUBSCRIPTION_QOS
#define DEFAULT_CLIENT_MQTT_SN_SUBSCRIPTION_QOS 0
#endif

#ifndef DEFAULT_CLIENT_MQTT_SN_SUBSCRIPTION_TOPIC_ID_TYPE
#define DEFAULT_CLIENT_MQTT_SN_SUBSCRIPTION_TOPIC_ID_TYPE CLIENT_MQTT_SN_SUBSCRIPTION_TOPIC_ID_TYPE_PREDEFINED
#endif

#ifndef DEFAULT_CLIENT_MQTT_SN_SUBSCRIPTION_TOPIC_ID
#define DEFAULT_CLIENT_MQTT_SN_SUBSCRIPTION_TOPIC_ID 0
#endif

#ifndef CLIENT_MQTT_SN_MAX_SUBSCRIPTIONS
#define CLIENT_MQTT_SN_MAX_SUBSCRIPTIONS 5
#endif


typedef struct client_subscription_config_ {
  int8_t qos;
  uint16_t topic_id;
  uint8_t topic_id_type;
  char *topic_name;
} client_subscription_config;

typedef struct client_subscribe_config_ {
  int8_t qos;
  uint16_t topic_id;
  char *topic_name;
  uint8_t topic_id_type;

  uint32_t subscription_list_max_len;
  client_subscription_config subscription_list[CLIENT_MQTT_SN_MAX_SUBSCRIPTIONS];
  uint32_t subscription_len;

} client_subscribe_config;

#ifndef MQTT_SN_SUBSCRIPTION_CONFIG_TOPIC_NAME_LENGTH
#define MQTT_SN_SUBSCRIPTION_CONFIG_TOPIC_NAME_LENGTH 255
#endif

typedef struct client_subscription_config_buffer_ {
  char topic_name[MQTT_SN_SUBSCRIPTION_CONFIG_TOPIC_NAME_LENGTH];
} client_subscription_config_buffer;

typedef struct client_subscribe_config_buffer_ {
  char topic_name[MQTT_SN_SUBSCRIPTION_CONFIG_TOPIC_NAME_LENGTH];
  client_subscription_config_buffer subscription_list[CLIENT_MQTT_SN_MAX_SUBSCRIPTIONS];
} client_subscribe_config_buffer;

int32_t client_subscribe_config_copy_to_buffer(client_subscribe_config* cfg, client_subscribe_config_buffer* cfg_buffer);

int32_t client_subscribe_config_init(client_subscribe_config *cfg);
void client_subscribe_config_cleanup(client_subscribe_config *cfg);
void client_subscription_config_cleanup(client_subscription_config *cfg);

int32_t is_client_subscribe_config_command(const char *arg, int *i);
int32_t client_subscribe_config_process_args(client_subscribe_config *cfg,
                                             const MqttSnLogger *logger,
                                             int argc,
                                             char *argv[]);

void client_subscribe_config_print_usage_short(const MqttSnLogger *logger, const char *indent);
void client_subscribe_config_print_usage_long(const MqttSnLogger *logger);

int32_t parse_client_subscription_config(const MqttSnLogger *logger, char *url, client_subscription_config *cfg);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_CONFIG_CLIENT_SUBSCRIBE_CONFIG_H_
