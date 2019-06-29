//
// Created by SomeDude on 23.05.2019.
//

#ifndef CMQTTSNFORWARDER_PARSER_MQTTSNPINGREQMESSAGE_H_
#define CMQTTSNFORWARDER_PARSER_MQTTSNPINGREQMESSAGE_H_

#include <stdint.h>
#include "MqttSnMessageParser.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MQTT_SN_MESSAGE_PINGREQ_HEADER_LENGTH             MQTT_SN_MESSAGE_HEADER_SHORT_LENGTH
#define MQTT_SN_MESSAGE_PINGREQ_CLIENTID_MIN_LENGTH       MQTT_SN_CLIENTID_MIN_LENGTH
#define MQTT_SN_MESSAGE_PINGREQ_CLIENTID_MAX_LENGTH       MQTT_SN_CLIENTID_MAX_LENGTH
#define MQTT_SN_MESSAGE_PINGREQ_WO_CLIENTID_LENGTH        MQTT_SN_MESSAGE_PINGREQ_HEADER_LENGTH
#define MQTT_SN_MESSAGE_PINGREQ_LENGTH                    MQTT_SN_MESSAGE_PINGREQ_WO_CLIENTID_LENGTH
#define MQTT_SN_MESSAGE_PINGREQ_MIN_LENGTH                MQTT_SN_MESSAGE_PINGREQ_HEADER_LENGTH
#define MQTT_SN_MESSAGE_PINGREQ_W_CLIENTID_MIN_LENGTH     (MQTT_SN_MESSAGE_PINGREQ_WO_CLIENTID_LENGTH + MQTT_SN_MESSAGE_PINGREQ_CLIENTID_MIN_LENGTH)
#define MQTT_SN_MESSAGE_PINGREQ_W_CLIENTID_MAX_LENGTH     (MQTT_SN_MESSAGE_PINGREQ_WO_CLIENTID_LENGTH + MQTT_SN_MESSAGE_PINGREQ_CLIENTID_MAX_LENGTH)

int32_t parse_ping_req_w_client_id_byte(char *client_id,
                                        uint16_t *client_id_length,
                                        uint8_t client_id_max_length,
                                        const uint8_t *data,
                                        uint16_t data_len);
int32_t parse_ping_req_byte(const uint8_t *data, uint16_t data_len);
int32_t generate_ping_req(uint8_t *dst, uint16_t dst_len);
#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_PARSER_MQTTSNPINGREQMESSAGE_H_
