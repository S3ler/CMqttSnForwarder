//
// Created by SomeDude on 19.05.2019.
//

#ifndef CMQTTSNFORWARDER_PARSER_MQTTSNPUBLISHMESSAGEPARSER_H_
#define CMQTTSNFORWARDER_PARSER_MQTTSNPUBLISHMESSAGEPARSER_H_

#include "MqttSnMessageParser.h"

int parse_publish(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len);

int generate_publish(uint8_t *dst,
                     uint16_t dst_len,
                     uint8_t dup,
                     int8_t qos,
                     uint8_t retain,
                     uint8_t topic_id_type,
                     uint16_t msg_id,
                     uint32_t topic_id,
                     uint8_t *data,
                     uint16_t data_len);

#endif //CMQTTSNFORWARDER_PARSER_MQTTSNPUBLISHMESSAGEPARSER_H_
