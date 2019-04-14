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

int parse_message_tolerant(ParsedMqttSnHeader *h,
                           MQTT_SN_MESSAGE_TYPE msg_type,
                           const uint8_t *data,
                           uint16_t data_len) {
  if (parse_header(h, data, data_len)) {
    return -1;
  }
  if (msg_type != ANY_MESSAGE_TYPE) {
    if (h->msg_type != msg_type) {
      return -1;
    }
  }
  int payload_offset = MQTT_SN_HEADER_LENGTH(h->indicator);
  if (h->msg_type == ENCAPSULATED_MESSAGE) {
    ParsedMqttSnHeader encapsulated_header = {0};
    if (parse_message_tolerant(&encapsulated_header,
                               ANY_MESSAGE_TYPE,
                               data + MQTT_SN_ENCAPSULATED_MESSAGE_HEADER_LENGTH(h->indicator),
                               data_len - MQTT_SN_ENCAPSULATED_MESSAGE_HEADER_LENGTH(h->indicator))) {
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
  return parse_message_tolerant(h, PUBLISH, data, data_len);
}

int parse_connect(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len) {
  return parse_message_tolerant(h, CONNECT, data, data_len);
}

int parse_connack(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len) {
  return parse_message_tolerant(h, CONNACK, data, data_len);
}

int parse_disconnect(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len) {
  return parse_message_tolerant(h, DISCONNECT, data, data_len);
}

int parse_encapsulation(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len) {
  return parse_message_tolerant(h, ENCAPSULATED_MESSAGE, data, data_len);
}

int generate_mqtt_sn_header(uint8_t *dst,
                            uint16_t dst_len,
                            uint16_t msg_len,
                            MQTT_SN_MESSAGE_TYPE msg_type,
                            uint16_t *used_bytes) {
  if (msg_len > UINT8_MAX) {
    *used_bytes = sizeof(MqttSnMessageHeaderThreeOctetsLengthField);
    if (dst_len < *used_bytes) {
      return -1;
    }
    MqttSnMessageHeaderThreeOctetsLengthField *header = (MqttSnMessageHeaderThreeOctetsLengthField *) dst;
    header->indicator = 1;
    header->length = msg_len;
    header->msg_type = msg_type;
    return 0;
  }
  *used_bytes = sizeof(MqttSnMessageHeaderOneOctetLengthField);
  if (dst_len < *used_bytes) {
    return -1;
  }
  MqttSnMessageHeaderOneOctetLengthField *header = (MqttSnMessageHeaderOneOctetLengthField *) dst;
  header->length = msg_len;
  header->msg_type = msg_type;
  return 0;
}

int generate_publish_header(uint8_t *dst, uint16_t dst_len, uint16_t data_len, uint16_t *used_bytes) {
  uint16_t total_length = data_len + MQTT_SN_PUBLISH_MESSAGE_HEADER_LENGTH(0) <= UINT8_MAX ? data_len
      + MQTT_SN_PUBLISH_MESSAGE_HEADER_LENGTH(0) : data_len + MQTT_SN_PUBLISH_MESSAGE_HEADER_LENGTH(1);
  if (dst_len < total_length) {
    return -1;
  }
  return generate_mqtt_sn_header(dst, 0, total_length, PUBLISH, used_bytes);
}

int generate_flags(uint8_t *dst,
                   uint16_t dst_len,
                   uint8_t dup,
                   int8_t qos,
                   uint8_t retain,
                   uint8_t will,
                   uint8_t clean_session,
                   uint8_t topic_id_type,
                   uint16_t *used_bytes) {
  *used_bytes += MQTT_SN_FLAG_LENGTH;
  if (dst_len < *used_bytes) {
    return -1;
  }
  uint8_t *flags = dst;
  if (dup) {
    SET_MQTT_SN_DUP_FLAG_FIRST_TIME(*flags);
  }
  if (qos) {
    if (qos == -1) {
      SET_MQTT_SN_FLAG_QOS_M1(*flags);
    } else if (qos == 0) {
      SET_MQTT_SN_FLAG_QOS_0(*flags);
    } else if (qos == 1) {
      SET_MQTT_SN_FLAG_QOS_1(*flags);
    } else if (qos == 2) {
      SET_MQTT_SN_FLAG_QOS_2(*flags);
    } else {
      return -1;
    }
  }
  if (retain) {
    SET_MQTT_SN_FLAG_RETAIN_TRUE(*flags);
  }
  if (will) {
    SET_MQTT_SN_FLAG_WILL_TRUE(*flags);
  }
  if (clean_session) {
    SET_MQTT_SN_CLEAN_SESSION_TRUE(*flags);
  }
  if (topic_id_type == MQTT_SN_FLAG_TOPIC_ID_TYPE_TOPIC_NAME) {
    SET_MQTT_SN_FLAG_TOPIC_ID_TYPE_TOPIC_NAME(*flags);
  } else if (topic_id_type == MQTT_SN_FLAG_TOPIC_ID_TYPE_PREDEFINED_TOPIC_ID) {
    SET_MQTT_SN_FLAG_TOPIC_ID_TYPE_PREDEFINED_TOPIC_ID(*flags);
  } else if (topic_id_type == MQTT_SN_FLAG_TOPIC_ID_TYPE_SHORT_TOPIC_NAME) {
    SET_MQTT_SN_FLAG_TOPIC_ID_TYPE_SHORT_TOPIC_NAME(*flags);
  } else if (topic_id_type == MQTT_SN_FLAG_TOPIC_ID_TYPE_RESERVED) {
    SET_MQTT_SN_FLAG_TOPIC_ID_TYPE_RESERVED(*flags);
  } else {
    return -1;
  }
  return 0;
}

int generate_topic_id(uint8_t *dst, uint16_t dst_len, uint16_t topic_id, uint16_t *used_bytes) {
  *used_bytes += MQTT_SN_TOPIC_ID_LENGTH;
  if (dst_len < *used_bytes) {
    return -1;
  }
  *dst = topic_id;
  return 0;
}

int generate_msg_id(uint8_t *dst, uint16_t dst_len, uint16_t msg_id, uint16_t *used_bytes) {
  *used_bytes += MQTT_SN_MESSAGE_ID_LENGTH;
  if (dst_len < *used_bytes) {
    return -1;
  }
  *dst = msg_id;
  return 0;
}

int generate_data(uint8_t *dst, uint16_t dst_len, uint8_t *data, uint16_t data_len, uint16_t *used_bytes) {
  *used_bytes += data_len;
  if (dst_len < *used_bytes) {
    return -1;
  }
  memcpy(dst, data, data_len);
  return 0;
}

int generate_publish(uint8_t *dst,
                     uint16_t dst_len,
                     uint8_t dup,
                     int8_t qos,
                     uint8_t retain,
                     uint8_t topic_id_type,
                     uint16_t msg_id,
                     uint32_t topic_id,
                     uint8_t *data,
                     uint16_t data_len) {
  uint16_t used_bytes = 0;
  if (generate_publish_header(dst, dst_len, data_len, &used_bytes)) {
    return -1;
  }
  if (generate_flags(dst + used_bytes,
                     dst_len,
                     dup,
                     qos,
                     retain,
                     MQTT_SN_FLAG_WILL_FALSE,
                     MQTT_SN_FLAG_CLEAN_SESSION_FALSE,
                     topic_id_type,
                     &used_bytes)) {
    return -1;
  }
  if (generate_topic_id(dst + used_bytes, dst_len, topic_id, &used_bytes)) {
    return -1;
  }
  if (generate_msg_id(dst + used_bytes, dst_len, msg_id, &used_bytes)) {
    return -1;
  }
  if (generate_data(dst + used_bytes, dst_len, data, data_len, &used_bytes)) {
    return -1;
  }
  return 0;
}
