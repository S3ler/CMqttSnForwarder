//
// Created by SomeDude on 21.05.2019.
//

#ifndef CMQTTSNFORWARDER_PARSER_MQTTSNREGACKMESSAGE_H_
#define CMQTTSNFORWARDER_PARSER_MQTTSNREGACKMESSAGE_H_

#include <stdint.h>
#include "MqttSnMessageParser.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MQTT_SN_MESSAGE_REGACK_HEADER_LENGTH MQTT_SN_MESSAGE_HEADER_SHORT_LENGTH
#define MQTT_SN_MESSAGE_REGACK_TOPICID_LENGTH MQTT_SN_TOPIC_ID_LENGTH
#define MQTT_SN_MESSAGE_REGACK_MESSAGEID_LENGTH MQTT_SN_MESSAGE_ID_LENGTH
#define MQTT_SN_MESSAGE_REGACK_RETURNCODE_LENGTH MQTT_SN_RETURNCODE_LENGTH
#define MQTT_SN_MESSAGE_REGACK_LENGTH                                                                                                      \
    (MQTT_SN_MESSAGE_REGACK_HEADER_LENGTH + MQTT_SN_MESSAGE_REGACK_TOPICID_LENGTH + MQTT_SN_MESSAGE_REGACK_MESSAGEID_LENGTH +              \
     MQTT_SN_MESSAGE_REGACK_RETURNCODE_LENGTH)

#pragma pack(push, 1)
typedef struct MqttSnRegAck_ {
    uint16_t            topic_id;
    uint16_t            msg_id;
    MQTT_SN_RETURN_CODE return_code;
} MqttSnRegack;
#pragma pack(pop)

int32_t generate_regack(uint8_t *dst, uint16_t dst_len, uint16_t topic_id, uint16_t msg_id, MQTT_SN_RETURN_CODE return_code);

int32_t parse_regack(MqttSnRegack *mqttSnRegAck, const uint8_t *data, uint16_t data_len);

int32_t parse_regack_bytes(uint16_t *topic_id, uint16_t *msg_id, MQTT_SN_RETURN_CODE *return_code, const uint8_t *data, uint16_t data_len);

#ifdef __cplusplus
}
#endif
#endif  //CMQTTSNFORWARDER_PARSER_MQTTSNREGACKMESSAGE_H_
