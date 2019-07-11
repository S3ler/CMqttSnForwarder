//
// Created by SomeDude on 18.06.2019.
//

#ifndef CMQTTSNFORWARDER_GATEWAY_CONFIG_GATEWAY_CONFIG_H_
#define CMQTTSNFORWARDER_GATEWAY_CONFIG_GATEWAY_CONFIG_H_

#include <config/common/mqtt_sn_version_config.h>
#include <config/common/mqtt_sn_logger_config.h>
#include <config/network/client/client_network_config.h>
#include <config/gateway/gateway_advertisment_config.h>
#include <config/gateway/gateway_client_connection_config.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MQTT_SN_GATEWAY_EXECUTABLE_NAME
#define MQTT_SN_GATEWAY_EXECUTABLE_NAME "mqtt_sn_gateway"
#endif

typedef struct mqtt_sn_gateway__config_ {
  char executable_name[sizeof(MQTT_SN_GATEWAY_EXECUTABLE_NAME)];
  mqtt_sn_version_config msvcfg;
  mqtt_sn_logger_config mslcfg;
  client_network_config cncfg;
  gateway_advertise_config gacfg;
  gateway_client_connection_config gccccfg;
} mqtt_sn_gateway__config;

int32_t mqtt_sn_gateway__config_init(mqtt_sn_gateway__config *cfg);
void mqtt_sn_gateway__config_cleanup(mqtt_sn_gateway__config *cfg);

int32_t process_mqtt_sn_gateway__config_line(mqtt_sn_gateway__config *cfg,
                                             const MqttSnLogger *logger,
                                             int argc,
                                             char *argv[]);

void mqtt_sn_gateway__config_print_usage(const MqttSnLogger *logger);
void mqtt_sn_gateway__config_print_usage_short(const MqttSnLogger *logger);
void mqtt_sn_gateway__config_print_usage_long(const MqttSnLogger *logger);
void mqtt_sn_gateway__config_print_see_usage(const MqttSnLogger *logger);

int32_t mqtt_sn_gateway__config_file_process_command_callback(void *cfg,
                                                              const MqttSnLogger *logger,
                                                              int argc,
                                                              char *argv[]);
#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_GATEWAY_CONFIG_GATEWAY_CONFIG_H_
