//
// Created by SomeDude on 19.06.2019.
//

#ifndef CMQTTSNFORWARDER_GATEWAY_DATABASE_DB_ENTRY_MQTT_SN_SUBSCRIPTION_H_
#define CMQTTSNFORWARDER_GATEWAY_DATABASE_DB_ENTRY_MQTT_SN_SUBSCRIPTION_H_

#include <stdint.h>
#include <platform/MqttSnMessageData.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct DB_ENTRY_MQTT_SN_SUBSCRIPTION_{
  uint16_t topic_id;
  uint8_t qos;
  char topic_name[MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH];
}DB_ENTRY_MQTT_SN_SUBSCRIPTION;

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_GATEWAY_DATABASE_DB_ENTRY_MQTT_SN_SUBSCRIPTION_H_
