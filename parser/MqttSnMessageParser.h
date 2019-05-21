//
// Created by SomeDude on 09.04.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNMESSAGEPARSER_H
#define CMQTTSNFORWARDER_MQTTSNMESSAGEPARSER_H
#ifdef __cplusplus
extern "C" {
#endif

#include <platform/device_address.h>
#include <unistd.h>

#define FOREACH_MQTT_SN_MESSAGE_TYPE(MESSAGE_TYPE) \
        MESSAGE_TYPE(ANY_MESSAGE_TYPE , -2)   \
        MESSAGE_TYPE(RESERVED_INVALID , -1)   \
        MESSAGE_TYPE(ADVERTISE , 0x00)   \
        MESSAGE_TYPE(SEARCHGW , 0x01)   \
        MESSAGE_TYPE(GWINFO , 0x02)   \
        MESSAGE_TYPE(RESERVED_03 , 0x03)   \
        MESSAGE_TYPE(CONNECT , 0x04)   \
        MESSAGE_TYPE(CONNACK , 0x05)   \
        MESSAGE_TYPE(WILLTOPICREQ , 0x06)   \
        MESSAGE_TYPE(WILLTOPIC , 0x07)   \
        MESSAGE_TYPE(WILLMSGREQ , 0x08)   \
        MESSAGE_TYPE(WILLMSG , 0x09)   \
        MESSAGE_TYPE(REGISTER , 0x0A)   \
        MESSAGE_TYPE(REGACK , 0x0B)   \
        MESSAGE_TYPE(PUBLISH , 0x0C)   \
        MESSAGE_TYPE(PUBACK , 0x0D)   \
        MESSAGE_TYPE(PUBCOMP , 0x0E)   \
        MESSAGE_TYPE(PUBREC , 0x0F)   \
        MESSAGE_TYPE(PUBREL , 0x10)   \
        MESSAGE_TYPE(RESERVED_11 , 0x11)   \
        MESSAGE_TYPE(SUBSCRIBE , 0x12)   \
        MESSAGE_TYPE(SUBACK , 0x13)   \
        MESSAGE_TYPE(UNSUBSCRIBE , 0x14)   \
        MESSAGE_TYPE(UNSUBACK , 0x15)   \
        MESSAGE_TYPE(PINGREQ , 0x16)   \
        MESSAGE_TYPE(PINGRESP , 0x17)   \
        MESSAGE_TYPE(DISCONNECT , 0x18)   \
        MESSAGE_TYPE(RESERVED_19 , 0x19)   \
        MESSAGE_TYPE(WILLTOPICUPD , 0x1A)   \
        MESSAGE_TYPE(WILLTOPICRESP , 0x1B)   \
        MESSAGE_TYPE(WILLMSGUPD , 0x1C)   \
        MESSAGE_TYPE(WILLMSGRESP , 0x1D)   \
        MESSAGE_TYPE(RESERVED_1E , 0x1E)   \
        MESSAGE_TYPE(RESERVED_FD , 0xFD)   \
        MESSAGE_TYPE(ENCAPSULATED_MESSAGE , 0xFE)   \
        MESSAGE_TYPE(RESERVED_FF , 0xFF)   \


#define GENERATE_MQTT_SN_MESSAGE_TYPE_ENUM(MESSAGE_TYPE, MESSAGE_CODE) MESSAGE_TYPE = MESSAGE_CODE,
#define GENERATE_MQTT_SN_MESSAGE_TYPE_STRING(STRING, NUMBER) #STRING,

typedef enum MQTT_SN_MESSAGE_TYPE_ {
  FOREACH_MQTT_SN_MESSAGE_TYPE(GENERATE_MQTT_SN_MESSAGE_TYPE_ENUM)
} MQTT_SN_MESSAGE_TYPE;

#define MQTT_SN_MESSAGE_TYPE_RESERVED(type) (type == RESERVED_03 || type == RESERVED_11 || type == RESERVED_19 \
                                            || (type >= RESERVED_1E && type <= RESERVED_FD) || type == RESERVED_FF)

