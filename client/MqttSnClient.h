

//
// Created by SomeDude on 31.05.2019.
//

#ifndef CMQTTSNFORWARDER_CLIENT_MQTTSNCLIENT_H_
#define CMQTTSNFORWARDER_CLIENT_MQTTSNCLIENT_H_

#include <stdint.h>
#include "MqttSnClientAwaitMessage.h"
#include <platform/device_address.h>
#include <parser/MqttSnMessageParser.h>
#include <parser/MqttSnAdvertiseMessage.h>
#include <parser/MqttSnGwInfoMessage.h>
#include <network/MqttSnGatewayNetworkInterface.h>
#include <string.h>
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
	MQTT_SN_CLIENT_RETURN_ERROR   = -1,
	MQTT_SN_CLIENT_RETURN_SUCCESS = 0,
	MQTT_SN_CLIENT_RETURN_CONJESTION,
	MQTT_SN_CLIENT_RETURN_NOT_SUPPORTED,
	MQTT_SN_CLIENT_RETURN_TIMEOUT,
} MQTT_SN_CLIENT_RETURN_CODE;
typedef struct MqttSnClientRegistration_ {
	char *   topic_name;
	uint16_t topic_id;
} MqttSnClientRegistration;
typedef struct MqttSnClientReceivedAdvertise_ {
	MqttSnAdvertise received_advertise;
	device_address  from;
	uint8_t		signal_strength;
} MqttSnClientReceivedAdvertise;
typedef struct MqttSnClientReceivedGwInfo_ {
	MqttSnGwInfo   received_gw_info;
	device_address from;
	uint8_t	signal_strength;
} MqttSnClientReceivedGwInfo;
/**
 * default_timeout: used for all operation when other value given
 */


typedef struct MqttSnClient_ {
  //device_address mqtt_sn_gateway_address;
  int32_t default_timeout;
  uint8_t default_signal_strength;
  uint8_t status;
  uint16_t msg_counter;

  uint16_t connect_duration;
  device_address mqtt_sn_gateway_address;
  char client_id_str[MQTT_SN_MAX_CLIENT_ID_LENGTH];
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
  void *adv_cb_context;
  int32_t (*adv_cb)(struct MqttSnClient_ *client,
                    MqttSnClientReceivedAdvertise *rec_advertise,
                    void *context);
  void *gwinfo_cb_context;
  int32_t (*gwinfo_cb)(struct MqttSnClient_ *client,
                       MqttSnClientReceivedGwInfo *rec_gw_info,
                       void *context);

  MqttSnGatewayNetworkInterface gatewayNetwork;
  int32_t gatewayNetworkSendTimeout;
  int32_t gatewayNetworkReceiveTimeout;

  void *gatewayNetworkContext;
#ifdef WITH_LOGGING
  MqttSnLogger logger;
#endif
  uint8_t connected;
  MQTT_SN_RETURN_CODE connect_return_code;
  MqttSnClientAwaitMessage await_msg[MQTT_SN_CLIENT_MAX_MESSAGES_IN_FLIGHT];

  uint64_t last_ping_req_received;
  uint64_t last_ping_resp_received;
  MqttSnClientAwaitMessage ping_req_await_msg;
  int32_t connect_timeout_offset;

  uint8_t client_protocol_id;
} MqttSnClient;

int32_t global_mqtt_sn_client_gwinfo_cb(MqttSnClient *client,
                                        MqttSnClientReceivedGwInfo *rec_gw_info,
                                        void *context);
int32_t global_mqtt_sn_client_adv_cb(MqttSnClient *client,
                                     MqttSnClientReceivedAdvertise *rec_advertise,
                                     void *context);
// init network too
int32_t MqttSnClientInit(MqttSnClient *client, const MqttSnLogger *logger, void *gatewayNetworkContext);
int32_t MqttSnClientDeinit(MqttSnClient *client);

int32_t MqttSnClientLoop(MqttSnClient *client);
// disconnect network too
// deinint network too

void MqttSnClientTimeoutOffset(MqttSnClient *client, int32_t connect_timeout_offset);

