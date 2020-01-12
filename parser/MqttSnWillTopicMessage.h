//
// Created by SomeDude on 21.05.2019.
//

#ifndef CMQTTSNFORWARDER_PARSER_MQTTSNWILLTOPICMESSAGE_H_
#define CMQTTSNFORWARDER_PARSER_MQTTSNWILLTOPICMESSAGE_H_

#include <stdint.h>
#include "MqttSnMessageParser.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MQTT_SN_MESSAGE_WILLTOPIC_HEADER_LENGTH(indicator)      (MQTT_SN_HEADER_LENGTH(indicator))
#define MQTT_SN_MESSAGE_WILLTOPIC_FLAGS_LENGTH                  MQTT_SN_FLAGS_LENGTH
#define MQTT_SN_MESSAGE_WILLTOPIC_WILLTOPIC_MIN_LENGTH          MQTT_SN_WILLTOPIC_MIN_LENGTH
#define MQTT_SN_MESSAGE_WILLTOPIC_MIN_LENGTH                    (MQTT_SN_MESSAGE_WILLTOPIC_HEADER_LENGTH(0) + \
                                                                MQTT_SN_MESSAGE_WILLTOPIC_FLAGS_LENGTH + \
                                                                MQTT_SN_MESSAGE_WILLTOPIC_WILLTOPIC_MIN_LENGTH)
#define MQTT_SN_MESSAGE_WILLTOPIC_WO_WILLTOPIC_MAX_LENGTH       (MQTT_SN_MESSAGE_WILLTOPIC_HEADER_LENGTH(1) + \
                                                                MQTT_SN_MESSAGE_WILLTOPIC_FLAGS_LENGTH)
#define MQTT_SN_MESSAGE_WILLTOPIC_WILLTOPIC_MAX_LENGTH          (MQTT_SN_WILLTOPIC_MAX_LENGTH - MQTT_SN_MESSAGE_WILLTOPIC_WO_WILLTOPIC_MAX_LENGTH)

int32_t get_will_topic_msg_len(uint16_t data_len);

int32_t generate_will_topic(uint8_t *dst,
                            uint16_t dst_len,
                            int8_t will_qos,
                            uint8_t will_retain,
                            const char* will_topic,
                            uint16_t will_topic_length);

int32_t parse_will_topic_byte(int8_t *qos,
                              uint8_t *retain,
                              char *will_topic,
                              uint16_t *will_topic_length,
                              uint16_t will_topic_max_length,
                              const uint8_t *data,
                              uint16_t data_len);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_PARSER_MQTTSNWILLTOPICMESSAGE_H_
