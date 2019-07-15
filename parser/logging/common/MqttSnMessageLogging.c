//
// Created by bele on 24.05.19.
//

#include <platform/platform_compatibility.h>
#include <parser/MqttSnAdvertiseMessage.h>
#include <parser/logging/MqttSnAdvertiseMessageLogging.h>
#include <parser/MqttSnSearchGwMessage.h>
#include <parser/logging/MqttSnSearchGwMessageLogging.h>
#include <parser/MqttSnGwInfoMessage.h>
#include <parser/logging/MqttSnGwInfoMessageLogging.h>
#include "MqttSnMessageLogging.h"

#define MQTT_SN_MESSAGE_TYPE_STRING_NEGATIVE_ENUMS_OFFSET 2
static const char *MQTT_SN_MESSAGE_TYPE_STRING[] = {
    FOREACH_MQTT_SN_MESSAGE_TYPE(GENERATE_MQTT_SN_MESSAGE_TYPE_STRING)
};

int32_t log_mqtt_sn_message_type(const MqttSnLogger *logger, MQTT_SN_MESSAGE_TYPE msg_type) {
  const char *msg_type_str = MQTT_SN_MESSAGE_TYPE_STRING[msg_type + MQTT_SN_MESSAGE_TYPE_STRING_NEGATIVE_ENUMS_OFFSET];
  return log_str(logger, msg_type_str);
}

int32_t log_mqtt_sn_comma(const MqttSnLogger *logger) {
  return log_str(logger, PSTR(", "));
}

int32_t log_mqtt_sn_flags_single(const MqttSnLogger *logger,
                                 uint8_t dup,
                                 int8_t qos,
                                 uint8_t retain,
                                 uint8_t will,
                                 uint8_t clean_session,
                                 uint8_t topic_id_type) {
  log_mqtt_sn_dup_flag(logger, dup);
  log_mqtt_sn_comma(logger);

  log_mqtt_sn_qos_flag(logger, qos);
  log_mqtt_sn_comma(logger);

  log_mqtt_sn_retain_flag(logger, retain);
  log_mqtt_sn_comma(logger);

  log_mqtt_sn_will_flag(logger, will);
  log_mqtt_sn_comma(logger);

  log_mqtt_sn_clean_session_flag(logger, clean_session);
  log_mqtt_sn_comma(logger);

  log_mqtt_sn_topic_id_type_flag(logger, topic_id_type);
  log_mqtt_sn_comma(logger);

  return log_status(logger);
}
int32_t log_mqtt_sn_dup_flag(const MqttSnLogger *logger, uint8_t dup) {
  return log_char_key_uint8_value(logger, 'd', dup);
}
int32_t log_mqtt_sn_qos_flag(const MqttSnLogger *logger, int8_t qos) {
  return log_char_key_int8_value(logger, 'q', qos);
}
int32_t log_mqtt_sn_retain_flag(const MqttSnLogger *logger, uint8_t retain) {
  return log_char_key_uint8_value(logger, 'r', retain);
}
int32_t log_mqtt_sn_will_flag(const MqttSnLogger *logger, uint8_t will) {
  return log_char_key_uint8_value(logger, 'w', will);
}
int32_t log_mqtt_sn_clean_session_flag(const MqttSnLogger *logger, uint8_t clean_session) {
  return log_char_key_uint8_value(logger, 'c', clean_session);
}
int32_t log_mqtt_sn_topic_id_type_flag(const MqttSnLogger *logger, uint8_t topicIdType) {
  return log_str_key_uint8_value(logger, PSTR("tt"), topicIdType);
}
int log_client_mqtt_sn_message_malformed(const MqttSnLogger *logger,
                                         const device_address *from,
                                         const uint8_t *data,
                                         uint16_t data_len,
                                         uint8_t signal_strength) {
  return log_mqtt_sn_message_malformed(logger, MQTT_SN_FORWARDER_NETWORK_CLIENT, from, data, data_len, signal_strength);
}
int log_mqtt_sn_message_malformed(const MqttSnLogger *logger,
                                  MQTT_SN_FORWARDER_NETWORK network,
                                  const device_address *from,
                                  const uint8_t *data,
                                  uint16_t data_len,
                                  uint8_t signal_strength) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
    return log_status(logger);
  }
  log_msg_start(logger);
  log_str(logger, PSTR("received malformed "));
  if (network == MQTT_SN_FORWARDER_NETWORK_CLIENT) {
    log_str(logger, PSTR("client"));
  }
  if (network == MQTT_SN_FORWARDER_NETWORK_GATEWAY) {
    log_str(logger, PSTR("gateway"));
  }
  if (signal_strength) {
    log_str(logger, PSTR(" signal strength: "));
    log_uint8(logger, signal_strength);
  }
  log_str(logger, PSTR(" message"));
  if (from) {
    log_str(logger, PSTR(" from "));
    log_device_address(logger, from);
  }
  log_str(logger, PSTR(" ( len"));
  log_uint16(logger, data_len);

