//
// Created by SomeDude on 11.04.2019.
//

#include "MqttSnForwarderLoggingMessages.h"
#include <platform/platform_compatibility.h>
#ifndef Arduino_h
#include <netinet/in.h>
#endif
// TODO PSTR
#define MQTT_SN_MESSAGE_TYPE_STRING_NEGATIVE_ENUMS_OFFSET 2
static const char *MQTT_SN_MESSAGE_TYPE_STRING[] = {
    FOREACH_MQTT_SN_MESSAGE_TYPE(GENERATE_MQTT_SN_MESSAGE_TYPE_STRING)
};

int log_mqtt_sn_flags(const MqttSnLogger *logger, uint8_t flags) {

  uint8_t dup = GET_MQTT_SN_DUP_FLAG(flags);
  log_dup_flag(logger, dup);
  log_comma(logger);

  int8_t qos = GET_MQTT_SN_QOS_FLAG(flags);
  log_qos_flag(logger, qos);
  log_comma(logger);

  uint8_t retain = GET_MQTT_SN_RETAIN_FLAG(flags);
  log_retain_flag(logger, retain);
  log_comma(logger);

  uint8_t will = GET_MQTT_SN_WILL_FLAG(flags);
  log_will_flag(logger, will);
  log_comma(logger);

  uint8_t clean_session = GET_MQTT_SN_CLEAN_SESSION(flags);
  log_clean_flag(logger, clean_session);
  log_comma(logger);

  uint8_t topic_id_type = GET_MQTT_SN_TOPIC_ID_TYPE(flags);
  log_topic_id_type_flag(logger, topic_id_type);
  log_comma(logger);

  return log_status(logger);
}
int log_gateway_id(const MqttSnLogger *logger, uint8_t gwId) {
  return log_str_key_uint8_value(logger, "gw", gwId);
}
int log_radius(const MqttSnLogger *logger, uint8_t radius) {
  return log_char_key_uint8_value(logger, 'r', radius);
}
int log_gw_add(const MqttSnLogger *logger, const device_address *gwAdd, uint16_t gwAdd_len) {
  log_str(logger, "gwAdd");
  const char *dot = ".";
  for (size_t i = 0; i < gwAdd_len; ++i) {
    if (log_uint8(logger, gwAdd->bytes[i])) {
      return -1;
    }
    if (i + 1 < sizeof(device_address)) {
      if (log_str(logger, dot)) {
        return -1;
      }
    }
  }
  return log_status(logger);
}
int log_message_id(const MqttSnLogger *logger, uint16_t msgId) {
  return log_char_key_uint16_value(logger, 'm', msgId);
}
int log_dup_flag(const MqttSnLogger *logger, uint8_t dup) {
  return log_char_key_uint8_value(logger, 'd', dup);
}
int log_clean_flag(const MqttSnLogger *logger, uint8_t clean_session) {
  return log_char_key_uint8_value(logger, 'c', clean_session);
}
int log_topic_id(const MqttSnLogger *logger, uint16_t topicId) {
  return log_char_key_uint16_value(logger, 't', topicId);
}
int log_topic_name(const MqttSnLogger *logger, const char *topicName) {
  return log_str(logger, topicName);
}
int log_topic_id_type_flag(const MqttSnLogger *logger, uint8_t topicIdType) {
  return log_str_key_uint8_value(logger, "tt", topicIdType);
}
int log_retain_flag(const MqttSnLogger *logger, uint8_t retain) {
  return log_char_key_uint8_value(logger, 'r', retain);
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
int log_will_flag(const MqttSnLogger *logger, uint8_t will) {
  return log_char_key_uint8_value(logger, 'w', will);
}
int log_pre_defined_topic_id(const MqttSnLogger *logger, uint16_t predDefinedTopicId) {
  return log_topic_id(logger, predDefinedTopicId);
}
int log_short_topic_name(const MqttSnLogger *logger, uint16_t shortTopicName) {
  return log_topic_id(logger, shortTopicName);
}
int log_qos_flag(const MqttSnLogger *logger, int8_t qos) {
  return log_char_key_int8_value(logger, 'q', qos);
}
int log_ctrl(const MqttSnLogger *logger, uint8_t ctrl) {
  return log_char_key_uint8_value(logger, 'c', ctrl);
}
int log_wireless_node_id(const MqttSnLogger *logger, const device_address *wireless_node_id) {
  return log_device_address(logger, wireless_node_id);
}
int log_msg_from(const MqttSnLogger *logger, MQTT_SN_MESSAGE_TYPE msg_type, const device_address *from) {
  if (MQTT_SN_MESSAGE_TYPE_RESERVED(msg_type)) {
    return log_status(logger);
  }
  const char *msg_type_str = MQTT_SN_MESSAGE_TYPE_STRING[msg_type + MQTT_SN_MESSAGE_TYPE_STRING_NEGATIVE_ENUMS_OFFSET];
  log_str(logger, msg_type_str);
  const char *publish_from = " from ";
  log_str(logger, publish_from);
  log_device_address(logger, from);
  log_char(logger, ' ');
  return log_status(logger);
}

int log_msg_to(const MqttSnLogger *logger, MQTT_SN_MESSAGE_TYPE msg_type, const device_address *from) {
  if (MQTT_SN_MESSAGE_TYPE_RESERVED(msg_type)) {
    return log_status(logger);
  }
  const char *msg_type_str = MQTT_SN_MESSAGE_TYPE_STRING[msg_type + MQTT_SN_MESSAGE_TYPE_STRING_NEGATIVE_ENUMS_OFFSET];
  log_str(logger, msg_type_str);
  const char *publish_to = " to ";
  log_str(logger, publish_to);
  log_device_address(logger, from);
  log_char(logger, ' ');
  return log_status(logger);
}

int log_open_braked(const MqttSnLogger *logger) {
  return log_str(logger, "( ");
}
int log_close_braked_dot(const MqttSnLogger *logger) {
  return log_str(logger, ").");
}

int log_comma(const MqttSnLogger *logger) {
  return log_str(logger, ", ");
}
int log_gateway_mqtt_sn_message_malformed(const MqttSnLogger *logger,
                                          const device_address *from,
                                          const uint8_t *data,
                                          uint16_t data_len,
                                          uint8_t bc_radius) {
  return log_mqtt_sn_message_malformed(logger, MQTT_SN_FORWARDER_NETWORK_GATEWAY, from, data, data_len, bc_radius);
}
int log_client_mqtt_sn_message_malformed(const MqttSnLogger *logger,
                                         const device_address *from,
                                         const uint8_t *data,
                                         uint16_t data_len,
                                         uint8_t bc_radius) {
  return log_mqtt_sn_message_malformed(logger, MQTT_SN_FORWARDER_NETWORK_CLIENT, from, data, data_len, bc_radius);
}
int log_mqtt_sn_message_malformed(const MqttSnLogger *logger,
                                  MQTT_SN_FORWARDER_NETWORK network,
                                  const device_address *from,
                                  const uint8_t *data,
                                  uint16_t data_len,
                                  uint8_t broadcast_radius) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return log_status(logger);
  }
  log_msg_start(logger);
  log_str(logger, "received malformed ");
  if (network == MQTT_SN_FORWARDER_NETWORK_CLIENT) {
    log_str(logger, "client");
  }
  if (network == MQTT_SN_FORWARDER_NETWORK_GATEWAY) {
    log_str(logger, "gateway");
  }
  if (broadcast_radius) {
    log_str(logger, " broadcast");
  }
  log_str(logger, " message");
  if (from) {
    log_str(logger, " from ");
    log_device_address(logger, from);
  }
  log_str(logger, " ( len");
  log_uint16(logger, data_len);

