//
// Created by SomeDude on 23.05.2019.
//

#ifndef CMQTTSNFORWARDER_PARSER_MQTTSNPINGRESPMESSAGE_H_
#define CMQTTSNFORWARDER_PARSER_MQTTSNPINGRESPMESSAGE_H_

#include <stdint.h>
#include "MqttSnMessageParser.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MQTT_SN_MESSAGE_PINGRESP_HEADER_LENGTH    MQTT_SN_MESSAGE_HEADER_SHORT_LENGTH
#define MQTT_SN_MESSAGE_PINGRESP_LENGTH            MQTT_SN_MESSAGE_PINGRESP_HEADER_LENGTH

int32_t parse_ping_resp_byte(const uint8_t *data, uint16_t data_len);
int32_t generate_ping_resp(uint8_t *dst, uint16_t dst_len);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_PARSER_MQTTSNPINGRESPMESSAGE_H_
