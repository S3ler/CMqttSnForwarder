//
// Created by SomeDude on 19.06.2019.
//

#ifndef CMQTTSNFORWARDER_GATEWAY_DATABASE_DB_ENTRY_MQTT_SN_CLIENT_WILL_MESSAGE_H_
#define CMQTTSNFORWARDER_GATEWAY_DATABASE_DB_ENTRY_MQTT_SN_CLIENT_WILL_MESSAGE_H_

#include <stdint.h>
#include <stdbool.h>
#include <platform/MqttSnMessageData.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct DB_ENTRY_MQTT_SN_WILL_MESSAGE_ {
  char willtopic[MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH];
  char willmsg[MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH];
  uint16_t willmsg_length;
  uint8_t qos;
  bool retain;
}DB_ENTRY_MQTT_SN_WILL_MESSAGE;

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_GATEWAY_DATABASE_DB_ENTRY_MQTT_SN_CLIENT_WILL_MESSAGE_H_
