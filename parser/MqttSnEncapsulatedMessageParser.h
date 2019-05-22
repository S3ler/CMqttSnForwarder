//
// Created by SomeDude on 19.05.2019.
//

#ifndef CMQTTSNFORWARDER_PARSER_MQTTSNENCAPSULATEDMESSAGEPARSER_H_
#define CMQTTSNFORWARDER_PARSER_MQTTSNENCAPSULATEDMESSAGEPARSER_H_

#include "MqttSnMessageParser.h"

int32_t parse_encapsulation_message(uint8_t *broadcast,
                                    device_address *wireless_node_id,
                                    uint8_t *mqtt_sn_message,
                                    uint16_t *mqtt_sn_message_len,
                                    uint16_t max_mqtt_sn_message_len,
                                    uint8_t *data,
                                    uint16_t data_len);

int parse_encapsulation_header(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len, int32_t *read_bytes);

#endif //CMQTTSNFORWARDER_PARSER_MQTTSNENCAPSULATEDMESSAGEPARSER_H_
