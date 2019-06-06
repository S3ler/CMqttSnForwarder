//
// Created by SomeDude on 03.06.2019.
//

#ifndef CMQTTSNFORWARDER_CLIENT_MQTTSNFINDGATEWAYCLIENT_H_
#define CMQTTSNFORWARDER_CLIENT_MQTTSNFINDGATEWAYCLIENT_H_

#include <stdint.h>
#include <platform/device_address.h>
#include <parser/MqttSnAdvertiseMessage.h>
#include <parser/MqttSnGwInfoMessage.h>
#include <network/MqttSnGatewayNetworkInterface.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MQTT_SN_FIND_GATEWAY_CLIENT_DISCOVERED_GATEWAY_LIST_LENGTH
#define MQTT_SN_FIND_GATEWAY_CLIENT_DISCOVERED_GATEWAY_LIST_LENGTH 5
#endif

typedef enum MqttSnDiscoveredGatewayState_ {
  MqttSnDiscoveredGatewayStateInactive = 0,
  MqttSnDiscoveredGatewayStateActive = 1
} MqttSnDiscoveredGatewayState;

typedef struct MqttSnDiscoveredGateway_ {
  uint8_t gw_id;
  uint16_t duration;
  uint16_t duration_timeout;
  device_address network_address;
  MqttSnDiscoveredGatewayState state;
} MqttSnDiscoveredGateway;

typedef struct MqttSnDiscoveredGatewayList_ {
  MqttSnDiscoveredGateway gateways[MQTT_SN_FIND_GATEWAY_CLIENT_DISCOVERED_GATEWAY_LIST_LENGTH];
  uint8_t max_len;
  uint8_t current_len;
} MqttSnDiscoveredGatewayList;

int32_t MqttSnDiscoveredGatewayListInit(MqttSnDiscoveredGatewayList *list);
int32_t MqttSnFindGatewayClientUpdateDiscoveredGatewayList(MqttSnDiscoveredGatewayList *list,
                                                           const device_address *from,
                                                           const MqttSnAdvertise *advertise,
                                                           const MqttSnGwInfo *gw_info);
int32_t MqttSnFindGatewayClientUpdateDiscoveredGatewayDurationTimeouts(MqttSnDiscoveredGatewayList *list,
                                                                       uint16_t passed_time);

typedef struct MqttSnReceivedAdvertise_ {
  device_address from;
  int32_t timeout;
  MqttSnAdvertise advertise;
} MqttSnReceivedAdvertise;

// if gw_info.address == from then its from a GW else from a client
typedef struct MqttSnReceivedGwInfo_ {
  device_address from;
  int32_t timeout;
  uint8_t radius;
  MqttSnGwInfo gw_info;
} MqttSnReceivedGwInfo;

typedef struct MqttSnFindGatewayClient_ {
  MqttSnGatewayNetworkInterface *gatewayNetwork;
  MqttSnDiscoveredGatewayList gatewayList;
  void *gatewayNetworkContext;
  int32_t gatewayNetworkSendTimeout;
  int32_t gatewayNetworkReceiveTimeout;
  MqttSnLogger *logger;
} MqttSnFindGatewayClient;

/**
 * Sets only values - does not connect or initializes any
 * @param client
 * @param gatewayNetwork
 * @param sendTimoeut
 * @param gatewayNetworkReceiveTimeout
 * @param logger
 * @return
 */
int32_t MqttSnFindGatewayClientInit(MqttSnFindGatewayClient *client,
                                    MqttSnLogger *logger,
                                    MqttSnGatewayNetworkInterface *gatewayNetwork,
                                    void *gatewayNetworkContext,
                                    int32_t gatewayNetworkSendTimeout,
                                    int32_t gatewayNetworkReceiveTimeout);

/**
 * Deinitializes internal datastructures - does not change networks or anythings
 * @param client
 * @return
 */
int32_t MqttSnFindGatewayClientDeinit(MqttSnFindGatewayClient *client);

int32_t MqttSnClientAwaitAdvertise(MqttSnFindGatewayClient *client,
                                   int32_t timeout,
                                   void *context,
                                   int32_t (*adv_cb)(const MqttSnFindGatewayClient *,
                                                     const int32_t,
                                                     const MqttSnReceivedAdvertise *,
                                                     void *));

int32_t MqttSnClientSearchGw(MqttSnFindGatewayClient *client,
                             int32_t timeout,
                             uint8_t radius,
                             void *context,
                             int32_t (*gwinfo_cb)(const MqttSnFindGatewayClient *,
                                                  const int32_t,
                                                  const MqttSnReceivedGwInfo *,
                                                  void *),
                             int32_t (*adv_cb)(const MqttSnFindGatewayClient *,
                                               const int32_t,
                                               const MqttSnReceivedAdvertise *,
                                               void *));

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_CLIENT_MQTTSNFINDGATEWAYCLIENT_H_