#define FOREACH_MQTT_SN_MESSAGE_RETURN_CODE(RETURN_CODE) \
        RETURN_CODE(RETURN_CODE_ANY , -2)   \
        RETURN_CODE(RETURN_CODE_RESERVED_INVALID , -1)   \
        RETURN_CODE(RETURN_CODE_ACCEPTED , 0x00)   \
        RETURN_CODE(RETURN_CODE_REJECTED_CONGESTION , 0x01)   \
        RETURN_CODE(RETURN_CODE_REJECTED_INVALID_TOPIC_ID , 0x02)   \
        RETURN_CODE(RETURN_CODE_REJCETED_NOT_SUPPORTED , 0x03)   \
        RETURN_CODE(RETURN_CODE_RESERVED_04 , 0x04)   \
        RETURN_CODE(RETURN_CODE_RESERVED_FF , 0xFF)   \


#define GENERATE_MQTT_SN_MESSAGE_RETURN_CODE_ENUM(RETURN_CODE, RETURN_VALUE) RETURN_CODE = RETURN_VALUE,
#define GENERATE_MQTT_SN_MESSAGE_RETURN_CODE_STRING(STRING, NUMBER) #STRING,

typedef enum MQTT_SN_RETURN_CODE_ {
  FOREACH_MQTT_SN_MESSAGE_RETURN_CODE(GENERATE_MQTT_SN_MESSAGE_RETURN_CODE_ENUM)
} MQTT_SN_RETURN_CODE;

#define MQTT_SN_RETURN_CODE_VALID(type) (type >= RETURN_CODE_ACCEPTED && type <= RETURN_CODE_REJCETED_NOT_SUPPORTED)
#define MQTT_SN_RETURN_CODE_RESERVED(type) (type >= RETURN_CODE_RESERVED_04 && type <= RETURN_CODE_RESERVED_FF)



// Header length
#define MQTT_SN_MESSAGE_HEADER_SHORT_LENGTHFIELD_LENGTH   1
#define MQTT_SN_MESSAGE_HEADER_LONG_LENGTHFIELD_LENGTH    3
#define MQTT_SN_MESSAGE_MSG_TYPE_LENGTH                   1

#define MQTT_SN_MESSAGE_HEADER_SHORT_LENGTH   (MQTT_SN_MESSAGE_HEADER_SHORT_LENGTHFIELD_LENGTH + MQTT_SN_MESSAGE_MSG_TYPE_LENGTH)
#define MQTT_SN_MESSAGE_HEADER_LONG_LENGTH    (MQTT_SN_MESSAGE_HEADER_LONG_LENGTHFIELD_LENGTH + MQTT_SN_MESSAGE_MSG_TYPE_LENGTH)
#define MQTT_SN_HEADER_LENGTH(indicator)      (indicator ? MQTT_SN_MESSAGE_HEADER_SHORT_LENGTH : MQTT_SN_MESSAGE_HEADER_LONG_LENGTH)

// Field length
#define MQTT_SN_GWID_LENGTH                   1
#define MQTT_SN_DURATION_LENGTH               2
#define MQTT_SN_RADIUS_LENGTH                 1
#define MQTT_SN_GWADD_MIN_LENGTH              1
#define MQTT_SN_GWADD_MAX_LENGTH              sizeof(device_address)
#define MQTT_SN_FLAGS_LENGTH                  1
#define MQTT_SN_PROTOCOLID_LENGTH             1
#define MQTT_SN_CLIENTID_MIN_LENGTH           1
#define MQTT_SN_CLIENTID_MAX_LENGTH           1
#define MQTT_SN_RETURNCODE_LENGTH             1

#define MQTT_SN_TOPIC_ID_LENGTH               2
#define MQTT_SN_MESSAGE_ID_LENGTH             2

