//
// Created by SomeDude on 11.04.2019.
//

#include <netinet/in.h>
#include "MqttSnForwarderLoggingMessages.h"

int log_mqtt_sn_flags(const MqttSnLogger *logger, uint8_t flags) {

  const char *comma_space = ", ";

  uint8_t dup = MQTT_SN_DUP_FLAG(flags);
  log_char_key_uint8_value(logger, 'd', dup);
  log_str(logger, comma_space);

  int8_t qos = MQTT_SN_QOS_FLAG(flags);
  log_char_key_uint8_value(logger, 'q', qos);
  log_str(logger, comma_space);

  uint8_t retain = MQTT_SN_RETAIN_FLAG(flags);
  log_char_key_uint8_value(logger, 'r', retain);
  log_str(logger, comma_space);

  uint8_t will = MQTT_SN_WILL_FLAG(flags);
  log_char_key_uint8_value(logger, 'w', will);
  log_str(logger, comma_space);

  uint8_t clean_session = MQTT_SN_CLEAN_SESSION(flags);
  log_char_key_uint8_value(logger, 'c', clean_session);
  log_str(logger, comma_space);

  uint8_t topic_id_type = MQTT_SN_TOPIC_ID_TYPE(flags);
  log_str_key_uint8_value(logger, "tt", topic_id_type);
  log_str(logger, comma_space);

  return log_status(logger);
}
int log_message_id(const MqttSnLogger *logger, uint16_t msgId) {
  return log_char_key_uint16_value(logger, 'm', msgId);
}
int log_topic_id(const MqttSnLogger *logger, uint16_t topicId) {
  return log_char_key_uint16_value(logger, 't', topicId);
}
int log_return_code(const MqttSnLogger *logger, uint8_t returnCode) {
  return log_str_key_uint8_value(logger, "rc", returnCode);
}
int log_duration(const MqttSnLogger *logger, uint16_t duration) {
  return log_str_key_uint16_value(logger, "dur", duration);
}
int log_protocol_id(const MqttSnLogger *logger, uint8_t protocolId) {
  return log_char_key_uint8_value(logger, 'p', protocolId);
}

int log_client_mqtt_sn_message(const MqttSnLogger *logger,
                               int level,
                               const device_address *from,
                               const uint8_t *data,
                               uint16_t data_len,
                               const char *additional_msg) {
  const char *network = "client";
  return log_mqtt_sn_message(logger, level, from, network, data, data_len, additional_msg);
}

int log_gateway_mqtt_sn_message(const MqttSnLogger *logger,
                                int level,
                                const device_address *from,
                                const uint8_t *data,
                                uint16_t data_len,
                                const char *additional_msg) {
  const char *network = "gateway";
  return log_mqtt_sn_message(logger, level, from, network, data, data_len, additional_msg);
}

int log_mqtt_sn_message(const MqttSnLogger *logger,
                        int level,
                        const device_address *from,
                        const char *network,
                        const uint8_t *data,
                        uint16_t data_len,
                        const char *additional_msg) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return 0;
  }
  ParsedMqttSnHeader header = {0};
  if (parse_message(&header, ANY_MESSAGE_TYPE, data, data_len)) {
    return 0;
  }
  if (additional_msg) {
    if (log_str(logger, additional_msg)) {
      return -1;
    }
  }
  switch (header.msg_type) {
    case PUBLISH: return log_publish_message(logger, &header, from, network, data, data_len);
    case CONNECT: return log_connect_message(logger, &header, from, network, data, data_len);
    case DISCONNECT: return log_disconnect_message(logger, &header, from, network, data, data_len);
    case CONNACK: return log_connack_message(logger, &header, from, network, data, data_len);
    case PUBACK: return log_puback_message(logger, &header, from, network, data, data_len);
    default:
      if (shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
        return 0;
      }
      break;
  }

  switch (header.msg_type) {
    case ADVERTISE: return log_advertise_message(logger, &header, from, network, data, data_len);
    case SEARCHGW: return log_searchgw_message(logger, &header, from, network, data, data_len);
    case GWINFO: return log_gwinfo_message(logger, &header, from, network, data, data_len);
    case WILLTOPICREQ: return 0; // FEATURE log this message
    case WILLTOPIC: return 0; // FEATURE log this message
    case WILLMSGREQ: return 0; // FEATURE log this message
    case WILLMSG: return 0; // FEATURE log this message
    case REGISTER: return log_register_message(logger, &header, from, network, data, data_len);
    case REGACK: return log_regack_message(logger, &header, from, network, data, data_len);
    case PUBREC: return 0; // FEATURE log this message
    case PUBREL: return 0; // FEATURE log this message
    case PUBCOMP: return 0; // FEATURE log this message
    case SUBSCRIBE: return log_subscribe_message(logger, &header, from, network, data, data_len);
    case SUBACK: return log_suback_message(logger, &header, from, network, data, data_len);
    case UNSUBSCRIBE: return log_unsubscribe_message(logger, &header, from, network, data, data_len);
    case UNSUBACK: return log_unsuback_message(logger, &header, from, network, data, data_len);
    case PINGREQ: return log_pingreq_message(logger, &header, from, network, data, data_len);
    case PINGRESP: return log_pingresp_message(logger, &header, from, network, data, data_len);
    case WILLTOPICUPD: return 0; // FEATURE log this message
    case WILLTOPICRESP: return 0; // FEATURE log this message
    case WILLMSGUPD: return 0; // FEATURE log this message
    case WILLMSGRESP: return 0; // FEATURE log this message
    case ENCAPSULATED_MESSAGE: return 0; // FEATURE log this message
    default:
      if (shall_not_be_logged(logger, LOG_LEVEL_DEBUG)) {
        return 0;
      }
      break;
  }
  return 0;
}

