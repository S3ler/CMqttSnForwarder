//
// Created by SomeDude on 09.04.2019.
//

#include "MqttSnMessageParser.h"
#include <string.h>
#include <netinet/in.h>

uint16_t get_message_length(const uint8_t *data) {
  MqttSnMessageHeaderThreeOctetsLengthField
      *threeOctetsLengthField = (MqttSnMessageHeaderThreeOctetsLengthField *) data;
  if (threeOctetsLengthField->indicator == 0x01) {
    return ntohs(threeOctetsLengthField->length);
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

  if (MQTT_SN_MESSAGE_TYPE_RESERVED(type)) {
    return RESERVED_INVALID;
  }
  return (MQTT_SN_MESSAGE_TYPE) type;
}
int is_valid_three_bytes_header(const uint8_t *data, ssize_t data_length) {
  MqttSnMessageHeaderThreeOctetsLengthField
      *threeOctetsLengthField = (MqttSnMessageHeaderThreeOctetsLengthField *) data;
  if (threeOctetsLengthField->indicator == 0x01
      && data_length > 2
      && ntohs(threeOctetsLengthField->length) == data_length) {
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
  if (is_three_bytes_header(data) && !is_valid_three_bytes_header(data, data_len)) {
    return -1;
  }
  h->length = get_message_length(data);
  if (h->length != data_len) {
    return -1;
  }
  h->indicator = is_three_bytes_header(data);
  h->msg_type = get_mqtt_sn_message_type(data);
  if (h->msg_type == RESERVED_INVALID) {
    return -1;
  }
  return 0;
}

int parse_message(ParsedMqttSnHeader *h, MQTT_SN_MESSAGE_TYPE msg_type, const uint8_t *data, uint16_t data_len) {
  if (parse_header(h, data, data_len)) {
    return -1;
  }
  if (msg_type != ANY_MESSAGE_TYPE) {
    if (h->msg_type != msg_type) {
      return -1;
    }
  }
  int payload_offset = MQTT_SN_HEADER_OFFSET_LENGTH(h->indicator);
  if (h->msg_type == ENCAPSULATED_MESSAGE) {
    ParsedMqttSnHeader encapsulated_header = {0};
    if (parse_message(&encapsulated_header,
                      ANY_MESSAGE_TYPE,
                      data + MQTT_SN_ENCAPSULATION_MESSAGE_HEADER_LENGTH(h->indicator),
                      data_len - MQTT_SN_ENCAPSULATION_MESSAGE_HEADER_LENGTH(h->indicator))) {
      return -1;
    }
  }
  // TODO check all other message length
  if (h->msg_type == PINGREQ) {
    if (h->length > MQTT_SN_PINGREQ_MESSAGE_HEADER_LENGTH) {
      if (h->length > MQTT_SN_PINGREQ_MESSAGE_HEADER_LENGTH + MQTT_SN_MAX_CLIENT_ID_LENGTH) {
        return -1;
      }
    }
  }
  if (h->msg_type == GWINFO) {
    if (h->length > 3) {
      if (h->length > MQTT_SN_GWINFO_MESSAGE_HEADER_LENGTH + sizeof(device_address)) {
        return -1;
      }
    }
  }
  h->payload = (MqttSnEncapsulatedMessage *) (data + payload_offset);
  return 0;
}

int parse_publish(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len) {
  return parse_message(h, PUBLISH, data, data_len);
}

int parse_connect(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len) {
  return parse_message(h, CONNECT, data, data_len);
}

int parse_connack(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len) {
  return parse_message(h, CONNACK, data, data_len);
}

int parse_disconnect(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len) {
  return parse_message(h, DISCONNECT, data, data_len);
}

int parse_encapsulation(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len) {
  return parse_message(h, ENCAPSULATED_MESSAGE, data, data_len);
}
