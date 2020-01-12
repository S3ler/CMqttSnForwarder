//
// Created by SomeDude on 17.08.2019.
//

#include <platform/platform_compatibility.h>
#include "MqttSnPublishMessageLogging.h"
#include "MqttSnMessageParserLogging.h"
int32_t log_publish_message(const MqttSnLogger *logger, const MqttSnPublish *publish) {
    // TODO implement me
    /*
    log_dup_flag(logger, publish->dup);
    log_str(logger, PSTR(", "));
    log_qos_flag(logger, publish->qos);
    log_str(logger, PSTR(", "));
    log_retain_flag(logger, publish->retain);
    log_str(logger, PSTR(", "));
    log_topic_id_type_flag(logger, publish->topic_id_type);
    log_str(logger, PSTR(", "));
    log_topic_id(logger, publish->topic_id);
    log_str(logger, PSTR(", "));
    log_msg_id(logger, publish->msg_id);
    log_str(logger, PSTR(", "));
    */
    log_mqtt_sn_payload(logger, publish->payload, publish->payload_length);

    log_flush(logger);
    return log_status(logger);
    /*
     int log_publish_message(const MqttSnLogger *logger, const ParsedMqttSnHeader *header) {
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

  log_str(logger, PSTR("... ("));
  log_uint16(logger, msg_bytes);
  log_str(logger, PSTR(" bytes)"));

  return log_status(logger);
}

     */
}
int32_t log_publish_gen_error(const MqttSnLogger *logger, const char *file_name, const char *function_name, int line_number) {
    return log_msg_gen_error(logger, PUBLISH, file_name, function_name, line_number);
}
