//
// Created by bele on 20.05.19.
//

#include <platform/platform_compatibility.h>
#include <parser/MqttSnMessageParser.h>
#include "MqttSnMessageParserLogging.h"


#define MQTT_SN_MESSAGE_TYPE_STRING_NEGATIVE_ENUMS_OFFSET 2
static const char *MQTT_SN_MESSAGE_TYPE_STRING[] = {
    FOREACH_MQTT_SN_MESSAGE_TYPE(GENERATE_MQTT_SN_MESSAGE_TYPE_STRING)
};

int log_duration(const MqttSnLogger *logger, uint16_t duration) {
  return log_str_key_uint16_value(logger, PSTR("dur"), duration);
}

int log_msg_from(const MqttSnLogger *logger, MQTT_SN_MESSAGE_TYPE msg_type, const device_address *from) {
  if (MQTT_SN_MESSAGE_TYPE_RESERVED(msg_type)) {
    return log_status(logger);
  }
  const char *msg_type_str = MQTT_SN_MESSAGE_TYPE_STRING[msg_type + MQTT_SN_MESSAGE_TYPE_STRING_NEGATIVE_ENUMS_OFFSET];
  log_str(logger, msg_type_str);
  log_str(logger, PSTR(" from "));
  log_device_address(logger, from);
  log_str(logger, PSTR(" "));
  return log_status(logger);
}

int log_msg_to(const MqttSnLogger *logger, MQTT_SN_MESSAGE_TYPE msg_type, const device_address *from) {
  if (MQTT_SN_MESSAGE_TYPE_RESERVED(msg_type)) {
    return log_status(logger);
  }
  const char *msg_type_str = MQTT_SN_MESSAGE_TYPE_STRING[msg_type + MQTT_SN_MESSAGE_TYPE_STRING_NEGATIVE_ENUMS_OFFSET];
  log_str(logger, msg_type_str);
  log_str(logger, PSTR(" to "));
  log_device_address(logger, from);
  log_char(logger, ' ');
  return log_status(logger);
}
int log_clean_session_flag(const MqttSnLogger *logger, uint8_t clean_session) {
  return log_char_key_uint8_value(logger, 'c', clean_session);
}
