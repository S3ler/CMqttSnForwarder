//
// Created by bele on 20.05.19.
//

#include "MqttSnGwInfoMessage.h"
#include "MqttSnMessageParser.h"

static int32_t parse_gwinfo_header(ParsedMqttSnHeader *h,
                                   const uint8_t *data,
                                   uint16_t data_len,
                                   int32_t *parsed_bytes);
static int32_t parse_gwinfo_gw_id(const uint8_t *src_pos, uint16_t src_len, int32_t *parsed_bytes, uint8_t *gw_id);
static int32_t parse_gwinfo_gw_add(const uint8_t *src_pos,
                                   uint16_t src_len,
                                   int32_t *parsed_bytes,
                                   device_address *gw_add,
                                   uint16_t *gw_add_len);

int32_t parse_gwinfo_message_byte(uint8_t *gw_id,
                                  device_address *gw_add,
                                  uint16_t *gw_add_len,
                                  const uint8_t *data,
                                  uint16_t data_len) {
  return parse_gwinfo_client_message_byte(gw_id, gw_add, gw_add_len, data, data_len);
}

int32_t parse_gwinfo_gateway_message_byte(uint8_t *gw_id, const uint8_t *data, uint16_t data_len) {
  int32_t parsed_bytes = 0;
  ParsedMqttSnHeader h = {0};
  if ((parsed_bytes = parse_gwinfo_header(&h, data, data_len, &parsed_bytes)) < 0) {
    return -1;
  }
  if ((parsed_bytes = parse_gwinfo_gw_id(data + parsed_bytes, data_len, &parsed_bytes, gw_id)) < 0) {
    return -1;
  }
  return parsed_bytes;
}
int32_t parse_gwinfo_client_message_byte(uint8_t *gw_id,
                                         device_address *gw_add,
                                         uint16_t *gw_add_len,
                                         const uint8_t *data,
                                         uint16_t data_len) {
  int32_t parsed_bytes = 0;
  if ((parsed_bytes = parse_gwinfo_gateway_message_byte(gw_id, data, data_len)) < 0) {
    return -1;
  }
  if ((parsed_bytes = parse_gwinfo_gw_add(data + parsed_bytes, data_len, &parsed_bytes, gw_add, gw_add_len)) < 0) {
    return -1;
  }
  return parsed_bytes;
}

int32_t parse_gwinfo_header(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len, int32_t *parsed_bytes) {
  if ((*parsed_bytes = parse_header(h, GWINFO, data, data_len, parsed_bytes)) < 0) {
    return -1;
  }
  if ((*parsed_bytes) < MQTT_SN_MESSAGE_GWINFO_HEADER_LENGTH) {
    return -1;
  }
  if ((*parsed_bytes) > (int32_t) MQTT_SN_MESSAGE_GWINFO_FROM_CLIENT_MAX_LENGTH) {
    // gw add too long to handle
    return -1;
  }
  return *parsed_bytes;
}
int32_t parse_gwinfo_gw_id(const uint8_t *src_pos, uint16_t src_len, int32_t *parsed_bytes, uint8_t *gw_id) {
  return parse_mqtt_sn_uint8_byte(src_pos, src_len, parsed_bytes, gw_id);
}
int32_t parse_gwinfo_gw_add(const uint8_t *src_pos,
                            uint16_t src_len,
                            int32_t *parsed_bytes,
                            device_address *gw_add,
                            uint16_t *gw_add_len) {
  return parse_mqtt_sn_device_address(src_pos, src_len, parsed_bytes, gw_add, gw_add_len);
}
