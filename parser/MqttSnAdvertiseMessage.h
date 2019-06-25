//
// Created by bele on 19.05.19.
//

#ifndef CMQTTSNFORWARDER_PARSER_MQTTSNADVERTISEMESSAGE_H_
#define CMQTTSNFORWARDER_PARSER_MQTTSNADVERTISEMESSAGE_H_

#include <stdint.h>
#include "MqttSnMessageParser.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MQTT_SN_MESSAGE_ADVERTISE_HEADER_LENGTH     MQTT_SN_MESSAGE_HEADER_SHORT_LENGTH
#define MQTT_SN_MESSAGE_ADVERTISE_GWID_LENGTH       MQTT_SN_GWID_LENGTH
#define MQTT_SN_MESSAGE_ADVERTISE_DURATION_LENGTH   MQTT_SN_DURATION_LENGTH
#define MQTT_SN_MESSAGE_ADVERTISE_LENGTH           (MQTT_SN_MESSAGE_ADVERTISE_HEADER_LENGTH + \
                                                    MQTT_SN_MESSAGE_ADVERTISE_GWID_LENGTH + \
                                                    MQTT_SN_MESSAGE_ADVERTISE_DURATION_LENGTH)

#pragma pack(push, 1)
typedef struct MqttSnAdvertise_ {
  uint8_t gwId;
  uint16_t duration;
} MqttSnAdvertise;
#pragma pack(pop)

int32_t parse_advertise(MqttSnAdvertise *mqtt_sn_advertise, const uint8_t *data, uint16_t data_len);
int32_t parse_advertise_byte(uint8_t *gw_id, uint16_t *duration, const uint8_t *data, uint16_t data_len);

/*
int32_t parse_advertise(uint8_t *gw_id, uint16_t *duration, const uint8_t *data, uint16_t data_len);

int32_t parse_advertise_to_structs(ParsedMqttSnHeader *h,
                                   MqttSnAdvertise *advertise,
                                   const uint8_t *data,
                                   uint16_t data_len);
*/
int32_t generate_advertise_message(uint8_t *dst, uint16_t dst_len, uint8_t gw_id, uint16_t duration);
#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_PARSER_MQTTSNADVERTISEMESSAGE_H_
