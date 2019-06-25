//
// Created by SomeDude on 21.05.2019.
//

#ifndef CMQTTSNFORWARDER_PARSER_MQTTSNCONNACKMESSAGE_H_
#define CMQTTSNFORWARDER_PARSER_MQTTSNCONNACKMESSAGE_H_

#include <stdint.h>
#include "MqttSnMessageParser.h"
#ifdef __cplusplus
extern "C" {
#endif

#define MQTT_SN_MESSAGE_CONNACK_HEADER_LENGTH       MQTT_SN_MESSAGE_HEADER_SHORT_LENGTH
#define MQTT_SN_MESSAGE_CONNACK_RETURNCODE_LENGTH   MQTT_SN_RETURNCODE_LENGTH
#define MQTT_SN_MESSAGE_CONNACK_LENGTH              (MQTT_SN_MESSAGE_CONNACK_HEADER_LENGTH + \
                                                     MQTT_SN_MESSAGE_CONNACK_RETURNCODE_LENGTH)

int32_t parse_connack_byte(MQTT_SN_RETURN_CODE *return_code, const uint8_t *data, uint16_t data_len);
int32_t generate_connack(uint8_t* dst, uint16_t dst_len, MQTT_SN_RETURN_CODE return_code);
#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_PARSER_MQTTSNCONNACKMESSAGE_H_
