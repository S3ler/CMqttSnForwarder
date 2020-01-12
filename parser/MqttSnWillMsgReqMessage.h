//
// Created by SomeDude on 21.05.2019.
//

#ifndef CMQTTSNFORWARDER_PARSER_MQTTSNWILLMSGREQMESSAGE_H_
#define CMQTTSNFORWARDER_PARSER_MQTTSNWILLMSGREQMESSAGE_H_
#include <stdint.h>
#include "MqttSnMessageParser.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MQTT_SN_MESSAGE_WILLMSGREQ_HEADER_LENGTH   MQTT_SN_MESSAGE_HEADER_SHORT_LENGTH
#define MQTT_SN_MESSAGE_WILLMSGREQ_LENGTH          MQTT_SN_MESSAGE_WILLMSGREQ_HEADER_LENGTH

int32_t parse_will_msg_req_byte(const uint8_t *data, uint16_t data_len);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_PARSER_MQTTSNWILLMSGREQMESSAGE_H_