#ifdef WITH_DEBUG_LOGGING
  if (!shall_not_be_logged(logger, LOG_LEVEL_DEBUG)) {
    log_str(logger, ", data( ");
    log_uint8_array(logger, data, data_len);
    log_str(logger, ")");
  }
#endif

  log_close_braked_dot(logger);
  log_flush(logger);
  return log_status(logger);
}

int log_could_not_generate_encapsulation_message(const MqttSnLogger *logger,
                                                 const MQTT_SN_FORWARDER_NETWORK network,
                                                 const device_address *from,
                                                 const uint8_t *data,
                                                 uint16_t data_len,
                                                 uint8_t broadcast_radius) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return log_status(logger);
  }
  log_msg_start(logger);
  log_str(logger, "could not generate encapsulation header for ");
  if (network == MQTT_SN_FORWARDER_NETWORK_CLIENT) {
    log_str(logger, "client");
  }
  if (network == MQTT_SN_FORWARDER_NETWORK_GATEWAY) {
    log_str(logger, "gateway");
  }
  if (broadcast_radius) {
    log_str(logger, " broadcast");
  }
  log_str(logger, " message");
  if (from) {
    log_str(logger, " from ");
    log_device_address(logger, from);
  }
  log_str(logger, " ( len");
  log_uint16(logger, data_len);

