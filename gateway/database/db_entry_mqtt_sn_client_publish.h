//
// Created by SomeDude on 19.06.2019.
//

#ifndef CMQTTSNFORWARDER_GATEWAY_DATABASE_DB_ENTRY_MQTT_SN_CLIENT_PUBLISH_H_
#define CMQTTSNFORWARDER_GATEWAY_DATABASE_DB_ENTRY_MQTT_SN_CLIENT_PUBLISH_H_

#include <stdint.h>
#include <stdbool.h>
#include <platform/MqttSnMessageData.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct DB_ENTRY_MQTT_SN_PUBLISH_ {
  MqttSnMessageData message_data;
  uint16_t topic_id;
  uint8_t qos;
  bool retain;
  bool dup;
  uint16_t msg_id;
  uint16_t publish_id;
  uint32_t retransmition_timeout;
} DB_ENTRY_MQTT_SN_PUBLISH;

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_GATEWAY_DATABASE_DB_ENTRY_MQTT_SN_CLIENT_PUBLISH_H_
