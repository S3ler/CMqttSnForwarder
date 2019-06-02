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

typedef struct MqttSnReceivedAdvertise_ {
  device_address from;
  int32_t timeout;
  MqttSnAdvertise advertise;
} MqttSnReceivedAdvertise;

// if gw_info.address == from then its from a GW else from a client
typedef struct MqttSnReceivedGwInfo_ {
  device_address from;
  int32_t timeout;
  MqttSnGwInfo gw_info;
} MqttSnReceivedGwInfo;

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
  uint16_t msg_id;
  uint16_t connect_duration;
  device_address *mqtt_sn_gateway_address;
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
} MqttSnClient;

// init network too
int32_t MqttSnClientInit(MqttSnClient *client,
                         log_level_t log_level,
                         int32_t (*publish_cb)(MqttSnClient, const char *, uint16_t, uint8_t *, uint16_t),
                         void *gatewayNetworkContext);
int32_t MqttSnClientDeinit(MqttSnClient *client);

int32_t MqttSnClientLoop(MqttSnClient *client);
// disconnect network too
// deinint network too

/**
 * Receive MqttSnAdvertise messages until either timeout runs out or advertise is full.
 * @param client to use for awaiting MqttSnAdvertise messages
 * @param timeout for receiving MqttSnAdvertise messages. Between -1 and INT32_MAX. Passed to the underlying GatewayNetwork. On -1 waits indefinitely. On 0 returns immediately.
 * @param advertise pointer to an advertise array to store received MqttSnAdvertise messages
 * @param advertise_len length of the advertise array
 * @return negative value on error, 0 on success.
 */
int32_t MqttSnClientAwaitAdvertise(MqttSnClient *client,
                                   int32_t timeout,
                                   MqttSnReceivedAdvertise *advertise,
                                   uint16_t advertise_len);

/**
 * Sends a MqttSnSearchGw and awaits MqttSnGwInfo as well as MqttSnAdvertise messages. Returns when timeout runs out or gw_info is full.
 * @param client to use for awaiting MqttSnGwInfo and MqttSnAdvertise messages
 * @param radius broadcast radius of the MqttSnSearchGw is limited
 * @param timeout until functions returns
 * @param gw_info to a MqttSnGwInfo array to store the received MqttSnGwInfo messages.
 * @param gw_info_len length of the MqttSnGwInfo array
 * @param advertise pointer to an advertise array to store received MqttSnAdvertise messages. May be NULL when advertise_len is 0.
 * @param advertise_len length of the advertise array. May be 0 then no MqttSnAdvertise messages are saved.
 * @return
 */
int32_t MqttSnClientSearchGw(MqttSnClient *client,
                             uint8_t radius,
                             int32_t timeout,
                             MqttSnReceivedGwInfo *gw_info,
                             uint16_t gw_info_len,
                             MqttSnReceivedAdvertise *advertise,
                             uint16_t advertise_len);

// uses default_timeout and saves duration as connect_duration
MQTT_SN_RETURN_CODE MqttSnClientConnect(MqttSnClient *client);

/**
 * Sends a MqttSnPublish with Quality of Service (qos) -1 to a predefined topic. Works with disconnected MqttSnClients.
 * @param client to use for sending
 * @param predefined_topic_id to publish to
 * @param data to publish
 * @param data_len length of data to publish
 * @return -1 on error, else the count of bytes send
 */
int32_t MqttSnClientPublishM1(MqttSnClient *client,
                              uint16_t predefined_topic_id,
                              uint8_t retain,
                              uint8_t *data,
                              uint16_t data_len);

/**
 * Registers (if not yet) and publishes message
 */
int32_t MqttSnClientPublishTopicName(MqttSnClient *client,
                                     const char* topic_name,
                                     uint16_t topic_name_length,
                                     uint8_t retain,
                                     uint8_t *data,
                                     uint16_t data_len);

int32_t MqttSnClientRegister(MqttSnClient *client, const char* topic_name, uint16_t topic_name_length);


#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_CLIENT_MQTTSNCLIENT_H_