#ifdef WITH_DEBUG_LOGGING
  if (!shall_not_be_logged(logger, LOG_LEVEL_DEBUG)) {
    log_str(logger, ", data( ");
    log_uint8_array(logger, data, data_len);
    log_str(logger, ")");
  }
#endif

  log_close_braked_dot(logger);
  log_flush(logger);
  return log_status(logger);
}
int log_client_mqtt_sn_message(const MqttSnLogger *logger,
                               const device_address *from,
                               const uint8_t *data,
                               uint16_t data_len,
                               const char *additional_msg) {
  return log_mqtt_sn_message(logger, from, MQTT_SN_FORWARDER_NETWORK_CLIENT, data, data_len, additional_msg);
}

int log_gateway_mqtt_sn_message(const MqttSnLogger *logger,
                                const device_address *from,
                                const uint8_t *data,
                                uint16_t data_len,
                                const char *additional_msg) {
  return log_mqtt_sn_message(logger, from, MQTT_SN_FORWARDER_NETWORK_GATEWAY, data, data_len, additional_msg);
}

int log_mqtt_sn_message(const MqttSnLogger *logger,
                        const device_address *from,
                        MQTT_SN_FORWARDER_NETWORK network,
                        const uint8_t *data,
                        uint16_t data_len,
                        const char *additional_msg) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return log_status(logger);
  }
  ParsedMqttSnHeader header = {0};
  if (parse_message_tolerant(&header, ANY_MESSAGE_TYPE, data, data_len)) {
    return log_status(logger);
  }
  log_msg_start(logger);

  if (additional_msg) {
    log_str(logger, additional_msg);
    log_char(logger, ' ');
    if (log_status(logger)) {
      return log_status(logger);
    }
  }
  if (network == MQTT_SN_FORWARDER_NETWORK_CLIENT) {
    log_str(logger, "client ");
    log_msg_from(logger, header.msg_type, from);
  }
  if (network == MQTT_SN_FORWARDER_NETWORK_GATEWAY) {
    log_str(logger, "gateway ");
    log_msg_to(logger, header.msg_type, from);
  }

  log_mqtt_sn_message_payload(logger, &header);
  return log_flush(logger);
}

int log_mqtt_sn_message_payload(const MqttSnLogger *logger, const ParsedMqttSnHeader *header) {
  log_open_braked(logger);

  log_default_mqtt_sn_message_payload(logger, header);
  log_verbose_mqtt_sn_message_payload(logger, header);

  log_close_braked_dot(logger);
  return log_status(logger);
}

