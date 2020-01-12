//
// Created by SomeDude on 04.08.2019.
//

#include <platform/platform_compatibility.h>
#include "mqtt_sn_client_interactive_cmd_result.h"
int32_t log_mqtt_sn_client_interactive_cmd_result(const MqttSnLogger *logger, const mqtt_sn_client_interactive_cmd_result *result) {
    log_msg_start(logger);
    return print_mqtt_sn_client_interactive_cmd_result(logger, result);
}
int32_t print_mqtt_sn_client_interactive_cmd_result(const MqttSnLogger *logger, const mqtt_sn_client_interactive_cmd_result *result) {
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
int32_t log_MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE(const MqttSnLogger *logger, MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE action_type) {
    switch (action_type) {
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_NO_COMMAND:
        return log_str(logger, PSTR("NOCOMMAND"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_REINIT_LOGGER:
        return log_str(logger, PSTR("REINITLOGGER"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_INIT_CLIENT:
        return log_str(logger, PSTR("INITCLIENT"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_DEINIT_CLIENT:
        return log_str(logger, PSTR("DEINITCLIENT"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_CONNECT_GATEWAY:
        return log_str(logger, PSTR("CONNECT"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_AUTO_CONNECT_GATEWAY:
        return log_str(logger, PSTR("AUTOCONNECT"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_DIRECT_CONNECT_GATEWAY:
        return log_str(logger, PSTR("DIRECTCONNECT"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_SEARCHGW_AND_CONNECT_GATEWAY:
        return log_str(logger, PSTR("SEARCHGWCONNECT"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_ADVERTISE_AND_CONNECT_GATEWAY:
        return log_str(logger, PSTR("ADVERTISECONNECT"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_DISCONNECT_GATEWAY:
        return log_str(logger, PSTR("DISCONNECT"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_REGISTER_GATEWAY:
        return log_str(logger, PSTR("REGISTER"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_SUBSCRIBE_GATEWAY:
        return log_str(logger, PSTR("SUBSCRIBE"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PREDEFINED_SUBSCRIBE_GATEWAY:
        return log_str(logger, PSTR("PREDEFINEDSUBSCRIBE"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_TOPICID_SUBSCRIBE_GATEWAY:
        return log_str(logger, PSTR("TOPICIDSUBSCRIBE"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_TOPICNAME_SUBSCRIBE_GATEWAY:
        return log_str(logger, PSTR("TOPICNAMESUBSCRIBE"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_UNSUBSCRIBE_GATEWAY:
        return log_str(logger, PSTR("UNSUBSCRIBE"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_GATEWAY:
        return log_str(logger, PSTR("PUBLISH"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_UNCONNECTED_PREDEFINED_QOSM1_GATEWAY:
        return log_str(logger, PSTR("UNCONNECTEDPREDEFINEDPUBLISHQOSM1"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_PREDEFINED_QOSM1_GATEWAY:
        return log_str(logger, PSTR("PREDEFINEDPUBLISHQOSM1"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_PREDEFINED_QOS0_GATEWAY:
        return log_str(logger, PSTR("PREDEFINEDPUBLISHQOS0"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_PREDEFINED_QOS1_GATEWAY:
        return log_str(logger, PSTR("PREDEFINEDPUBLISHQOS1"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_PREDEFINED_QOS2_GATEWAY:
        return log_str(logger, PSTR("PREDEFINEDPUBLISHQOS2"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_TOPICNAME_QOSM1_GATEWAY:
        return log_str(logger, PSTR("TOPICNAMEPUBLISHQOSM1"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_TOPICNAME_QOS0_GATEWAY:
        return log_str(logger, PSTR("TOPICNAMEPUBLISHQOS0"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_TOPICNAME_QOS1_GATEWAY:
        return log_str(logger, PSTR("TOPICNAMEPUBLISHQOS1"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_TOPICNAME_QOS2_GATEWAY:
        return log_str(logger, PSTR("TOPICNAMEPUBLISHQOS2"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_TOPICID_QOSM1_GATEWAY:
        return log_str(logger, PSTR("TOPICIDPUBLISHQOSM1"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_TOPICID_QOS0_GATEWAY:
        return log_str(logger, PSTR("TOPICIDPUBLISHQOS0"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_TOPICID_QOS1_GATEWAY:
        return log_str(logger, PSTR("TOPICIDPUBLISHQOS1"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_TOPICID_QOS2_GATEWAY:
        return log_str(logger, PSTR("TOPICIDPUBLISHQOS2"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_RECEIVE_GATEWAY:
        return log_str(logger, PSTR("PUBLISHRECEIVE"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_RECEIVE_PUBLISH_PREDEFINED_QOSM1_GATEWAY:
        return log_str(logger, PSTR("PREDEFINEDPUBLISHRECEIVEQOSM1"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_RECEIVE_PUBLISH_PREDEFINED_QOS0_GATEWAY:
        return log_str(logger, PSTR("PREDEFINEDPUBLISHRECEIVEQOS0"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_RECEIVE_PUBLISH_PREDEFINED_QOS1_GATEWAY:
        return log_str(logger, PSTR("PREDEFINEDPUBLISHRECEIVEQOS1"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_RECEIVE_PUBLISH_PREDEFINED_QOS2_GATEWAY:
        return log_str(logger, PSTR("PREDEFINEDPUBLISHRECEIVEQOS2"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_RECEIVE_PUBLISH_TOPICNAME_QOSM1_GATEWAY:
        return log_str(logger, PSTR("TOPICNAMEPUBLISHRECEIVEQOSM1"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_RECEIVE_PUBLISH_TOPICNAME_QOS0_GATEWAY:
        return log_str(logger, PSTR("TOPICNAMEDPUBLISHRECEIVEQOS0"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_RECEIVE_PUBLISH_TOPICNAME_QOS1_GATEWAY:
        return log_str(logger, PSTR("TOPICNAMEDPUBLISHRECEIVEQOS1"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_RECEIVE_PUBLISH_TOPICNAME_QOS2_GATEWAY:
        return log_str(logger, PSTR("TOPICNAMEDPUBLISHRECEIVEQOS2"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_RECEIVE_PUBLISH_TOPICID_QOSM1_GATEWAY:
        return log_str(logger, PSTR("TOPICIDPUBLISHRECEIVEQOSM1"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_RECEIVE_PUBLISH_TOPICID_QOS0_GATEWAY:
        return log_str(logger, PSTR("TOPICIDDPUBLISHRECEIVEQOS0"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_RECEIVE_PUBLISH_TOPICID_QOS1_GATEWAY:
        return log_str(logger, PSTR("TOPICIDDPUBLISHRECEIVEQOS1"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_RECEIVE_PUBLISH_TOPICID_QOS2_GATEWAY:
        return log_str(logger, PSTR("TOPICIDDPUBLISHRECEIVEQOS2"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_SLEEP_GATEWAY:
        return log_str(logger, PSTR("SLEEP"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_WAKEUP_GATEWAY:
        return log_str(logger, PSTR("WAKEUP"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_RECONNECT_GATEWAY:
        return log_str(logger, PSTR("RECONNECT"));
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PING_GATEWAY:
        return log_str(logger, PSTR("PING"));
    default:
        return log_str(logger, PSTR("UNKNOWNACTION"));
    }
}
int32_t log_MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE(const MqttSnLogger *                          logger,
                                                          MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE action_result) {
    switch (action_result) {
    case MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS:
        return log_str(logger, PSTR("SUCCESS"));
    case MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR:
        return log_str(logger, PSTR("ERROR"));
    case MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_TIMEOUT:
        return log_str(logger, PSTR("TIMEOUT"));
    case MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_UNKNOWNACTION:
        return log_str(logger, PSTR("UNKNOWNACTION"));
    default:
        return log_str(logger, PSTR("UNKNOWNACTION"));
    }
}
int32_t mqtt_sn_client_interactive_cmd_result_init(mqtt_sn_client_interactive_cmd_result *result, const char *identifier,
                                                   uint64_t action_number, MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE action_type,
                                                   MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE action_result, uint64_t start_t_s,
                                                   uint64_t start_t_ns, uint64_t end_t_s, uint64_t end_t_ns) {
    memset(result, 0, sizeof(*result));
    strncpy(result->identifier, identifier, sizeof(result->identifier) - 1);
    result->action_number = action_number;
    result->action_type   = action_type;
    result->action_result = action_result;
    result->start_ms      = timestamp_s_ns_to_ms(start_t_s, start_t_ns);
    result->end_ms        = timestamp_s_ns_to_ms(end_t_s, end_t_ns);
    result->duration_ns   = timestamp_s_ns_to_ns(end_t_s, end_t_ns) - timestamp_s_ns_to_ns(start_t_s, start_t_ns);
    return 0;
}
mqtt_sn_client_interactive_cmd_result mqtt_sn_client_interactive_cmd_result_error_only(MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE action_type) {
    mqtt_sn_client_interactive_cmd_result result = { 0 };
    result.action_type                           = action_type;
    result.action_result                         = MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR;
    return result;
}
