//
// Created by SomeDude on 23.05.2019.
//

#ifndef CMQTTSNFORWARDER_PARSER_MQTTSNWILLMSGRESPMESSAGE_H_
#define CMQTTSNFORWARDER_PARSER_MQTTSNWILLMSGRESPMESSAGE_H_

#include <stdint.h>
#include "MqttSnMessageParser.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MQTT_SN_MESSAGE_WILLMSGRESP_HEADER_LENGTH       MQTT_SN_MESSAGE_HEADER_SHORT_LENGTH
#define MQTT_SN_MESSAGE_WILLMSGRESP_RETURNCODE_LENGTH   MQTT_SN_RETURNCODE_LENGTH
#define MQTT_SN_MESSAGE_WILLMSGRESP_LENGTH              (MQTT_SN_MESSAGE_WILLMSGRESP_HEADER_LENGTH + MQTT_SN_MESSAGE_WILLMSGRESP_RETURNCODE_LENGTH)

int32_t parse_will_msg_resp_byte(MQTT_SN_RETURN_CODE *return_code,
                                 const uint8_t *data,
                                 uint16_t data_len);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_PARSER_MQTTSNWILLMSGRESPMESSAGE_H_
