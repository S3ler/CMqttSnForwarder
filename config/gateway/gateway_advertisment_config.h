//
// Created by SomeDude on 30.05.2019.
//

#ifndef CMQTTSNFORWARDER_CONFIG_GATEWAY_ADVERTISMENT_CONFIG_H_
#define CMQTTSNFORWARDER_CONFIG_GATEWAY_ADVERTISMENT_CONFIG_H_

#include <stdint.h>
#include <logging/MqttSnLoggingInterface.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MQTT_SN_GATEWAY_ADVERTISEMENT_DEFAULT_DURATION
#define MQTT_SN_GATEWAY_ADVERTISEMENT_DEFAULT_DURATION 600
#endif

#ifndef MQTT_SN_GATEWAY_ADVERTISEMENT_DEFAULT_RADIUS
#define MQTT_SN_GATEWAY_ADVERTISEMENT_DEFAULT_RADIUS UINT8_MAX
#endif

typedef struct gateway_advertisement_config_ {
  uint16_t advertisement_duration;
  uint8_t advertisement_radius;
} gateway_advertisement_config;

int32_t gateway_advertisement_config_init(gateway_advertisement_config *cfg);
void gateway_advertisement_config_cleanup(gateway_advertisement_config *cfg);

int32_t is_gateway_advertisement_config_command(const char* arg, int *i);
int32_t gateway_advertisement_config_process_args(gateway_advertisement_config *cfg,
                                                  const MqttSnLogger *logger,
                                                  int argc,
                                                  char **argv);

void gateway_advertisement_config_print_usage_short(const MqttSnLogger *logger, const char *indent);
void gateway_advertisement_config_print_usage_long(const MqttSnLogger *logger);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_CONFIG_GATEWAY_ADVERTISMENT_CONFIG_H_
