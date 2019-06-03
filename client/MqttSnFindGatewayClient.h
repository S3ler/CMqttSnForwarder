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
  void *gatewayNetworkContext;
  int32_t gatewayNetworkSendTimeout;
  int32_t gatewayNetworkReceiveTimeout;
  MqttSnLogger* logger;
} MqttSnFindGatewayClient;

int32_t MqttSnClientAwaitAdvertiseCb(MqttSnFindGatewayClient *client,
                                     int32_t timeout,
                                     int32_t (*adv_cb)(const MqttSnFindGatewayClient *,
                                                       const int32_t,
                                                       const MqttSnReceivedAdvertise *));

int32_t MqttSnClientSearchGw(MqttSnFindGatewayClient *client,
                             int32_t timeout,
                             uint8_t radius,
                             int32_t (*gwinfo_cb)(const MqttSnFindGatewayClient *client,
                                                  const int32_t timeout_left,
                                                  const MqttSnReceivedGwInfo *radius),
                             int32_t (*adv_cb)(const MqttSnFindGatewayClient *client,
                                               const int32_t timeout_left,
                                               const MqttSnReceivedAdvertise *));

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_CLIENT_MQTTSNFINDGATEWAYCLIENT_H_