int log_default_mqtt_sn_message_payload(const MqttSnLogger *logger, const ParsedMqttSnHeader *header) {
  switch (header->msg_type) {
    case PUBLISH: return log_publish_message(logger, header);
    case CONNECT: return log_connect_message(logger, header);
    case CONNACK: return log_connack_message(logger, header);
    case DISCONNECT: return log_disconnect_message(logger, header);
    default:
      if (shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
        return log_status(logger);
      }
      break;
  }
  return log_status(logger);
}
int log_verbose_mqtt_sn_message_payload(const MqttSnLogger *logger, const ParsedMqttSnHeader *header) {
  switch (header->msg_type) {
    case ADVERTISE: return log_advertise_message(logger, header);
    case SEARCHGW: return log_searchgw_message(logger, header);
    case GWINFO: return log_gwinfo_message(logger, header);
    case WILLTOPICREQ: return 0; // FEATURE log this message
    case WILLTOPIC: return 0; // FEATURE log this message
    case WILLMSGREQ: return 0; // FEATURE log this message
    case WILLMSG: return 0; // FEATURE log this message
    case REGISTER: return log_register_message(logger, header);
    case REGACK: return log_regack_message(logger, header);
    case PUBACK: return log_puback_message(logger, header);
    case PUBREC: return 0; // FEATURE log this message
    case PUBREL: return 0; // FEATURE log this message
    case PUBCOMP: return 0; // FEATURE log this message
    case SUBSCRIBE: return log_subscribe_message(logger, header);
    case SUBACK: return log_suback_message(logger, header);
    case UNSUBSCRIBE: return log_unsubscribe_message(logger, header);
    case UNSUBACK:return log_unsuback_message(logger, header);
    case PINGREQ: return log_pingreq_message(logger, header);
    case PINGRESP: return log_pingresp_message(logger);
    case WILLTOPICUPD: return 0; // FEATURE log this message
    case WILLTOPICRESP: return 0; // FEATURE log this message
    case WILLMSGUPD: return 0; // FEATURE log this message
    case WILLMSGRESP: return 0; // FEATURE log this message
    case ENCAPSULATED_MESSAGE: return log_encapsulated_message(logger, header);
    default:
      if (shall_not_be_logged(logger, LOG_LEVEL_DEBUG)) {
        return log_status(logger);
      }
      break;
  }
  return log_status(logger);
}
int log_advertise_message(const MqttSnLogger *logger, const ParsedMqttSnHeader *header) {
  MqttSnAdvertise *a = (MqttSnAdvertise *) header->payload;
  uint8_t gw_id = a->gwId;
  uint16_t duration = ntohs(a->duration);
  log_open_braked(logger);
  log_gateway_id(logger, gw_id);
  log_comma(logger);
  log_duration(logger, duration);
  log_close_braked_dot(logger);
  return log_status(logger);
}
int log_searchgw_message(const MqttSnLogger *logger, const ParsedMqttSnHeader *header) {
  MqttSnSearchGw *s = (MqttSnSearchGw *) header->payload;
  uint8_t radius = s->radius;
  log_open_braked(logger);
  log_radius(logger, radius);
  log_close_braked_dot(logger);
  return log_status(logger);
}
int log_gwinfo_message(const MqttSnLogger *logger, const ParsedMqttSnHeader *header) {
  MqttSnGwInfo *g = (MqttSnGwInfo *) header->payload;
  uint8_t gw_id = g->gwId;
  const device_address *gw_add = &g->gwAdd;
  uint16_t gwAdd_len = header->length - MQTT_SN_GWINFO_MESSAGE_HEADER_LENGTH;
  log_open_braked(logger);
  log_gateway_id(logger, gw_id);
  log_comma(logger);
  log_gw_add(logger, gw_add, gwAdd_len);
  log_close_braked_dot(logger);
  return log_status(logger);
}
int log_connect_message(const MqttSnLogger *logger, const ParsedMqttSnHeader *header) {
  MqttSnMessageConnect *c = (MqttSnMessageConnect *) header->payload;
  uint8_t flags = c->flags;
  uint8_t protocol_id = ntohs(c->protocolId);
  uint16_t duration = ntohs(c->duration);
  const char *client_id = c->clientId;

  const char *as = " as ";

  log_str(logger, as);
  log_str(logger, client_id);
  log_char(logger, ' ');

  log_open_braked(logger);
  log_mqtt_sn_flags(logger, flags);
  log_protocol_id(logger, protocol_id);
  log_comma(logger);
  log_duration(logger, duration);
  log_close_braked_dot(logger);

  return log_flush(logger);
}

int log_connack_message(const MqttSnLogger *logger, const ParsedMqttSnHeader *header) {
  MqttSnMessageConnack *c = (MqttSnMessageConnack *) header->payload;
  uint8_t return_code = c->returnCode;

  log_open_braked(logger);
  log_return_code(logger, return_code);
  log_close_braked_dot(logger);
  return log_flush(logger);
}

