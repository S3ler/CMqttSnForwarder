//
// Created by SomeDude on 04.08.2019.
//

#ifndef CMQTTSNFORWARDER_CLIENT_INTERACTIVE_LINUX_STARTER_MQTT_SN_CLIENT_INTERACTIVE_CMD_RESULT_H_
#define CMQTTSNFORWARDER_CLIENT_INTERACTIVE_LINUX_STARTER_MQTT_SN_CLIENT_INTERACTIVE_CMD_RESULT_H_

#include <stdint.h>
#include "mqtt_sn_client_interactive_action_type.h"
#include "mqtt_sn_client_interactive_action_result_type.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_IDENTIFIER_LENGTH
#define MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_IDENTIFIER_LENGTH 64
#endif

typedef struct mqtt_sn_client_interactive_cmd_result_ {
  char identifier[MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_IDENTIFIER_LENGTH];
  uint64_t action_number;
  MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE action_type;
  MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE action_result;
  uint64_t start_ms;
  uint64_t end_ms;
  uint64_t duration_ns;
} mqtt_sn_client_interactive_cmd_result;

mqtt_sn_client_interactive_cmd_result mqtt_sn_client_interactive_cmd_result_error_only(
    MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE action_type);

int32_t mqtt_sn_client_interactive_cmd_result_init(mqtt_sn_client_interactive_cmd_result *result,
                                                   const char *identifier,
                                                   uint64_t action_number,
                                                   MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE action_type,
                                                   MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE action_result,
                                                   uint64_t start_t_s,
                                                   uint64_t start_t_ns,
                                                   uint64_t end_t_s,
                                                   uint64_t end_t_ns);

int32_t log_mqtt_sn_client_interactive_cmd_result(const MqttSnLogger *logger,
                                                  const mqtt_sn_client_interactive_cmd_result *result);
int32_t print_mqtt_sn_client_interactive_cmd_result(const MqttSnLogger *logger,
                                                    const mqtt_sn_client_interactive_cmd_result *result);
int32_t log_MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE(const MqttSnLogger *logger,
                                                   MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE action_type);
int32_t log_MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE(const MqttSnLogger *logger,
                                                          MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE action_result);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_CLIENT_INTERACTIVE_LINUX_STARTER_MQTT_SN_CLIENT_INTERACTIVE_CMD_RESULT_H_
