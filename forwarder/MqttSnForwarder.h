//
// Created by bele on 25.01.19.
//

#ifndef CMQTTSNFORWARDER_MQTTSNFORWARDER_H
#define CMQTTSNFORWARDER_MQTTSNFORWARDER_H

#include "network/MqttSnGatewayNetworkInterface.h"
#include "network/MqttSnClientNetworkInterface.h"
#include <stdint.h>
#include <common/discoveredgw/MqttSnClientDiscoveredGateway.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef WITHOUT_MQTT_SN_CLIENT_NETWORK_BLACKLIST
#define WITH_MQTT_SN_CLIENT_NETWORK_BLACKLIST
#endif

#ifndef WITHOUT_ADVERTISE_GATEWAY_DISCOVERED_LIST
#define WITH_ADVERTISE_GATEWAY_DISCOVERED_LIST
#endif

#ifndef WITHOUT_ADVERTISE_GATEWAY_DISCOVERED_LIST
#define WITH_ADVERTISE_GATEWAY_DISCOVERED_LIST
#endif

#ifndef WITHOUT_GWINFO_GATEWAY_DISCOVERED_LIST
#define WITH_GWINFO_GATEWAY_DISCOVERED_LIST
#endif

//#define WITHOUT_MQTT_SN_FORWARDER_GATEWAY_NETWORK_WHITELIST
//#define WITHOUT_MQTT_SN_FORWARDER_GATEWAY_NETWORK_BLACKLIST

//#define WITHOUT_MQTT_SN_FORWARDER_CLIENT_NETWORK_WHITELIST
//#define WITHOUT_MQTT_SN_FORWARDER_CLIENT_NETWORK_BLACKLIST

#ifndef WITHOUT_MQTT_SN_FORWARDER_GATEWAY_NETWORK_WHITELIST
#define WITH_MQTT_SN_FORWARDER_GATEWAY_NETWORK_WHITELIST
#endif
#ifdef WITH_MQTT_SN_FORWARDER_GATEWAY_NETWORK_WHITELIST
#define MQTT_SN_FORWARDER_GATEWAY_NETWORK_GW_ID_WHITELIST_LENGTH 5
#define MQTT_SN_FORWARDER_GATEWAY_ADDRESS_WHITELIST_LENGTH 5
#endif

#ifndef WITHOUT_MQTT_SN_FORWARDER_GATEWAY_NETWORK_BLACKLIST
#define WITH_MQTT_SN_FORWARDER_GATEWAY_NETWORK_BLACKLIST
#endif
#ifdef WITH_MQTT_SN_FORWARDER_GATEWAY_NETWORK_BLACKLIST
#define MQTT_SN_FORWARDER_GATEWAY_NETWORK_GW_ID_BLACKLIST_LENGTH 5
#define MQTT_SN_FORWARDER_GATEWAY_ADDRESS_BLACKLIST_LENGTH 5
#endif

#ifndef WITHOUT_MQTT_SN_FORWARDER_CLIENT_NETWORK_WHITELIST
#define WITH_MQTT_SN_FORWARDER_CLIENT_NETWORK_WHITELIST
#endif
#ifdef WITH_MQTT_SN_FORWARDER_CLIENT_NETWORK_WHITELIST
#define MQTT_SN_FORWARDER_CLIENT_ADDRESS_WHITELIST_LENGTH 5
#endif

#ifndef WITHOUT_MQTT_SN_FORWARDER_CLIENT_NETWORK_BLACKLIST
#define WITH_MQTT_SN_FORWARDER_CLIENT_NETWORK_BLACKLIST
#endif
#ifdef WITH_MQTT_SN_FORWARDER_CLIENT_NETWORK_BLACKLIST
#define MQTT_SN_FORWARDER_CLIENT_ADDRESS_BLACKLIST_LENGTH 5
#endif

