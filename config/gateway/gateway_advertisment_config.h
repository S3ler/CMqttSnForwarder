//
// Created by SomeDude on 30.05.2019.
//

#ifndef CMQTTSNFORWARDER_CONFIG_GATEWAY_ADVERTISMENT_CONFIG_H_
#define CMQTTSNFORWARDER_CONFIG_GATEWAY_ADVERTISMENT_CONFIG_H_

#include <stdint.h>
#include <logging/MqttSnLogging.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MQTT_SN_GATEWAY_ADVERTISEMENT_STANDBY_MONITORING
#define MQTT_SN_GATEWAY_ADVERTISEMENT_STANDBY_MONITORING 0
#endif

#ifndef MQTT_SN_GATEWAY_ADVERTISEMENT_DEFAULT_DURATION
#define MQTT_SN_GATEWAY_ADVERTISEMENT_DEFAULT_DURATION 900
#endif

#ifndef MQTT_SN_GATEWAY_ADVERTISEMENT_DEFAULT_RADIUS
#define MQTT_SN_GATEWAY_ADVERTISEMENT_DEFAULT_RADIUS 1
#endif

#ifndef MQTT_SN_GATEWAY_ADVERTISEMENT_DEFAULT_GW_ID
#define MQTT_SN_GATEWAY_ADVERTISEMENT_DEFAULT_GW_ID 1
#endif

#ifndef MQTT_SN_GATEWAY_ADVERTISEMENT_MISSING_ADVERTISE_COUNT
#define MQTT_SN_GATEWAY_ADVERTISEMENT_MISSING_ADVERTISE_COUNT 2
#endif

#ifndef MQTT_SN_GATEWAY_ADVERTISEMENT_GATEWAY_PING_MONITORING
#define MQTT_SN_GATEWAY_ADVERTISEMENT_GATEWAY_PING_MONITORING 1
#endif


typedef struct gateway_advertisement_config_ {
  uint8_t  advertisement_standby_monitoring;
  uint16_t advertisement_duration;
  uint8_t advertisement_radius;
  uint8_t gateway_id;
  uint8_t missing_advertise_count;
  uint8_t gateway_ping_monitoring;
} gateway_advertise_config;

int32_t gateway_advertisement_config_init(gateway_advertise_config *cfg);
void gateway_advertisement_config_cleanup(gateway_advertise_config *cfg);

int32_t is_gateway_advertisement_config_command(const char* arg, int *i);
int32_t gateway_advertisement_config_process_args(gateway_advertise_config *cfg,
                                                  const MqttSnLogger *logger,
                                                  int argc,
                                                  char **argv);

void gateway_advertisement_config_print_usage_short(const MqttSnLogger *logger, const char *indent);
void gateway_advertisement_config_print_usage_long(const MqttSnLogger *logger);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_CONFIG_GATEWAY_ADVERTISMENT_CONFIG_H_
