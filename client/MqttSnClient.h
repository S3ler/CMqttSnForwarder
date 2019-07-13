//
// Created by SomeDude on 31.05.2019.
//

#ifndef CMQTTSNFORWARDER_CLIENT_MQTTSNCLIENT_H_
#define CMQTTSNFORWARDER_CLIENT_MQTTSNCLIENT_H_


#include <stdint.h>
#include <stdbool.h>
#include "MqttSnClientAwaitMessage.h"
#include <platform/device_address.h>
#include <parser/MqttSnMessageParser.h>
#include <parser/MqttSnAdvertiseMessage.h>
#include <parser/MqttSnGwInfoMessage.h>
#include <network/MqttSnGatewayNetworkInterface.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MQTT_SN_CLIENT_DEFAULT_TIMEOUT_MS
#define MQTT_SN_CLIENT_DEFAULT_TIMEOUT_MS 10000
#endif

#ifndef MQTT_SN_CLIENT_DEFAULT_SIGNAL_STRENGTH
#define MQTT_SN_CLIENT_DEFAULT_SIGNAL_STRENGTH UINT8_MAX
#endif

#ifndef MQTT_SN_CLIENT_DEFAULT_CONNECT_DURATION
#define MQTT_SN_CLIENT_DEFAULT_CONNECT_DURATION 5000
#endif

#ifndef MQTT_SN_CLIENT_MAX_REGISTRATIONS
#define MQTT_SN_CLIENT_MAX_REGISTRATIONS 5
#endif

#ifndef MQTT_SN_CLIENT_MAX_REGISTRATION_TOPIC_NAME_LENGTH
#define MQTT_SN_CLIENT_MAX_REGISTRATION_TOPIC_NAME_LENGTH 255
#endif

#ifndef MQTT_SN_CLIENT_MAX_MESSAGES_IN_FLIGHT
#define MQTT_SN_CLIENT_MAX_MESSAGES_IN_FLIGHT 5
#endif

#ifndef MQTT_SN_CLIENT_PING_REQ_ADVANCE_TIME_S
#define MQTT_SN_CLIENT_PING_REQ_ADVANCE_TIME_S 10
#endif



typedef enum MQTT_SN_CLIENT_RETURN_CODE_ {
  MQTT_SN_CLIENT_RETURN_SUCCESS = 0,
  MQTT_SN_CLIENT_RETURN_CONJESTION,
  MQTT_SN_CLIENT_RETURN_NOT_SUPPORTED,
  MQTT_SN_CLIENT_RETURN_TIMEOUT,
  MQTT_SN_CLIENT_RETURN_ERROR
} MQTT_SN_CLIENT_RETURN_CODE;
typedef struct MqttSnClientRegistration_ {
  char *topic_name;
  uint16_t topic_id;
} MqttSnClientRegistration;
/**
 * default_timeout: used for all operation when other value given
 */
typedef struct MqttSnClient_ {
  int32_t default_timeout;
  uint8_t default_signal_strength;
  uint8_t status;
  uint16_t msg_counter;

  uint16_t connect_duration;
  char *client_id;
  char *will_topic;
  uint8_t *will_msg;
  uint16_t will_msg_len;

  MqttSnClientRegistration registrations[MQTT_SN_CLIENT_MAX_REGISTRATIONS];
  uint16_t registrations_len;

  int32_t (*publish_cb)(struct MqttSnClient_ client,
                        const char *topic_name,
                        uint16_t topic_id,
                        uint8_t *data,
                        uint16_t data_len);

  MqttSnGatewayNetworkInterface gatewayNetwork;
  int32_t gatewayNetworkSendTimeout;
  int32_t gatewayNetworkReceiveTimeout;

  void *gatewayNetworkContext;
#ifdef WITH_LOGGING
  MqttSnLogger logger;
#endif
  bool connected;
  MQTT_SN_RETURN_CODE connect_return_code;
  MqttSnClientAwaitMessage await_msg[MQTT_SN_CLIENT_MAX_MESSAGES_IN_FLIGHT];

  uint64_t last_ping_req_received;
  uint64_t last_ping_resp_received;
  MqttSnClientAwaitMessage ping_req_await_msg;
  int32_t connect_timeout_offset;

  uint8_t client_protocol_id;
} MqttSnClient;

// init network too
int32_t MqttSnClientInit(MqttSnClient *client, const MqttSnLogger *logger, void *gatewayNetworkContext);
int32_t MqttSnClientDeinit(MqttSnClient *client);

int32_t MqttSnClientLoop(MqttSnClient *client);
// disconnect network too
// deinint network too

void MqttSnClientTimeoutOffset(MqttSnClient *client, int32_t connect_timeout_offset);

MQTT_SN_CLIENT_RETURN_CODE MqttSnClientConnect(MqttSnClient *client,
                                               device_address *mqtt_sn_gateway_address,
                                               int32_t connect_timeout_ms,
                                               bool clean_session,
                                               const char *client_id,
                                               uint16_t connect_duration);
/**
 * Sends a MqttSnPublish with Quality of Service (qos) -1 to a predefined topic. Works with disconnected MqttSnClients.
 * @param client to use for sending
 * @param predefined_topic_id to publish to
 * @param data to publish
 * @param data_len length of data to publish
 * @return -1 on error, else the count of bytes send
 */
int32_t MqttSnClientPublishPredefinedM1(MqttSnClient *client,
                                        uint16_t predefined_topic_id,
                                        uint8_t retain,
                                        uint8_t *data,
                                        uint16_t data_len);

/**
 * Registers (if not yet) and publishes message
 * Look up if registered => else registers => then publishes
 */
int32_t MqttSnClientPublishTopicName(MqttSnClient *client,
                                     const char *topic_name,
                                     uint16_t topic_name_length,
                                     uint8_t retain,
                                     int8_t qos,
                                     uint8_t *data,
                                     uint16_t data_len);
/**
 * Publishes to predefined topic id with given qos
 */
int32_t MqttSnClientPublishPredefined(MqttSnClient *client,
                                      uint16_t predefined_topic_id,
                                      uint8_t retain,
                                      int8_t qos,
                                      uint8_t *data,
                                      uint16_t data_len);
/**
 * Publishes to topic id with given qos
 */
int32_t MqttSnClientPublishTopicId(MqttSnClient *client,
                                   uint16_t predefined_topic_id,
                                   uint8_t retain,
                                   int8_t qos,
                                   uint8_t *data,
                                   uint16_t data_len);

/**
 * Registers topic_name to gateway
 */
int32_t MqttSnClientRegister(MqttSnClient *client, const char *topic_name);

int32_t MqttSnPingGateway(MqttSnClient *client);

int32_t is_await_msg(MqttSnClient *client, MQTT_SN_MESSAGE_TYPE msg_type, uint16_t msg_id);
int32_t set_await_msg_status(MqttSnClient *client, int32_t await_fd, MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS status);
MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS get_await_status(MqttSnClient *client, int32_t await_fd);
int32_t free_await_msg(MqttSnClient *client, int32_t await_fd);
int32_t new_await_msg(MqttSnClient *client, MQTT_SN_MESSAGE_TYPE msg_type, uint16_t msg_id);




#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_CLIENT_MQTTSNCLIENT_H_