MQTT_SN_CLIENT_RETURN_CODE MqttSnClientDirectConnect(MqttSnClient *client,
                                                     device_address *mqtt_sn_gateway_address,
                                                     int32_t connect_timeout_ms,
                                                     uint8_t clean_session,
                                                     const char *client_id,
                                                     uint16_t connect_duration);
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientDisconnect(MqttSnClient *client);
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
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientPublishTopicName(MqttSnClient *client,
                                                        const char *topic_name,
                                                        uint16_t topic_name_length,
                                                        uint8_t retain,
                                                        int8_t qos,
                                                        uint8_t *data,
                                                        uint16_t data_len);
/**
 * Publishes to predefined topic id with given qos
 */
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientPublishPredefined(MqttSnClient *client,
                                                         uint16_t predefined_topic_id,
                                                         uint8_t retain,
                                                         int8_t qos,
                                                         uint8_t *data,
                                                         uint16_t data_len);
/**
 * Publishes to topic id with given qos
 */
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientPublishTopicId(MqttSnClient *client,
                                                      uint16_t short_topic_id,
                                                      uint8_t retain,
                                                      int8_t qos,
                                                      uint8_t *data,
                                                      uint16_t data_len);

/**
 * Registers topic_name to gateway
 */
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientRegister(MqttSnClient *client, const char *topic_name);

MQTT_SN_CLIENT_RETURN_CODE MqttSnClientSubscribePredefined(MqttSnClient *client, int8_t qos);
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientSubscribeShort(MqttSnClient *client, int8_t qos);
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientSubscribeTopicName(MqttSnClient *client, const char *topic_name);

int32_t MqttSnClientUnsubscribePredefined(MqttSnClient *client, int8_t qos);
int32_t MqttSnClientUnsubscribeShort(MqttSnClient *client, int8_t qos);
int32_t MqttSnClientUnsubscribeTopicName(MqttSnClient *client, const char *topic_name);

MQTT_SN_CLIENT_RETURN_CODE MqttSnClientSleep(MqttSnClient *client, uint16_t sleep_duration);
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientWakeup(MqttSnClient *client);

int32_t MqttSnPingGateway(MqttSnClient *client);

/*
    int32_t find_gateway_monitor_adv_cb( MqttSnClient *client,
                                        const MqttSnClientReceivedAdvertise *advertise,
                                        void* context)

 */

MQTT_SN_CLIENT_RETURN_CODE MqttSnClientAwaitAdvertise(MqttSnClient *client,
                                                      int32_t timeout_s,
                                                      MqttSnClientReceivedAdvertise *dst_advertise);
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientAwaitAdvertiseCallback(MqttSnClient *client,
                                                              int32_t timeout_s,
                                                              void *context,
                                                              int32_t (*adv_cb)(const MqttSnClient *client,
                                                                                const MqttSnClientReceivedAdvertise *rec_advertise,
                                                                                void *context));
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientSearchGw(MqttSnClient *client,
                                                int32_t timeout_ms,
                                                uint8_t radius,
                                                MqttSnClientReceivedGwInfo *dst_gwinfo);
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientSearchGwCallback(MqttSnClient *client,
                                                        int32_t timeout_ms,
                                                        uint8_t radius,
                                                        void *context,
                                                        int32_t (*gwinfo_cb)(const MqttSnClient *client,
                                                                             const MqttSnClientReceivedGwInfo *rec_gw_info,
                                                                             void *context));

int32_t MqttSnClientSafeStrlen(const char *str, int32_t max_len);
int32_t parse_and_handle_advertise(MqttSnClient *client, MqttSnMessageData *msg);
int32_t parse_and_handle_gwinfo(MqttSnClient *client, MqttSnMessageData *msg);

int32_t is_await_msg(MqttSnClient *client, MQTT_SN_MESSAGE_TYPE msg_type, uint16_t msg_id);
int32_t set_await_msg_status(MqttSnClient *client, int32_t await_fd, MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS status);
MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS get_await_status(MqttSnClient *client, int32_t await_fd);
int32_t free_await_msg(MqttSnClient *client, int32_t await_fd);
int32_t new_await_msg(MqttSnClient *client, MQTT_SN_MESSAGE_TYPE msg_type, uint16_t msg_id);
MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS get_await_status_timeout_ms(MqttSnClient *client, int32_t await_fd,
                                                                uint64_t start_ts_ms, uint64_t timeout_ms);
#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_CLIENT_MQTTSNCLIENT_H_