typedef struct MqttSnForwarder_ {
#if defined(WITH_ADVERTISE_GATEWAY_DISCOVERED_LIST) || defined(WITH_GWINFO_GATEWAY_DISCOVERED_LIST)
    MqttSnDiscoveredGatewayList discovered_gw_list;
#endif

    uint8_t        disconnected;
    device_address mqtt_sn_gateway_address;
    uint8_t        pin_connection;

    MqttSnClientNetworkInterface  clientNetwork;
    MqttSnGatewayNetworkInterface gatewayNetwork;

    MqttSnFixedSizeRingBuffer clientNetworkReceiveBuffer;
    MqttSnFixedSizeRingBuffer clientNetworkSendBuffer;
    MqttSnFixedSizeRingBuffer gatewayNetworkReceiveBuffer;
    MqttSnFixedSizeRingBuffer gatewayNetworkSendBuffer;

    int clientNetworkSendTimeout;
    int clientNetworkReceiveTimeout;
    int gatewayNetworkSendTimeout;
    int gatewayNetworkReceiveTimeout;

#ifdef WITH_LOGGING
    MqttSnLogger logger;
#endif
#ifdef WITH_MQTT_SN_FORWARDER_GATEWAY_NETWORK_WHITELIST
    uint8_t  gn_gw_id_whitelist[MQTT_SN_FORWARDER_GATEWAY_NETWORK_GW_ID_WHITELIST_LENGTH];
    uint16_t gn_gw_id_whitelist_len;

    device_address gn_addr_whitelist[MQTT_SN_FORWARDER_GATEWAY_ADDRESS_WHITELIST_LENGTH];
    uint16_t       gn_addr_whitelist_len;
#endif
#ifdef WITH_MQTT_SN_FORWARDER_GATEWAY_NETWORK_BLACKLIST
    uint8_t  gn_gw_id_blacklist[MQTT_SN_FORWARDER_GATEWAY_NETWORK_GW_ID_BLACKLIST_LENGTH];
    uint16_t gn_gw_id_blacklist_len;

    device_address gn_addr_blacklist[MQTT_SN_FORWARDER_GATEWAY_ADDRESS_BLACKLIST_LENGTH];
    uint16_t       gn_addr_blacklist_len;
#endif
#ifdef WITH_MQTT_SN_FORWARDER_CLIENT_NETWORK_WHITELIST
    device_address cn_addr_whitelist[MQTT_SN_FORWARDER_CLIENT_ADDRESS_WHITELIST_LENGTH];
    uint16_t       cn_addr_whitelist_len;
#endif
#ifdef WITH_MQTT_SN_FORWARDER_CLIENT_NETWORK_BLACKLIST
    device_address cn_addr_blacklist[MQTT_SN_FORWARDER_CLIENT_ADDRESS_BLACKLIST_LENGTH];
    uint16_t       cn_addr_blacklist_len;
#endif
} MqttSnForwarder;

int MqttSnForwarderInit(MqttSnForwarder *mqttSnForwarder, const MqttSnLogger *logger, const device_address *mqtt_sn_gateway_address);

int MqttSnForwarderLoop(MqttSnForwarder *);

void MqttSnForwarderDeinit(MqttSnForwarder *);

int AddForwardingHeaderToClientMessages(MqttSnForwarder *forwarder, MqttSnMessageData *clientMessageData,
                                        MqttSnMessageData *gatewayMessageData);

int RemoveForwardingHeaderFromGatewayMessages(MqttSnForwarder *forwarder, MqttSnMessageData *gatewayMessageData,
                                              MqttSnMessageData *clientMessageData);

int32_t remove_mqtt_sn_forwarder_encapsulation_frame(MqttSnMessageData *gatewayMessageData, MqttSnMessageData *clientMessageData,
                                                     const device_address *client_network_broadcast_address, const MqttSnLogger *logger);

int32_t add_mqtt_sn_forwarder_encapsulation_frame(MqttSnMessageData *clientMessageData, MqttSnMessageData *gatewayMessageData,
                                                  const device_address *mqtt_sn_gateway_network_address,
                                                  const device_address *client_network_broadcast_address, const MqttSnLogger *logger);

void sendBufferedMessagesToGateway(MqttSnForwarder *forwarder);

void sendBufferedMessagesToClients(MqttSnForwarder *forwarder);

