//
// Created by SomeDude on 19.06.2019.
//

#ifndef CMQTTSNFORWARDER_GATEWAY_DATABASE_DB_ENTRY_MQTT_SUBSCRIPTION_H_
#define CMQTTSNFORWARDER_GATEWAY_DATABASE_DB_ENTRY_MQTT_SUBSCRIPTION_H_

#include <stdint.h>
#include <platform/MqttSnMessageData.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct DB_ENTRY_MQTT_SUBSCRIPTION_ {
  uint32_t client_subscription_count;
  char topic_name[MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH];
} DB_ENTRY_MQTT_SUBSCRIPTION;

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_GATEWAY_DATABASE_DB_ENTRY_MQTT_SUBSCRIPTION_H_
