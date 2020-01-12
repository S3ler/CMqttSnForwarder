//
// Created by SomeDude on 03.09.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNCLIENTDISCOVEREDGATEWAY_H
#define CMQTTSNFORWARDER_MQTTSNCLIENTDISCOVEREDGATEWAY_H
#include <stdint.h>
#include <platform/device_address.h>
#include <parser/MqttSnAdvertiseMessage.h>
#include <parser/MqttSnGwInfoMessage.h>
#ifdef __cplusplus
extern "C" {
#endif

#ifndef MQTT_SN_FIND_GATEWAY_CLIENT_DISCOVERED_GATEWAY_LIST_LENGTH
#define MQTT_SN_FIND_GATEWAY_CLIENT_DISCOVERED_GATEWAY_LIST_LENGTH 255
#endif

typedef enum MqttSnDiscoveredGatewayState_ {
    MqttSnDiscoveredGatewayStateInactive = 0,
    MqttSnDiscoveredGatewayStateActive   = 1
} MqttSnDiscoveredGatewayState;

typedef struct MqttSnDiscoveredGateway_ {
    uint8_t                      gw_id;
    uint16_t                     duration;
    device_address               network_address;
    uint8_t                      signal_strength;
    MqttSnDiscoveredGatewayState state;
    uint64_t                     last_message_s;
} MqttSnDiscoveredGateway;

typedef struct MqttSnDiscoveredGatewayList_ {
    MqttSnDiscoveredGateway gateways[MQTT_SN_FIND_GATEWAY_CLIENT_DISCOVERED_GATEWAY_LIST_LENGTH];
    uint8_t                 max_len;
    uint8_t                 current_len;
} MqttSnDiscoveredGatewayList;

typedef struct MqttSnReceivedAdvertise_ {
    device_address  from;
    int32_t         timeout;
    MqttSnAdvertise advertise;
} MqttSnReceivedAdvertise;

typedef struct MqttSnReceivedGwInfo_ {
    device_address from;
    int32_t        timeout;
    uint8_t        radius;
    MqttSnGwInfo   gw_info;
} MqttSnReceivedGwInfo;

int32_t MqttSnDiscoveredGatewayListInit(MqttSnDiscoveredGatewayList *list);
int32_t MqttSnDiscoveredGatewayListRemoveInactive(MqttSnDiscoveredGatewayList *list);
void    MqttSnDiscoveredGatewayListUpdateStates(MqttSnDiscoveredGatewayList *list, uint64_t current_time_s);
int32_t MqttSnDiscoveredGatewayListUpdate(MqttSnDiscoveredGatewayList *list, const device_address *from, uint8_t signal_strength,
                                          const MqttSnAdvertise *advertise, const MqttSnGwInfo *gw_info, uint64_t current_time_s);
uint8_t MqttSnDiscoveredGatewayListExists(MqttSnDiscoveredGatewayList *list, const device_address *address, int32_t gw_id);

#ifdef __cplusplus
}
#endif
#endif  //CMQTTSNFORWARDER_MQTTSNCLIENTDISCOVEREDGATEWAY_H
