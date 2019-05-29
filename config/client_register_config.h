//
// Created by SomeDude on 28.05.2019.
//

#ifndef CMQTTSNFORWARDER_CONFIG_CLIENT_REGISTER_CONFIG_H_
#define CMQTTSNFORWARDER_CONFIG_CLIENT_REGISTER_CONFIG_H_

#include <stdint.h>
#include <logging/MqttSnLoggingInterface.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CLIENT_MQTT_SN_MAX_REGISTRATIONS
#define CLIENT_MQTT_SN_MAX_REGISTRATIONS 5
#endif

typedef struct client_registration_config_ {
  char *topic_name;
} client_registration_config;

typedef struct client_register_config_ {
  uint16_t registration_list_max_len;
  client_registration_config registration_list[CLIENT_MQTT_SN_MAX_REGISTRATIONS];
  uint16_t registration_list_len;
} client_register_config;

int32_t client_register_config_init(client_register_config *cfg);
void client_register_config_cleanup(client_register_config *cfg);

int32_t is_client_register_config_command(const char *arg, int *i);
int32_t client_register_config_process_args(client_register_config *cfg,
                                            const MqttSnLogger *logger,
                                            int argc,
                                            char *argv[]);

void client_register_config_print_usage_short(const MqttSnLogger *logger, const char *indent);
void client_register_config_print_usage_long(const MqttSnLogger *logger);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_CONFIG_CLIENT_REGISTER_CONFIG_H_