int log_register_message(const MqttSnLogger *logger, const ParsedMqttSnHeader *header) {
  MqttSnMessageRegister *r = (MqttSnMessageRegister *) header->payload;
  uint16_t topic_id = ntohs(r->topicId);
  uint16_t msg_id = ntohs(r->msgId);
  const char *topic_name = r->topicName;

  log_open_braked(logger);

  log_message_id(logger, msg_id);
  log_comma(logger);
  log_topic_id(logger, topic_id);
  log_comma(logger);
  log_topic_name(logger, topic_name);

  log_close_braked_dot(logger);
  return log_flush(logger);
}

int log_regack_message(const MqttSnLogger *logger, const ParsedMqttSnHeader *header) {
  MqttSnMessageRegack *r = (MqttSnMessageRegack *) header->payload;
  uint16_t topic_id = ntohs(r->topicId);
  uint16_t msg_id = ntohs(r->msgId);
  uint8_t return_code = r->returnCode;

  log_open_braked(logger);

  log_message_id(logger, msg_id);
  log_comma(logger);
  log_topic_id(logger, topic_id);
  log_comma(logger);
  log_return_code(logger, return_code);

  log_close_braked_dot(logger);
  return log_flush(logger);
}

int log_publish_message(const MqttSnLogger *logger, const ParsedMqttSnHeader *header) {

  const char *dots = "... (";
  const char *bytes_str = " bytes)";

  MqttSnMessagePublish *p = (MqttSnMessagePublish *) header->payload;
  uint16_t msg_id = ntohs(p->msgId);
  uint16_t topic_id = ntohs(((MqttSnMessagePublish *) header->payload)->topicId);
  uint8_t flags = ((MqttSnMessagePublish *) header->payload)->flags;
  uint16_t msg_bytes = header->length;

  log_message_id(logger, msg_id);
  log_comma(logger);
  log_topic_id(logger, topic_id);
  log_comma(logger);
  log_mqtt_sn_flags(logger, flags);

  log_str(logger, dots);
  log_uint16(logger, msg_bytes);
  log_str(logger, bytes_str);

  return log_status(logger);
}

int log_puback_message(const MqttSnLogger *logger, const ParsedMqttSnHeader *header) {
  MqttSnMessagePuback *p = (MqttSnMessagePuback *) header->payload;
  uint16_t topic_id = ntohs(p->topicId);
  uint16_t msg_id = ntohs(p->msgId);
  uint8_t return_code = p->returnCode;

  log_open_braked(logger);

  log_message_id(logger, msg_id);
  log_comma(logger);
  log_topic_id(logger, topic_id);
  log_comma(logger);
  log_return_code(logger, return_code);

  log_close_braked_dot(logger);
  return log_flush(logger);
}

int log_subscribe_message(const MqttSnLogger *logger, const ParsedMqttSnHeader *header) {
  MqttSnMessageSubcribe *s = (MqttSnMessageSubcribe *) header->payload;
  uint8_t flags = s->flags;
  uint16_t msg_id = ntohs(s->msgId);

  log_open_braked(logger);

  log_message_id(logger, msg_id);
  log_comma(logger);
  log_mqtt_sn_flags(logger, flags);
  log_comma(logger);
  if (GET_MQTT_SN_TOPIC_ID_TYPE(flags) == 0x00) { // topic name
    const char *topicName = s->topicNameOrTopicId.topicName;
    log_str(logger, topicName);
  } else if (GET_MQTT_SN_TOPIC_ID_TYPE(flags) == 0x01) { // pre-defined topic id
    uint16_t pre_defined_topic_id = ntohs(s->topicNameOrTopicId.topicId);
    log_pre_defined_topic_id(logger, pre_defined_topic_id);
  } else if (GET_MQTT_SN_TOPIC_ID_TYPE(flags) == 0x02) {// short topic name
    uint16_t short_topic_name = ntohs(s->topicNameOrTopicId.topicId);
    log_short_topic_name(logger, short_topic_name);
  }

  log_close_braked_dot(logger);
  return log_flush(logger);
}

