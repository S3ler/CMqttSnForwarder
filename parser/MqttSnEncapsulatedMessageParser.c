//
// Created by SomeDude on 19.05.2019.
//

#include "MqttSnEncapsulatedMessageParser.h"
#include <string.h>

int32_t parse_encapsulation_message(uint8_t *broadcast,
                                    device_address *wireless_node_id,
                                    uint8_t *mqtt_sn_message,
                                    uint16_t *mqtt_sn_message_len,
                                    uint16_t max_mqtt_sn_message_len,
                                    uint8_t *data,
                                    uint16_t data_len) {
  ParsedMqttSnHeader h = {0};
  int32_t read_bytes = 0;
  if (parse_encapsulation_header(&h, data, data_len, &read_bytes)) {
    return -1;
  }
  MqttSnEncapsulatedMessage *encapsulated_header = (MqttSnEncapsulatedMessage *) h.payload;
  if (encapsulated_header->crtl >= MQTT_SN_ENCAPSULATED_MESSAGE_CRTL_BROADCAST_RADIUS) {
    return -1;
  }
  *broadcast = encapsulated_header->crtl;
  *wireless_node_id = encapsulated_header->wireless_node_id;
  *mqtt_sn_message_len = data_len - read_bytes;
  if (*mqtt_sn_message > max_mqtt_sn_message_len) {
    return -1;
  }
  memcpy(mqtt_sn_message, &data[read_bytes], *mqtt_sn_message_len);
  read_bytes += *mqtt_sn_message_len;
  return read_bytes;
}

int parse_encapsulation_header(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len, int32_t *read_bytes) {
  if (data_len < 2) {
    return -1;
  }
  h->indicator = is_three_bytes_header(data);
  h->length = get_message_length(data);
  h->msg_type = get_mqtt_sn_message_type(data);
  if (h->msg_type != ENCAPSULATED_MESSAGE) {
    return -1;
  }
  if (h->length < MQTT_SN_ENCAPSULATED_MESSAGE_HEADER_LENGTH(h->indicator)) {
    return -1;
  }
  if (h->length != data_len) {
    return -1;
  }
  h->payload = (void *) &data[MQTT_SN_HEADER_LENGTH(h->indicator)];
  *read_bytes += MQTT_SN_ENCAPSULATED_MESSAGE_HEADER_LENGTH(h->indicator);
  return 0;
}