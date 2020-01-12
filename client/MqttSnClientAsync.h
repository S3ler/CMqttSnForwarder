//
// Created by SomeDude on 01.06.2019.
//

#ifndef CMQTTSNFORWARDER__MQTTSNCLIENTASYNC_H_
#define CMQTTSNFORWARDER__MQTTSNCLIENTASYNC_H_

#include <stdint.h>
#include <platform/device_address.h>
#include <parser/MqttSnMessageParser.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MQTT_SN_CLIENT_ASYNC_MAX_INFLIGHT_MESSAGES
#define MQTT_SN_CLIENT_ASYNC_MAX_INFLIGHT_MESSAGES 5
#endif

typedef struct MqttSnClientSyncAwaitMessages_ {
  MQTT_SN_MESSAGE_TYPE type;
  void* cb_function;
} MqttSnClientSyncAwaitMessages;

typedef struct MqttSnClientAsync_ {
  int32_t default_timeoout;
  uint16_t connect_duration;
  int32_t (*connect_cb)(struct MqttSnClientAsync_ *c, device_address *msga, MQTT_SN_RETURN_CODE rt);
} MqttSnClientAsync;

int32_t MqttSnClientAsyncInit(MqttSnClientAsync *client);
int32_t MqttSnClientAsyncDeinit(MqttSnClientAsync *client);

int32_t MqttSnClientAsyncConnect(MqttSnClientAsync *client,
                                 device_address *mqtt_sn_gateway_address,
                                 int32_t timeout,
                                 uint16_t duration,
                                 const char *client_id,
                                 const char *will_topic,
                                 uint8_t *will_msg,
                                 uint16_t will_msg_len,
                                 int32_t (*connect_cb)(MqttSnClientAsync *c,
                                                       device_address *msga,
                                                       MQTT_SN_RETURN_CODE rt));

int32_t MqttSnClientAsyncAwaitAdvertise(MqttSnClientAsync *client,
                                        int32_t timeout,
                                        int32_t (*await_advertisement_cb)(MqttSnClientAsync *client,
                                                                          int32_t timeout_left,
                                                                          device_address from,
                                                                          uint8_t gw_id,
                                                                          uint16_t duration));

int32_t MqttSnClientAsyncSearchGw(MqttSnClientAsync *client,
                                  int32_t timeout,
                                  uint8_t radius,
                                  int32_t (*callback)(MqttSnClientAsync *client,
                                                      device_address from,
                                                      int32_t timeout_left,
                                                      uint8_t gw_id,
                                                      uint16_t duration));

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER__MQTTSNCLIENTASYNC_H_
