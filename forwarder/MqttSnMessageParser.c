//
// Created by SomeDude on 09.04.2019.
//

#include "MqttSnMessageParser.h"
#include <string.h>

uint16_t get_message_length(const uint8_t *data) {
  MqttSnMessageHeaderThreeOctetsLengthField
      *threeOctetsLengthField = (MqttSnMessageHeaderThreeOctetsLengthField *) data;
  if (threeOctetsLengthField->indicator == 0x01) {
    uint16_t length = 0;
    length += (((uint16_t) threeOctetsLengthField->msb_length) << 8);
    length += (((uint16_t) threeOctetsLengthField->lsb_length) << 0);
    return length;
  }
  MqttSnMessageHeaderOneOctetLengthField
      *oneOctetLengthField = (MqttSnMessageHeaderOneOctetLengthField *) data;
  return oneOctetLengthField->length;
}
MQTT_SN_MESSAGE_TYPE get_mqtt_sn_message_type(const uint8_t *data) {
  uint8_t type = 0;
  if (is_three_bytes_header(data)) {
    type = data[3];
  } else {
    type = data[1];
  }

  if (type == RESERVED_03 || type == RESERVED_11 || type == RESERVED_19 ||
      (type >= RESERVED_1E && type <= RESERVED_FD) || type == RESERVED_FF) {
    return RESERVED_INVALID;
  }
  return (MQTT_SN_MESSAGE_TYPE) type;
}
int is_valid_three_bytes_header(const uint8_t *data, ssize_t data_length) {
  MqttSnMessageHeaderThreeOctetsLengthField
      *threeOctetsLengthField = (MqttSnMessageHeaderThreeOctetsLengthField *) data;
  if (threeOctetsLengthField->indicator == 0x01 && data_length > 2) {
    return 1;
  }
  return 0;
}
uint8_t is_three_bytes_header(const uint8_t *data) {
  MqttSnMessageHeaderThreeOctetsLengthField
      *threeOctetsLengthField = (MqttSnMessageHeaderThreeOctetsLengthField *) data;
  if (threeOctetsLengthField->indicator == 0x01) {
    return 1;
  }
  return 0;
}

int parse_header(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len) {
  if (data_len < 2) {
    return -1;
  }
  if (!is_valid_three_bytes_header(data, data_len) && is_three_bytes_header(data)) {
    return -1;
  }
  h->length = get_message_length(data);
  if (h->length != data_len) {
    return -1;
  }
  h->indicator = is_three_bytes_header(data);
  h->msg_type = get_mqtt_sn_message_type(data);
  return 0;
}

int parse_publish(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len) {
  if (parse_header(h, data, data_len)) {
    return -1;
  }
  int offset = (h->indicator ? 4 : 2);
  h->payload = (MqttSnMessageConnect *) (data + offset);
  return 0;
}

int parse_connect(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len) {
  if (parse_header(h, data, data_len)) {
    return -1;
  }
  int offset = (h->indicator ? 4 : 2);
  h->payload = (MqttSnMessageConnect *) (data + offset);
  return 0;
}
int parse_connack(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len) {
  if (parse_header(h, data, data_len)) {
    return -1;
  }
  int offset = (h->indicator ? 4 : 2);
  h->payload = (MqttSnMessageConnack *) (data + offset);
  return 0;
}

int parse_disconnect(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len) {
  if (parse_header(h, data, data_len)) {
    return -1;
  }
  int offset = (h->indicator ? 4 : 2);
  h->payload = (MqttSnMessageDisconnect *) (data + offset);
  return 0;
}

int parse_encapsulation(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len) {
  if (parse_header(h, data, data_len)) {
    return -1;
  }
  if (h->msg_type != Encapsulated_message) {
    return -1;
  }
  int offset = (h->indicator ? 4 : 2);
  if (h->length < (offset + 1 + sizeof(device_address))) {// 1 = Ctrl
    return -1;
  }
  h->payload = (MqttSnEncapsulatedMessage *) (data + offset);
  return 0;
}
