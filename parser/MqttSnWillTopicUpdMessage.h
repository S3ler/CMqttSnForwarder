//
// Created by SomeDude on 23.05.2019.
//

#ifndef CMQTTSNFORWARDER_PARSER_MQTTSNWILLTOPICUPDMESSAGE_H_
#define CMQTTSNFORWARDER_PARSER_MQTTSNWILLTOPICUPDMESSAGE_H_

#include <stdint.h>
#include "MqttSnMessageParser.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MQTT_SN_MESSAGE_WILLTOPICUPD_HEADER_LENGTH(indicator)   (MQTT_SN_HEADER_LENGTH(indicator))
#define MQTT_SN_MESSAGE_WILLTOPICUPD_FLAGS_LENGTH               MQTT_SN_FLAGS_LENGTH
#define MQTT_SN_MESSAGE_WILLTOPICUPD_WILLTOPIC_MIN_LENGTH       MQTT_SN_WILLTOPIC_MIN_LENGTH
#define MQTT_SN_MESSAGE_WILLTOPICUPD_MIN_LENGTH                 (MQTT_SN_MESSAGE_WILLTOPICUPD_HEADER_LENGTH(0) + \
                                                                MQTT_SN_MESSAGE_WILLTOPICUPD_FLAGS_LENGTH + \
                                                                MQTT_SN_MESSAGE_WILLTOPICUPD_WILLTOPIC_MIN_LENGTH)
#define MQTT_SN_MESSAGE_WILLTOPICUPD_WO_WILLTOPIC_MAX_LENGTH    (MQTT_SN_MESSAGE_WILLTOPICUPD_HEADER_LENGTH(1) + \
                                                                MQTT_SN_MESSAGE_WILLTOPICUPD_FLAGS_LENGTH)
#define MQTT_SN_MESSAGE_WILLTOPICUPD_WILLTOPIC_MAX_LENGTH       (MQTT_SN_WILLTOPIC_MAX_LENGTH - MQTT_SN_MESSAGE_WILLTOPICUPD_WO_WILLTOPIC_MAX_LENGTH)
//#define MQTT_SN_MESSAGE_WILLTOPICUPD_MAX_LENGTH                 (MQTT_SN_WILLTOPIC_MAX_LENGTH - MQTT_SN_MESSAGE_WILLTOPIC_WO_WILLTOPIC_MAX_LENGTH)

int32_t parse_will_topic_upd_byte(const uint8_t *data,
                                  uint16_t data_len,
                                  char *will_topic,
                                  uint16_t *will_topic_length,
                                  uint16_t will_topic_max_length);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_PARSER_MQTTSNWILLTOPICUPDMESSAGE_H_