int log_suback_message(const MqttSnLogger *logger, const ParsedMqttSnHeader *header) {
  MqttSnMessageSuback *s = (MqttSnMessageSuback *) header->payload;
  uint8_t flags = s->flags;
  uint16_t topic_id = ntohs(s->flags);
  uint16_t msg_id = ntohs(s->msgId);
  uint8_t return_code = s->returnCode;

  log_open_braked(logger);

  log_message_id(logger, msg_id);
  log_comma(logger);
  log_mqtt_sn_flags(logger, flags);
  log_comma(logger);
  log_topic_id(logger, topic_id);
  log_comma(logger);
  log_return_code(logger, return_code);

  log_close_braked_dot(logger);
  return log_flush(logger);
}
int log_unsubscribe_message(const MqttSnLogger *logger, const ParsedMqttSnHeader *header) {
  MqttSnMessageUnsubscribe *u = (MqttSnMessageUnsubscribe *) header->payload;
  uint8_t flags = u->flags;
  uint16_t msg_id = ntohs(u->msgId);

  log_open_braked(logger);

  log_message_id(logger, msg_id);
  log_comma(logger);
  log_mqtt_sn_flags(logger, flags);
  log_comma(logger);
  if (GET_MQTT_SN_TOPIC_ID_TYPE(flags) == 0x00) { // topic name
    const char *topicName = u->topicNameOrTopicId.topicName;
    log_str(logger, topicName);
  } else if (GET_MQTT_SN_TOPIC_ID_TYPE(flags) == 0x01) { // pre-defined topic id
    uint16_t pre_defined_topic_id = ntohs(u->topicNameOrTopicId.topicId);
    log_pre_defined_topic_id(logger, pre_defined_topic_id);
  } else if (GET_MQTT_SN_TOPIC_ID_TYPE(flags) == 0x02) { // short topic name
    uint16_t short_topic_name = ntohs(u->topicNameOrTopicId.topicId);
    log_short_topic_name(logger, short_topic_name);
  }

  log_close_braked_dot(logger);
  return log_flush(logger);
}
int log_unsuback_message(const MqttSnLogger *logger, const ParsedMqttSnHeader *header) {
  MqttSnMessageUnsuback *s = (MqttSnMessageUnsuback *) header->payload;
  uint16_t msg_id = ntohs(s->msgId);

  log_open_braked(logger);
  log_message_id(logger, msg_id);
  log_close_braked_dot(logger);
  return log_flush(logger);
}
int log_pingreq_message(const MqttSnLogger *logger, const ParsedMqttSnHeader *header) {
  if (header->length > 2) {
    MqttSnMessagePingReq *p = (MqttSnMessagePingReq *) header->payload;
    const char *clientId = p->clientId;

    const char *is_awake = "is awake ";

    log_str(logger, is_awake);
    log_str(logger, clientId);
    log_char(logger, '.');
  }
  return log_status(logger);
}
int log_pingresp_message(const MqttSnLogger *logger) {
  return log_status(logger);
}
int log_disconnect_message(const MqttSnLogger *logger, const ParsedMqttSnHeader *header) {
  if (header->length == 4) {
    MqttSnMessageDisconnect *p = (MqttSnMessageDisconnect *) header->payload;
    uint16_t duration = ntohs(p->duration);
    log_open_braked(logger);
    log_duration(logger, duration);
    log_close_braked_dot(logger);
  }
  return log_status(logger);
}
int log_encapsulated_message(const MqttSnLogger *logger, const ParsedMqttSnHeader *header) {
  MqttSnEncapsulatedMessage *e = (MqttSnEncapsulatedMessage *) header->payload;
  uint8_t ctrl = e->crtl;
  const device_address *wireless_node_id = &e->wireless_node_id;

  log_open_braked(logger);
  log_ctrl(logger, ctrl);
  log_comma(logger);
  log_wireless_node_id(logger, wireless_node_id);
  log_open_braked(logger);

  log_char(logger, ')');
  log_close_braked_dot(logger);
  return log_flush(logger);
}


