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
#include "common/discoveredgw/MqttSnClientDiscoveredGateway.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE_ {
    MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE_ERROR   = -1,
    MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE_SUCCESS = 0
} MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE;

typedef struct MqttSnFindGatewayMonitor_ {
    MqttSnGatewayNetworkInterface *gatewayNetwork;
    MqttSnDiscoveredGatewayList    gatewayList;
    void *                         gatewayNetworkContext;
    int32_t                        gatewayNetworkSendTimeout;
    int32_t                        gatewayNetworkReceiveTimeout;
    MqttSnLogger *                 logger;
} MqttSnFindGatewayMonitor;

/**
 * Sets only values - does not connect or initializes any
 * @param client
 * @param gatewayNetwork
 * @param sendTimoeut
 * @param gatewayNetworkReceiveTimeout
 * @param logger
 * @return
 */
MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE MqttSnFindGatewayClientInit(MqttSnFindGatewayMonitor *client, MqttSnLogger *logger,
                                                                     MqttSnGatewayNetworkInterface *gatewayNetwork,
                                                                     void *gatewayNetworkContext, int32_t gatewayNetworkSendTimeout,
                                                                     int32_t gatewayNetworkReceiveTimeout);

/**
 * Deinitializes internal datastructures - does not change networks or anythings
 */
void MqttSnFindGatewayClientDeinit(MqttSnFindGatewayMonitor *client);

MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE MqttSnFindGatewayAwaitAdvertise(MqttSnFindGatewayMonitor *client, int32_t timeout, void *context,
                                                                         int32_t (*adv_cb)(const MqttSnFindGatewayMonitor *, const int32_t,
                                                                                           const MqttSnReceivedAdvertise *, void *));

MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE
MqttSnFindGatewaySearchGw(MqttSnFindGatewayMonitor *client, int32_t timeout, uint8_t radius, void *context,
                          int32_t (*gwinfo_cb)(const MqttSnFindGatewayMonitor *, const int32_t, const MqttSnReceivedGwInfo *, void *),
                          int32_t (*adv_cb)(const MqttSnFindGatewayMonitor *, const int32_t, const MqttSnReceivedAdvertise *, void *));

#ifdef __cplusplus
}
#endif
#endif  //CMQTTSNFORWARDER_CLIENT_MQTTSNFINDGATEWAYCLIENT_H_
