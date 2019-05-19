//
// Created by bele on 19.05.19.
//

#ifndef CMQTTSNFORWARDER_PARSER_MQTTSNADVERTISEMESSAGE_H_
#define CMQTTSNFORWARDER_PARSER_MQTTSNADVERTISEMESSAGE_H_

#include <stdint.h>
#include "MqttSnMessageParser.h"

//#define MQTT_SN_MESSAGE_ADVERTISE_MIN_LENGTH 5
#define MQTT_SN_MESSAGE_ADVERTISE_LENGTH 5
//#define MQTT_SN_MESSAGE_ADVERTISE_MAX_LENGTH 5

#pragma pack(push, 1)
typedef struct MqttSnAdvertise_ {
  uint8_t gwId;
  uint16_t duration;
} MqttSnAdvertise;
#pragma pack(pop)

int32_t parse_advertise(uint8_t *gw_id, uint16_t *duration, const uint8_t *data, uint16_t data_len);
int32_t parse_advertise_to_structs(ParsedMqttSnHeader *h,
                                   MqttSnAdvertise *advertise,
                                   const uint8_t *data,
                                   uint16_t data_len);

int32_t parse_advertise_duration(uint16_t *duration, ParsedMqttSnHeader *h, int32_t *parsed_bytes);

int32_t generate_advertise(uint8_t *dst, uint16_t dst_len, uint8_t gw_id, uint16_t duration);


#endif //CMQTTSNFORWARDER_PARSER_MQTTSNADVERTISEMESSAGE_H_