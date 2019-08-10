//
// Created by SomeDude on 28.05.2019.
//

#ifndef CMQTTSNFORWARDER_CONFIG_CLIENT_PUBLISH_CONFIG_H_
#define CMQTTSNFORWARDER_CONFIG_CLIENT_PUBLISH_CONFIG_H_

#include <stdint.h>
#include <logging/MqttSnLoggingInterface.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CLIENT_MQTT_SN_MAX_PUBLISH
#define CLIENT_MQTT_SN_MAX_PUBLISH 5
#endif

#ifndef CLIENT_MQTT_SN_PUBLISH_FROM_CONSOLE_IN_DEFAULT
#define CLIENT_MQTT_SN_PUBLISH_FROM_CONSOLE_IN_DEFAULT 0
#endif

#ifndef DEFAULT_CLIENT_MQTT_SN_PUBLISH_TOPIC_NAME
#define DEFAULT_CLIENT_MQTT_SN_PUBLISH_TOPIC_NAME "default/topic/name"
#endif
// TODO default value definition
#define CLIENT_MQTT_SN_PUBLISH_TOPIC_ID_TYPE_TOPICNAME 0
#define CLIENT_MQTT_SN_PUBLISH_TOPIC_ID_TYPE_PREDEFINED 1
#define CLIENT_MQTT_SN_PUBLISH_TOPIC_ID_TYPE_SHORT 2

typedef struct single_client_publish_config_ {
  int8_t qos;
  uint8_t topic_id_type;
  uint16_t topic_id;
  uint8_t retain;
  char *topic_name;
  uint8_t *data;
  uint16_t data_length;
} single_client_publish_config;

typedef struct client_publish_config_ {
  int8_t qos;
  uint8_t topic_id_type; // 0 topicname, 1 predefined, 2 short/registered
  uint16_t topic_id;
  uint8_t retain;
  char default_topic_name[sizeof(DEFAULT_CLIENT_MQTT_SN_PUBLISH_TOPIC_NAME)];
  char *topic_name;

  uint32_t publish_list_max_len;
  single_client_publish_config publish_list[CLIENT_MQTT_SN_MAX_PUBLISH];
  uint32_t publish_list_len;

  uint8_t publish_clear;
} client_publish_config;

#ifndef CLIENT_MQTT_SN_PUBLISH_TOPIC_NAME_LENGTH
#define CLIENT_MQTT_SN_PUBLISH_TOPIC_NAME_LENGTH 255
#endif

#ifndef CLIENT_MQTT_SN_PUBLISH_DATA_LENGTH
#define CLIENT_MQTT_SN_PUBLISH_DATA_LENGTH 1024
#endif

typedef struct single_client_publish_config_buffer_ {
  char topic_name[CLIENT_MQTT_SN_PUBLISH_TOPIC_NAME_LENGTH];
  uint8_t data[CLIENT_MQTT_SN_PUBLISH_DATA_LENGTH];
  uint16_t data_length;
} single_client_publish_config_buffer;

typedef struct client_publish_config_buffer_ {
  char topic_name[CLIENT_MQTT_SN_PUBLISH_TOPIC_NAME_LENGTH];
  single_client_publish_config_buffer publish_list[CLIENT_MQTT_SN_MAX_PUBLISH];
} client_publish_config_buffer;

int32_t client_publish_config_copy_to_buffer(client_publish_config *cfg, client_publish_config_buffer *cfg_buffer);

int32_t client_publish_config_init(client_publish_config *cfg);
void client_publish_config_cleanup(client_publish_config *cfg);

int32_t is_client_publish_config_command(const char *arg, int *i);
int32_t client_publish_config_process_args(client_publish_config *cfg,
                                           const MqttSnLogger *logger,
                                           int argc,
                                           char *argv[]);

void client_publish_config_print_usage_short(const MqttSnLogger *logger, const char *indent);
void client_publish_config_print_usage_long(const MqttSnLogger *logger);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_CONFIG_CLIENT_PUBLISH_CONFIG_H_
