//
// Created by bele on 24.05.19.
//

#ifndef CMQTTSNFORWARDER_PARSER_LOGGING_COMMON_MQTTSNMESSAGELOGGING_H_
#define CMQTTSNFORWARDER_PARSER_LOGGING_COMMON_MQTTSNMESSAGELOGGING_H_

#include <logging/MqttSnLoggingInterface.h>

#ifdef __cplusplus
extern "C" {
#endif

int32_t log_mqtt_sn_comma(const MqttSnLogger *logger);

// mqtt-sn specific logging

int32_t log_mqtt_sn_flags(const MqttSnLogger *logger,
                          uint8_t dup,
                          int8_t qos,
                          uint8_t retain,
                          uint8_t will,
                          uint8_t clean_session,
                          uint8_t topic_id_type);

int32_t log_mqtt_sn_dup_flag(const MqttSnLogger *logger, uint8_t dup);

int32_t log_mqtt_sn_qos_flag(const MqttSnLogger *logger, int8_t qos);

int32_t log_mqtt_sn_retain_flag(const MqttSnLogger *logger, uint8_t retain);

int32_t log_mqtt_sn_will_flag(const MqttSnLogger *logger, uint8_t will);

int32_t log_mqtt_sn_clean_session_flag(const MqttSnLogger *logger, uint8_t clean_session);

int32_t log_mqtt_sn_topic_id_type_flag(const MqttSnLogger *logger, uint8_t topicIdType);

int32_t log_mqtt_sn_ctrl(const MqttSnLogger *logger, uint8_t ctrl);

int32_t log_mqtt_sn_duration(const MqttSnLogger *logger, uint16_t duration);

int32_t log_mqtt_sn_message_id(const MqttSnLogger *logger, uint16_t msgId);


#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_PARSER_LOGGING_COMMON_MQTTSNMESSAGELOGGING_H_
