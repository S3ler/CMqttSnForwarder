//
// Created by SomeDude on 27.06.2019.
//

#ifndef CMQTTSNFORWARDER_CLIENT_MQTTSNCLIENTAWAITMESSAGE_H_
#define CMQTTSNFORWARDER_CLIENT_MQTTSNCLIENTAWAITMESSAGE_H_

#include <stdint.h>
#include <parser/MqttSnMessageParser.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_ {
  MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_SUCCESS = 0,
  MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_AWAIT,
  MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_ERROR,
  MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_TIMEOUT
} MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS;

typedef struct MqttSnClientAwaitMessage_ {
  MQTT_SN_MESSAGE_TYPE msg_type;
  uint16_t msg_id;
  MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS status;
} MqttSnClientAwaitMessage;

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_CLIENT_MQTTSNCLIENTAWAITMESSAGE_H_
