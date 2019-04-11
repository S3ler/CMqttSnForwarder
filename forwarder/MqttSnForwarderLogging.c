//
// Created by SomeDude on 09.04.2019.
//

#include <time.h>
#include "MqttSnForwarderLogging.h"
#include "MqttSnMessageParser.h"
#include <string.h>
#include <stdint.h>
#include <logging/linux/stdout/StdoutLogging.h>
#include <netinet/in.h>

int log_int8(const MqttSnLogger *logger, int8_t n) {
  const char *int8_max_str = "255";
  int int8_max_str_length = strlen(int8_max_str);
  char n_str[int8_max_str_length + 1];
  snprintf((char *) &n_str, int8_max_str_length + 1, "%d", n);
  return log_str(logger, n_str);
}

int log_uint8(const MqttSnLogger *logger, uint8_t n) {
  const char *uint8_max_str = "255";
  int uint8_max_str_length = strlen(uint8_max_str);
  char n_str[uint8_max_str_length + 1];
  snprintf((char *) &n_str, uint8_max_str_length + 1, "%u", n);
  return log_str(logger, n_str);
}

int log_uint16(const MqttSnLogger *logger, uint16_t n) {
  const char *uint16_max_str = "65535";
  int uint16_max_str_length = strlen(uint16_max_str);
  char n_str[uint16_max_str_length + 1];
  snprintf((char *) &n_str, uint16_max_str_length + 1, "%u", n);
  return log_str(logger, n_str);
}

int log_uint32(const MqttSnLogger *logger, uint32_t n) {
  const char *uint32_max_str = "4294967295"; // uint32_t 4294967295
  int uint32_max_str_length = strlen(uint32_max_str);
  char n_str[uint32_max_str_length + 1];
  snprintf((char *) &n_str, uint32_max_str_length + 1, "%u", n);
  return log_str(logger, n_str);
}

int log_uintmax(const MqttSnLogger *logger, uintmax_t n) {
  const char *uintmax_str = "18446744073709551615"; // uintmax_t 18446744073709551615
  int uintmax_str_length = strlen(uintmax_str);
  char t_str[uintmax_str_length + 1];
  snprintf((char *) &t_str, uintmax_str_length + 1, "%ju", n);
  return log_str(logger, t_str);
}

int get_timestamp(uint64_t *t) {
  time_t result = time(NULL);
  if (result == -1) {
    return -1;
  }
  *t = result;
  return 0;
}

int log_current_time(const MqttSnLogger *logger) {
  uintmax_t t;
  if (get_timestamp(&t)) {
    return -1;
  }
  return log_uintmax(logger, t);
}

int log_msg_start(const MqttSnLogger *logger) {
  if (log_current_time(logger)) { return -1; }
  const char *ds = ": ";
  return log_str(logger, ds);
}

int log_device_address(const MqttSnLogger *logger, const device_address *address) {
  const char *dot = ".";
  for (size_t i = 0; i < sizeof(device_address); ++i) {
    if (log_uint8(logger, address->bytes[i])) {
      return -1;
    }
    if (i + 1 < sizeof(device_address)) {
      if (log_str(logger, dot)) {
        return -1;
      }
    }
  }
  return 0;
}

