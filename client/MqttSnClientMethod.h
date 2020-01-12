

//
// Created by SomeDude on 31.05.2019.
//

#ifndef CMQTTSNFORWARDER_CLIENT_MQTTSNCLIENT_H_
#define CMQTTSNFORWARDER_CLIENT_MQTTSNCLIENT_H_

#include <stdint.h>
#include <string.h>
#include <parser/MqttSnPublishMessage.h>
#include "MqttSnClientConnectionStatus.h"
#include "MqttSnClient.h"
#include "MqttSnClientMsgHandlerReturnValue.h"
#include "MqttSnClientReturnCode.h"

#ifdef __cplusplus
extern "C" {
#endif

MQTT_SN_CLIENT_STATUS           MqttSnClientInit(MqttSnClient *client, const MqttSnLogger *logger);
MQTT_SN_CLIENT_STATUS           MqttSnClientDeinit(MqttSnClient *client);
MQTT_SN_CLIENT_LOOP_RETURN_CODE MqttSnClientLoop(MqttSnClient *client);

void                       MqttSnClientSetTimeoutOffset(MqttSnClient *client, int32_t timeout_offset_s);
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientSetRetryTimeout(MqttSnClient *client, int32_t retry_timeout_ms);
void                       MqttSnClientSetRetryCount(MqttSnClient *client, uint16_t retry_count);
void                       MqttSnClientSetCongestionDelay(MqttSnClient *client, uint64_t congestion_delay_s);
void                       MqttSnClientSetDefaultSignalStrength(MqttSnClient *client, uint8_t default_signal_strength);

void    MqttSnClientEnableSearchGwReply(MqttSnClient *client);
void    MqttSnClientDisableSearchGwReply(MqttSnClient *client);
void    MqttSnClientSearchGwGwInfoReplyDelay(MqttSnClient *client, uint16_t gw_info_delay_ms);
uint8_t MqttSnClientIsSearchGwReplyEnabled(MqttSnClient *client);

MQTT_SN_CLIENT_RETURN_CODE MqttSnClientSetMqttSnGatewayAddress(MqttSnClient *client, device_address *mqtt_sn_gateway_address);
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientSetMqttSnBroadcastAddress(MqttSnClient *client, device_address *mqtt_sn_broadcast_address);
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientSetPublishCallback(MqttSnClient *client,
                                                          int32_t (*publish_cb)(const MqttSnClient *, const MqttSnPublish *));

MQTT_SN_CLIENT_RETURN_CODE MqttSnClientAwaitAdvertise(MqttSnClient *client, int32_t timeout_s,
                                                      MqttSnClientReceivedAdvertise *dst_advertise);
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientSearchGw(MqttSnClient *client, int32_t random_timeout_ms, uint8_t radius,
                                                MqttSnClientReceivedGwInfo *dst_gwinfo);

MQTT_SN_CLIENT_RETURN_CODE MqttSnClientAutoConnect(MqttSnClient *client, MQTT_SN_CLIENT_FIND_GW_PATTERN find_gateway_pattern,
                                                   int32_t find_gateway_wait_timeout_ms, device_address *mqtt_sn_gateway_address,
                                                   uint8_t clean_session, const char *client_id, uint16_t connect_duration,
                                                   const char *will_topic, int8_t will_qos, uint8_t will_retain, const uint8_t *will_msg,
                                                   uint16_t will_msg_length);
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientDirectConnect(MqttSnClient *client, device_address *mqtt_sn_gateway_address, uint8_t clean_session,
                                                     const char *client_id, uint16_t connect_duration, const char *will_topic,
                                                     int8_t will_qos, uint8_t will_retain, const uint8_t *will_msg,
                                                     uint16_t will_msg_length);
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientDirectConnectWithWill(MqttSnClient *client, device_address *mqtt_sn_gateway_address,
                                                             uint8_t clean_session, const char *client_id, uint16_t connect_duration,
                                                             const char *will_topic, int8_t will_qos, uint8_t will_retain,
                                                             const uint8_t *will_msg, uint16_t will_msg_length);
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientDirectConnectWithoutWill(MqttSnClient *client, device_address *mqtt_sn_gateway_address,
                                                                uint8_t clean_session, const char *client_id, uint16_t connect_duration);
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientReconnect(MqttSnClient *client);
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientDisconnect(MqttSnClient *client);

MQTT_SN_CLIENT_RETURN_CODE MqttSnClientPing(MqttSnClient *client);
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientRegister(MqttSnClient *client, const char *topic_name, uint16_t topic_name_length);

/**
 * Sends a MqttSnPublish with Quality of Service (qos) -1 to a predefined topic. Works with disconnected MqttSnClients.
 * @param client to use for sending
 * @param predefined_topic_id to publish to
 * @param data to publish
 * @param data_len length of data to publish
 * @return -1 on error, else the count of bytes send
 */
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientPublishPredefinedM1(MqttSnClient *client, uint16_t predefined_topic_id, uint8_t retain,
                                                           uint8_t *data, uint16_t data_len);

/**
 * Registers (if not yet) and publishes message
 * Look up if registered => else registers => then publishes
 */
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientPublishTopicName(MqttSnClient *client, const char *topic_name, uint16_t topic_name_length,
                                                        uint8_t retain, int8_t qos, uint8_t *data, uint16_t data_len);
/**
 * Publishes to predefined topic id with given qos
 */
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientPublishPredefined(MqttSnClient *client, uint16_t predefined_topic_id, uint8_t retain, int8_t qos,
                                                         uint8_t *data, uint16_t data_len);
/**
 * Publishes to topic id with given qos
 */
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientPublishTopicId(MqttSnClient *client, uint16_t topic_id, uint8_t retain, int8_t qos, uint8_t *data,
                                                      uint16_t data_len);

MQTT_SN_CLIENT_RETURN_CODE MqttSnClientPublishByRegistrationIndex(MqttSnClient *client, uint16_t registration_index, uint8_t retain,
                                                                  int8_t qos, uint8_t *data, uint16_t data_len);

MQTT_SN_CLIENT_RETURN_CODE MqttSnClientGetRegisteredTopicId(MqttSnClient *client, const char *topic_name, uint16_t *dst_topic_id);

/**
 * Registers topic_name to gateway
 */

MQTT_SN_CLIENT_RETURN_CODE MqttSnClientSubscribePredefined(MqttSnClient *client, int8_t qos, uint16_t topic_id);
// crashes paho MQTT_SN_CLIENT_RETURN_CODE MqttSnClientSubscribeShort(MqttSnClient *client, int8_t qos, uint16_t topic_id);
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientSubscribeShort(MqttSnClient *client, int8_t qos, uint16_t topic_id);
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientSubscribeTopicName(MqttSnClient *client, int8_t qos, const char *topic_name,
                                                          uint16_t topic_name_length);

MQTT_SN_CLIENT_RETURN_CODE MqttSnClientUnsubscribePredefined(MqttSnClient *client, uint16_t topic_id);
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientUnsubscribeShort(MqttSnClient *client, uint16_t topic_id);
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientUnsubscribeTopicName(MqttSnClient *client, const char *topic_name, uint16_t topic_name_length);

MQTT_SN_CLIENT_RETURN_CODE MqttSnClientSleep(MqttSnClient *client, uint16_t sleep_duration);
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientWakeup(MqttSnClient *client);

#if defined(WITH_MQTT_SN_CLIENT_AUTO_RECONNECT)
void    MqttSnClientEnableAutoReconnect(MqttSnClient *client);
void    MqttSnClientDisableAutoReconnect(MqttSnClient *client);
uint8_t MqttSnClientIsAutoReconnectEnabled(MqttSnClient *client);
#endif
#ifdef WITH_MQTT_SN_CLIENT_WHITELIST
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientAddWhitelistGwId(MqttSnClient *client, uint8_t gw_id);
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientRemoveWhitelistGwId(MqttSnClient *client, uint8_t gw_id);
void                       MqttSnClientClearWhitelistGwId(MqttSnClient *client);
uint8_t                    MqttSnClientWhitelistContainsGwId(MqttSnClient *client, uint8_t gw_id);

MQTT_SN_CLIENT_RETURN_CODE MqttSnClientAddWhitelistNetworkAddress(MqttSnClient *client, device_address address);
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientRemoveWhitelistNetworkAddress(MqttSnClient *client, device_address address);
void                       MqttSnClientClearWhitelistNetworkAddress(MqttSnClient *client);
uint8_t                    MqttSnClientWhitelistContainsNetworkAddress(MqttSnClient *client, device_address address);
#endif
#ifdef WITH_MQTT_SN_CLIENT_BLACKLIST
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientAddBlacklistGwId(MqttSnClient *client, uint8_t gw_id);
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientRemoveBlacklistGwId(MqttSnClient *client, uint8_t gw_id);
void                       MqttSnClientClearBlacklistGwId(MqttSnClient *client);
uint8_t                    MqttSnClientBlacklistContainsGwId(MqttSnClient *client, uint8_t gw_id);

MQTT_SN_CLIENT_RETURN_CODE MqttSnClientAddBlacklistNetworkAddress(MqttSnClient *client, device_address address);
MQTT_SN_CLIENT_RETURN_CODE MqttSnClientRemoveBlacklistNetworkAddress(MqttSnClient *client, device_address address);
void                       MqttSnClientClearBlacklistNetworkAddress(MqttSnClient *client);
uint8_t                    MqttSnClientBlacklistContainsNetworkAddress(MqttSnClient *client, device_address address);
#endif
#ifdef __cplusplus
}
#endif
#endif  //CMQTTSNFORWARDER_CLIENT_MQTTSNCLIENT_H_
