//
// Created by SomeDude on 21.05.2019.
//

#ifndef CMQTTSNFORWARDER_PARSER_MQTTSNWILLMSGMESSAGE_H_
#define CMQTTSNFORWARDER_PARSER_MQTTSNWILLMSGMESSAGE_H_

#include <stdint.h>
#include "MqttSnMessageParser.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MQTT_SN_MESSAGE_WILLMSG_HEADER_LENGTH(indicator)    (MQTT_SN_HEADER_LENGTH(indicator))
#define MQTT_SN_MESSAGE_WILLMSG_WILLMSG_MIN_LENGTH          MQTT_SN_WILLMSG_MIN_LENGTH
#define MQTT_SN_MESSAGE_WILLMSG_MIN_LENGTH                  (MQTT_SN_MESSAGE_WILLMSG_HEADER_LENGTH(0) + \
                                                            MQTT_SN_MESSAGE_WILLMSG_WILLMSG_MIN_LENGTH)
#define MQTT_SN_MESSAGE_WO_WILLMSG_MAX_LENGTH               (MQTT_SN_MESSAGE_WILLMSG_HEADER_LENGTH(1) + \
                                                            MQTT_SN_MESSAGE_WILLMSG_WILLMSG_MIN_LENGTH)
#define MQTT_SN_MESSAGE_WILLMSG_WILLMSG_MAX_LENGTH          (MQTT_SN_WILLMSG_MAX_LENGTH - MQTT_SN_MESSAGE_WO_WILLMSG_MAX_LENGTH)

int32_t get_will_msg_msg_len(uint16_t data_len);

int32_t generate_will_msg(uint8_t *dst,
                          uint16_t dst_len,
                          const uint8_t *will_data,
                          uint16_t will_data_len);

int32_t parse_will_msg_byte(char *will_msg,
                            uint16_t *will_msg_length,
                            uint16_t will_msg_max_length,
                            const uint8_t *data,
                            uint16_t data_len);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_PARSER_MQTTSNWILLMSGMESSAGE_H_
