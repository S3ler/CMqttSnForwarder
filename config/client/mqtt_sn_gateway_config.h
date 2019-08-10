//
// Created by SomeDude on 27.05.2019.
//

#ifndef CMQTTSNFORWARDER_CONFIG_MQTT_SN_GATEWAY_CONFIG_H_
#define CMQTTSNFORWARDER_CONFIG_MQTT_SN_GATEWAY_CONFIG_H_

#include <stdint.h>
#include <stddef.h>
#include <logging/MqttSnLoggingInterface.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MQTT_SN_PROTOCOLID_V1 0x01

#ifndef DEFAULT_MQTT_SN_PROTOCOL_ID
#define DEFAULT_MQTT_SN_PROTOCOL_ID MQTT_SN_PROTOCOLID_V1
#endif

#ifdef Arduino_h
#ifndef DEFAULT_MQTT_SN_GATEWAY_HOST
#define DEFAULT_MQTT_SN_GATEWAY_HOST "arsmb.de"
#endif
#else
#ifndef DEFAULT_MQTT_SN_GATEWAY_HOST
#define DEFAULT_MQTT_SN_GATEWAY_HOST "localhost"
#endif
#endif

#ifndef DEFAULT_MQTT_SN_GATEWAY_PORT
#define DEFAULT_MQTT_SN_GATEWAY_PORT 8888
#endif

/**
 * default hostname - fixed either character or address in characters form
 * mqtt_sn_gateway_host - either characters or address in characters form
 * mqtt_sn_gateway_port - added at the end of the device_address for the gateway generated - ignored on -1;
 */
typedef struct mqtt_sn_gateway_config_ {
  int32_t protocol_id;
  char default_mqtt_sn_gateway_host[sizeof(DEFAULT_MQTT_SN_GATEWAY_HOST)];
  char *mqtt_sn_gateway_host;
  int32_t mqtt_sn_gateway_port;
} mqtt_sn_gateway_config;

#ifndef DEFAULT_MQTT_SN_GATEWAY_CONFIG_GATEWAY_HOST_LENGTH
#define DEFAULT_MQTT_SN_GATEWAY_CONFIG_GATEWAY_HOST_LENGTH UINT16_MAX
#endif
typedef struct mqtt_sn_gateway_config_buffer_ {
  char mqtt_sn_gateway_host[DEFAULT_MQTT_SN_GATEWAY_CONFIG_GATEWAY_HOST_LENGTH];
} mqtt_sn_gateway_config_buffer;

int32_t mqtt_sn_gateway_config_buffer_is_copyable_to_buffer(mqtt_sn_gateway_config *cfg);

int32_t mqtt_sn_gateway_config_buffer_copy_to_buffer(mqtt_sn_gateway_config *cfg,
                                                     mqtt_sn_gateway_config_buffer *cfg_buffer);

int32_t mqtt_sn_gateway_config_init(mqtt_sn_gateway_config *msgcfg);
void mqtt_sn_gateway_config_cleanup(mqtt_sn_gateway_config *msgcfg);

int32_t is_mqtt_sn_gateway_config_command(const char *arg, int *i);
int32_t mqtt_sn_gateway_config_process_args(mqtt_sn_gateway_config *msgcfg,
                                            const MqttSnLogger *logger,
                                            int argc,
                                            char **argv);

void mqtt_sn_gateway_config_print_usage_short(const MqttSnLogger *logger, const char *indent);
void mqtt_sn_gateway_config_print_usage_long(const MqttSnLogger *logger);
#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_CONFIG_MQTT_SN_GATEWAY_CONFIG_H_