#define MQTT_SN_FLAG_DUP_POS            0x80 // 0b1000 0000
#define MQTT_SN_FLAG_QOS_POS            0x60 // 0b0110 0000
#define MQTT_SN_FLAG_RETAIN_POS         0x10 // 0b0001 0000
#define MQTT_SN_FLAG_WILL_POS           0x08 // 0b0000 1000
#define MQTT_SN_FLAG_CLEAN_SESSION_POS  0x04 // 0b0000 0100
#define MQTT_SN_FLAG_TOPIC_ID_TYPE_POS  0x03 // 0b0000 0011

#define MQTT_SN_FLAG_DUP_SHIFT            7
#define MQTT_SN_FLAG_QOS_SHIFT            5
#define MQTT_SN_FLAG_RETAIN_SHIFT         4
#define MQTT_SN_FLAG_WILL_SHIFT           3
#define MQTT_SN_FLAG_CLEAN_SESSION_SHIFT  2
#define MQTT_SN_FLAG_TOPIC_ID_TYPE_SHIFT  0

#define GET_MQTT_SN_DUP_FLAG(flags)       ((flags & MQTT_SN_FLAG_DUP_POS) >> MQTT_SN_FLAG_DUP_SHIFT)
#define GET_MQTT_SN_QOS_FLAG(flags)       ((flags & MQTT_SN_FLAG_QOS_POS) >> MQTT_SN_FLAG_QOS_SHIFT)
#define GET_MQTT_SN_RETAIN_FLAG(flags)    ((flags & MQTT_SN_FLAG_RETAIN_POS) >> MQTT_SN_FLAG_RETAIN_SHIFT) == 2 ? \
                                            -1 : ((flags & MQTT_SN_FLAG_RETAIN_POS) >> MQTT_SN_FLAG_RETAIN_SHIFT)
#define GET_MQTT_SN_WILL_FLAG(flags)      ((flags & MQTT_SN_FLAG_WILL_POS) >> MQTT_SN_FLAG_WILL_SHIFT)
#define GET_MQTT_SN_CLEAN_SESSION(flags)  ((flags & MQTT_SN_FLAG_CLEAN_SESSION_POS) >> MQTT_SN_FLAG_CLEAN_SESSION_SHIFT)
#define GET_MQTT_SN_TOPIC_ID_TYPE(flags)  ((flags & MQTT_SN_FLAG_TOPIC_ID_TYPE_POS) >> MQTT_SN_FLAG_TOPIC_ID_TYPE_SHIFT)

#define MQTT_SN_FLAG_DUP_FIRST_TIME     0x00 // 0b0
#define MQTT_SN_FLAG_DUB_RETRANSMITTED  0x01 // 0b1

#define MQTT_SN_FLAG_QOS_M1 0x03 // 0b11
#define MQTT_SN_FLAG_QOS_2  0x02 // 0b10
#define MQTT_SN_FLAG_QOS_1  0x01 // 0b01
#define MQTT_SN_FLAG_QOS_0  0x00 // 0b00

#define MQTT_SN_FLAG_RETAIN_TRUE  0x01 // 0b1
#define MQTT_SN_FLAG_RETAIN_FALSE 0x00 // 0b0

#define MQTT_SN_FLAG_WILL_TRUE 0x01 // 0b01
#define MQTT_SN_FLAG_WILL_FALSE 0x01 // 0b01

#define MQTT_SN_FLAG_CLEAN_SESSION_TRUE   0x01 // 0b1
#define MQTT_SN_FLAG_CLEAN_SESSION_FALSE  0x00 // 0b0

#define MQTT_SN_FLAG_TOPIC_ID_TYPE_RESERVED             0x03 // 0b11
#define MQTT_SN_FLAG_TOPIC_ID_TYPE_SHORT_TOPIC_NAME     0x02 // 0b10
#define MQTT_SN_FLAG_TOPIC_ID_TYPE_PREDEFINED_TOPIC_ID  0x01 // 0b01
#define MQTT_SN_FLAG_TOPIC_ID_TYPE_TOPIC_NAME           0x00 // 0b00

