//
// Created by SomeDude on 28.05.2019.
//

#ifndef CMQTTSNFORWARDER_CONFIG_CLIENT_SUBSCRIBE_CONFIG_H_
#define CMQTTSNFORWARDER_CONFIG_CLIENT_SUBSCRIBE_CONFIG_H_

#include <stdint.h>
#include <logging/MqttSnLogging.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CLIENT_MQTT_SN_MAX_SUBSCRIPTIONS
#define CLIENT_MQTT_SN_MAX_SUBSCRIPTIONS 5
#endif

#define CLIENT_MQTT_SN_SUBSCRIPTION_TOPIC_ID_TYPE_TOPICNAME 0
#define CLIENT_MQTT_SN_SUBSCRIPTION_TOPIC_ID_TYPE_PREDEFINED 1
#define CLIENT_MQTT_SN_SUBSCRIPTION_TOPIC_ID_TYPE_TOPICID 2

#ifndef DEFAULT_CLIENT_MQTT_SN_SUBSCRIPTION_QOS
#define DEFAULT_CLIENT_MQTT_SN_SUBSCRIPTION_QOS 0
#endif

typedef struct client_subscription_topic_ {
    uint8_t  topic_id_type;
    int8_t   qos;
    uint16_t topic_id;
    char *   topic_name;
} client_subscription_topic;

typedef struct client_subscribe_config_ {
    int8_t                    qos;
    uint32_t                  subscription_list_max_len;
    client_subscription_topic subscription_list[CLIENT_MQTT_SN_MAX_SUBSCRIPTIONS];
    uint32_t                  subscription_len;
    uint8_t                   subscribe_clear;
} client_subscribe_config;

#ifndef MQTT_SN_SUBSCRIPTION_CONFIG_TOPIC_NAME_LENGTH
#define MQTT_SN_SUBSCRIPTION_CONFIG_TOPIC_NAME_LENGTH 255
#endif

typedef struct client_subscription_config_buffer_ {
    char topic_name[MQTT_SN_SUBSCRIPTION_CONFIG_TOPIC_NAME_LENGTH];
} client_subscription_config_buffer;

typedef struct client_subscribe_config_buffer_ {
    client_subscription_config_buffer subscription_list[CLIENT_MQTT_SN_MAX_SUBSCRIPTIONS];
} client_subscribe_config_buffer;

int32_t client_subscribe_config_copy_to_buffer(client_subscribe_config *cfg, client_subscribe_config_buffer *cfg_buffer);

int32_t client_subscribe_config_init(client_subscribe_config *cfg);
void    client_subscribe_config_cleanup(client_subscribe_config *cfg);

int32_t is_client_subscribe_config_command(const char *arg, int *i);
int32_t client_subscribe_config_process_args(client_subscribe_config *cfg, const MqttSnLogger *logger, int argc, char *argv[]);

void client_subscribe_config_print_usage_short(const MqttSnLogger *logger, const char *indent);
void client_subscribe_config_print_usage_long(const MqttSnLogger *logger);

int32_t parse_client_subscription_config(const MqttSnLogger *logger, char *url, client_subscription_topic *scfg);

#ifdef __cplusplus
}
#endif
#endif  //CMQTTSNFORWARDER_CONFIG_CLIENT_SUBSCRIBE_CONFIG_H_
