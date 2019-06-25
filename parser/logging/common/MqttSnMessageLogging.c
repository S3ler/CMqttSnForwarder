//
// Created by bele on 24.05.19.
//

#include <platform/platform_compatibility.h>
#include "MqttSnMessageLogging.h"

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