#define SET_MQTT_SN_DUP_FLAG_FIRST_TIME(flags)    ((MQTT_SN_FLAG_DUP_FIRST_TIME << MQTT_SN_FLAG_DUP_SHIFT) | flags)
#define SET_MQTT_SN_DUP_FLAG_RETRANSMITTED(flags) ((MQTT_SN_FLAG_DUB_RETRANSMITTED << MQTT_SN_FLAG_DUP_SHIFT) | flags)

#define SET_MQTT_SN_FLAG_QOS_M1(flags)  ((MQTT_SN_FLAG_QOS_M1 << MQTT_SN_FLAG_QOS_SHIFT) | flags)
#define SET_MQTT_SN_FLAG_QOS_2(flags)   ((MQTT_SN_FLAG_QOS_2 << MQTT_SN_FLAG_QOS_SHIFT) | flags)
#define SET_MQTT_SN_FLAG_QOS_1(flags)   ((MQTT_SN_FLAG_QOS_1 << MQTT_SN_FLAG_QOS_SHIFT) | flags)
#define SET_MQTT_SN_FLAG_QOS_0(flags)   ((MQTT_SN_FLAG_QOS_0 << MQTT_SN_FLAG_QOS_SHIFT) | flags)

#define SET_MQTT_SN_FLAG_RETAIN_TRUE(flags) ((MQTT_SN_FLAG_RETAIN_TRUE << MQTT_SN_FLAG_RETAIN_SHIFT) | flags)
#define SET_MQTT_SN_FLAG_RETAIN_FALSE(flags) ((MQTT_SN_FLAG_RETAIN_FALSE << MQTT_SN_FLAG_RETAIN_SHIFT) | flags)

#define SET_MQTT_SN_FLAG_WILL_TRUE(flags) ((MQTT_SN_FLAG_WILL_TRUE << MQTT_SN_FLAG_WILL_SHIFT) | flags)
#define SET_MQTT_SN_FLAG_WILL_FALSE(flags) ((MQTT_SN_FLAG_CLEAN_SESSION_FALSE << MQTT_SN_FLAG_WILL_SHIFT) | flags)

#define SET_MQTT_SN_CLEAN_SESSION_TRUE(flags) ((MQTT_SN_FLAG_CLEAN_SESSION_TRUE << MQTT_SN_FLAG_CLEAN_SESSION_SHIFT) | flags)
#define SET_MQTT_SN_CLEAN_SESSION_FALSE(flags) ((MQTT_SN_FLAG_CLEAN_SESSION_FALSE << MQTT_SN_FLAG_CLEAN_SESSION_SHIFT) | flags)

#define SET_MQTT_SN_FLAG_TOPIC_ID_TYPE_RESERVED(flags)              ((MQTT_SN_FLAG_TOPIC_ID_TYPE_RESERVED << MQTT_SN_FLAG_TOPIC_ID_TYPE_SHIFT) | flags)
#define SET_MQTT_SN_FLAG_TOPIC_ID_TYPE_SHORT_TOPIC_NAME(flags)      ((MQTT_SN_FLAG_TOPIC_ID_TYPE_SHORT_TOPIC_NAME << MQTT_SN_FLAG_TOPIC_ID_TYPE_SHIFT) | flags)
#define SET_MQTT_SN_FLAG_TOPIC_ID_TYPE_PREDEFINED_TOPIC_ID(flags)   ((MQTT_SN_FLAG_TOPIC_ID_TYPE_PREDEFINED_TOPIC_ID << MQTT_SN_FLAG_TOPIC_ID_TYPE_SHIFT) | flags)
#define SET_MQTT_SN_FLAG_TOPIC_ID_TYPE_TOPIC_NAME(flags)            ((MQTT_SN_FLAG_TOPIC_ID_TYPE_TOPIC_NAME << MQTT_SN_FLAG_TOPIC_ID_TYPE_SHIFT) | flags)

#define MQTT_SN_PINGREQ_MESSAGE_HEADER_LENGTH 2
#define MQTT_SN_MAX_CLIENT_ID_STRING_LENGTH 23 // without null terminator
#define MQTT_SN_MAX_CLIENT_ID_LENGTH 24 // including null terminator

