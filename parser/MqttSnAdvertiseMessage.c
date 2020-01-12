//
// Created by bele on 19.05.19.
//

#include "MqttSnAdvertiseMessage.h"
// //TODO only linux? #include <arpa/inet.h>

static int32_t parse_advertise_header(ParsedMqttSnHeader *h,
                                      const uint8_t *data,
                                      uint16_t data_len,
                                      int32_t *parsed_bytes);
static int32_t parse_advertise_gw_id_byte(const uint8_t *src_pos,
                                          uint16_t src_len,
                                          int32_t *parsed_bytes,
                                          uint8_t *gw_id);
static int32_t parse_advertise_duration_byte(const uint8_t *src_pos,
                                             uint16_t src_len,
                                             int32_t *parsed_bytes,
                                             uint16_t *duration);
int32_t parse_advertise(MqttSnAdvertise *mqtt_sn_advertise, const uint8_t *data, uint16_t data_len) {
  return parse_advertise_byte(&mqtt_sn_advertise->gwId, &mqtt_sn_advertise->duration, data, data_len);
}
int32_t parse_advertise_byte(uint8_t *gw_id, uint16_t *duration, const uint8_t *data, uint16_t data_len) {
  int32_t parsed_bytes = 0;
  ParsedMqttSnHeader h = {0};
  if (parse_advertise_header(&h, data, data_len, &parsed_bytes) < 0) {
    return -1;
  }
  if (parse_advertise_gw_id_byte(data + parsed_bytes, data_len, &parsed_bytes, gw_id) < 0) {
    return -1;
  }
  if (parse_advertise_duration_byte(data + parsed_bytes, data_len, &parsed_bytes, duration) < 0) {
    return -1;
  }
  return parsed_bytes;
}
int32_t parse_advertise_header(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len, int32_t *parsed_bytes) {
  if (parse_header(h, ADVERTISE, data, data_len, parsed_bytes) < 0) {
    return -1;
  }
  if (h->length != MQTT_SN_MESSAGE_ADVERTISE_LENGTH) {
    // invalid message length for advertise
    return -1;
  }
  return (*parsed_bytes);
}
int32_t parse_advertise_gw_id_byte(const uint8_t *src_pos, uint16_t src_len, int32_t *parsed_bytes, uint8_t *gw_id) {
  return parse_mqtt_sn_uint8_byte(src_pos, src_len, parsed_bytes, gw_id);
}
int32_t parse_advertise_duration_byte(const uint8_t *src_pos,
                                      uint16_t src_len,
                                      int32_t *parsed_bytes,
                                      uint16_t *duration) {
  return parse_mqtt_sn_uint16_byte(src_pos, src_len, parsed_bytes, duration);
}

static int32_t generate_advertise_header(uint8_t *dst_pos, uint16_t dst_len, int32_t *used_bytes);
static int32_t generate_advertise_duration(uint8_t *dst_pos, uint16_t dst_len, uint16_t duration, int32_t *used_bytes);
static int32_t generate_advertise_gw_id(uint8_t *dst_pos, uint16_t dst_len, uint8_t gw_id, int32_t *used_bytes);

int32_t generate_advertise_message(uint8_t *dst, uint16_t dst_len, uint8_t gw_id, uint16_t duration) {
  int32_t used_bytes = 0;
  if (generate_advertise_header(dst + used_bytes, dst_len, &used_bytes) < 0) {
    return -1;
  }
  if (generate_advertise_gw_id(dst + used_bytes, dst_len, gw_id, &used_bytes) < 0) {
    return -1;
  }
  if (generate_advertise_duration(dst + used_bytes, dst_len, duration, &used_bytes) < 0) {
    return -1;
  }
  return used_bytes;
}

int32_t generate_advertise_header(uint8_t *dst_pos, uint16_t dst_len, int32_t *used_bytes) {
  return generate_mqtt_sn_header(dst_pos, dst_len, used_bytes, MQTT_SN_MESSAGE_ADVERTISE_LENGTH, ADVERTISE);
}
int32_t generate_advertise_gw_id(uint8_t *dst_pos, uint16_t dst_len, uint8_t gw_id, int32_t *used_bytes) {
  return generate_mqtt_sn_uint8(dst_pos, dst_len, gw_id, used_bytes);
}
int32_t generate_advertise_duration(uint8_t *dst_pos, uint16_t dst_len, uint16_t duration, int32_t *used_bytes) {
  return generate_mqtt_sn_uint16(dst_pos, dst_len, duration, used_bytes);
}


/*
static int32_t parse_advertise_gw_id(uint8_t *gw_id, ParsedMqttSnHeader *h, int32_t *parsed_bytes);
static int32_t parse_advertise_duration(uint16_t *duration, ParsedMqttSnHeader *h, int32_t *parsed_bytes);

int32_t parse_advertise(uint8_t *gw_id, uint16_t *duration, const uint8_t *data, uint16_t data_len) {
  ParsedMqttSnHeader h = {0};
  int32_t parsed_bytes = parse_advertise_to_structs(&h, NULL, data, data_len);
  if (parsed_bytes < 0) {
    return -1;
  }
  parse_advertise_gw_id(gw_id, &h, &parsed_bytes);
  parse_advertise_duration(duration, &h, &parsed_bytes);
  return parsed_bytes;
}
int32_t parse_advertise_duration(uint16_t *duration, ParsedMqttSnHeader *h, int32_t *parsed_bytes) {
  MqttSnAdvertise *advertise = (MqttSnAdvertise *) h->payload;
  (*duration) = ntohs(advertise->duration);
  (*parsed_bytes) += 2;
  return *parsed_bytes;
}
int32_t parse_advertise_gw_id(uint8_t *gw_id, ParsedMqttSnHeader *h, int32_t *parsed_bytes) {
  MqttSnAdvertise *advertise = (MqttSnAdvertise *) h->payload;
  *gw_id = advertise->gwId;
  (*parsed_bytes) += 1;
  return (*parsed_bytes);
}
int32_t parse_advertise_to_structs(ParsedMqttSnHeader *h,
                                   MqttSnAdvertise *advertise,
                                   const uint8_t *data,
                                   uint16_t data_len) {
  int32_t parsed_bytes = 0;
  if (parse_advertise_header(h, data, data_len, &parsed_bytes) < 0) {
    return -1;
  }
  if (advertise == NULL) {
    return parsed_bytes;
  }
  parse_advertise_gw_id(&advertise->gwId, h, &parsed_bytes);
  parse_advertise_duration(&advertise->duration, h, &parsed_bytes);
  return parsed_bytes;
}
*/





