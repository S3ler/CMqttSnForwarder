//
// Created by SomeDude on 23.05.2019.
//

#ifndef CMQTTSNFORWARDER_PARSER_MQTTSNUNSUBSCRIBEMESSAGE_H_
#define CMQTTSNFORWARDER_PARSER_MQTTSNUNSUBSCRIBEMESSAGE_H_

#include <stdint.h>
#include "MqttSnMessageParser.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MQTT_SN_MESSAGE_UNSUBSCRIBE_HEADER_LENGTH(indicator) (MQTT_SN_HEADER_LENGTH(indicator))
#define MQTT_SN_MESSAGE_UNSUBSCRIBE_FLAGS_LENGTH MQTT_SN_FLAGS_LENGTH
#define MQTT_SN_MESSAGE_UNSUBSCRIBE_MSG_ID_LENGTH MQTT_SN_MESSAGE_ID_LENGTH
#define MQTT_SN_MESSAGE_UNSUBSCRIBE_TOPIC_ID_LENGTH MQTT_SN_TOPIC_ID_LENGTH
#define MQTT_SN_MESSAGE_UNSUBSCRIBE_W_TOPICID_LENGTH                                                                                       \
    (MQTT_SN_MESSAGE_UNSUBSCRIBE_HEADER_LENGTH(0) + MQTT_SN_MESSAGE_UNSUBSCRIBE_FLAGS_LENGTH + MQTT_SN_MESSAGE_UNSUBSCRIBE_MSG_ID_LENGTH + \
     MQTT_SN_MESSAGE_UNSUBSCRIBE_TOPIC_ID_LENGTH)
#define MQTT_SN_MESSAGE_UNSUBSCRIBE_TOPICNAME_MIN_LENGTH MQTT_SN_TOPICNAME_MIN_LENGTH
#define MQTT_SN_MESSAGE_UNSUBSCRIBE_W_TOPICNAME_MIN_LENGTH                                                                                 \
    (MQTT_SN_MESSAGE_UNSUBSCRIBE_HEADER_LENGTH(0) + MQTT_SN_MESSAGE_UNSUBSCRIBE_FLAGS_LENGTH + MQTT_SN_MESSAGE_UNSUBSCRIBE_MSG_ID_LENGTH + \
     MQTT_SN_MESSAGE_UNSUBSCRIBE_TOPICNAME_MIN_LENGTH)
#define MQTT_SN_MESSAGE_UNSUBSCRIBE_MIN_LENGTH MQTT_SN_MESSAGE_UNSUBSCRIBE_W_TOPICNAME_MIN_LENGTH
#define MQTT_SN_MESSAGE_UNSUBSCRIBE_WO_TOPICNAME_MIN_LENGTH                                                                                \
    (MQTT_SN_MESSAGE_UNSUBSCRIBE_W_TOPICNAME_MIN_LENGTH - MQTT_SN_MESSAGE_UNSUBSCRIBE_TOPICNAME_MIN_LENGTH)
#define MQTT_SN_MESSAGE_UNSUBSCRIBE_WO_TOPICNAME_MAX_LENGTH                                                                                \
    (MQTT_SN_MESSAGE_UNSUBSCRIBE_HEADER_LENGTH(1) + MQTT_SN_MESSAGE_UNSUBSCRIBE_FLAGS_LENGTH + MQTT_SN_MESSAGE_UNSUBSCRIBE_MSG_ID_LENGTH)
#define MQTT_SN_MESSAGE_UNSUBSCRIBE_TOPICNAME_MAX_LENGTH                                                                                   \
    (MQTT_SN_TOPICNAME_MAX_LENGTH - MQTT_SN_MESSAGE_UNSUBSCRIBE_WO_TOPICNAME_MAX_LENGTH)
#define MQTT_SN_MESSAGE_UNSUBSCRIBE_W_TOPICNAME_MAX_LENGTH                                                                                 \
    (MQTT_SN_MESSAGE_UNSUBSCRIBE_WO_TOPICNAME_MAX_LENGTH + MQTT_SN_MESSAGE_UNSUBSCRIBE_WO_TOPICNAME_MAX_LENGTH)

int32_t get_unsubscribe_msg_len(uint16_t topic_name_length);
int32_t generate_unsubscribe(uint8_t *dst, uint16_t dst_len, uint8_t dup, uint8_t topic_id_type, uint16_t msg_id, uint16_t topic_id,
                             const char *topic_name, uint16_t topic_name_length);
int32_t parse_unsubscribe_byte(uint8_t *dup, int8_t *qos, uint8_t *topic_id_type, uint16_t *msg_id, uint16_t *topic_id, char *topic_name,
                               uint16_t *topic_name_length, uint16_t topic_name_max_length, const uint8_t *data, uint16_t data_len);
#ifdef __cplusplus
}
#endif
#endif  //CMQTTSNFORWARDER_PARSER_MQTTSNUNSUBSCRIBEMESSAGE_H_
