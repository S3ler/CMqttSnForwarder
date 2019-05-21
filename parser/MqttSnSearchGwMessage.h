//
// Created by bele on 20.05.19.
//

#ifndef CMQTTSNFORWARDER_PARSER_MQTTSNSEARCHGWMESSAGE_H_
#define CMQTTSNFORWARDER_PARSER_MQTTSNSEARCHGWMESSAGE_H_

#include <stdint.h>
#include "MqttSnMessageParser.h"
#ifdef __cplusplus
extern "C" {
#endif

#define MQTT_SN_MESSAGE_SEARCHGW_HEADER_LENGTH     MQTT_SN_MESSAGE_HEADER_SHORT_LENGTH
#define MQTT_SN_RADIUS_SEARCHGW_LENGTH             MQTT_SN_RADIUS_LENGTH
#define MQTT_SN_MESSAGE_SEARCHGW_LENGTH            (MQTT_SN_MESSAGE_SEARCHGW_HEADER_LENGTH + \
                                                    MQTT_SN_RADIUS_SEARCHGW_LENGTH)

#pragma pack(push, 1)
typedef struct MqttSnSearchGw_ {
  uint8_t radius;
} MqttSnSearchGw;
#pragma pack(pop)

int32_t parse_searchgw_message_byte(uint8_t *gw_id, const uint8_t *data, uint16_t data_len);
#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_PARSER_MQTTSNSEARCHGWMESSAGE_H_
