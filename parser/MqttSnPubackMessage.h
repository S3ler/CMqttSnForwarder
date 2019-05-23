//
// Created by SomeDude on 21.05.2019.
//

#ifndef CMQTTSNFORWARDER_PARSER_MQTTSNPUBACKMESSAGE_H_
#define CMQTTSNFORWARDER_PARSER_MQTTSNPUBACKMESSAGE_H_

#include <stdint.h>
#include "MqttSnMessageParser.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MQTT_SN_MESSAGE_PUBACK_HEADER_LENGTH                  MQTT_SN_MESSAGE_HEADER_SHORT_LENGTH
#define MQTT_SN_MESSAGE_PUBACK_TOPICID_LENGTH                 MQTT_SN_TOPIC_ID_LENGTH
#define MQTT_SN_MESSAGE_PUBACK_MESSAGEID_LENGTH               MQTT_SN_MESSAGE_ID_LENGTH
#define MQTT_SN_MESSAGE_PUBACK_RETURNCODE_LENGTH              MQTT_SN_RETURNCODE_LENGTH
#define MQTT_SN_MESSAGE_PUBACK_LENGTH                         (MQTT_SN_MESSAGE_PUBACK_HEADER_LENGTH + \
                                                              MQTT_SN_MESSAGE_PUBACK_TOPICID_LENGTH + \
                                                              MQTT_SN_MESSAGE_PUBACK_MESSAGEID_LENGTH + \
                                                              MQTT_SN_MESSAGE_PUBACK_RETURNCODE_LENGTH)

int32_t parse_puback_bytes(uint16_t *topic_id,
                           uint16_t *msg_id,
                           MQTT_SN_RETURN_CODE *return_code,
                           const uint8_t *data,
                           uint16_t data_len);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_PARSER_MQTTSNPUBACKMESSAGE_H_
