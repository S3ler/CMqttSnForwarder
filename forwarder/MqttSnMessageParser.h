//
// Created by SomeDude on 09.04.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNMESSAGEPARSER_H
#define CMQTTSNFORWARDER_MQTTSNMESSAGEPARSER_H

#include <stdint.h>
#include <unistd.h>
#include "global_defines.h"

#define FORWARDER_HEADER_LEN 3

#define GATEWAY_NETWORK_MAX_DATA_LEN 255
#define CLIENT_NETWORK_MAX_DATA_LEN 255

//#define Encapsulated_message 0xFE
typedef enum MQTT_SN_MESSAGE_TYPE_ {
  RESERVED_INVALID = -1,
  ADVERTISE = 0x00,
  SEARCHGW = 0x01,
  GWINFO = 0x02,
  RESERVED_03 = 0x03,
  CONNECT = 0x04,
  CONNACK = 0x05,
  REGISTER = 0x0A,
  REGACK = 0x0B,
  PUBLISH = 0x0C,
  PUBACK = 0x0D,
  RESERVED_11 = 0x11,
  SUBSCRIBE = 0x12,
  SUBACK = 0x13,
  UNSUBSCRIBE = 0x14,
  UNSUBACK = 0x15,
  PINGREQ = 0x16,
  PINGRESP = 0x17,
  DISCONNECT = 0x18,
  RESERVED_19 = 0x19,
  RESERVED_1E = 0x1E,
  RESERVED_FD = 0xFD,
  Encapsulated_message = 0xFE,
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

#pragma pack(push, 1)
typedef struct MQTT_SN_FORWARD_ENCAPSULATION {
  uint8_t length;
  uint8_t msg_type;
  uint8_t crtl;
  device_address wireless_node_id;
  uint8_t mqtt_sn_message[GATEWAY_NETWORK_MAX_DATA_LEN - FORWARDER_HEADER_LEN - sizeof(device_address)];

} MQTT_SN_FORWARD_ENCAPSULATION;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct MqttSnMessageHeaderOneOctetLengthField {
  uint8_t length;
  uint8_t msg_type;
} MqttSnMessageHeaderOneOctetLengthField;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct MqttSnMessageHeaderThreeOctetsLengthField {
  uint8_t three_octets_length_field_indicator;
  uint8_t msb_length;
  uint8_t lsb_length;
  uint8_t msg_type;
} MqttSnMessageHeaderThreeOctetsLengthField;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct MqttSnHeader_ {
  uint8_t indicator;
  uint16_t length;
  MQTT_SN_MESSAGE_TYPE msg_type;
  void *payload;
} MqttSnHeader;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct MqttSnMessagePublish_ {
  uint8_t flags;
  uint16_t topicId;
  uint16_t msgId;
  uint16_t *data;
} MqttSnMessagePublish;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct MqttSnMessageConnect_ {
  uint8_t flags;
  uint8_t protocolId;
  uint16_t duration;
  char clientId[24];
} MqttSnMessageConnect;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct MqttSnMessageConnack_ {
  uint8_t returnCode;
} MqttSnMessageConnack;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct MqttSnMessageDisconnect_ {
  uint16_t duration;
} MqttSnMessageDisconnect;
#pragma pack(pop)

int parse_header(MqttSnHeader *h, const uint8_t *data, uint16_t data_len);

int parse_publish(MqttSnHeader *h, const uint8_t *data, uint16_t data_len);

int parse_connect(MqttSnHeader *h, const uint8_t *data, uint16_t data_len);

int parse_connack(MqttSnHeader *h, const uint8_t *data, uint16_t data_len);

int parse_disconnect(MqttSnHeader *h, const uint8_t *data, uint16_t data_len);

uint16_t get_message_length(const uint8_t *data);

MQTT_SN_MESSAGE_TYPE get_mqtt_sn_message_type(const uint8_t *data);

int is_valid_three_bytes_header(const uint8_t *data, ssize_t data_len);

uint8_t is_three_bytes_header(const uint8_t *data);

#endif //CMQTTSNFORWARDER_MQTTSNMESSAGEPARSER_H
