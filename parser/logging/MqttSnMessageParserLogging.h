//
// Created by bele on 20.05.19.
//

#ifndef CMQTTSNFORWARDER_PARSER_LOGGING_MQTTSNMESSAGEPARSERLOGGING_H_
#define CMQTTSNFORWARDER_PARSER_LOGGING_MQTTSNMESSAGEPARSERLOGGING_H_

#include <logging/MqttSnLoggingInterface.h>
#include <parser/MqttSnMessageParser.h>
#ifdef __cplusplus
extern "C" {
#endif

int log_mqtt_sn_flags(const MqttSnLogger *logger, uint8_t flags);

int log_dup_flag(const MqttSnLogger *logger, uint8_t dup);

int log_qos_flag(const MqttSnLogger *logger, int8_t qos);

int log_retain_flag(const MqttSnLogger *logger, uint8_t retain);

int log_will_flag(const MqttSnLogger *logger, uint8_t will);

int log_clean_session_flag(const MqttSnLogger *logger, uint8_t clean_session);

int log_topic_id_type_flag(const MqttSnLogger *logger, uint8_t topicIdType);

int log_duration(const MqttSnLogger *logger, uint16_t duration);

int log_msg_from(const MqttSnLogger *logger, MQTT_SN_MESSAGE_TYPE msg_type, const device_address *from);

int log_msg_to(const MqttSnLogger *logger, MQTT_SN_MESSAGE_TYPE msg_type, const device_address *from);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_PARSER_LOGGING_MQTTSNMESSAGEPARSERLOGGING_H_
