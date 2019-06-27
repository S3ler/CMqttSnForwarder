//
// Created by SomeDude on 31.05.2019.
//

#ifndef CMQTTSNFORWARDER_CLIENT_MQTTSNCLIENT_H_
#define CMQTTSNFORWARDER_CLIENT_MQTTSNCLIENT_H_

#include <stdint.h>
#include <platform/device_address.h>
#include <parser/MqttSnMessageParser.h>
#include <parser/MqttSnAdvertiseMessage.h>
#include <parser/MqttSnGwInfoMessage.h>
#include <network/MqttSnGatewayNetworkInterface.h>
#include <stdbool.h>

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
} MqttSnClient;

// init network too
int32_t MqttSnClientInit(MqttSnClient *client, log_level_t log_level, void *gatewayNetworkContext);
int32_t MqttSnClientDeinit(MqttSnClient *client);

int32_t MqttSnClientLoop(MqttSnClient *client);
// disconnect network too
// deinint network too

MQTT_SN_RETURN_CODE MqttSnClientConnect(MqttSnClient *client,
                                        const device_address *mqtt_sn_gateway_address,
                                        int32_t timeout_ms,
                                        bool clean_session,
                                        const char *client_id,
                                        uint16_t duration);
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

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_CLIENT_MQTTSNCLIENT_H_
