//
// Created by SomeDude on 19.06.2019.
//

#ifndef CMQTTSNFORWARDER_GATEWAY_DATABASE_DB_ENTRY_MQTT_SN_CLIENT_REGISTRATION_H_
#define CMQTTSNFORWARDER_GATEWAY_DATABASE_DB_ENTRY_MQTT_SN_CLIENT_REGISTRATION_H_

#include <stdint.h>
#include <stdbool.h>
#include <platform/MqttSnMessageData.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct DB_ENTRY_MQTT_SN_REGISTRATION_ {
  uint16_t topic_id;
  char topic_name[MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH];
  bool known;
} DB_ENTRY_MQTT_SN_REGISTRATION;

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_GATEWAY_DATABASE_DB_ENTRY_MQTT_SN_CLIENT_REGISTRATION_H_
