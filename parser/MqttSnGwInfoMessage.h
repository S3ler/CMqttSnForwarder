//
// Created by bele on 20.05.19.
//

#ifndef CMQTTSNFORWARDER_PARSER_MQTTSNGWINFOMESSAGE_H_
#define CMQTTSNFORWARDER_PARSER_MQTTSNGWINFOMESSAGE_H_

#include <stdint.h>
#include <platform/device_address.h>
#ifdef __cplusplus
extern "C" {
#endif

#define MQTT_SN_MESSAGE_GWINFO_HEADER_LENGTH          MQTT_SN_MESSAGE_HEADER_SHORT_LENGTH
#define MQTT_SN_MESSAGE_GWINFO_GWID_LENGTH            MQTT_SN_GWID_LENGTH
#define MQTT_SN_MESSAGE_GWINFO_GWADD_MIN_LENGTH       MQTT_SN_GWADD_MIN_LENGTH
#define MQTT_SN_MESSAGE_GWINFO_GWADD_MAX_LENGTH       MQTT_SN_GWADD_MAX_LENGTH
#define MQTT_SN_MESSAGE_GWINFO_WO_GWADD_LENGTH       (MQTT_SN_MESSAGE_GWINFO_HEADER_LENGTH + \
                                                      MQTT_SN_MESSAGE_GWINFO_GWID_LENGTH)
#define MQTT_SN_MESSAGE_GWINFO_W_GWADD_MIN_LENGTH    (MQTT_SN_MESSAGE_GWINFO_WO_GWADD_LENGTH + \
                                                      MQTT_SN_MESSAGE_GWINFO_GWADD_MIN_LENGTH)
#define MQTT_SN_MESSAGE_GWINFO_W_GWADD_MAX_LENGTH    (MQTT_SN_MESSAGE_GWINFO_WO_GWADD_LENGTH + \
                                                      MQTT_SN_MESSAGE_GWINFO_GWADD_MAX_LENGTH)
#pragma pack(push, 1)
typedef struct MqttSnGwInfo_ {
  uint8_t gwId;
  device_address gwAdd;
  uint16_t gwAddLen;
} MqttSnGwInfo;
#pragma pack(pop)
int32_t parse_gwinfo(MqttSnGwInfo *gw_info, const uint8_t *data, uint16_t data_len);
int32_t parse_gwinfo_message_byte(uint8_t *gw_id,
                                  device_address *gw_add,
                                  uint16_t *gw_add_len,
                                  const uint8_t *data,
                                  uint16_t data_len);
int32_t parse_gwinfo_gateway_message_byte(uint8_t *gw_id, const uint8_t *data, uint16_t data_len);
int32_t parse_gwinfo_client_message_byte(uint8_t *gw_id,
                                         device_address *gw_add,
                                         uint16_t *gw_add_len,
                                         const uint8_t *data,
                                         uint16_t data_len);
int32_t generate_gwinfo_message(uint8_t *dst,
                                uint16_t dst_len,
                                uint8_t gw_id,
                                const device_address *gw_add,
                                uint16_t gw_add_len);
int32_t generate_gwinfo_gateway_message(uint8_t *dst, uint16_t dst_len, uint8_t gw_id);
int32_t generate_gwinfo_client_message(uint8_t *dst,
                                       uint16_t dst_len,
                                       uint8_t gw_id,
                                       const device_address *gw_add,
                                       uint16_t gw_add_len);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_PARSER_MQTTSNGWINFOMESSAGE_H_