#ifdef WITH_DEBUG_LOGGING
  if (!shall_not_be_logged(logger, LOG_LEVEL_DEBUG)) {
    log_str(logger, PSTR(", data( "));
    log_uint8_array(logger, data, data_len);
    log_str(logger, PSTR(")"));
  }
#endif

  log_str(logger, PSTR(")."));
  log_flush(logger);
  return log_status(logger);
}
int32_t log_malformed_message(const MqttSnLogger *logger,
                              MQTT_SN_MESSAGE_TYPE msg_type,
                              const device_address *from,
                              uint8_t signal_strength,
                              const uint8_t *data,
                              uint16_t data_len) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
    return log_status(logger);
  }
  log_msg_start(logger);
  log_str(logger, PSTR("received malformed "));
  log_mqtt_sn_message_type(logger, msg_type);

  log_str(logger, PSTR(" message"));

  log_str(logger, PSTR(" from "));
  log_device_address(logger, from);

  log_str(logger, PSTR(" signal strength: "));
  log_uint8(logger, signal_strength);

  log_str(logger, PSTR(" ( len"));
  log_uint16(logger, data_len);

#ifdef WITH_DEBUG_LOGGING
  if (!shall_not_be_logged(logger, LOG_LEVEL_DEBUG) && data != NULL) {
    log_str(logger, PSTR(", data( "));
    log_uint8_array(logger, data, data_len);
    log_str(logger, PSTR(")"));
  }
#endif
  log_str(logger, PSTR(")"));
  log_flush(logger);
  return log_status(logger);
}
int32_t log_msg_gen_error(const MqttSnLogger *logger,
                          MQTT_SN_MESSAGE_TYPE msg_type,
                          const char *file_name,
                          const char *function_name,
                          int line_number) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
    return log_status(logger);
  }
  log_msg_start(logger);

  log_str(logger, PSTR("msg gen error: "));
  log_mqtt_sn_message_type(logger, msg_type);
  log_str(logger, PSTR(" file: "));
  log_str(logger, file_name);
  log_str(logger, PSTR(" function: "));
  log_str(logger, function_name);
  log_str(logger, PSTR(" line: "));
  log_int32(logger, line_number);

  log_flush(logger);
  return log_status(logger);
}
int32_t log_malformed_message_header(const MqttSnLogger *logger,
                                     const device_address *from,
                                     uint8_t signal_strength,
                                     const uint8_t *data,
                                     uint16_t data_len) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
    return log_status(logger);
  }
  log_msg_start(logger);
  log_str(logger, PSTR("received malformed header "));

  log_str(logger, PSTR(" message"));

  log_str(logger, PSTR(" from "));
  log_device_address(logger, from);

  log_str(logger, PSTR(" signal strength: "));
  log_uint8(logger, signal_strength);

  log_str(logger, PSTR(" ( len"));
  log_uint16(logger, data_len);

#ifdef WITH_DEBUG_LOGGING
  if (!shall_not_be_logged(logger, LOG_LEVEL_DEBUG) && data != NULL) {
    log_str(logger, PSTR(", data( "));
    log_uint8_array(logger, data, data_len);
    log_str(logger, PSTR(")"));
  }
#endif
  log_str(logger, PSTR(")"));
  log_flush(logger);
  return log_status(logger);
}
int32_t log_any_message(const MqttSnLogger *logger,
                        const device_address *from,
                        uint8_t signal_strength,
                        const uint8_t *data,
                        uint16_t data_length) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
    return log_status(logger);
  }
  log_msg_start(logger);
  return print_any_message(logger, from, signal_strength, data, data_length);
}
int32_t print_any_message(const MqttSnLogger *logger,
                          const device_address *from,
                          uint8_t signal_strength,
                          const uint8_t *data,
                          uint16_t data_length) {
  ParsedMqttSnHeader header = {0};
  int32_t parsed_header_bytes = 0;
  if (parse_header(&header,
                   ANY_MESSAGE_TYPE,
                   data,
                   data_length,
                   &parsed_header_bytes) < 0) {
#ifdef WITH_LOGGING
    log_malformed_message_header(logger, from, signal_strength, data, data_length);
#endif
    return 0;
  }
  log_str(logger, PSTR("from "));
  log_device_address(logger, from);
  log_str(logger, PSTR(" signal strength "));
  log_signal_strength(logger, signal_strength);

  switch (header.msg_type) {
    case ADVERTISE: {
      MqttSnAdvertise advertise = {0};
      if (parse_advertise(&advertise, data, data_length) > 0) {
        return log_advertise_message_struct(logger, &advertise);
      }
      break;
    }
    case SEARCHGW: {
      MqttSnSearchGw search_gw = {0};
      if (parse_searchgw(&search_gw, data, data_length) > 0) {
        return log_searchgw_message_struct(logger, &search_gw);
      }
      break;
    }
    case GWINFO: {
      MqttSnGwInfo gw_info = {0};
      if (parse_gwinfo(&gw_info, data, data_length) > 0) {
        return log_gwinfo_message_struct(logger, &gw_info);
      }
      break;
    }
    default:break;
  }
  return log_malformed_message(logger, ANY_MESSAGE_TYPE, from, signal_strength, data, data_length);
}
