//
// Created by SomeDude on 31.05.2019.
//

#ifndef CMQTTSNFORWARDER_FORWARDER_CONFIG_FORWARDER_CONFIG_H_
#define CMQTTSNFORWARDER_FORWARDER_CONFIG_FORWARDER_CONFIG_H_

#include <config/common/mqtt_sn_version_config.h>
#include <config/client/mqtt_sn_gateway_config.h>
#include <config/common/mqtt_sn_logger_config.h>
#include <config/network/gateway/gateway_network_config.h>
#include <config/network/client/client_network_config.h>
#include <config/client/client_find_mqtt_sn_gateway_config.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MQTT_SN_FORWARDER_EXECUTABLE_NAME
#define MQTT_SN_FORWARDER_EXECUTABLE_NAME "cmqttsnforwarder"
#endif


typedef struct forwarder_config_ {
  char executable_name[sizeof(MQTT_SN_FORWARDER_EXECUTABLE_NAME)];

  mqtt_sn_version_config msvcfg;
  mqtt_sn_gateway_config msgcfg;
  client_find_mqtt_sn_gateway_config cfmsngcfg;
  mqtt_sn_logger_config mslcfg;

  gateway_network_config gncfg;
  client_network_config cncfg;
} forwarder_config;

int32_t forwarder_config_init(forwarder_config *cfg);
void forwarder_config_cleanup(forwarder_config *cfg);

int32_t forwarder_config_process_line(forwarder_config* cfg, const MqttSnLogger* logger, int argc, char* argv[]);

int32_t is_forwarder_config_command(const char* arg, int *i);
int32_t forwarder_config_process_args(forwarder_config* cfg, const MqttSnLogger* logger, int argc, char* argv[]);

void forwarder_config_print_usage(const MqttSnLogger *logger);
void forwarder_config_print_usage_short(const MqttSnLogger *logger);
void forwarder_config_print_usage_long(const MqttSnLogger *logger);
void forwarder_config_print_see_usage(const MqttSnLogger *logger);


int32_t forwarder_config_file_process_command_callback(void *cfg,
                                                       const MqttSnLogger *logger,
                                                       int argc,
                                                       char *argv[]);


#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_FORWARDER_CONFIG_FORWARDER_CONFIG_H_
