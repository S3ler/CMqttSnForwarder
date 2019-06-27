//
// Created by SomeDude on 21.05.2019.
//

#ifndef CMQTTSNFORWARDER_PARSER_MQTTSNCONNECTMESSAGE_H_
#define CMQTTSNFORWARDER_PARSER_MQTTSNCONNECTMESSAGE_H_

#include <stdint.h>
#include "MqttSnMessageParser.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MQTT_SN_MESSAGE_CONNECT_HEADER_LENGTH         MQTT_SN_MESSAGE_HEADER_SHORT_LENGTH
#define MQTT_SN_MESSAGE_CONNECT_FLAGS_LENGTH          MQTT_SN_FLAGS_LENGTH
#define MQTT_SN_MESSAGE_CONNECT_PROTOCOLID_LENGTH     MQTT_SN_PROTOCOLID_LENGTH
#define MQTT_SN_MESSAGE_CONNECT_DURATION_LENGTH       MQTT_SN_DURATION_LENGTH
#define MQTT_SN_MESSAGE_CONNECT_CLIENTID_MIN_LENGTH   MQTT_SN_CLIENTID_MIN_LENGTH
#define MQTT_SN_MESSAGE_CONNECT_CLIENTID_MAX_LENGTH   MQTT_SN_CLIENTID_MAX_LENGTH
#define MQTT_SN_MESSAGE_CONNECT_WO_CLIENTID_LENGTH    (MQTT_SN_MESSAGE_CONNECT_HEADER_LENGTH + \
                                                       MQTT_SN_MESSAGE_CONNECT_FLAGS_LENGTH + \
                                                       MQTT_SN_MESSAGE_CONNECT_PROTOCOLID_LENGTH + \
                                                       MQTT_SN_MESSAGE_CONNECT_DURATION_LENGTH)
#define MQTT_SN_MESSAGE_CONNECT_MIN_LENGTH            (MQTT_SN_MESSAGE_CONNECT_WO_CLIENTID_LENGTH + \
                                                       MQTT_SN_MESSAGE_CONNECT_CLIENTID_MIN_LENGTH)
#define MQTT_SN_MESSAGE_CONNECT_MAX_LENGTH            (MQTT_SN_MESSAGE_CONNECT_WO_CLIENTID_LENGTH + \
                                                       MQTT_SN_MESSAGE_CONNECT_CLIENTID_MAX_LENGTH)

typedef struct ParsedMqttSnConnect_ {
  uint8_t will;
  uint8_t clean_session;
  uint8_t protocol_id;
  uint16_t duration;
  char client_id[MQTT_SN_MESSAGE_CONNECT_CLIENTID_MAX_LENGTH];
  uint16_t client_id_length;
} ParsedMqttSnConnect;

int32_t parse_connect(ParsedMqttSnConnect *mqtt_sn_connect, const uint8_t *data, uint16_t data_len);

int32_t parse_connect_message_byte(uint8_t *will,
                                   uint8_t *clean_session,
                                   uint8_t *protocol_id,
                                   uint16_t *duration,
                                   char *client_id,
                                   uint16_t *client_id_length,
                                   uint8_t client_id_max_length,
                                   const uint8_t *data,
                                   uint16_t data_len);
int32_t generate_connect(uint8_t *dst,
                         uint16_t dst_len,
                         uint8_t will,
                         uint8_t clean_session,
                         uint8_t protocol_id,
                         uint16_t duration,
                         const char *client_id);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_PARSER_MQTTSNCONNECTMESSAGE_H_
