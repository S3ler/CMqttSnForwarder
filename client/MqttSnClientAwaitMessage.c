//
// Created by SomeDude on 27.06.2019.
//

#include "MqttSnClientAwaitMessage.h"
#include "MqttSnClient.h"

MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS get_await_status(MqttSnClient *client, int32_t await_fd) {
  if (await_fd < 0 || await_fd > MQTT_SN_CLIENT_MAX_MESSAGES_IN_FLIGHT - 1) { // maybe use assert instead
    return MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_ERROR;
  }
  return client->await_msg[await_fd].status;
}

int32_t is_await_msg(MqttSnClient *client, MQTT_SN_MESSAGE_TYPE msg_type, uint16_t msg_id) {
  for (uint16_t i = 0; i < MQTT_SN_CLIENT_MAX_MESSAGES_IN_FLIGHT; i++) {
    MqttSnClientAwaitMessage await_message = client->await_msg[i];
    if (await_message.msg_type == ANY_MESSAGE_TYPE) {
      continue;
    }
    if (client->await_msg[i].status == MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_AWAIT
        && client->await_msg[i].msg_type == msg_type
        && client->await_msg[i].msg_id == msg_id) {
      return i;
    }
  }
  return -1;
}
int32_t set_await_msg_status(MqttSnClient *client, int32_t await_fd, MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS status) {
  if (await_fd < 0 || await_fd > MQTT_SN_CLIENT_MAX_MESSAGES_IN_FLIGHT - 1) { // maybe use assert instead
    return -1;
  }
  client->await_msg[await_fd].status = status;
  return 0;
}
int32_t free_await_msg(MqttSnClient *client, int32_t await_fd) {
  if (await_fd < 0 || await_fd > MQTT_SN_CLIENT_MAX_MESSAGES_IN_FLIGHT - 1) { // maybe use assert instead
    return -1;
  }
  client->await_msg[await_fd].msg_type = ANY_MESSAGE_TYPE;
  client->await_msg[await_fd].msg_id = 0;
  client->await_msg[await_fd].status = MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_SUCCESS;
  return 0;
}
int32_t new_await_msg(MqttSnClient *client, MQTT_SN_MESSAGE_TYPE msg_type, uint16_t msg_id) {
  for (uint16_t i = 0; i < MQTT_SN_CLIENT_MAX_MESSAGES_IN_FLIGHT; i++) {
    if (client->await_msg[i].msg_type == ANY_MESSAGE_TYPE) {
      client->await_msg[i].msg_type = msg_type;
      client->await_msg[i].msg_id = msg_id;
      client->await_msg[i].status = MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_AWAIT;
      return i;
    }
  }
  return -1;
}
void MqttSnClientTimeoutOffset(MqttSnClient *client, int32_t connect_timeout_offset) {
  client->connect_timeout_offset = connect_timeout_offset;
}