int log_advertise_message(const MqttSnLogger *logger,
                          ParsedMqttSnHeader *header,
                          const device_address *from,
                          const char *network,
                          const uint8_t *data,
                          uint16_t len) {
  // TODO implement
  return 0;
}
int log_searchgw_message(const MqttSnLogger *logger,
                         ParsedMqttSnHeader *header,
                         const device_address *from,
                         const char *network,
                         const uint8_t *data,
                         uint16_t len) {
  // TODO implement
  return 0;
}
int log_gwinfo_message(const MqttSnLogger *logger,
                       ParsedMqttSnHeader *header,
                       const device_address *from,
                       const char *network,
                       const uint8_t *data,
                       uint16_t len) {
  // TODO implement
  return 0;
}
int log_connect_message(const MqttSnLogger *logger,
                        const ParsedMqttSnHeader *header,
                        const device_address *address,
                        const char *network,
                        const uint8_t *data,
                        uint16_t data_len) {

  MqttSnMessageConnect *c = (MqttSnMessageConnect *) header->payload;
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

int log_connack_message(const MqttSnLogger *logger,
                        ParsedMqttSnHeader *header,
                        const device_address *from,
                        const char *network,
                        const uint8_t *data,
                        uint16_t len) {
  // TODO implement
  log_str(logger, "// TODO implement");
  return log_flush(logger);
}

int log_register_message(const MqttSnLogger *logger,
                         ParsedMqttSnHeader *header,
                         const device_address *from,
                         const char *network,
                         const uint8_t *data,
                         uint16_t len) {
  // TODO implement
  log_str(logger, "// TODO implement");
  return log_flush(logger);
}

int log_regack_message(const MqttSnLogger *logger,
                       ParsedMqttSnHeader *header,
                       const device_address *from,
                       const char *network,
                       const uint8_t *data,
                       uint16_t len) {
  // TODO implement
  log_str(logger, "// TODO implement");
  return log_flush(logger);
}

int log_publish_message(const MqttSnLogger *logger,
                        const ParsedMqttSnHeader *header,
                        const device_address *from,
                        const char *network,
                        const uint8_t *data,
                        uint16_t data_len) {

  const char *publish_from = ": client PUBLISH from ";
  const char *open_braked = " ( ";
  const char *comma = ", ";
  const char *dots = "... (";
  const char *bytes_close_braked = " bytes)).";

  MqttSnMessagePublish *p = (MqttSnMessagePublish *) header->payload;
  uint16_t msg_id = ntohs(p->msgId);
  uint16_t topic_id = ntohs(((MqttSnMessagePublish *) header->payload)->topicId);
  uint8_t flags = ((MqttSnMessagePublish *) header->payload)->flags;
  uint16_t msg_bytes = header->length;

  log_msg_start(logger);
  if (network != NULL) {
    log_str(logger, network);
  }

  return (log_str(logger, publish_from) ||
      log_device_address(logger, from) ||
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

int log_puback_message(const MqttSnLogger *logger,
                       ParsedMqttSnHeader *header,
                       const device_address *from,
                       const char *network,
                       const uint8_t *data,
                       uint16_t len) {
  // TODO implement
  log_str(logger, "// TODO implement");
  return log_flush(logger);
}

int log_subscribe_message(const MqttSnLogger *logger,
                          ParsedMqttSnHeader *header,
                          const device_address *from,
                          const char *network,
                          const uint8_t *data,
                          uint16_t len) {
  // TODO implement
  log_str(logger, "// TODO implement");
  return log_flush(logger);
}

int log_suback_message(const MqttSnLogger *logger,
                       ParsedMqttSnHeader *header,
                       const device_address *from,
                       const char *network,
                       const uint8_t *data,
                       uint16_t len) {
  // TODO implement
  log_str(logger, "// TODO implement");
  return log_flush(logger);
}
int log_unsubscribe_message(const MqttSnLogger *logger,
                            ParsedMqttSnHeader *header,
                            const device_address *from,
                            const char *network,
                            const uint8_t *data,
                            uint16_t len) {
  // TODO implement
  log_str(logger, "// TODO implement");
  return log_flush(logger);
}
int log_unsuback_message(const MqttSnLogger *logger,
                         ParsedMqttSnHeader *header,
                         const device_address *from,
                         const char *network,
                         const uint8_t *data,
                         uint16_t len) {
  // TODO implement
  log_str(logger, "// TODO implement");
  return log_flush(logger);
}
int log_pingreq_message(const MqttSnLogger *logger,
                        ParsedMqttSnHeader *header,
                        const device_address *from,
                        const char *network,
                        const uint8_t *data,
                        uint16_t len) {
  // TODO implement
  log_str(logger, "// TODO implement");
  return log_flush(logger);
}
int log_pingresp_message(const MqttSnLogger *logger,
                         ParsedMqttSnHeader *header,
                         const device_address *from,
                         const char *network,
                         const uint8_t *data,
                         uint16_t len) {
  // TODO implement
  log_str(logger, "// TODO implement");
  return log_flush(logger);
}
int log_disconnect_message(const MqttSnLogger *logger,
                           const ParsedMqttSnHeader *header,
                           const device_address *address,
                           const char *network,
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