#define MQTT_SN_ENCAPSULATED_MESSAGE_CRTL_BYTE_LENGTH           1
#define MQTT_SN_ENCAPSULATED_MESSAGE_CRTL_BROADCAST_RADIUS      1
#define MQTT_SN_ENCAPSULATED_MESSAGE_HEADER_LENGTH(indicator)   (MQTT_SN_HEADER_LENGTH(indicator) \
                                                                 + MQTT_SN_ENCAPSULATED_MESSAGE_CRTL_BYTE_LENGTH \
                                                                 + sizeof(device_address))

#define MQTT_SN_PINGRESP_MESSAGE_HEADER_LENGTH  sizeof(MqttSnMessagePingResp)
#define MQTT_SN_MINIMAL_MESSAGE_LENGTH          MQTT_SN_PINGRESP_MESSAGE_HEADER_LENGTH
#define MQTT_SN_MAXIMUM_MESSAGE_LENGTH          UINT16_MAX

#define MQTT_SN_PUBLISH_MESSAGE_WO_HEADER_LENGTH (MQTT_SN_FLAGS_LENGTH + MQTT_SN_TOPIC_ID_LENGTH + MQTT_SN_MESSAGE_ID_LENGTH)
#define MQTT_SN_PUBLISH_MESSAGE_HEADER_LENGTH(indicator) (MQTT_SN_HEADER_LENGTH(indicator) + MQTT_SN_PUBLISH_MESSAGE_WO_HEADER_LENGTH)

#pragma pack(push)
#pragma pack(1)
typedef struct MqttSnMessageHeaderOneOctetLengthField_ {
  uint8_t length;
  uint8_t msg_type;
} MqttSnMessageHeaderOneOctetLengthField;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct MqttSnMessageHeaderThreeOctetsLengthField_ {
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
  uint8_t *data;
} MqttSnMessagePublish;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct MqttSnMessagePuback_ {
  uint16_t topicId;
  uint16_t msgId;
  uint8_t returnCode;
} MqttSnMessagePuback;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct MqttSnMessageSubcribe_ {
  uint8_t flags;
  uint16_t msgId;
  union {
    const char *topicName;
    uint16_t topicId;
  } topicNameOrTopicId;
} MqttSnMessageSubcribe;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct MqttSnMessageSuback_ {
  uint8_t flags;
  uint16_t topicId;
  uint16_t msgId;
  uint8_t returnCode;
} MqttSnMessageSuback;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct MqttSnMessageUnsubscribe_ {
  uint8_t flags;
  uint16_t msgId;
  union {
    const char *topicName;
    uint16_t topicId;
  } topicNameOrTopicId;
} MqttSnMessageUnsubscribe;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct MqttSnMessageUnsuback_ {
  uint16_t msgId;
} MqttSnMessageUnsuback;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct MqttSnMessageConnect_ {
  uint8_t flags;
  uint8_t protocolId;
  uint16_t duration;
  char clientId[MQTT_SN_MAX_CLIENT_ID_LENGTH];
} MqttSnMessageConnect;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct MqttSnMessageConnack_ {
  uint8_t returnCode;
} MqttSnMessageConnack;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct MqttSnMessageRegister_ {
  uint16_t topicId;
  uint16_t msgId;
  const char *topicName;
} MqttSnMessageRegister;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct MqttSnMessageRegack_ {
  uint16_t topicId;
  uint16_t msgId;
  uint8_t returnCode;
} MqttSnMessageRegack;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct MqttSnMessagePingReq_ {
  char clientId[MQTT_SN_MAX_CLIENT_ID_LENGTH];
} MqttSnMessagePingReq;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct MqttSnMessagePingResp_ {
  uint8_t length;
  uint8_t msgType;
} MqttSnMessagePingResp;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct MqttSnMessageDisconnect_ {
  uint16_t duration;
} MqttSnMessageDisconnect;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct MqttSnEncapsulatedMessage_ {
  uint8_t crtl;
  device_address wireless_node_id;
} MqttSnEncapsulatedMessage;
#pragma pack(pop)

