//
// Created by SomeDude on 21.05.2019.
//

#ifndef CMQTTSNFORWARDER_PARSER_MQTTSNPUBLISHMESSAGE_H_
#define CMQTTSNFORWARDER_PARSER_MQTTSNPUBLISHMESSAGE_H_

#include <stdint.h>
#include "MqttSnMessageParser.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MQTT_SN_MESSAGE_PUBLISH_HEADER_LENGTH(indicator)       (MQTT_SN_HEADER_LENGTH(indicator))
#define MQTT_SN_MESSAGE_PUBLISH_FLAGS_LENGTH                    MQTT_SN_FLAGS_LENGTH
#define MQTT_SN_MESSAGE_PUBLISH_TOPICID_LENGTH                  MQTT_SN_TOPIC_ID_LENGTH
#define MQTT_SN_MESSAGE_PUBLISH_MSGID_LENGTH                    MQTT_SN_MESSAGE_ID_LENGTH
#define MQTT_SN_MESSAGE_PUBLISH_DATA_MIN_LENGTH                 MQTT_SN_DATA_MIN_LENGTH
#define MQTT_SN_MESSAGE_PUBLISH_MIN_LENGTH                      (MQTT_SN_MESSAGE_PUBLISH_HEADER_LENGTH(0) + \
                                                                MQTT_SN_MESSAGE_PUBLISH_FLAGS_LENGTH + \
                                                                MQTT_SN_MESSAGE_PUBLISH_TOPICID_LENGTH + \
                                                                MQTT_SN_MESSAGE_PUBLISH_MSGID_LENGTH + \
                                                                MQTT_SN_MESSAGE_PUBLISH_DATA_MIN_LENGTH)
#define MQTT_SN_MESSAGE_PUBLISH_WO_DATA_MAX_LENGTH              (MQTT_SN_MESSAGE_PUBLISH_HEADER_LENGTH(1) + \
                                                                MQTT_SN_MESSAGE_PUBLISH_FLAGS_LENGTH + \
                                                                MQTT_SN_MESSAGE_PUBLISH_TOPICID_LENGTH + \
                                                                MQTT_SN_MESSAGE_PUBLISH_MSGID_LENGTH)
#define MQTT_SN_MESSAGE_PUBLISH_MAX_LENGTH                      (MQTT_SN_DATA_MAX_LENGTH - MQTT_SN_MESSAGE_PUBLISH_WO_DATA_MAX_LENGTH)

int32_t parse_publish_byte(uint8_t *dup,
                           int8_t *qos,
                           uint8_t *retain,
                           uint8_t *topic_id_type,
                           uint16_t *topic_id,
                           uint16_t *msg_id,
                           uint8_t *data,
                           uint16_t *data_length,
                           uint16_t data_max_length,
                           const uint8_t *src,
                           uint16_t src_len);
int32_t generate_publish(uint8_t *dst,
                         uint16_t dst_len,
                         uint8_t dup,
                         int8_t qos,
                         uint8_t retain,
                         uint8_t topic_id_type,
                         uint32_t topic_id,
                         uint16_t msg_id,
                         uint8_t *data,
                         uint16_t data_len);
#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_PARSER_MQTTSNPUBLISHMESSAGE_H_
