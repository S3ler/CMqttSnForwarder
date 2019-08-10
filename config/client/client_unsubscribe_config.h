//
// Created by SomeDude on 06.08.2019.
//

#ifndef CMQTTSNFORWARDER_CONFIG_CLIENT_CLIENT_UNSUBSCRIBE_CONFIG_H_
#define CMQTTSNFORWARDER_CONFIG_CLIENT_CLIENT_UNSUBSCRIBE_CONFIG_H_

#include <stdint.h>
#include <logging/MqttSnLoggingInterface.h>
#ifdef __cplusplus
extern "C" {
#endif

#define CLIENT_MQTT_SN_UNSUBSCRIPTION_TOPIC_ID_TYPE_TOPICNAME 0
#define CLIENT_MQTT_SN_UNSUBSCRIPTION_TOPIC_ID_TYPE_PREDEFINED 1
#define CLIENT_MQTT_SN_UNSUBSCRIPTION_TOPIC_ID_TYPE_SHORT 2
/*
#ifndef DEFAULT_CLIENT_MQTT_SN_UNSUBSCRIPTION_TOPIC_ID_TYPE
#define DEFAULT_CLIENT_MQTT_SN_UNSUBSCRIPTION_TOPIC_ID_TYPE CLIENT_MQTT_SN_UNSUBSCRIPTION_TOPIC_ID_TYPE_PREDEFINED
#endif

#ifndef DEFAULT_CLIENT_MQTT_SN_UNSUBSCRIPTION_TOPIC_ID
#define DEFAULT_CLIENT_MQTT_SN_UNSUBSCRIPTION_TOPIC_ID 0
#endif
*/
#ifndef CLIENT_MQTT_SN_MAX_UNSUBSCRIBE
#define CLIENT_MQTT_SN_MAX_UNSUBSCRIBE 5
#endif

typedef struct single_client_unsubscribe_config_ {
  uint16_t topic_id;
  uint8_t topic_id_type;
  char *topic_name;
} single_client_unsubscribe_config;

typedef struct client_unsubscribe_config_ {
  uint32_t unsubscribe_list_max_len;
  single_client_unsubscribe_config unsubscribe_list[CLIENT_MQTT_SN_MAX_UNSUBSCRIBE];
  uint32_t unsubscribe_list_len;

  uint8_t unsubscribe_clear;
} client_unsubscribe_config;

#ifndef CLIENT_MQTT_SN_UNSUBSCRIBE_TOPIC_NAME_LENGTH
#define CLIENT_MQTT_SN_UNSUBSCRIBE_TOPIC_NAME_LENGTH 255
#endif

typedef struct single_client_unsubscribe_config_buffer {
  char topic_name[CLIENT_MQTT_SN_UNSUBSCRIBE_TOPIC_NAME_LENGTH];
} single_client_unsubscribe_config_buffer;

typedef struct client_unsubscribe_config_buffer_ {
  single_client_unsubscribe_config_buffer unsubscribe_list[CLIENT_MQTT_SN_MAX_UNSUBSCRIBE];
} client_unsubscribe_config_buffer;

int32_t client_unsubscribe_config_copy_to_buffer(client_unsubscribe_config *cfg,
                                                 client_unsubscribe_config_buffer *cfg_buffer);

int32_t client_unsubscribe_config_init(client_unsubscribe_config *cfg);
void client_unsubscribe_config_cleanup(client_unsubscribe_config *cfg);

int32_t is_client_unsubscribe_config_command(char *arg, int *i);
int32_t client_unsubscribe_config_process_args(client_unsubscribe_config *cfg,
                                               const MqttSnLogger *logger,
                                               int argc,
                                               char *argv[]);

void client_unsubscribe_config_print_usage_short(const MqttSnLogger *logger, const char *indent);
void client_unsubscribe_config_print_usage_long(const MqttSnLogger *logger);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_CONFIG_CLIENT_CLIENT_UNSUBSCRIBE_CONFIG_H_
