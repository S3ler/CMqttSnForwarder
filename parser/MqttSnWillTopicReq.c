//
// Created by SomeDude on 21.05.2019.
//

#include "MqttSnWillTopicReq.h"

static int32_t parse_will_topic_req_header(ParsedMqttSnHeader *h,
                                           const uint8_t *data,
                                           uint16_t data_len,
                                           int32_t *parsed_bytes);

int32_t parse_will_topic_req_byte(const uint8_t *data, uint16_t data_len) {
  int32_t parsed_bytes = 0;
  ParsedMqttSnHeader h = {0};
  if ((parsed_bytes = parse_will_topic_req_header(&h, data, data_len, &parsed_bytes) < 0)) {
    return -1;
  }
  return parsed_bytes;
}

int32_t parse_will_topic_req_header(ParsedMqttSnHeader *h,
                                    const uint8_t *data,
                                    uint16_t data_len,
                                    int32_t *parsed_bytes) {
  if (parse_header(h, WILLTOPICREQ, data, data_len, parsed_bytes) < 0) {
    return -1;
  }
  if (h->length != MQTT_SN_MESSAGE_WILLTOPICREQ_LENGTH) {
    // invalid message length for will topic request
    return -1;
  }
  return *parsed_bytes;
}
