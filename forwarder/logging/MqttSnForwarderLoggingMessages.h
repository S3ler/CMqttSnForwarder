//
// Created by SomeDude on 11.04.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNFORWARDERLOGGINGMESSAGES_H
#define CMQTTSNFORWARDER_MQTTSNFORWARDERLOGGINGMESSAGES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <forwarder/MqttSnForwarderLogging.h>
#include <forwarder/MqttSnMessageParser.h>

int log_mqtt_sn_flags(const MqttSnLogger *logger, uint8_t flags);

int log_gateway_id(const MqttSnLogger *logger, uint8_t gwId);

int log_radius(const MqttSnLogger *logger, uint8_t radius);

int log_message_id(const MqttSnLogger *logger, uint16_t msgId);

int log_dup_flag(const MqttSnLogger *logger, uint8_t dup);

int log_clean_flag(const MqttSnLogger *logger, uint8_t clean_session);

int log_duration(const MqttSnLogger *logger, uint16_t duration);

int log_protocol_id(const MqttSnLogger *logger, uint8_t protocolId);

int log_will_flag(const MqttSnLogger *logger, uint8_t will);

int log_return_code(const MqttSnLogger *logger, uint8_t returnCode);

int log_topic_id(const MqttSnLogger *logger, uint16_t topicId);

int log_topic_name(const MqttSnLogger *logger, const char *topicName);

int log_topic_id_type_flag(const MqttSnLogger *logger, uint8_t topicIdType);

int log_retain_flag(const MqttSnLogger *logger, uint8_t retain);

int log_qos_flag(const MqttSnLogger *logger, int8_t qos);

int log_ctrl(const MqttSnLogger *logger, uint8_t ctrl);

int log_msg_from(const MqttSnLogger *logger, MQTT_SN_MESSAGE_TYPE msg_type, const device_address *from);
int log_msg_to(const MqttSnLogger *logger, MQTT_SN_MESSAGE_TYPE msg_type, const device_address *from);

int log_open_braked(const MqttSnLogger *logger);
int log_close_braked_dot(const MqttSnLogger *logger);
int log_comma(const MqttSnLogger *logger);

int log_gateway_mqtt_sn_message_malformed(const MqttSnLogger *logger,
                                          const device_address *from,
                                          const uint8_t *data,
                                          uint16_t data_len);
int log_client_mqtt_sn_message_malformed(const MqttSnLogger *logger,
                                         const device_address *from,
                                         const uint8_t *data,
                                         uint16_t data_len);
int log_mqtt_sn_message_malformed(const MqttSnLogger *logger,
                                  MQTT_SN_FORWARDER_NETWORK network,
                                  const device_address *from,
                                  const uint8_t *data,
                                  uint16_t data_len);

int log_client_mqtt_sn_message(const MqttSnLogger *logger,
                               const device_address *from,
                               const uint8_t *data,
                               uint16_t data_len,
                               const char *additional_msg);

int log_gateway_mqtt_sn_message(const MqttSnLogger *logger,
                                const device_address *from,
                                const uint8_t *data,
                                uint16_t data_len,
                                const char *additional_msg);
int log_mqtt_sn_message(const MqttSnLogger *logger,
                        const device_address *from,
                        MQTT_SN_FORWARDER_NETWORK network,
                        const uint8_t *data,
                        uint16_t data_len,
                        const char *additional_msg);

int log_mqtt_sn_message_payload(const MqttSnLogger *logger, const ParsedMqttSnHeader *header);
int log_default_mqtt_sn_message_payload(const MqttSnLogger *logger, const ParsedMqttSnHeader *header);
int log_verbose_mqtt_sn_message_payload(const MqttSnLogger *logger, const ParsedMqttSnHeader *header);

int log_advertise_message(const MqttSnLogger *logger, const ParsedMqttSnHeader *header);

int log_searchgw_message(const MqttSnLogger *logger, const ParsedMqttSnHeader *header);

int log_gwinfo_message(const MqttSnLogger *logger, const ParsedMqttSnHeader *header);

int log_connect_message(const MqttSnLogger *logger, const ParsedMqttSnHeader *header);

int log_connack_message(const MqttSnLogger *logger, const ParsedMqttSnHeader *header);

int log_register_message(const MqttSnLogger *logger, const ParsedMqttSnHeader *header);

int log_regack_message(const MqttSnLogger *logger, const ParsedMqttSnHeader *header);

int log_publish_message(const MqttSnLogger *logger, const ParsedMqttSnHeader *header);

int log_puback_message(const MqttSnLogger *logger, const ParsedMqttSnHeader *header);

int log_subscribe_message(const MqttSnLogger *logger, const ParsedMqttSnHeader *header);

int log_suback_message(const MqttSnLogger *logger, const ParsedMqttSnHeader *header);

int log_unsubscribe_message(const MqttSnLogger *logger, const ParsedMqttSnHeader *header);

int log_unsuback_message(const MqttSnLogger *logger, const ParsedMqttSnHeader *header);

int log_pingreq_message(const MqttSnLogger *logger, const ParsedMqttSnHeader *header);

int log_pingresp_message(const MqttSnLogger *logger);

int log_disconnect_message(const MqttSnLogger *logger, const ParsedMqttSnHeader *header);

int log_encapsulated_message(const MqttSnLogger *logger, const ParsedMqttSnHeader *header);

#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_MQTTSNFORWARDERLOGGINGMESSAGES_H
