//
// Created by SomeDude on 19.05.2019.
//

#include "MqttSnPublishMessageParser.h"

int parse_publish(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len) {
  return parse_message_tolerant(h, PUBLISH, data, data_len);
}