int32_t parse_header(ParsedMqttSnHeader *h,
                     MQTT_SN_MESSAGE_TYPE msg_type,
                     const uint8_t *data,
                     uint16_t data_len,
                     int32_t *parsed_bytes);

int parse_message_tolerant(ParsedMqttSnHeader *h,
                           MQTT_SN_MESSAGE_TYPE msg_type,
                           const uint8_t *data,
                           uint16_t data_len);

int32_t parse_encapsulation_message(uint8_t *broadcast,
                                    device_address *wireless_node_id,
                                    uint8_t *mqtt_sn_message,
                                    uint16_t *mqtt_sn_message_len,
                                    uint16_t max_mqtt_sn_message_len,
                                    uint8_t *data,
                                    uint16_t data_len);

int parse_encapsulation_header(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len, int32_t *read_bytes);

int parse_encapsulation(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len);

int parse_publish(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len);

int parse_connect(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len);

int parse_connack(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len);

int parse_disconnect(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len);

uint16_t get_message_length(const uint8_t *data);

MQTT_SN_MESSAGE_TYPE get_mqtt_sn_message_type(const uint8_t *data);

int is_valid_three_bytes_header(const uint8_t *data, ssize_t data_len);

uint8_t is_three_bytes_header(const uint8_t *data);

int32_t generate_mqtt_sn_header(uint8_t *dst,
                                uint16_t dst_len,
                                uint16_t msg_len,
                                MQTT_SN_MESSAGE_TYPE msg_type,
                                int32_t *used_bytes);

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

/**
 * Generates a MQTT-SN Forwarder Encapsulation frame.
 * @param dst pointer to the destination buffer.
 * @param dst_len length of the destination buffer.
 * @param broadcast indicator if the message was send with broadcast radius.
 * @param from sender address.
 * @param data to encapsulate MQTT-SN message. Any data is accepted
 * @param data_len of the to encapsulate MQTT-SN message.
 * @return -1 if the broadcast value is not allowed or if frame does not fit into the dst buffer.
 */
int generate_encapsulation_message(uint8_t *dst,
                                   uint16_t dst_len,
                                   uint8_t broadcast,
                                   const device_address *from,
                                   const uint8_t *data,
                                   uint16_t data_len);

int32_t parse_mqtt_sn_uint8_byte(const uint8_t *src_pos, uint16_t src_len, int32_t *parsed_bytes, uint8_t *dst);
int32_t parse_mqtt_sn_uint16_byte(const uint8_t *src_pos, uint16_t src_len, int32_t *parsed_bytes, uint16_t *dst);
int32_t parse_mqtt_sn_device_address(const uint8_t *src_pos,
                                     uint16_t src_len,
                                     int32_t *parsed_bytes,
                                     device_address *dst_add,
                                     uint16_t *dst_len);
int32_t parse_mqtt_sn_client_id_byte(const uint8_t *src_pos,
                                     uint16_t src_len,
                                     int32_t *parsed_bytes,
                                     char *client_id,
                                     uint8_t *client_id_length,
                                     uint8_t client_id_max_length);
int32_t parse_mqtt_sn_return_code_byte(const uint8_t *src_pos,
                                       uint16_t src_len,
                                       int32_t *parsed_bytes,
                                       MQTT_SN_RETURN_CODE *return_code);
int32_t parse_mqtt_sn_flags(const uint8_t *src_pos,
                            uint16_t src_len,
                            int32_t *parsed_bytes,
                            uint8_t *dup,
                            int8_t *qos,
                            uint8_t *retain,
                            uint8_t *will,
                            uint8_t *clean_session,
                            uint8_t *topic_id_type);
int32_t generate_mqtt_sn_uint8(uint8_t *dst_pos, uint16_t dst_len, uint8_t value, int32_t *used_bytes);
int32_t generate_mqtt_sn_uint16(uint8_t *dst_pos, uint16_t dst_len, uint16_t value, int32_t *used_bytes);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_MQTTSNMESSAGEPARSER_H
