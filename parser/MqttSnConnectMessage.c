//
// Created by SomeDude on 21.05.2019.
//

#include "MqttSnConnectMessage.h"
#include <string.h>

static int32_t parse_connect_header(ParsedMqttSnHeader *h,
                                    const uint8_t *data,
                                    uint16_t data_len,
                                    int32_t *parsed_bytes);
static int32_t parse_connect_flags(const uint8_t *src_pos,
                                   uint16_t src_len,
                                   int32_t *parsed_bytes,
                                   uint8_t *will,
                                   uint8_t *clean_session);
static int32_t parse_connect_protocol_id(const uint8_t *src_pos,
                                         uint16_t src_len,
                                         int32_t *parsed_bytes,
                                         uint8_t *protocol_id);
static int32_t parse_connect_duration(const uint8_t *src_pos,
                                      uint16_t src_len,
                                      int32_t *parsed_bytes,
                                      uint16_t *duration);
static int32_t parse_connect_client_id(const uint8_t *src_pos,
                                       uint16_t src_len,
                                       int32_t *parsed_bytes,
                                       char *client_id,
                                       uint16_t *client_id_length,
                                       uint8_t client_id_max_length);
int generate_connect_header(uint8_t *dst, uint16_t dst_len, const char *client_id, int32_t *used_bytes);
int generate_protocol_id(uint8_t *dst, uint16_t dst_len, uint8_t protocol_id, int32_t *used_bytes);
int generate_duration(uint8_t *dst, uint16_t dst_len, uint16_t duration, int32_t *used_bytes);
int generate_client_id(uint8_t *dst, uint16_t dst_len, const char *client_id, int32_t *used_bytes);
int32_t get_client_id_length(const char *client_id);
int32_t parse_connect(ParsedMqttSnConnect *mqtt_sn_connect, const uint8_t *data, uint16_t data_len) {
  return parse_connect_message_byte(&mqtt_sn_connect->will,
                                    &mqtt_sn_connect->clean_session,
                                    &mqtt_sn_connect->protocol_id,
                                    &mqtt_sn_connect->duration,
                                    mqtt_sn_connect->client_id,
                                    &mqtt_sn_connect->client_id_length,
                                    sizeof(mqtt_sn_connect->client_id),
                                    data,
                                    data_len);
}
int32_t parse_connect_message_byte(uint8_t *will,
                                   uint8_t *clean_session,
                                   uint8_t *protocol_id,
                                   uint16_t *duration,
                                   char *client_id,
                                   uint16_t *client_id_length,
                                   uint8_t client_id_max_length,
                                   const uint8_t *data,
                                   uint16_t data_len) {
  int32_t parsed_bytes = 0;
  ParsedMqttSnHeader h = {0};
  if ((parsed_bytes = parse_connect_header(&h, data, data_len, &parsed_bytes)) < 0) {
    return -1;
  }
  if ((parsed_bytes = parse_connect_flags(data + parsed_bytes,
                                          data_len,
                                          &parsed_bytes,
                                          will,
                                          clean_session)) < 0) {
    return -1;
  }
  if ((parsed_bytes = parse_connect_protocol_id(data + parsed_bytes, data_len, &parsed_bytes, protocol_id)) < 0) {
    return -1;
  }
  if ((parsed_bytes = parse_connect_duration(data + parsed_bytes, data_len, &parsed_bytes, duration)) < 0) {
    return -1;
  }
  if ((parsed_bytes = parse_connect_client_id(data + parsed_bytes,
                                              data_len,
                                              &parsed_bytes,
                                              client_id,
                                              client_id_length,
                                              client_id_max_length)) < 0) {
    return -1;
  }
  return parsed_bytes;
}
int32_t generate_connect(uint8_t *dst,
                         uint16_t dst_len,
                         uint8_t will,
                         uint8_t clean_session,
                         uint8_t protocol_id,
                         uint16_t duration,
                         const char *client_id) {
  int32_t used_bytes = 0;

  if (generate_connect_header(dst, dst_len, client_id, &used_bytes) < 0) {
    return -1;
  }
  if (generate_flags(dst + used_bytes,
                     dst_len,
                     MQTT_SN_FLAG_DUP_FIRST_TIME,
                     MQTT_SN_FLAG_QOS_0,
                     MQTT_SN_FLAG_RETAIN_FALSE,
                     will,
                     clean_session,
                     MQTT_SN_FLAG_TOPIC_ID_TYPE_TOPIC_NAME,
                     &used_bytes) < 0) {
    return -1;
  }
  if (generate_protocol_id(dst + used_bytes, dst_len, protocol_id, &used_bytes) < 0) {
    return -1;
  }
  if (generate_duration(dst + used_bytes, dst_len, duration, &used_bytes) < 0) {
    return -1;
  }
  if (generate_client_id(dst + used_bytes, dst_len, client_id, &used_bytes) < 0) {
    return -1;
  }
  return used_bytes;
}
int32_t generate_client_id(uint8_t *dst, uint16_t dst_len, const char *client_id, int32_t *used_bytes) {
  int32_t client_id_len = get_client_id_length(client_id);
  if (client_id_len < 0) {
    return -1;
  }
  (*used_bytes) += client_id_len;
  if (dst_len < (*used_bytes)) {
    return -1;
  }
  memcpy(dst, client_id, client_id_len);
  return (*used_bytes);
}
int32_t get_client_id_length(const char *client_id) {
  uint8_t client_id_len;
  for (client_id_len = 0; client_id_len < MQTT_SN_MESSAGE_CONNECT_CLIENTID_MAX_LENGTH; client_id_len++) {
    if (client_id[client_id_len] == '\0') {
      break;
    }
  }
  if (client_id_len < MQTT_SN_MESSAGE_CONNECT_CLIENTID_MIN_LENGTH) {
    return -1;
  }
  if (client_id_len == MQTT_SN_MESSAGE_CONNECT_CLIENTID_MAX_LENGTH
      && client_id[client_id_len] != '\0') {
    return -1;
  }

  return client_id_len;
}

