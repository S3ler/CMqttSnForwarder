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
static int32_t generate_gwinfo_header(uint8_t *dst,
                                      uint16_t dst_len,
                                      uint64_t gw_add_len,
                                      int32_t *used_bytes);
static int32_t generate_gwinfo_radius(uint8_t *dst_pos, uint16_t dst_len, uint8_t gw_id, int32_t *used_bytes);
static int32_t generate_gwinfo_gw_add(uint8_t *dst_pos,
                                      uint16_t dst_len,
                                      const device_address *gw_add,
                                      uint16_t gw_add_len,
                                      int32_t *used_bytes);
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
  int32_t parsed_bytes = parse_gwinfo_gateway_message_byte(gw_id, data, data_len);
  if (parsed_bytes < 0) {
    return -1;
  }
  if (parsed_bytes == data_len) {
    (*gw_add_len) = 0;
    return parsed_bytes;
  }
  if (parse_gwinfo_gw_add(data + parsed_bytes, data_len, &parsed_bytes, gw_add, gw_add_len) < 0) {
    return -1;
  }
  return parsed_bytes;
}

int32_t parse_gwinfo_header(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len, int32_t *parsed_bytes) {
  if ((*parsed_bytes = parse_header(h, GWINFO, data, data_len, parsed_bytes)) < 0) {
    return -1;
  }
  if (h->length < MQTT_SN_MESSAGE_GWINFO_WO_GWADD_LENGTH) {
    return -1;
  }
  if (h->length > MQTT_SN_MESSAGE_GWINFO_W_GWADD_MAX_LENGTH) {
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
int32_t generate_gwinfo_message(uint8_t *dst,
                                uint16_t dst_len,
                                uint8_t gw_id,
                                const device_address *gw_add,
                                uint16_t gw_add_len) {
  if (gw_add == NULL || gw_add_len == 0) {
    return generate_gwinfo_gateway_message(dst, dst_len, gw_id);
  }
  return generate_gwinfo_client_message(dst, dst_len, gw_id, gw_add, gw_add_len);
}

int32_t generate_gwinfo_gateway_message(uint8_t *dst, uint16_t dst_len, uint8_t gw_id) {
  int32_t used_bytes = 0;
  if (generate_gwinfo_header(dst, dst_len, 0, &used_bytes) < 0) {
    return -1;
  }
  if (generate_gwinfo_radius(dst + used_bytes, dst_len, gw_id, &used_bytes) < 0) {
    return -1;
  }
  return used_bytes;
}
int32_t generate_gwinfo_radius(uint8_t *dst_pos, uint16_t dst_len, uint8_t gw_id, int32_t *used_bytes) {
  return generate_mqtt_sn_uint8(dst_pos, dst_len, gw_id, used_bytes);
}
int32_t generate_gwinfo_client_message(uint8_t *dst,
                                       uint16_t dst_len,
                                       uint8_t gw_id,
                                       const device_address *gw_add,
                                       uint16_t gw_add_len) {
  int32_t used_bytes = 0;
  if (generate_gwinfo_header(dst, dst_len, gw_add_len, &used_bytes) < 0) {
    return -1;
  }
  if (generate_gwinfo_radius(dst + used_bytes, dst_len, gw_id, &used_bytes) < 0) {
    return -1;
  }
  if (generate_gwinfo_gw_add(dst + used_bytes, dst_len, gw_add, gw_add_len, &used_bytes) < 0) {
    return -1;
  }
  return 0;
}
int32_t generate_gwinfo_gw_add(uint8_t *dst_pos,
                               uint16_t dst_len,
                               const device_address *gw_add,
                               uint16_t gw_add_len,
                               int32_t *used_bytes) {
  return generate_mqtt_sn_device_address(dst_pos, dst_len, used_bytes, gw_add, gw_add_len);
}
int32_t generate_gwinfo_header(uint8_t *dst, uint16_t dst_len, uint64_t gw_add_len, int32_t *used_bytes) {
  uint16_t total_length = MQTT_SN_MESSAGE_GWINFO_WO_GWADD_LENGTH + gw_add_len;
  if (dst_len < total_length) {
    return -1;
  }
  return generate_mqtt_sn_header(dst, dst_len, used_bytes, total_length, GWINFO);
}
