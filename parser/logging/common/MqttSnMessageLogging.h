//
// Created by bele on 24.05.19.
//

#ifndef CMQTTSNFORWARDER_PARSER_LOGGING_COMMON_MQTTSNMESSAGELOGGING_H_
#define CMQTTSNFORWARDER_PARSER_LOGGING_COMMON_MQTTSNMESSAGELOGGING_H_

#include <logging/MqttSnLoggingInterface.h>
#include <parser/MqttSnMessageParser.h>

#ifdef __cplusplus
extern "C" {
#endif

// TODO move functionality to MqttSnMessageParserLogging

int32_t log_mqtt_sn_comma(const MqttSnLogger *logger);

int32_t log_mqtt_sn_message_type(const MqttSnLogger *logger, MQTT_SN_MESSAGE_TYPE msg_type);

// mqtt-sn specific logging

int32_t log_mqtt_sn_flags_single(const MqttSnLogger *logger,
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

int32_t log_any_message(const MqttSnLogger *logger,
                        const device_address *from,
                        uint8_t signal_strength,
                        const uint8_t *data,
                        uint16_t data_length);

int32_t print_any_message(const MqttSnLogger *logger,
                          const device_address *from,
                          uint8_t signal_strength,
                          const uint8_t *data,
                          uint16_t data_length);

int log_client_mqtt_sn_message_malformed(const MqttSnLogger *logger,
                                         const device_address *from,
                                         const uint8_t *data,
                                         uint16_t data_len,
                                         uint8_t signal_strength);
int log_mqtt_sn_message_malformed(const MqttSnLogger *logger,
                                  MQTT_SN_FORWARDER_NETWORK network,
                                  const device_address *from,
                                  const uint8_t *data,
                                  uint16_t data_len,
                                  uint8_t signal_strength);

int32_t log_malformed_message_header(const MqttSnLogger *logger,
                                     const device_address *from,
                                     uint8_t signal_strength,
                                     const uint8_t *data,
                                     uint16_t data_len);

int32_t log_malformed_message(const MqttSnLogger *logger,
                              MQTT_SN_MESSAGE_TYPE msg_type,
                              const device_address *from,
                              uint8_t signal_strength,
                              const uint8_t *data,
                              uint16_t data_len);

int32_t log_msg_gen_error(const MqttSnLogger *logger,
                          MQTT_SN_MESSAGE_TYPE msg_type,
                          const char *file_name,
                          const char *function_name,
                          int line_number);
#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_PARSER_LOGGING_COMMON_MQTTSNMESSAGELOGGING_H_
