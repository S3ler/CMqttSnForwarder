//
// Created by bele on 20.05.19.
//

#ifndef CMQTTSNFORWARDER_PARSER_MQTTSNGWINFOMESSAGE_H_
#define CMQTTSNFORWARDER_PARSER_MQTTSNGWINFOMESSAGE_H_

#include <stdint.h>
#include <platform/device_address.h>

#define MQTT_SN_MESSAGE_GWINFO_HEADER_LENGTH  3
#define MQTT_SN_MESSAGE_GWINFO_FROM_GATEWAY_LENGTH    MQTT_SN_MESSAGE_GWINFO_HEADER_LENGTH
#define MQTT_SN_MESSAGE_GWINFO_FROM_CLIENT_MAX_LENGTH (MQTT_SN_MESSAGE_GWINFO_HEADER_LENGTH + sizeof(device_address))

#pragma pack(push, 1)
typedef struct MqttSnGwInfo_ {
  uint8_t gwId;
  device_address gwAdd;
} MqttSnGwInfo;
#pragma pack(pop)
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

#endif //CMQTTSNFORWARDER_PARSER_MQTTSNGWINFOMESSAGE_H_
