//
// Created by SomeDude on 28.05.2019.
//

#ifndef CMQTTSNFORWARDER_CONFIG_CLIENT_CONNECT_CONFIG_H_
#define CMQTTSNFORWARDER_CONFIG_CLIENT_CONNECT_CONFIG_H_

#include <stdint.h>
#include <logging/MqttSnLoggingInterface.h>
#include <stdlib.h>
#include <parser/MqttSnMessageParser.h>
#ifdef __cplusplus
extern "C" {
#endif

#ifndef DEFAULT_CLIENT_CONNECT_MQTT_SN_GATEWAY_CONFIG_CLIENT_ID
#if defined(Arduino_h)
#else
#define DEFAULT_CLIENT_CONNECT_MQTT_SN_GATEWAY_CONFIG_CLIENT_ID { (char)('A' + (rand() % (2*26))), \
                                                                  (char)('A' + (rand() % (2*26))), \
                                                                  (char)('A' + (rand() % (2*26))), \
                                                                  (char)('A' + (rand() % (2*26))), \
                                                                  (char)('A' + (rand() % (2*26))), \
                                                                  (char)('A' + (rand() % (2*26))), '\0'}
#endif
#endif

/* // ToDO
#ifndef DEFAULT_MQTT_SN_CLIENT_PING_REQUEST_DEFAULT_ENABLED
#define DEFAULT_MQTT_SN_CLIENT_PING_REQUEST_DEFAULT_ENABLED 1
#endif
*/
#ifndef DEFAULT_MQTT_SN_CLIENT_CONNECT_DURATION
#define DEFAULT_MQTT_SN_CLIENT_CONNECT_DURATION 60
#endif

#ifndef DEFAULT_MQTT_SN_CLIENT_PING_REQUEST_OFFSET_TIME_DEFAULT_VALUE
#define DEFAULT_MQTT_SN_CLIENT_PING_REQUEST_OFFSET_TIME_DEFAULT_VALUE -10
#endif

#ifndef DEFAULT_MQTT_SN_CLIENT_CONNECT_TIMEOUT
#define DEFAULT_MQTT_SN_CLIENT_CONNECT_TIMEOUT 30000
#endif

typedef struct client_connect_config_ {
  uint16_t connect_duration;
  char default_client_id[24];
  char *client_id;
  char *will_topic;
  char *will_msg;
  uint8_t clean_session;
  int32_t client_connect_timeout_ms;
  int32_t connect_timeout_offset;
} client_connect_config;

#ifndef DEFAULT_MQTT_SN_CLIENT_CONNECT_CONFIG_BUFFER_CLIENT_IT_LENGTH
#define DEFAULT_MQTT_SN_CLIENT_CONNECT_CONFIG_BUFFER_CLIENT_IT_LENGTH  MQTT_SN_MAX_CLIENT_ID_LENGTH
#endif
#ifndef DEFAULT_MQTT_SN_CLIENT_CONNECT_CONFIG_BUFFER_WILL_TOPIC_LENGTH
#define DEFAULT_MQTT_SN_CLIENT_CONNECT_CONFIG_BUFFER_WILL_TOPIC_LENGTH UINT16_MAX
#endif
#ifndef DEFAULT_MQTT_SN_CLIENT_CONNECT_CONFIG_BUFFER_WILL_MSG_LENGTH
#define DEFAULT_MQTT_SN_CLIENT_CONNECT_CONFIG_BUFFER_WILL_MSG_LENGTH UINT16_MAX
#endif
typedef struct client_connect_config_buffer_ {
  char client_id[DEFAULT_MQTT_SN_CLIENT_CONNECT_CONFIG_BUFFER_CLIENT_IT_LENGTH];
  char will_topic[DEFAULT_MQTT_SN_CLIENT_CONNECT_CONFIG_BUFFER_WILL_TOPIC_LENGTH];
  char will_msg[DEFAULT_MQTT_SN_CLIENT_CONNECT_CONFIG_BUFFER_WILL_MSG_LENGTH];
} client_connect_config_buffer;

int32_t client_connect_config_copy_to_buffer(client_connect_config* cfg, client_connect_config_buffer* cfg_buffer);

int32_t client_connect_config_init(client_connect_config *cfg);
void client_connect_config_cleanup(client_connect_config *cfg);

int32_t is_client_connect_config_command(const char *arg, int *i);
int32_t client_connect_config_process_args(client_connect_config *cfg,
                                           const MqttSnLogger *logger,
                                           int argc,
                                           char *argv[]);

void client_connect_config_print_usage_short(const MqttSnLogger *logger, const char *indent);
void client_connect_config_print_usage_long(const MqttSnLogger *logger);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_CONFIG_CLIENT_CONNECT_CONFIG_H_
