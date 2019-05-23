//
// Created by SomeDude on 19.05.2019.
//

#ifndef CMQTTSNFORWARDER_PARSER_MQTTSNFORWARDERENCAPSULATIONMESSAGE_H_
#define CMQTTSNFORWARDER_PARSER_MQTTSNFORWARDERENCAPSULATIONMESSAGE_H_

#include <stdint.h>
#include "MqttSnMessageParser.h"
#include <platform/device_address.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MQTT_SN_MESSAGE_FORWARDER_ENCAPSULATION_HEADER_LENGTH(indicator)  (MQTT_SN_HEADER_LENGTH(indicator))
#define MQTT_SN_MESSAGE_FORWARDER_ENCAPSULATION_CTRL_LENGTH               MQTT_SN_CTRL_LENGTH
#define MQTT_SN_MESSAGE_FORWARDER_ENCAPSULATION_WIRELESSNODEID_LENGTH     MQTT_SN_WIRELESSNODEID_LENGTH
#define MQTT_SN_MESSAGE_FORWARDER_ENCAPSULATION_MIN_LENGTH                (MQTT_SN_MESSAGE_FORWARDER_ENCAPSULATION_HEADER_LENGTH(0) + \
                                                                          MQTT_SN_MESSAGE_FORWARDER_ENCAPSULATION_CTRL_LENGTH + \
                                                                          MQTT_SN_MESSAGE_FORWARDER_ENCAPSULATION_WIRELESSNODEID_LENGTH)

#define MQTT_SN_MESSAGE_FORWARDER_ENCAPSULATION_CRTL_BROADCAST_RADIUS     1

int32_t parse_forwarder_encapsulation_bytes(uint8_t *radius,
                                            device_address *wireless_node_id,
                                            uint8_t *mqtt_sn_message,
                                            uint16_t *mqtt_sn_message_len,
                                            uint16_t max_mqtt_sn_message_len,
                                            const uint8_t *data,
                                            uint16_t data_len);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_PARSER_MQTTSNFORWARDERENCAPSULATIONMESSAGE_H_
