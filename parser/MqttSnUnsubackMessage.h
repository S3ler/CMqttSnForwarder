//
// Created by SomeDude on 23.05.2019.
//

#ifndef CMQTTSNFORWARDER_PARSER_MQTTSNUNSUBACKMESSAGE_H_
#define CMQTTSNFORWARDER_PARSER_MQTTSNUNSUBACKMESSAGE_H_

#include <stdint.h>
#include "MqttSnMessageParser.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MQTT_SN_MESSAGE_UNSUBACK_HEADER_LENGTH      MQTT_SN_MESSAGE_HEADER_SHORT_LENGTH
#define MQTT_SN_MESSAGE_UNSUBACK_MESSAGEID_LENGTH   MQTT_SN_MESSAGE_ID_LENGTH
#define MQTT_SN_MESSAGE_UNSUBACK_LENGTH             (MQTT_SN_MESSAGE_UNSUBACK_HEADER_LENGTH + \
                                                    MQTT_SN_MESSAGE_UNSUBACK_MESSAGEID_LENGTH)

#pragma pack(push, 1)
typedef struct MqttSnUnsuback_ {
    uint16_t            topic_id;
    uint16_t            msg_id;
} MqttSnUnsuback;
#pragma pack(pop)

int32_t parse_unsuback(MqttSnUnsuback* unsuback, const uint8_t* data, uint16_t data_len);

int32_t parse_unsuback_byte(uint16_t *msg_id,
                            const uint8_t *data,
                            uint16_t data_len);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_PARSER_MQTTSNUNSUBACKMESSAGE_H_
