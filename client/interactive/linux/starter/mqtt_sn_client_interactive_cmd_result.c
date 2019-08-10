//
// Created by SomeDude on 04.08.2019.
//

#include <platform/platform_compatibility.h>
#include "mqtt_sn_client_interactive_cmd_result.h"
int32_t log_mqtt_sn_client_interactive_cmd_result(const MqttSnLogger *logger,
                                                  const mqtt_sn_client_interactive_cmd_result *result) {
  log_msg_start(logger);
  log_flush(logger);
  return print_mqtt_sn_client_interactive_cmd_result(logger, result);
}
int32_t print_mqtt_sn_client_interactive_cmd_result(const MqttSnLogger *logger,
                                                    const mqtt_sn_client_interactive_cmd_result *result) {
  log_str(logger, result->identifier);
  log_str(logger, ",");
  log_uint64(logger, result->action_number);
  log_str(logger, ",");
  log_MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE(logger, result->action_type);
  log_str(logger, ",");
  log_MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE(logger, result->action_result);
  log_str(logger, ",");
  log_uint64(logger, result->start_ms);
  log_str(logger, ",");
  log_uint64(logger, result->end_ms);
  log_str(logger, ",");
  log_uint64(logger, result->duration_ns);
  log_flush(logger);
  return log_status(logger);
}
int32_t log_MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE(const MqttSnLogger *logger,
                                                   MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE action_type) {
  switch (action_type) {
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_NO_COMMAND:return log_str(logger, PSTR("NOCOMMAND"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_REINIT_LOGGER:return log_str(logger,PSTR("REINITLOGGER"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_INIT_CLIENT:return log_str(logger, PSTR("INITCLIENT"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_DEINIT_CLIENT:return log_str(logger, PSTR("DEINITCLIENT"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_CONNECT_GATEWAY:return log_str(logger, PSTR("CONNECT"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_DIRECT_CONNECT_GATEWAY:return log_str(logger, PSTR("DIRECTCONNECT"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_SEARCHGW_AND_CONNECT_GATEWAY:
      return log_str(logger,
                     PSTR("SEARCHGWCONNECT"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_ADVERTISE_AND_CONNECT_GATEWAY:
      return log_str(logger,
                     PSTR("ADVERTISECONNECT"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_DISCONNECT_GATEWAY:return log_str(logger, PSTR("DISCONNECT"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_REGISTER_GATEWAY:return log_str(logger, PSTR("REGISTER"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_SUBSCRIBE_GATEWAY:return log_str(logger, PSTR("SUBSCRIBE"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_UNSUBSCRIBE_GATEWAY:return log_str(logger, PSTR("UNSUBSCRIBE"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_GATEWAY:return log_str(logger, PSTR("PUBLISH"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_QOSM1_GATEWAY:return log_str(logger, PSTR("PUBLISHQOSM1"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_QOSM1_CONNECTED_GATEWAY:
      return log_str(logger,
                     PSTR("PUBLISHQOSM1CONNECTED"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_QOS0_GATEWAY:return log_str(logger, PSTR("PUBLISHQOS0"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_QOS1_GATEWAY:return log_str(logger, PSTR("PUBLISHQOS1"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_QOS2_GATEWAY:return log_str(logger, PSTR("PUBLISHQOS2"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_SLEEP_GATEWAY:return log_str(logger, PSTR("SLEEP"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_WAKEUP_GATEWAY:return log_str(logger, PSTR("WAKEUP"));
    default: return log_str(logger, PSTR("UNKNOWNACTION"));
  }
}
int32_t log_MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE(const MqttSnLogger *logger,
                                                          MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE action_result) {
  switch (action_result) {
    case MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS:return log_str(logger, PSTR("SUCCESS"));
    case MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR:return log_str(logger, PSTR("ERROR"));
    case MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_TIMEOUT:return log_str(logger, PSTR("TIMEOUT"));
    case MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_UNKNOWNACTION:return log_str(logger, PSTR("UNKNOWNACTION"));
    default: return log_str(logger, PSTR("UNKNOWNACTION"));
  }
}
int32_t mqtt_sn_client_interactive_cmd_result_init(mqtt_sn_client_interactive_cmd_result *result,
                                                   const char *identifier,
                                                   uint64_t action_number,
                                                   MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE action_type,
                                                   MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE action_result,
                                                   uint64_t start_t_s,
                                                   uint64_t start_t_ns,
                                                   uint64_t end_t_s,
                                                   uint64_t end_t_ns) {
  memset(result, 0, sizeof(*result));
  strncpy(result->identifier, identifier, sizeof(result->identifier) - 1);
  result->action_number = action_number;
  result->action_type = action_type;
  result->action_result = action_result;
  result->start_ms = timestamp_s_ns_to_ns(start_t_s, start_t_ns) / 1000000L;
  result->end_ms = timestamp_s_ns_to_ns(end_t_s, end_t_ns) / 1000000L;
  result->duration_ns = timestamp_s_ns_to_ns(start_t_s, start_t_ns) - timestamp_s_ns_to_ns(end_t_s, end_t_ns);
  return 0;
}
mqtt_sn_client_interactive_cmd_result mqtt_sn_client_interactive_cmd_result_error_only(
    MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE action_type) {
  mqtt_sn_client_interactive_cmd_result result = {0};
  result.action_type = action_type;
  result.action_result = MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
  return result;
}
