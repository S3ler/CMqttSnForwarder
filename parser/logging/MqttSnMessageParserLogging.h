//
// Created by bele on 20.05.19.
//

#ifndef CMQTTSNFORWARDER_PARSER_LOGGING_MQTTSNMESSAGEPARSERLOGGING_H_
#define CMQTTSNFORWARDER_PARSER_LOGGING_MQTTSNMESSAGEPARSERLOGGING_H_

#include <logging/MqttSnLogging.h>
#include <parser/MqttSnMessageParser.h>
#include <platform/MqttSnMessageData.h>
#ifdef __cplusplus
extern "C" {
#endif

//#define MQTT_SN_MESSAGE_TYPE_STRING_NEGATIVE_ENUMS_OFFSET 2
//static const char *MQTT_SN_MESSAGE_TYPE_STRING[] = { FOREACH_MQTT_SN_MESSAGE_TYPE(GENERATE_MQTT_SN_MESSAGE_TYPE_STRING) };

int log_mqtt_sn_flags(const MqttSnLogger *logger, uint8_t flags);

int log_dup_flag(const MqttSnLogger *logger, uint8_t dup);

int log_qos_flag(const MqttSnLogger *logger, int8_t qos);

int log_retain_flag(const MqttSnLogger *logger, uint8_t retain);

int log_will_flag(const MqttSnLogger *logger, uint8_t will);

int log_clean_session_flag(const MqttSnLogger *logger, uint8_t clean_session);

int log_topic_id_type_flag(const MqttSnLogger *logger, uint8_t topicIdType);

int log_topic_id(const MqttSnLogger *logger, uint16_t topic_id);

int log_msg_id(const MqttSnLogger *logger, uint16_t msg_id);

int log_duration(const MqttSnLogger *logger, uint16_t duration);

int log_msg_from(const MqttSnLogger *logger, MQTT_SN_MESSAGE_TYPE msg_type, const device_address *from);

int log_msg_to(const MqttSnLogger *logger, MQTT_SN_MESSAGE_TYPE msg_type, const device_address *from);

int32_t log_mqtt_sn_payload(const MqttSnLogger *logger, const uint8_t *payload, uint16_t payload_length);

int32_t log_mqtt_sn_message_type(const MqttSnLogger *logger, MQTT_SN_MESSAGE_TYPE msg_type);

int32_t log_any_message(const MqttSnLogger *logger, const MqttSnMessageData *msg);

int32_t print_any_message(const MqttSnLogger *logger, const MqttSnMessageData *msg);

int32_t print_any_message_bytes(const MqttSnLogger *logger, const device_address *from, const device_address *to, uint8_t signal_strength,
                                const uint8_t *data, uint16_t data_length);

int32_t log_malformed_message(const MqttSnLogger *logger, const MqttSnMessageData *msg);

int32_t log_malformed_message_header(const MqttSnLogger *logger, const device_address *from, uint8_t signal_strength, const uint8_t *data,
                                     uint16_t data_len);

int32_t print_malformed_message(const MqttSnLogger *logger, MQTT_SN_MESSAGE_TYPE optional_msg_type, const device_address *from,
                              const device_address *to, uint8_t signal_strength, const uint8_t *data, uint16_t data_len);

int32_t log_msg_gen_error(const MqttSnLogger *logger, MQTT_SN_MESSAGE_TYPE msg_type, const char *file_name, const char *function_name,
                          int line_number);

int32_t log_fatal_cannot_add_dup_flag(const MqttSnLogger *logger,const char *file_name, const char *function_name,
                              int line_number, const uint8_t* msg_data, uint16_t msg_len);

#ifdef __cplusplus
}
#endif
#endif  //CMQTTSNFORWARDER_PARSER_LOGGING_MQTTSNMESSAGEPARSERLOGGING_H_
