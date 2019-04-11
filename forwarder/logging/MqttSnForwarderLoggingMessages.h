//
// Created by SomeDude on 11.04.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNFORWARDERLOGGINGMESSAGES_H
#define CMQTTSNFORWARDER_MQTTSNFORWARDERLOGGINGMESSAGES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "MqttSnForwarderLogging.h"
#include "MqttSnForwarderLoggingBasic.h"

int log_mqtt_sn_flags(const MqttSnLogger *logger, uint8_t flags);

int log_gateway_id(const MqttSnLogger *logger, uint8_t gwId);

int log_gateway_add(const MqttSnLogger *logger, uint8_t gwAdd);

int log_message_id(const MqttSnLogger *logger, uint16_t msgId);

int log_clean_flag(const MqttSnLogger *logger, uint8_t clean);

int log_duration(const MqttSnLogger *logger, uint16_t duration);

int log_protocol_id(const MqttSnLogger *logger, uint8_t protocolId);

int log_will_flag(const MqttSnLogger *logger, uint8_t will);

int log_return_code(const MqttSnLogger *logger, uint8_t returnCode);

int log_topic_id(const MqttSnLogger *logger, uint16_t topicId);

int log_topic_name(const MqttSnLogger *logger, const char *topicName);

int log_topic_id_type(const MqttSnLogger *logger, uint8_t topicIdType);

int log_retain_flag(const MqttSnLogger *logger, uint8_t retainFlag);

int log_data_byte_counter(const MqttSnLogger *logger, uint32_t dataByteCount);

int log_qos(const MqttSnLogger *logger, int8_t qos);


int log_client_mqtt_sn_message(const MqttSnLogger *logger,
                               int level,
                               const device_address *from,
                               const uint8_t *data,
                               uint16_t data_len,
                               const char *additional_msg);

int log_gateway_mqtt_sn_message(const MqttSnLogger *logger,
                                int level,
                                const device_address *from,
                                const uint8_t *data,
                                uint16_t data_len,
                                const char *additional_msg);


int log_mqtt_sn_message(const MqttSnLogger *logger,
                        int level,
                        const device_address *from,
                        const char *network,
                        const uint8_t *data,
                        uint16_t data_len,
                        const char *additional_msg);

int log_advertise_message(const MqttSnLogger *logger,
                          ParsedMqttSnHeader *header,
                          const device_address *from,
                          const char *network,
                          const uint8_t *data,
                          uint16_t len);

int log_searchgw_message(const MqttSnLogger *logger,
                         ParsedMqttSnHeader *header,
                         const device_address *from,
                         const char *network,
                         const uint8_t *data,
                         uint16_t len);

int log_gwinfo_message(const MqttSnLogger *logger,
                       ParsedMqttSnHeader *header,
                       const device_address *from,
                       const char *network,
                       const uint8_t *data,
                       uint16_t len);

int log_connect_message(const MqttSnLogger *logger,
                               const ParsedMqttSnHeader *header,
                               const device_address *address,
                               const char *network,
                               const uint8_t *data,
                               uint16_t data_len);

int log_connack_message(const MqttSnLogger *logger,
                        ParsedMqttSnHeader *header,
                        const device_address *from,
                        const char *network,
                        const uint8_t *data,
                        uint16_t len);

int log_register_message(const MqttSnLogger *logger,
                         ParsedMqttSnHeader *header,
                         const device_address *from,
                         const char *network,
                         const uint8_t *data,
                         uint16_t len);

int log_regack_message(const MqttSnLogger *logger,
                       ParsedMqttSnHeader *header,
                       const device_address *from,
                       const char *network,
                       const uint8_t *data,
                       uint16_t len);

int log_publish_message(const MqttSnLogger *logger,
                        const ParsedMqttSnHeader *header,
                        const device_address *from,
                        const char *network,
                        const uint8_t *data,
                        uint16_t data_len);

int log_puback_message(const MqttSnLogger *logger,
                       ParsedMqttSnHeader *header,
                       const device_address *from,
                       const char *network,
                       const uint8_t *data,
                       uint16_t len);

int log_subscribe_message(const MqttSnLogger *logger,
                          ParsedMqttSnHeader *header,
                          const device_address *from,
                          const char *network,
                          const uint8_t *data,
                          uint16_t len);

int log_suback_message(const MqttSnLogger *logger,
                       ParsedMqttSnHeader *header,
                       const device_address *from,
                       const char *network,
                       const uint8_t *data,
                       uint16_t len);

int log_unsubscribe_message(const MqttSnLogger *logger,
                            ParsedMqttSnHeader *header,
                            const device_address *from,
                            const char *network,
                            const uint8_t *data,
                            uint16_t len);

int log_unsuback_message(const MqttSnLogger *logger,
                         ParsedMqttSnHeader *header,
                         const device_address *from,
                         const char *network,
                         const uint8_t *data,
                         uint16_t len);

int log_pingreq_message(const MqttSnLogger *logger,
                        ParsedMqttSnHeader *header,
                        const device_address *from,
                        const char *network,
                        const uint8_t *data,
                        uint16_t len);

int log_pingresp_message(const MqttSnLogger *logger,
                         ParsedMqttSnHeader *header,
                         const device_address *from,
                         const char *network,
                         const uint8_t *data,
                         uint16_t len);

int log_disconnect_message(const MqttSnLogger *logger,
                           const ParsedMqttSnHeader *header,
                           const device_address *address,
                           const char *network,
                           const uint8_t *data,
                           uint16_t data_len);

#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_MQTTSNFORWARDERLOGGINGMESSAGES_H