int32_t generate_protocol_id(uint8_t *dst, uint16_t dst_len, uint8_t protocol_id, int32_t *used_bytes) {
  (*used_bytes) += MQTT_SN_MESSAGE_CONNECT_PROTOCOLID_LENGTH;
  if (dst_len < (*used_bytes)) {
    return -1;
  }
  (*dst) = protocol_id;
  return (*used_bytes);
}
int32_t generate_connect_header(uint8_t *dst, uint16_t dst_len, const char *client_id, int32_t *used_bytes) {
  int32_t client_id_len = get_client_id_length(client_id);
  if (client_id_len < 0) {
    return -1;
  }
  uint16_t total_length = MQTT_SN_MESSAGE_CONNECT_WO_CLIENTID_LENGTH + client_id_len;
  if (total_length > MQTT_SN_MESSAGE_CONNECT_MAX_LENGTH) {
    return -1;
  }

  if (dst_len < total_length) {
    return -1;
  }
  return generate_mqtt_sn_header(dst, dst_len, used_bytes, total_length, CONNECT);
}

int32_t parse_connect_header(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len, int32_t *parsed_bytes) {
  if ((*parsed_bytes = parse_header(h, CONNECT, data, data_len, parsed_bytes)) < 0) {
    return -1;
  }
  if (h->length < MQTT_SN_MESSAGE_CONNECT_MIN_LENGTH) {
    return -1;
  }
  if (h->length > MQTT_SN_MESSAGE_CONNECT_MAX_LENGTH) {
    return -1;
  }
  return *parsed_bytes;
}
int32_t parse_connect_flags(const uint8_t *src_pos,
                            uint16_t src_len,
                            int32_t *parsed_bytes,
                            uint8_t *will,
                            uint8_t *clean_session) {
  return parse_mqtt_sn_flags(src_pos, src_len, parsed_bytes, NULL, NULL, NULL, will, clean_session, NULL);
}
int32_t parse_connect_protocol_id(const uint8_t *src_pos,
                                  uint16_t src_len,
                                  int32_t *parsed_bytes,
                                  uint8_t *protocol_id) {
  return parse_mqtt_sn_uint8_byte(src_pos, src_len, parsed_bytes, protocol_id);
}
int32_t parse_connect_duration(const uint8_t *src_pos, uint16_t src_len, int32_t *parsed_bytes, uint16_t *duration) {
  return parse_mqtt_sn_uint16_byte(src_pos, src_len, parsed_bytes, duration);
}
int32_t parse_connect_client_id(const uint8_t *src_pos,
                                uint16_t src_len,
                                int32_t *parsed_bytes,
                                char *client_id,
                                uint16_t *client_id_length,
                                uint8_t client_id_max_length) {
  if (client_id_max_length > MQTT_SN_MESSAGE_CONNECT_CLIENTID_MAX_LENGTH) {
    client_id_max_length = MQTT_SN_MESSAGE_CONNECT_CLIENTID_MAX_LENGTH;
  }
  return parse_mqtt_sn_client_id_byte(src_pos,
                                      src_len,
                                      parsed_bytes,
                                      client_id,
                                      client_id_length,
                                      client_id_max_length);
}