int log_forwarder_started(const MqttSnLogger *logger,
                          int level,
                          const char *version,
                          int major,
                          int minor,
                          int tweak,
                          const char *build_date) {
  if (is_logger_broken(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return 0;
  }

  const char *name_version = ": cmqttsnforwarder version ";
  const char *is = " is ";
  const char *dot = ".";
  const char *build_date_str = " (build date ";
  const char *action = ") started.";

  return (log_current_time(logger) ||
      log_str(logger, name_version) ||
      log_str(logger, version) ||
      log_str(logger, is) ||
      log_uint32(logger, major) ||
      log_str(logger, dot) ||
      log_uint32(logger, minor) ||
      log_str(logger, dot) ||
      log_uintmax(logger, tweak) ||
      log_str(logger, build_date_str) ||
      log_str(logger, build_date) ||
      log_str(logger, action) ||
      log_flush(logger) != 0);
}
int log_forwarder_terminated(const MqttSnLogger *logger,
                             int level,
                             const char *version,
                             uint32_t major,
                             uint32_t minor,
                             uintmax_t tweak) {
  if (is_logger_broken(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return 0;
  }

  const char *name_version = ": cmqttsnforwarder version ";
  const char *is = " is ";
  const char *dot = ".";
  const char *action = " terminated.";

  return (log_current_time(logger) ||
      log_str(logger, name_version) ||
      log_str(logger, version) ||
      log_str(logger, is) ||
      log_uint32(logger, major) ||
      log_str(logger, dot) ||
      log_uint32(logger, minor) ||
      log_str(logger, dot) ||
      log_uintmax(logger, tweak) ||
      log_str(logger, action) ||
      log_flush(logger) != 0);
}
int MqttSnLoggerInit(MqttSnLogger *logger, log_level_t log_level) {
  logger->log_init = stdout_log_init;
  logger->log_deinit = stdout_log_deinit;
  logger->log_flush = stdout_log_flush;
  logger->log_str = stdout_log_str;
  logger->log_char = stdout_log_char;
  logger->log_level = log_level;
  logger->status = 0;
  return logger->log_init(logger);
}

void MqttSnLoggerDeinit(MqttSnLogger *logger) {
  logger->log_deinit(logger);
  logger->status = -1;
}

int is_logger_broken(const MqttSnLogger *logger) {
  if (logger->status) {
    return 1;
  }
  return 0;
}

int shall_not_be_logged(const MqttSnLogger *logger, int max_level) {
  if (logger->log_level <= LOG_LEVEL_QUIET) {
    return 1;
  }
  if (logger->log_level < max_level) {
    return 1;
  }
  return 0;
}

int log_flush(const MqttSnLogger *logger) {
  return logger->log_flush(logger);
}
int log_char(const MqttSnLogger *logger, char c) {
  return logger->log_char(c);
}
int log_str(const MqttSnLogger *logger, const char *str) {
  return logger->log_str(str);
}

int log_uint8_array(const MqttSnLogger *logger, const uint8_t *data, uint16_t data_length) {
  const char *comma = ", ";
  for (uint16_t i = 0; i < data_length; ++i) {
    if (log_uint8(logger, data[i])) {
      return -1;
    }
    if (i + 1 < data_length) {
      if (log_str(logger, comma)) {
        return -1;
      }
    }
  }
  return 0;
}
int log_client_mqtt_sn_message(const MqttSnLogger *logger,
                               int level,
                               const device_address *address,
                               const uint8_t *data,
                               uint16_t data_len) {
  if (is_logger_broken(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return 0;
  }

  int invalid_three_bytes_header = is_valid_three_bytes_header(data, data_len);
  uint16_t mqtt_sn_message_length = get_message_length(data);
  if (!invalid_three_bytes_header) {
    // invalid header
    // TODO lösen
  }
  if (data_len < mqtt_sn_message_length) {
    // too short
    // TODO lösen
  }

  MQTT_SN_MESSAGE_TYPE mqtt_sn_message_type = get_mqtt_sn_message_type(data);
  switch (mqtt_sn_message_type) {
    case PUBLISH: return log_client_publish_message(logger, level, address, data, data_len);
    case CONNECT: return log_client_connect_message(logger, level, address, data, data_len);
    case DISCONNECT: return log_client_disconnect_message(logger, level, address, data, data_len);
    default:
      // unknown or not logged message for LOG_LEVEL_DEFAULT
      break;
  }

  if (level < LOG_LEVEL_VERBOSE) {
    return 0;
  }

  switch (mqtt_sn_message_type) {
    case PUBACK: return 0; // TODO implement
    case ADVERTISE: return 0; // TODO implement
    case SEARCHGW: return 0; // TODO implement
    case GWINFO: return 0; // TODO implement
    case REGISTER: return 0; // TODO implement
    case REGACK: return 0; // TODO implement
    case SUBSCRIBE: return 0; // TODO implement
    case SUBACK: return 0; // TODO implement
    case UNSUBSCRIBE: return 0; // TODO implement
    case UNSUBACK: return 0; // TODO implement
    default:
      // unknown or not logged message for LOG_LEVEL_DEFAULT
      break;
  }

  return 0;
}

int log_gateway_mqtt_sn_message(const MqttSnLogger *logger,
                                int level,
                                const device_address *address,
                                const uint8_t *data,
                                uint16_t data_len,
                                const char *description) {
  if (is_logger_broken(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return 0;
  }

  const char *d = ": ";
  if (log_current_time(logger)) {
    return -1;
  }
  if (log_str(logger, d)) {
    return -1;
  }
  if (description) {
    if (log_str(logger, description)) {
      return -1;
    }
  }

  MQTT_SN_MESSAGE_TYPE mqtt_sn_message_type = get_mqtt_sn_message_type(data);
  switch (mqtt_sn_message_type) {
    case PUBLISH: return log_gateway_publish_message(logger, level, address, data, data_len);
    case CONNACK:return log_gateway_connack_message(logger, level, address, data, data_len);
    case DISCONNECT:return log_gateway_disconnect_message(logger, level, address, data, data_len);
    default:
      // unknown or not logged message for LOG_LEVEL_DEFAULT
      break;
  }

  if (level < LOG_LEVEL_VERBOSE) {
    return 0;
  }

  switch (mqtt_sn_message_type) {
    case PUBACK: return 0; // TODO implement
    case ADVERTISE: return 0; // TODO implement
    case SEARCHGW: return 0; // TODO implement
    case GWINFO: return 0; // TODO implement
    case REGISTER: return 0; // TODO implement
    case REGACK: return 0; // TODO implement
    case SUBSCRIBE: return 0; // TODO implement
    case SUBACK: return 0; // TODO implement
    case UNSUBSCRIBE: return 0; // TODO implement
    case UNSUBACK: return 0; // TODO implement
    default:
      // unknown or not logged message for LOG_LEVEL_DEFAULT
      break;
  }

  return 0;
}
int log_client_connect_message(const MqttSnLogger *logger,
                               int level,
                               const device_address *address,
                               const uint8_t *data,
                               uint16_t data_len) {

  ParsedMqttSnHeader h = {0};
  if (parse_connect(&h, data, data_len)) {
    // TODO decide what to do
    return (log_current_time(logger) ||
        log_str(logger, "Invalid Message.") ||
        log_flush(logger) != 0);
  }

  MqttSnMessageConnect *c = (MqttSnMessageConnect *) h.payload;
  uint8_t flags = c->flags;
  uint8_t protocol_id = ntohs(c->protocolId);
  uint16_t duration = ntohs(c->duration);
  const char *client_id = c->clientId;

  const char *connect_from = ": CONNECT from ";
  const char *as = " as ";
  const char *open_braked = "( ";
  const char *comma = ", ";
  const char *close_braked = ").";

  return (log_current_time(logger) ||
      log_str(logger, connect_from) ||
      log_device_address(logger, address) ||
      log_str(logger, as) ||
      log_str(logger, client_id) ||
      log_str(logger, open_braked) ||
      log_mqtt_sn_flags(logger, flags) ||
      log_str(logger, comma) ||
      log_protocol_id(logger, protocol_id) ||
      log_str(logger, comma) ||
      log_duration(logger, duration) ||
      log_str(logger, close_braked) ||
      log_flush(logger) != 0);
}

int log_gateway_connack_message(const MqttSnLogger *logger,
                                int level,
                                const device_address *address,
                                const uint8_t *data,
                                uint16_t data_len) {
  ParsedMqttSnHeader h = {0};
  if (parse_connack(&h, data, data_len)) {
    // TODO decide what to do
    return (log_current_time(logger) ||
        log_str(logger, "Invalid Message.") ||
        log_flush(logger) != 0);
  }
  MqttSnMessageConnack *c = (MqttSnMessageConnack *) h.payload;
  uint8_t return_code = c->returnCode;

  const char *connack_to = "CONNACK to ";
  const char *open_braked = " ( ";
  const char *close_braked = ").";
  // 1554750679: CONNACK to 127.0.0.1 (rc0).

  return (
      log_str(logger, connack_to) ||
          log_device_address(logger, address) ||
          log_str(logger, open_braked) ||
          log_return_code(logger, return_code) ||
          log_str(logger, close_braked) ||
          log_flush(logger) != 0);
}

int log_client_publish_message(const MqttSnLogger *logger,
                               int level,
                               const device_address *address,
                               const uint8_t *data,
                               uint16_t data_len) {
  ParsedMqttSnHeader h = {0};
  if (parse_publish(&h, data, data_len)) {
    // TODO decide what to do
    return (log_current_time(logger) ||
        log_str(logger, "Invalid Message.") ||
        log_flush(logger) != 0);
  }

  const char *publish_from = ": client PUBLISH from ";
  const char *open_braked = " ( ";
  const char *comma = ", ";
  const char *dots = "... (";
  const char *bytes_close_braked = " bytes)).";

  MqttSnMessagePublish *p = (MqttSnMessagePublish *) h.payload;
  uint16_t msg_id = ntohs(p->msgId);
  uint16_t topic_id = ntohs(((MqttSnMessagePublish *) h.payload)->topicId);
  uint8_t flags = ((MqttSnMessagePublish *) h.payload)->flags;
  uint16_t msg_bytes = h.length;

  return (log_current_time(logger) ||
      log_str(logger, publish_from) ||
      log_device_address(logger, address) ||
      log_str(logger, open_braked) ||
      log_message_id(logger, msg_id) ||
      log_str(logger, comma) ||
      log_topic_id(logger, topic_id) ||
      log_str(logger, comma) ||
      log_mqtt_sn_flags(logger, flags) ||
      log_str(logger, comma) ||
      log_str(logger, dots) ||
      log_uint16(logger, msg_bytes) ||
      log_str(logger, bytes_close_braked) ||
      log_flush(logger) != 0);
}

int log_gateway_publish_message(const MqttSnLogger *logger,
                                int level,
                                const device_address *address,
                                const uint8_t *data,
                                uint16_t data_len) {
  ParsedMqttSnHeader h = {0};
  if (parse_publish(&h, data, data_len)) {
    return 0;
  }

  const char *publish_to = "gateway PUBLISH to ";
  const char *open_braked = " ( ";
  const char *comma = ", ";
  const char *dots = "... (";
  const char *bytes_close_braked = " bytes)).";

  MqttSnMessagePublish *p = (MqttSnMessagePublish *) h.payload;
  uint16_t msg_id = ntohs(p->msgId);
  uint16_t topic_id = ntohs(((MqttSnMessagePublish *) h.payload)->topicId);
  uint8_t flags = ((MqttSnMessagePublish *) h.payload)->flags;
  uint16_t msg_bytes = h.length;

  return (
      log_str(logger, publish_to) ||
          log_device_address(logger, address) ||
          log_str(logger, open_braked) ||
          log_message_id(logger, msg_id) ||
          log_str(logger, comma) ||
          log_topic_id(logger, topic_id) ||
          log_str(logger, comma) ||
          log_mqtt_sn_flags(logger, flags) ||
          log_str(logger, comma) ||
          log_str(logger, dots) ||
          log_uint16(logger, msg_bytes) ||
          log_str(logger, bytes_close_braked) ||
          log_flush(logger) != 0);
}

int log_duration(const MqttSnLogger *logger, uint16_t duration) {
  const char *d = "d";
  return (log_str(logger, d) || log_uint16(logger, duration) != 0);
}
int log_protocol_id(const MqttSnLogger *logger, uint8_t protocolId) {
  const char *p = "p";
  return (log_str(logger, p) || log_uint8(logger, protocolId) != 0);
}

int log_mqtt_sn_flags(const MqttSnLogger *logger, uint8_t flags) {

  const char *comma = ", ";

  const char *d = "d";
  uint8_t dup = (flags & MQTT_SN_FLAG_DUP) >> MQTT_SN_FLAG_DUP_SHIFT;
  const char *q = "q";
  int8_t qos = ((flags & MQTT_SN_FLAG_QOS) >> MQTT_SN_FLAG_QOS_SHIFT);
  if (qos > 2) {
    qos = -1;
  }
  const char *r = "r";
  uint8_t retain = (flags & MQTT_SN_FLAG_RETAIN) >> MQTT_SN_FLAG_RETAIN_SHIFT;
  const char *w = "w";
  uint8_t will = (flags & MQTT_SN_FLAG_WILL) >> MQTT_SN_FLAG_WILL_SHIFT;
  const char *c = "c";
  uint8_t clean_session = (flags & MQTT_SN_FLAG_CLEAN_SESSION) >> MQTT_SN_FLAG_CLEAN_SESSION_SHIFT;
  const char *tt = "tt";
  uint8_t topic_id_type = (flags & MQTT_SN_FLAG_TOPIC_ID_TYPE) >> MQTT_SN_FLAG_TOPIC_ID_TYPE_SHIFT;//TODO

  return (log_str(logger, d) || log_uint8(logger, dup) || log_str(logger, comma) ||
      log_str(logger, q) || log_int8(logger, qos) || log_str(logger, comma) ||
      log_str(logger, r) || log_uint8(logger, retain) || log_str(logger, comma) ||
      log_str(logger, w) || log_uint8(logger, will) || log_str(logger, comma) ||
      log_str(logger, c) || log_uint8(logger, clean_session) || log_str(logger, comma) ||
      log_str(logger, tt) || log_uint8(logger, topic_id_type) != 0);
}

int log_message_id(const MqttSnLogger *logger, uint16_t msgId) {
  const char *m = "m";
  return (log_str(logger, m) || log_uint16(logger, msgId) != 0);
}
int log_topic_id(const MqttSnLogger *logger, uint16_t topicId) {
  const char *t = "t";
  return (log_str(logger, t) || log_uint16(logger, topicId) != 0);

}
int log_return_code(const MqttSnLogger *logger, uint8_t returnCode) {
  const char *rc = "rc";
  return (log_str(logger, rc) || log_uint8(logger, returnCode) != 0);
}
int log_client_disconnect_message(const MqttSnLogger *logger,
                                  int level,
                                  const device_address *address,
                                  const uint8_t *data,
                                  uint16_t data_len) {
  ParsedMqttSnHeader h = {0};
  if (parse_disconnect(&h, data, data_len)) {
    // TODO decide what to do
    return (log_current_time(logger) ||
        log_str(logger, "Invalid Message.") ||
        log_flush(logger) != 0);
  }

  const char *disconnect_from = ": client DISCONNECT from ";
  const char *open_braked = " (";
  const char *close_braked = " ).";

  if (h.length == 2) {
    return (log_current_time(logger) ||
        log_str(logger, disconnect_from) ||
        log_device_address(logger, address) ||
        log_flush(logger) != 0);
  }
  if (h.length == 4) {
    MqttSnMessageDisconnect *p = (MqttSnMessageDisconnect *) h.payload;
    uint16_t duration = ntohs(p->duration);

    return (log_current_time(logger) ||
        log_str(logger, disconnect_from) ||
        log_device_address(logger, address) ||
        log_str(logger, open_braked) ||
        log_duration(logger, duration) ||
        log_str(logger, close_braked) ||
        log_flush(logger) != 0);
  }
  return 0;
}
int log_gateway_disconnect_message(const MqttSnLogger *logger,
                                   int level,
                                   const device_address *address,
                                   const uint8_t *data,
                                   uint16_t data_len) {
  ParsedMqttSnHeader h = {0};
  if (parse_disconnect(&h, data, data_len)) {
    // TODO decide what to do
    return (log_current_time(logger) ||
        log_str(logger, "Invalid Message.") ||
        log_flush(logger) != 0);
  }

  const char *disconnect_from = "gateway DISCONNECT from ";
  const char *open_braked = " (";
  const char *close_braked = " ).";

  if (h.length == 2) {
    return (log_current_time(logger) ||
        log_str(logger, disconnect_from) ||
        log_device_address(logger, address) ||
        log_flush(logger) != 0);
  }
  if (h.length == 4) {
    MqttSnMessageDisconnect *p = (MqttSnMessageDisconnect *) h.payload;
    uint16_t duration = ntohs(p->duration);

    return (
        log_str(logger, disconnect_from) ||
            log_device_address(logger, address) ||
            log_str(logger, open_braked) ||
            log_duration(logger, duration) ||
            log_str(logger, close_braked) ||
            log_flush(logger) != 0);
  }
  return 0;
}

int log_network_connect(const MqttSnLogger *logger,
                        int level,
                        const char *protocol,
                        const char *network_name,
                        const device_address *address) {
  if (is_logger_broken(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return 0;
  }

  const char *connect = ": Connect ";
  const char *space = " ";
  const char *network_as = " network connect as  ";
  const char *dot = ".";

  return (log_current_time(logger) ||
      log_str(logger, connect) ||
      log_str(logger, protocol) ||
      log_str(logger, space) ||
      log_str(logger, network_name) ||
      log_str(logger, network_as) ||
      log_device_address(logger, address) ||
      log_str(logger, dot) ||
      log_flush(logger) != 0);
}

int log_network_connect_fail(const MqttSnLogger *logger,
                             const char *network_name,
                             const device_address *as,
                             const device_address *to) {
  if (is_logger_broken(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return 0;
  }
  const char *connect = " Connect to ";
  const char *network_as = " network as ";
  const char *failed_dot = " failed.";
  if (log_msg_start(logger)) {
    return -1;
  }
  if (log_str(logger, connect)) {
    return -1;
  }
  if (log_str(logger, network_name)) {
    return -1;
  }
  if (log_str(logger, network_as)) {
    return -1;
  }
  if (log_device_address(logger, as)) {
    return -1;
  }
  if (to != NULL) {
    const char *to_str = " to ";
    if (log_str(logger, to_str)) {
      return -1;
    }
    if (log_device_address(logger, to)) {
      return -1;
    }
  }
  if (log_str(logger, failed_dot)) {
    return -1;
  }
  return 0;
}

int log_network_disconnect(const MqttSnLogger *logger,
                           int level,
                           const char *protocol,
                           const char *network_name,
                           const device_address *address) {
  if (is_logger_broken(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return 0;
  }

  const char *connect = ": Disconnect ";
  const char *space = " ";
  const char *network_as = " network connect as  ";
  const char *dot = ".";

  return (log_current_time(logger) ||
      log_str(logger, connect) ||
      log_str(logger, protocol) ||
      log_str(logger, space) ||
      log_str(logger, network_name) ||
      log_str(logger, network_as) ||
      log_device_address(logger, address) ||
      log_str(logger, dot) ||
      log_flush(logger) != 0);
}

int log_protocol_mismatch(const MqttSnLogger *logger, int level, const char *expected, const char *actual) {
  if (is_logger_broken(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return 0;
  }

  const char *mismatch = ": plugin protocol mismatch - expected: ";
  const char *actual_str = " actual: ";
  const char *dot = ".";

  if (log_current_time(logger)) {
    return -1;
  }
  if (log_str(logger, mismatch)) {
    return -1;
  }
  if (log_str(logger, expected)) {
    return -1;
  }
  if (log_str(logger, actual_str)) {
    return -1;
  }
  if (log_str(logger, actual)) {
    return -1;
  }
  if (log_str(logger, dot)) {
    return -1;
  }
  if (log_flush(logger)) {
    return -1;
  }

  return 0;
}

int log_too_long_message(const MqttSnLogger *logger,
                         int level,
                         const device_address *address,
                         const uint8_t *data,
                         uint16_t data_len) {
  if (is_logger_broken(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEBUG)) {
    return 0;
  }

  const char *description = "dropping too long message: ";
  if (log_gateway_mqtt_sn_message(logger, level, address, data, data_len, description)) {
    return -1;
  }
  return 0;
}
int log_incomplete_message(const MqttSnLogger *logger,
                           int level,
                           const device_address *address,
                           const uint8_t *data,
                           uint16_t data_len) {
  if (is_logger_broken(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEBUG)) {
    return 0;
  }

  const char *description = "could not send message completely - try again later: ";
  if (log_gateway_mqtt_sn_message(logger, level, address, data, data_len, description)) {
    return -1;
  }
  return 0;
}

int log_rec_gateway_message(const MqttSnLogger *logger,
                            int level,
                            const device_address *from,
                            const uint8_t *data,
                            uint16_t data_len) {
  if (is_logger_broken(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEBUG)) {
    return 0;
  }

  const char *send_g_msg_from = ": receive gateway message from ";
  const char *to_str = " to ";
  const char *braked_open_len = " ( len";
  const char *comma_data_bracked_open = ", data( ";
  const char *double_bracked_close_dot = ")).";
  //device_address* std; // TODO get dst

  return (log_current_time(logger) ||
      log_str(logger, send_g_msg_from) ||
      log_device_address(logger, from) ||
      log_str(logger, to_str) ||
      //log_device_address(logger, dst) ||
      log_str(logger, braked_open_len) ||
      log_uint32(logger, data_len) ||
      log_str(logger, comma_data_bracked_open) ||
      log_uint8_array(logger, data, data_len) ||
      log_str(logger, double_bracked_close_dot) ||
      log_flush(logger) != 0);
}
int log_rec_client_message(const MqttSnLogger *logger,
                           int level,
                           const device_address *from,
                           const device_address *to,
                           const uint8_t *data,
                           uint16_t data_len) {
  if (is_logger_broken(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEBUG)) {
    return 0;
  }

  const char *rec_client_msg_from = "received client message from ";
  const char *to_str = " to ";
  const char *braked_open_len = " ( len";
  const char *comma_data_braked_open = ", data( ";
  const char *double_braked_close_dot = ")).";

  return (log_msg_start(logger) ||
      log_str(logger, rec_client_msg_from) ||
      log_device_address(logger, from) ||
      log_str(logger, to_str) ||
      log_device_address(logger, to) ||
      log_str(logger, braked_open_len) ||
      log_uint32(logger, data_len) ||
      log_str(logger, comma_data_braked_open) ||
      log_uint8_array(logger, data, data_len) ||
      log_str(logger, double_braked_close_dot) ||
      log_flush(logger) != 0);
}

int log_send_client_message(const MqttSnLogger *logger,
                            int level,
                            const device_address *from,
                            const device_address *dst,
                            const uint8_t *data,
                            uint16_t data_len) {
  if (is_logger_broken(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEBUG)) {
    return 0;
  }

  const char *send_c_msg_from = ": send client message from ";
  const char *to_str = " to ";
  const char *braked_open_len = " ( len";
  const char *comma_data_bracked_open = ", data( ";
  const char *double_bracked_close_dot = ")).";

  return (log_current_time(logger) ||
      log_str(logger, send_c_msg_from) ||
      log_device_address(logger, from) ||
      log_str(logger, to_str) ||
      log_device_address(logger, dst) ||
      log_str(logger, braked_open_len) ||
      log_uint32(logger, data_len) ||
      log_str(logger, comma_data_bracked_open) ||
      log_uint8_array(logger, data, data_len) ||
      log_str(logger, double_bracked_close_dot) ||
      log_flush(logger) != 0);
}

int log_send_gateway_message(const MqttSnLogger *logger,
                             int level,
                             const device_address *from,
                             const device_address *dst,
                             const uint8_t *data,
                             uint16_t data_len) {
  if (is_logger_broken(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEBUG)) {
    return 0;
  }

  const char *send_g_msg_from = ": send gateway message from ";
  const char *to_str = " to ";
  const char *braked_open_len = " ( len";
  const char *comma_data_bracked_open = ", data( ";
  const char *double_bracked_close_dot = ")).";

  return (log_current_time(logger) ||
      log_str(logger, send_g_msg_from) ||
      log_device_address(logger, from) ||
      log_str(logger, to_str) ||
      log_device_address(logger, dst) ||
      log_str(logger, braked_open_len) ||
      log_uint32(logger, data_len) ||
      log_str(logger, comma_data_bracked_open) ||
      log_uint8_array(logger, data, data_len) ||
      log_str(logger, double_bracked_close_dot) ||
      log_flush(logger) != 0);
}

