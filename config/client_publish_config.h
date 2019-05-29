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

// TODO default value definition

typedef struct single_client_publish_config_ {
  int8_t qos;
  uint8_t topic_id_type;
  uint16_t topic_id;
  uint8_t *data;
  uint16_t data_length;
  // TODO retain
  // TODO qos
} single_client_publish_config;

typedef struct client_publish_config_ {
  int8_t default_qos;
  uint8_t default_topic_id_type;
  uint16_t default_topic_id;

  uint16_t publish_list_max_len;
  single_client_publish_config publish_list[CLIENT_MQTT_SN_MAX_PUBLISH];
  uint16_t publish_list_len;
} client_publish_config;

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
