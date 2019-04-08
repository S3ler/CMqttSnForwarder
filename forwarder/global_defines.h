#ifndef GATEWAY_GLOBAL_DEFINES_H
#define GATEWAY_GLOBAL_DEFINES_H

#include <stdint.h>

#define FORWARDER_HEADER_LEN 3
#define GATEWAY_NETWORK_MAX_DATA_LEN 255
#define CLIENT_NETWORK_MAX_DATA_LEN 255

#define Encapsulated_message 0xFE
#define CMQTTSNFORWARDER_DEVICE_ADDRESS_LENGTH 6
typedef struct device_address {
  uint8_t bytes[CMQTTSNFORWARDER_DEVICE_ADDRESS_LENGTH];
} device_address;

#pragma pack(push, 1)
typedef struct MQTT_SN_FORWARD_ENCAPSULATION {
  uint8_t length;
  uint8_t msg_type;
  uint8_t crtl;
  device_address wireless_node_id;
  uint8_t mqtt_sn_message[GATEWAY_NETWORK_MAX_DATA_LEN - FORWARDER_HEADER_LEN - sizeof(device_address)];

} MQTT_SN_FORWARD_ENCAPSULATION;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct MqttSnMessageHeaderOneOctetLengthField {
  uint8_t length;
  uint8_t msg_type;
} MqttSnMessageHeaderOneOctetLengthField;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct MqttSnMessageHeaderThreeOctetsLengthField {
  uint8_t three_octets_length_field_indicator;
  uint8_t msb_length;
  uint8_t lsb_length;
  uint8_t msg_type;
} MqttSnMessageHeaderThreeOctetsLengthField;
#pragma pack(pop)

#endif //GATEWAY_GLOBAL_DEFINES_H
