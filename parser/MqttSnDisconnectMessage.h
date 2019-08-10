//
// Created by SomeDude on 23.05.2019.
//

#ifndef CMQTTSNFORWARDER_PARSER_MQTTSNDISCONNECTMESSAGE_H_
#define CMQTTSNFORWARDER_PARSER_MQTTSNDISCONNECTMESSAGE_H_

#include <stdint.h>
#include "MqttSnMessageParser.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MQTT_SN_MESSAGE_DISCONNECT_HEADER_LENGTH              MQTT_SN_MESSAGE_HEADER_SHORT_LENGTH
#define MQTT_SN_MESSAGE_DISCONNECT_DURATION_LENGTH            MQTT_SN_DURATION_LENGTH
#define MQTT_SN_MESSAGE_DISCONNECT_MIN_LENGTH                 MQTT_SN_MESSAGE_DISCONNECT_HEADER_LENGTH
#define MQTT_SN_MESSAGE_DISCONNECT_MAX_LENGTH                 (MQTT_SN_MESSAGE_DISCONNECT_HEADER_LENGTH + MQTT_SN_MESSAGE_DISCONNECT_DURATION_LENGTH)

int32_t parse_disconnect(const uint8_t *data, uint16_t data_len);

int32_t parse_disconnect_duration(uint16_t *duration, const uint8_t *data, uint16_t data_len);
int32_t generate_disconnect_message(uint8_t *dst, uint16_t dst_len);
int32_t generate_disconnect_duration(uint8_t *dst, uint16_t dst_len,uint16_t duration);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_PARSER_MQTTSNDISCONNECTMESSAGE_H_
