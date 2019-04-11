//
// Created by SomeDude on 09.04.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNMESSAGEPARSER_H
#define CMQTTSNFORWARDER_MQTTSNMESSAGEPARSER_H

#include <stdint.h>
#include <unistd.h>
#include "global_defines.h"

typedef enum MQTT_SN_MESSAGE_TYPE_ {
  ANY_MESSAGE_TYPE = -2,
  RESERVED_INVALID = -1,
  ADVERTISE = 0x00,
  SEARCHGW = 0x01,
  GWINFO = 0x02,
  RESERVED_03 = 0x03,
  CONNECT = 0x04,
  CONNACK = 0x05,
  WILLTOPICREQ = 0x06,
  WILLTOPIC = 0x07,
  WILLMSGREQ = 0x08,
  WILLMSG = 0x09,
  REGISTER = 0x0A,
  REGACK = 0x0B,
  PUBLISH = 0x0C,
  PUBACK = 0x0D,
  PUBCOMP = 0x0E,
  PUBREC = 0x0F,
  PUBREL = 0x10,
  RESERVED_11 = 0x11,
  SUBSCRIBE = 0x12,
  SUBACK = 0x13,
  UNSUBSCRIBE = 0x14,
  UNSUBACK = 0x15,
  PINGREQ = 0x16,
  PINGRESP = 0x17,
  DISCONNECT = 0x18,
  RESERVED_19 = 0x19,
  WILLTOPICUPD = 0x1A,
  WILLTOPICRESP = 0x1B,
  WILLMSGUPD = 0x1C,
  WILLMSGRESP = 0x1D,
  RESERVED_1E = 0x1E,
  RESERVED_FD = 0xFD,
  ENCAPSULATED_MESSAGE = 0xFE,
  RESERVED_FF = 0xFF,
} MQTT_SN_MESSAGE_TYPE;

#define MQTT_SN_FLAG_DUP            0x80 // 0b1000 0000
#define MQTT_SN_FLAG_QOS            0x60 // 0b0110 0000
#define MQTT_SN_FLAG_RETAIN         0x10 // 0b0001 0000
#define MQTT_SN_FLAG_WILL           0x08 // 0b0000 1000
#define MQTT_SN_FLAG_CLEAN_SESSION  0x04 // 0b0000 0100
#define MQTT_SN_FLAG_TOPIC_ID_TYPE  0x03 // 0b0000 0011

#define MQTT_SN_FLAG_DUP_SHIFT            7
#define MQTT_SN_FLAG_QOS_SHIFT            5
#define MQTT_SN_FLAG_RETAIN_SHIFT         4
#define MQTT_SN_FLAG_WILL_SHIFT           3
#define MQTT_SN_FLAG_CLEAN_SESSION_SHIFT  2
#define MQTT_SN_FLAG_TOPIC_ID_TYPE_SHIFT  0

#define MQTT_SN_DUP_FLAG(flags)       ((flags & MQTT_SN_FLAG_DUP) >> MQTT_SN_FLAG_DUP_SHIFT)
#define MQTT_SN_QOS_FLAG(flags)       ((flags & MQTT_SN_FLAG_QOS) >> MQTT_SN_FLAG_QOS_SHIFT)
#define MQTT_SN_RETAIN_FLAG(flags)    ((flags & MQTT_SN_FLAG_RETAIN) >> MQTT_SN_FLAG_RETAIN_SHIFT) == 2 ? \
                                      -1 : ((flags & MQTT_SN_FLAG_RETAIN) >> MQTT_SN_FLAG_RETAIN_SHIFT)
#define MQTT_SN_WILL_FLAG(flags)      ((flags & MQTT_SN_FLAG_WILL) >> MQTT_SN_FLAG_WILL_SHIFT)
#define MQTT_SN_CLEAN_SESSION(flags)  ((flags & MQTT_SN_FLAG_CLEAN_SESSION) >> MQTT_SN_FLAG_CLEAN_SESSION_SHIFT)
#define MQTT_SN_TOPIC_ID_TYPE(flags)  ((flags & MQTT_SN_FLAG_TOPIC_ID_TYPE) >> MQTT_SN_FLAG_TOPIC_ID_TYPE_SHIFT);

#define MQTT_SN_HEADER_OFFSET_LENGTH(indicator)                 (indicator ? 4 : 2)
#define MQTT_SN_ENCAPSULATION_MESSAGE_CRTL_BYTE_LENGTH          1
#define MQTT_SN_ENCAPSULATION_MESSAGE_HEADER_LENGTH(indicator)  (MQTT_SN_HEADER_OFFSET_LENGTH(indicator) \
                                                                 + MQTT_SN_ENCAPSULATION_MESSAGE_CRTL_BYTE_LENGTH \
                                                                 + sizeof(device_address))

#pragma pack(push, 1)
typedef struct MqttSnEncapsulatedMessage_ {
  uint8_t crtl;
  device_address wireless_node_id;
  uint8_t mqtt_sn_message;
} MqttSnEncapsulatedMessage;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct MqttSnMessageHeaderOneOctetLengthField {
  uint8_t length;
  uint8_t msg_type;
} MqttSnMessageHeaderOneOctetLengthField;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct MqttSnMessageHeaderThreeOctetsLengthField {
  uint8_t indicator;
  uint16_t length;
  uint8_t msg_type;
} MqttSnMessageHeaderThreeOctetsLengthField;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct ParsedMqttSnHeader_ {
  uint8_t indicator;
  uint16_t length;
  MQTT_SN_MESSAGE_TYPE msg_type;
  void *payload;
} ParsedMqttSnHeader;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct MqttSnMessagePublish_ {
  uint8_t flags;
  uint16_t topicId;
  uint16_t msgId;
  uint16_t *data;
} MqttSnMessagePublish;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct MqttSnMessageConnect_ {
  uint8_t flags;
  uint8_t protocolId;
  uint16_t duration;
  char clientId[24];
} MqttSnMessageConnect;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct MqttSnMessageConnack_ {
  uint8_t returnCode;
} MqttSnMessageConnack;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct MqttSnMessageDisconnect_ {
  uint16_t duration;
} MqttSnMessageDisconnect;
#pragma pack(pop)

int parse_header(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len);

int parse_message(ParsedMqttSnHeader *h, MQTT_SN_MESSAGE_TYPE msg_type, const uint8_t *data, uint16_t data_len);

int parse_encapsulation(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len);

int parse_publish(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len);

int parse_connect(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len);

int parse_connack(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len);

int parse_disconnect(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len);

uint16_t get_message_length(const uint8_t *data);

MQTT_SN_MESSAGE_TYPE get_mqtt_sn_message_type(const uint8_t *data);

int is_valid_three_bytes_header(const uint8_t *data, ssize_t data_len);

uint8_t is_three_bytes_header(const uint8_t *data);

#endif //CMQTTSNFORWARDER_MQTTSNMESSAGEPARSER_H
