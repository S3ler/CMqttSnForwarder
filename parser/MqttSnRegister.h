//
// Created by SomeDude on 21.05.2019.
//

#ifndef CMQTTSNFORWARDER_PARSER_MQTTSNREGISTER_H_
#define CMQTTSNFORWARDER_PARSER_MQTTSNREGISTER_H_

#include <stdint.h>
#include "MqttSnMessageParser.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MQTT_SN_MESSAGE_REGISTER_HEADER_LENGTH(indicator)       (MQTT_SN_HEADER_LENGTH(indicator))
#define MQTT_SN_MESSAGE_REGISTER_TOPICID_LENGTH                 MQTT_SN_TOPIC_ID_LENGTH
#define MQTT_SN_MESSAGE_REGISTER_MESSAGEID_LENGTH               MQTT_SN_MESSAGE_ID_LENGTH
#define MQTT_SN_MESSAGE_REGISTER_TOPICNAME_MIN_LENGTH           MQTT_SN_TOPICNAME_MIN_LENGTH
#define MQTT_SN_MESSAGE_REGISTER_MIN_LENGTH                     (MQTT_SN_MESSAGE_REGISTER_HEADER_LENGTH(0) + \
                                                                MQTT_SN_MESSAGE_REGISTER_TOPICID_LENGTH + \
                                                                MQTT_SN_MESSAGE_REGISTER_MESSAGEID_LENGTH + \
                                                                MQTT_SN_MESSAGE_REGISTER_TOPICNAME_MIN_LENGTH)
#define MQTT_SN_MESSAGE_REGISTER_WO_TOPICNAME_MAX_LENGTH        (MQTT_SN_MESSAGE_REGISTER_HEADER_LENGTH(1) + \
                                                                MQTT_SN_MESSAGE_REGISTER_TOPICID_LENGTH + \
                                                                MQTT_SN_MESSAGE_REGISTER_MESSAGEID_LENGTH)
#define MQTT_SN_MESSAGE_REGISTER_TOPICNAME_MAX_LENGTH           (MQTT_SN_TOPICNAME_MAX_LENGTH - MQTT_SN_MESSAGE_REGISTER_WO_TOPICNAME_MAX_LENGTH)

int32_t parse_register_byte(uint16_t *topic_id,
                            uint16_t *msg_id,
                            char *topic_name,
                            uint16_t *topic_name_length,
                            uint16_t max_topic_name_length,
                            const uint8_t *data,
                            uint16_t data_len);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_PARSER_MQTTSNREGISTER_H_