int AddMqttSnForwardingHeader(MqttSnMessageData *clientMessageData, MqttSnMessageData *gatewayMessageData,
                              const device_address *mqtt_sn_gateway_network_address,
                              const device_address *client_network_broadcast_address);

int  RemoveMqttSnForwardingHeader(MqttSnMessageData *gatewayMessageData, MqttSnMessageData *clientMessageData,
                                  const device_address *client_network_broadcast_address);
void MqttSnForwarderSetPinConnection(MqttSnForwarder *forwarder, uint8_t pin_connection);
void MqttSnForwarderSetConnected(MqttSnForwarder *forwarder, const device_address *mqtt_sn_gateway_address);
void MqttSnForwarderSetDisconnected(MqttSnForwarder *forwarder);

#if defined(WITH_ADVERTISE_GATEWAY_DISCOVERED_LIST) || defined(WITH_GWINFO_GATEWAY_DISCOVERED_LIST)
int MqttSnForwarderHandleGatewayPseudoConnect(MqttSnForwarder *forwarder);
int MqttSnForwardingUpdateDiscoveredGw(MqttSnForwarder *forwarder, MqttSnMessageData *msg);
#endif

#ifdef WITH_MQTT_SN_FORWARDER_GATEWAY_NETWORK_WHITELIST
int32_t MqttSnForwarderAddGatewayNetworkWhitelistGwId(MqttSnForwarder *forwarder, uint8_t gw_id);
void    MqttSnForwarderClearGatewayNetworkWhitelistGwId(MqttSnForwarder *forwarder);
int32_t MqttSnForwarderContainsGatewayNetworkWhitelistGwId(MqttSnForwarder *forwarder, uint8_t gw_id);

int32_t MqttSnForwarderAddGatewayNetworkWhitelistAddr(MqttSnForwarder *forwarder, const device_address *address);
void    MqttSnForwarderClearGatewayNetworkWhitelistAddr(MqttSnForwarder *forwarder);
int32_t MqttSnForwarderContainsGatewayNetworkWhitelistAddr(MqttSnForwarder *forwarder, const device_address *address);
#endif
#ifdef WITH_MQTT_SN_FORWARDER_GATEWAY_NETWORK_BLACKLIST
int32_t MqttSnForwarderAddGatewayNetworkBlacklistGwId(MqttSnForwarder *forwarder, uint8_t gw_id);
void    MqttSnForwarderClearGatewayNetworkBlacklistGwId(MqttSnForwarder *forwarder);
int32_t MqttSnForwarderContainsGatewayNetworkBlacklistGwId(MqttSnForwarder *forwarder, uint8_t gw_id);

int32_t MqttSnForwarderAddGatewayNetworkBlacklistAddr(MqttSnForwarder *forwarder, const device_address *address);
void    MqttSnForwarderClearGatewayNetworkBlacklistAddr(MqttSnForwarder *forwarder);
int32_t MqttSnForwarderContainsGatewayNetworkBlacklistAddr(MqttSnForwarder *forwarder, const device_address *address);
#endif
#ifdef WITH_MQTT_SN_FORWARDER_CLIENT_NETWORK_WHITELIST
int32_t MqttSnForwarderAddClientNetworkWhitelistAddr(MqttSnForwarder *forwarder, const device_address *address);
void    MqttSnForwarderClearClientNetworkWhitelistAddr(MqttSnForwarder *forwarder);
int32_t MqttSnForwarderContainsClientNetworkWhitelistAddr(MqttSnForwarder *forwarder, const device_address *address);
#endif
#ifdef WITH_MQTT_SN_FORWARDER_CLIENT_NETWORK_BLACKLIST
int32_t MqttSnForwarderAddClientNetworkBlacklistAddr(MqttSnForwarder *forwarder, const device_address *address);
void MqttSnForwarderClearClientNetworkBlacklistAddr(MqttSnForwarder *forwarder);
int32_t MqttSnForwarderContainsClientNetworkBlacklistAddr(MqttSnForwarder *forwarder, const device_address *address);
#endif
#ifdef __cplusplus
}
#endif
#endif  //CMQTTSNFORWARDER_MQTTSNFORWARDER_H
