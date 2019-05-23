//
// Created by SomeDude on 23.05.2019.
//

#ifndef CMQTTSNFORWARDER_PARSER_MQTTSNWILLMSGUPDMESSAGE_H_
#define CMQTTSNFORWARDER_PARSER_MQTTSNWILLMSGUPDMESSAGE_H_

#include <stdint.h>
#include "MqttSnMessageParser.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MQTT_SN_MESSAGE_WILLMSGUPD_HEADER_LENGTH(indicator)     (MQTT_SN_HEADER_LENGTH(indicator))
#define MQTT_SN_MESSAGE_WILLMSGUPD_WILLMSG_MIN_LENGTH           MQTT_SN_WILLMSG_MIN_LENGTH
#define MQTT_SN_MESSAGE_WILLMSGUPD_WO_WILLMSG_MAX_LENGTH        MQTT_SN_MESSAGE_WILLMSGUPD_HEADER_LENGTH(1)
#define MQTT_SN_MESSAGE_WILLMSGUPD_MIN_LENGTH                   MQTT_SN_MESSAGE_WILLMSGUPD_HEADER_LENGTH(0)
#define MQTT_SN_MESSAGE_WILLMSGUPD_WILLMSG_MAX_LENGTH           (MQTT_SN_WILLMSG_MAX_LENGTH - MQTT_SN_MESSAGE_WILLMSGUPD_WO_WILLMSG_MAX_LENGTH)

int32_t parse_will_msg_upd_byte(const uint8_t *data,
                                uint16_t data_len,
                                uint8_t *will_msg,
                                uint16_t *will_msg_length,
                                uint16_t will_msg_max_length);

#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_PARSER_MQTTSNWILLMSGUPDMESSAGE_H_
