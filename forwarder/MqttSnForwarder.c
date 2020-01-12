//
// Created by bele on 26.01.19.
//

#include "MqttSnForwarder.h"
#include "ringbuffer/MqttSnFixedSizeRingBuffer.h"
#include "network/MqttSnClientNetworkInterface.h"
#include "MqttSnForwarderLogging.h"
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <platform/platform_compatibility.h>
#include <logging/MqttSnLogging.h>
#include <parser/logging/MqttSnForwarderLoggingMessages.h>
#include <parser/MqttSnForwarderEncapsulationMessage.h>
#include <parser/logging/MqttSnMessageParserLogging.h>

int MqttSnForwarderInit(MqttSnForwarder *mqttSnForwarder, const MqttSnLogger *logger, const device_address *mqtt_sn_gateway_address) {
#ifdef WITH_LOGGING
    mqttSnForwarder->logger = *logger;
#endif
#if defined(WITH_ADVERTISE_GATEWAY_DISCOVERED_LIST) || defined(WITH_GWINFO_GATEWAY_DISCOVERED_LIST)
    if (MqttSnDiscoveredGatewayListInit(&mqttSnForwarder->discovered_gw_list) < 0) {
        return -1;
    }
#endif

    mqttSnForwarder->gatewayNetwork.logger = &mqttSnForwarder->logger;
    mqttSnForwarder->clientNetwork.logger  = &mqttSnForwarder->logger;

    MqttSnFixedSizeRingBufferInit(&mqttSnForwarder->clientNetworkReceiveBuffer);
    MqttSnFixedSizeRingBufferInit(&mqttSnForwarder->clientNetworkSendBuffer);

    MqttSnFixedSizeRingBufferInit(&mqttSnForwarder->gatewayNetworkReceiveBuffer);
    MqttSnFixedSizeRingBufferInit(&mqttSnForwarder->gatewayNetworkSendBuffer);

    if (ClientNetworkConnect(&mqttSnForwarder->clientNetwork, mqttSnForwarder->clientNetwork.context) != 0) {
        MqttSnForwarderDeinit(mqttSnForwarder);
        return -1;
    }

    if (GatewayNetworkConnect(&mqttSnForwarder->gatewayNetwork, mqttSnForwarder->gatewayNetwork.context) != 0) {
        MqttSnForwarderDeinit(mqttSnForwarder);
        return -1;
    }

#ifdef WITH_LOGGING
    if (log_status(&mqttSnForwarder->logger)) {
        MqttSnForwarderDeinit(mqttSnForwarder);
        return -1;
    }
#endif

    if (mqtt_sn_gateway_address) {
        MqttSnForwarderSetConnected(mqttSnForwarder, mqtt_sn_gateway_address);
    } else {
        MqttSnForwarderSetDisconnected(mqttSnForwarder);
    }

    return 0;
}

/**
 * disconnects the gateway and client network
 * @param forwarder
 */
void MqttSnForwarderDeinit(MqttSnForwarder *forwarder) {
    GatewayNetworkDisconnect(&forwarder->gatewayNetwork, forwarder->gatewayNetwork.context);
    ClientNetworkDisconnect(&forwarder->clientNetwork, forwarder->clientNetwork.context);
}

int MqttSnForwarderLoop(MqttSnForwarder *forwarder) {
#if defined(WITH_ADVERTISE_GATEWAY_DISCOVERED_LIST) || defined(WITH_GWINFO_GATEWAY_DISCOVERED_LIST)
    if (forwarder->disconnected) {
        if (!forwarder->pin_connection) {
            return MqttSnForwarderHandleGatewayPseudoConnect(forwarder);
        }
    } else {
        uint64_t current_time_s = 0;
        if (get_timestamp_s(&current_time_s) < 0) {
            return -1;
        }
        MqttSnDiscoveredGatewayListUpdate(&forwarder->discovered_gw_list, NULL, 0, NULL, NULL, current_time_s);
        if (!MqttSnDiscoveredGatewayListExists(&forwarder->discovered_gw_list, &forwarder->mqtt_sn_gateway_address, -1))
            MqttSnForwarderSetDisconnected(forwarder);
        if (forwarder->disconnected) {
            sendBufferedMessagesToClients(forwarder);
            return MqttSnForwarderHandleGatewayPseudoConnect(forwarder);
        }
    }
#endif

    if (ClientNetworkReceive(&forwarder->clientNetwork, &forwarder->clientNetworkReceiveBuffer, forwarder->clientNetworkReceiveTimeout,
                             forwarder->clientNetwork.context) < 0) {
        ClientNetworkDisconnect(&forwarder->clientNetwork, forwarder->clientNetwork.context);
    }
#ifdef Arduino_h
    yield();
#endif
    {
        MqttSnMessageData clientMessageData  = { 0 };
        MqttSnMessageData gatewayMessageData = { 0 };
        if (AddForwardingHeaderToClientMessages(forwarder, &clientMessageData, &gatewayMessageData) != 0) {
            ClientNetworkDisconnect(&forwarder->clientNetwork, forwarder->clientNetwork.context);
            GatewayNetworkDisconnect(&forwarder->gatewayNetwork, forwarder->gatewayNetwork.context);
        }
    }
#ifdef Arduino_h
    yield();
#endif
    if (GatewayNetworkSend(&forwarder->gatewayNetwork, &forwarder->gatewayNetworkSendBuffer, forwarder->gatewayNetworkSendTimeout,
                           forwarder->gatewayNetwork.context) < 0) {
        GatewayNetworkDisconnect(&forwarder->gatewayNetwork, forwarder->gatewayNetwork.context);
    }
#ifdef Arduino_h
    yield();
#endif
    if (GatewayNetworkReceive(&forwarder->gatewayNetwork, &forwarder->gatewayNetworkReceiveBuffer, forwarder->gatewayNetworkReceiveTimeout,
                              forwarder->gatewayNetwork.context) < 0) {
        GatewayNetworkDisconnect(&forwarder->gatewayNetwork, forwarder->gatewayNetwork.context);
    }
#ifdef Arduino_h
    yield();
#endif
    {
        MqttSnMessageData gatewayMessageData = { 0 };
        MqttSnMessageData clientMessageData  = { 0 };
        if (RemoveForwardingHeaderFromGatewayMessages(forwarder, &gatewayMessageData, &clientMessageData) != 0) {
            ClientNetworkDisconnect(&forwarder->clientNetwork, forwarder->clientNetwork.context);
            GatewayNetworkDisconnect(&forwarder->gatewayNetwork, forwarder->gatewayNetwork.context);
        }
    }
#ifdef Arduino_h
    yield();
#endif
    if (ClientNetworkSend(&forwarder->clientNetwork, &forwarder->clientNetworkSendBuffer, forwarder->clientNetworkSendTimeout,
                          forwarder->clientNetwork.context) < 0) {
        ClientNetworkDisconnect(&forwarder->clientNetwork, forwarder->clientNetwork.context);
    }
#ifdef Arduino_h
    yield();
#endif
    if (forwarder->clientNetwork.status <= 0) {
        if (forwarder->gatewayNetwork.status <= 0) {
            // both networks down => end
#ifdef WITH_LOGGING
            log_mqtt_sn_forwarder_client_and_gateway_network_connection_lost(&forwarder->logger);
#endif
            MqttSnForwarderDeinit(forwarder);
            return -1;
        }
        // only client network is down
#ifdef WITH_LOGGING
        log_mqtt_sn_forwarder_client_network_connection_lost(&forwarder->logger);
#endif
        // send out buffered MqttSnMessageData to Gateway
        sendBufferedMessagesToGateway(forwarder);
        MqttSnForwarderDeinit(forwarder);
        return -1;
    }

    if (forwarder->gatewayNetwork.status <= 0) {
        if (forwarder->clientNetwork.status <= 0) {
            // both networks down => end
#ifdef WITH_LOGGING
            log_mqtt_sn_forwarder_client_and_gateway_network_connection_lost(&forwarder->logger);
#endif
            MqttSnForwarderDeinit(forwarder);
            return -1;
        }
        // only gateway network is down
#ifdef WITH_LOGGING
        log_mqtt_sn_forwarder_gateway_network_connection_lost(&forwarder->logger);
#endif
        // send out buffered MqttSnMessageData to Client
        sendBufferedMessagesToClients(forwarder);
        MqttSnForwarderDeinit(forwarder);
        return -1;
    }

    return 0;
}

void sendBufferedMessagesToClients(MqttSnForwarder *forwarder) {
    // FEATURE add Timeout or starvation detection to the loop
#ifdef WITH_LOGGING
    log_mqtt_sn_forwarder_sending_buffer_gateway_messages(&forwarder->logger);
#endif
    while (!isEmpty(&forwarder->gatewayNetworkReceiveBuffer) || !isEmpty(&forwarder->clientNetworkSendBuffer)) {
        {
            MqttSnMessageData gatewayMessageData = { 0 };
            MqttSnMessageData clientMessageData  = { 0 };
            if (RemoveForwardingHeaderFromGatewayMessages(forwarder, &gatewayMessageData, &clientMessageData) < 0) {
                break;
            }
        }
        if (ClientNetworkSend(&forwarder->clientNetwork, &forwarder->clientNetworkSendBuffer, forwarder->clientNetworkSendTimeout,
                              forwarder->clientNetwork.context) < 0) {
#ifdef WITH_LOGGING
            log_mqtt_sn_forwarder_client_and_gateway_network_connection_lost(&forwarder->logger);
#endif
            break;
        }
    }
#ifdef WITH_DEBUG_LOGGING
    log_mqtt_sn_forwarder_buffer_gateway_messages_send(&forwarder->logger);
#endif
}

void sendBufferedMessagesToGateway(MqttSnForwarder *forwarder) {
    // FEATURE add Timeout or starvation detection to the loop
#ifdef WITH_DEBUG_LOGGING
    log_mqtt_sn_forwarder_sending_buffer_client_messages(&forwarder->logger);
#endif
    while (!isEmpty(&forwarder->clientNetworkReceiveBuffer) || !isEmpty(&forwarder->gatewayNetworkSendBuffer)) {
        {
            MqttSnMessageData gatewayMessageData = { 0 };
            MqttSnMessageData clientMessageData  = { 0 };
            if (AddForwardingHeaderToClientMessages(forwarder, &clientMessageData, &gatewayMessageData) != 0) {
                break;
            }
        }
        if (GatewayNetworkSend(&forwarder->gatewayNetwork, &forwarder->gatewayNetworkSendBuffer, forwarder->gatewayNetworkSendTimeout,
                               forwarder->gatewayNetwork.context) < 0) {
#ifdef WITH_LOGGING
            log_mqtt_sn_forwarder_gateway_network_connection_lost(&forwarder->logger);
#endif
            break;
        }
    }
#ifdef WITH_DEBUG_LOGGING
    log_mqtt_sn_forwarder_buffer_client_messages_send(&forwarder->logger);
#endif
}

int RemoveForwardingHeaderFromGatewayMessages(MqttSnForwarder *forwarder, MqttSnMessageData *gatewayMessageData,
                                              MqttSnMessageData *clientMessageData) {
    memset(gatewayMessageData, 0, sizeof(MqttSnMessageData));
    memset(clientMessageData, 0, sizeof(MqttSnMessageData));

    if (isFull(&forwarder->clientNetworkSendBuffer)) {
        return 0;
    }

    if (pop(&forwarder->gatewayNetworkReceiveBuffer, gatewayMessageData) != 0) {
        return 0;
    }

#ifdef WITH_MQTT_SN_FORWARDER_GATEWAY_NETWORK_WHITELIST
    if (forwarder->gn_addr_whitelist_len != 0) {
        if (!MqttSnForwarderContainsGatewayNetworkWhitelistAddr(forwarder, &gatewayMessageData->from)) {
            return 0;
        }
    }
#endif
#ifdef WITH_MQTT_SN_FORWARDER_GATEWAY_NETWORK_BLACKLIST
    if (MqttSnForwarderContainsGatewayNetworkBlacklistAddr(forwarder, &gatewayMessageData->from)) {
        return 0;
    }
#endif

#if defined(WITH_ADVERTISE_GATEWAY_DISCOVERED_LIST) || defined(WITH_GWINFO_GATEWAY_DISCOVERED_LIST)
    if (MqttSnForwardingUpdateDiscoveredGw(forwarder, gatewayMessageData) < 0) {
        return -1;
    }
#endif

    if (remove_mqtt_sn_forwarder_encapsulation_frame(gatewayMessageData, clientMessageData,
                                                     forwarder->clientNetwork.client_network_broadcast_address, &forwarder->logger) != 0) {
#ifdef WITH_LOGGING
        log_gateway_mqtt_sn_message_malformed(&forwarder->logger, gatewayMessageData);
#endif
        return 0;
    }

#ifdef WITH_LOGGING
    if (log_gateway_mqtt_sn_message(&forwarder->logger, &clientMessageData->to, clientMessageData->data, clientMessageData->data_length,
                                    NULL)) {
        return -1;
    }
#endif

    if (put(&forwarder->clientNetworkSendBuffer, clientMessageData) != 0) {
        // we do our best to not drop message due to RAM loss
        put(&forwarder->gatewayNetworkReceiveBuffer, gatewayMessageData);
    }
    return 0;
}

int AddForwardingHeaderToClientMessages(MqttSnForwarder *forwarder, MqttSnMessageData *clientMessageData,
                                        MqttSnMessageData *gatewayMessageData) {
    memset(clientMessageData, 0, sizeof(MqttSnMessageData));
    memset(gatewayMessageData, 0, sizeof(MqttSnMessageData));

    if (isFull(&forwarder->gatewayNetworkSendBuffer)) {
        return 0;
    }

    if (pop(&forwarder->clientNetworkReceiveBuffer, clientMessageData) != 0) {
        return 0;
    }

#ifdef WITH_MQTT_SN_FORWARDER_CLIENT_NETWORK_WHITELIST
    if (forwarder->cn_addr_whitelist_len != 0) {
        if (!MqttSnForwarderContainsClientNetworkWhitelistAddr(forwarder, &clientMessageData->from)) {
            return 0;
        }
    }
#endif
#ifdef WITH_MQTT_SN_FORWARDER_CLIENT_NETWORK_BLACKLIST
    if (MqttSnForwarderContainsClientNetworkBlacklistAddr(forwarder, &clientMessageData->from)) {
        return 0;
    }
#endif

#ifdef WITH_LOGGING
    log_client_mqtt_sn_message(&forwarder->logger, &clientMessageData->from, clientMessageData->data, clientMessageData->data_length, NULL);
#endif
#ifdef WITH_DEBUG_LOGGING
    ParsedMqttSnHeader header = { 0 };
    if (parse_message_tolerant(&header, ANY_MESSAGE_TYPE, clientMessageData->data, clientMessageData->data_length) < 0) {
#ifdef WITH_LOGGING
        log_malformed_message(&forwarder->logger, clientMessageData);
#endif
    }
#endif
    if (add_mqtt_sn_forwarder_encapsulation_frame(clientMessageData, gatewayMessageData, &forwarder->mqtt_sn_gateway_address,
                                                  forwarder->clientNetwork.client_network_broadcast_address, NULL) != 0) {
#ifdef WITH_DEBUG_LOGGING
        log_could_not_generate_encapsulation_message(&forwarder->logger, MQTT_SN_FORWARDER_NETWORK_GATEWAY, &clientMessageData->from,
                                                     clientMessageData->data, clientMessageData->data_length,
                                                     clientMessageData->signal_strength);
#endif
        return 0;
    }
    if (put(&forwarder->gatewayNetworkSendBuffer, gatewayMessageData) != 0) {
        // we do our best to not drop message due to RAM loss
        put(&forwarder->clientNetworkReceiveBuffer, clientMessageData);
    }
    return 0;
}

int32_t remove_mqtt_sn_forwarder_encapsulation_frame(MqttSnMessageData *gatewayMessageData, MqttSnMessageData *clientMessageData,
                                                     const device_address *client_network_broadcast_address, const MqttSnLogger *logger) {
    uint8_t radius = 0;
    int32_t p_rc =
      parse_forwarder_encapsulation_byte(&radius, &clientMessageData->to, clientMessageData->data, &clientMessageData->data_length,
                                         sizeof(clientMessageData->data), gatewayMessageData->data, gatewayMessageData->data_length);
    if (p_rc < 0) {
#ifdef WITH_LOGGING
        log_gateway_mqtt_sn_message_malformed(logger, gatewayMessageData);
#endif
        return -1;
    }
    clientMessageData->from = gatewayMessageData->from;
    if (client_network_broadcast_address && radius == MQTT_SN_MESSAGE_FORWARDER_ENCAPSULATION_CRTL_BROADCAST_RADIUS) {
        clientMessageData->to = *client_network_broadcast_address;
    }
#ifdef WITH_LOGGING
    log_any_message(logger, clientMessageData);
#endif
    return 0;
}
int32_t add_mqtt_sn_forwarder_encapsulation_frame(MqttSnMessageData *clientMessageData, MqttSnMessageData *gatewayMessageData,
                                                  const device_address *mqtt_sn_gateway_network_address,
                                                  const device_address *client_network_broadcast_address, const MqttSnLogger *logger) {
    uint8_t broadcast = 0;
    if (client_network_broadcast_address &&
        memcmp(&gatewayMessageData->to, client_network_broadcast_address, sizeof(device_address)) == 0) {
        broadcast = MQTT_SN_MESSAGE_FORWARDER_ENCAPSULATION_CRTL_BROADCAST_RADIUS;
        // TODO mention later in doku: Radius: broadcast radius (only relevant in direction GW to forwarder). We ignore the section: "(only relevant in direction GW to forwarder)" as in the translation from one network to another it is not clear what the broadcast address ist.
        // example different UDP broadcast address or in SN network own defined BC address. So we uses these to indicator for this in GW direction to indicate that the message was broadcasted.
    }
    int rc = generate_forwarder_encapsulation_byte(gatewayMessageData->data, sizeof(gatewayMessageData->data), broadcast,
                                                   &clientMessageData->from, clientMessageData->data, clientMessageData->data_length);
    if (rc < 0) {
        return -1;
    }
    gatewayMessageData->data_length = rc;
    gatewayMessageData->to          = *mqtt_sn_gateway_network_address;
    gatewayMessageData->from        = clientMessageData->from;

    return 0;
}
#if defined(WITH_ADVERTISE_GATEWAY_DISCOVERED_LIST) || defined(WITH_GWINFO_GATEWAY_DISCOVERED_LIST)
int MqttSnForwardingUpdateDiscoveredGw(MqttSnForwarder *forwarder, MqttSnMessageData *msg) {
    ParsedMqttSnHeader header = { 0 };
    if (parse_message_tolerant(&header, ANY_MESSAGE_TYPE, msg->data, msg->data_length) > 0) {
    }
    if (header.msg_type == ADVERTISE) {
        MqttSnAdvertise mqtt_sn_advertise = { 0 };
        if (parse_advertise(&mqtt_sn_advertise, msg->data, msg->data_length) > 0) {
#ifdef WITH_MQTT_SN_FORWARDER_GATEWAY_NETWORK_WHITELIST
            if (forwarder->gn_gw_id_whitelist_len != 0) {
                if (MqttSnForwarderContainsGatewayNetworkWhitelistGwId(forwarder, mqtt_sn_advertise.gwId)) {
                    return 0;
                }
            }
#endif
#ifdef WITH_MQTT_SN_FORWARDER_GATEWAY_NETWORK_BLACKLIST
            if (MqttSnForwarderContainsGatewayNetworkBlacklistGwId(forwarder, mqtt_sn_advertise.gwId)) {
                return 0;
            }
#endif

            uint64_t current_time_s;
            if (get_timestamp_s(&current_time_s) < 0) {
                return -1;
            }

            if (MqttSnDiscoveredGatewayListUpdate(&forwarder->discovered_gw_list, &msg->from, msg->signal_strength, &mqtt_sn_advertise,
                                                  NULL, current_time_s) < 0) {
                return -1;
            }
        }
    } else if (header.msg_type == GWINFO) {
        MqttSnGwInfo mqtt_sn_gwinfo = { 0 };
        if (parse_gwinfo(&mqtt_sn_gwinfo, msg->data, msg->data_length) > 0) {

#ifdef WITH_MQTT_SN_FORWARDER_GATEWAY_NETWORK_WHITELIST
            if (forwarder->gn_gw_id_whitelist_len != 0) {
                if (MqttSnForwarderContainsGatewayNetworkWhitelistGwId(forwarder, mqtt_sn_gwinfo.gwId)) {
                    return 0;
                }
            }
#endif
#ifdef WITH_MQTT_SN_FORWARDER_GATEWAY_NETWORK_BLACKLIST
            if (MqttSnForwarderContainsGatewayNetworkBlacklistGwId(forwarder, mqtt_sn_gwinfo.gwId)) {
                return 0;
            }
#endif

            uint64_t current_time_s;
            if (get_timestamp_s(&current_time_s) < 0) {
                return -1;
            }
            if (MqttSnDiscoveredGatewayListUpdate(&forwarder->discovered_gw_list, &msg->from, msg->signal_strength, NULL, &mqtt_sn_gwinfo,
                                                  current_time_s) < 0) {
                return -1;
            }
        }
    }
    return 0;
}
void MqttSnForwarderSetPinConnection(MqttSnForwarder *forwarder, uint8_t pin_connection) {
    forwarder->pin_connection = pin_connection;
}
void MqttSnForwarderSetConnected(MqttSnForwarder *forwarder, const device_address *mqtt_sn_gateway_address) {
    forwarder->mqtt_sn_gateway_address = (*mqtt_sn_gateway_address);
    forwarder->disconnected            = 0;
#ifdef WITH_LOGGING
    log_mqtt_sn_forwarder_connected(&forwarder->logger, &forwarder->mqtt_sn_gateway_address);
#endif
}
void MqttSnForwarderSetDisconnected(MqttSnForwarder *forwarder) {
    if (!forwarder->pin_connection) {
        forwarder->disconnected = 1;
#ifdef WITH_LOGGING
        log_mqtt_sn_forwarder_disconnect(&forwarder->logger, &forwarder->mqtt_sn_gateway_address);
#endif
    }
}
int MqttSnForwarderHandleGatewayPseudoConnect(MqttSnForwarder *forwarder) {
    if (forwarder->disconnected) {
        if (forwarder->discovered_gw_list.current_len > 0) {
            MqttSnDiscoveredGateway *discovered_gateway = &forwarder->discovered_gw_list.gateways[0];
            // get the discovery gateway with the least signal_strength and then smaller received Advertise or SearchGw (last_message_s)
            for (uint8_t i = 0; i < forwarder->discovered_gw_list.current_len; i++) {
                if (forwarder->discovered_gw_list.gateways[i].signal_strength == discovered_gateway->signal_strength &&
                    forwarder->discovered_gw_list.gateways[i].last_message_s > discovered_gateway->last_message_s) {
                    discovered_gateway = &forwarder->discovered_gw_list.gateways[i];
                } else if (forwarder->discovered_gw_list.gateways[i].signal_strength < discovered_gateway->signal_strength) {
                    discovered_gateway = &forwarder->discovered_gw_list.gateways[i];
                }
            }
            MqttSnForwarderSetConnected(forwarder, &discovered_gateway->network_address);
            return 0;
        } else {
            if (GatewayNetworkReceive(&forwarder->gatewayNetwork, &forwarder->gatewayNetworkReceiveBuffer,
                                      forwarder->gatewayNetworkReceiveTimeout, forwarder->gatewayNetwork.context) < 0) {
                GatewayNetworkDisconnect(&forwarder->gatewayNetwork, forwarder->gatewayNetwork.context);
            }

#ifdef Arduino_h
            yield();
#endif
            MqttSnMessageData gatewayMessageData = { 0 };
            if (pop(&forwarder->clientNetworkReceiveBuffer, &gatewayMessageData) != 0) {
                return 0;
            }
#ifdef WITH_MQTT_SN_FORWARDER_GATEWAY_NETWORK_WHITELIST
            if (forwarder->gn_addr_whitelist_len != 0) {
                if (!MqttSnForwarderContainsGatewayNetworkWhitelistAddr(forwarder, &gatewayMessageData.from)) {
                    return 0;
                }
            }
#endif
#ifdef WITH_MQTT_SN_FORWARDER_GATEWAY_NETWORK_BLACKLIST
            if (MqttSnForwarderContainsGatewayNetworkBlacklistAddr(forwarder, &gatewayMessageData.from)) {
                return 0;
            }
#endif
            if (MqttSnForwardingUpdateDiscoveredGw(forwarder, &gatewayMessageData) < 0) {
                return -1;
            }
            if (forwarder->discovered_gw_list.current_len > 0) {
                MqttSnForwarderSetConnected(forwarder, &forwarder->discovered_gw_list.gateways[0].network_address);
            }
        }
    }
    return 0;
}

#ifdef WITH_MQTT_SN_FORWARDER_GATEWAY_NETWORK_WHITELIST
int32_t MqttSnForwarderAddGatewayNetworkWhitelistGwId(MqttSnForwarder *forwarder, uint8_t gw_id) {
    if (forwarder->gn_gw_id_whitelist_len + 1 >= MQTT_SN_FORWARDER_GATEWAY_NETWORK_GW_ID_WHITELIST_LENGTH) {
        return -1;
    }
    for (int32_t i = 0; i < forwarder->gn_gw_id_whitelist_len; i++) {
        if (forwarder->gn_gw_id_whitelist[i] == gw_id) {
            return 0;
        }
    }
    forwarder->gn_gw_id_whitelist[forwarder->gn_gw_id_whitelist_len] = gw_id;
    forwarder->gn_gw_id_whitelist_len += 1;
    return 0;
}
void MqttSnForwarderClearGatewayNetworkWhitelistGwId(MqttSnForwarder *forwarder) {
    for (int32_t i = 0; i < forwarder->gn_gw_id_whitelist_len; i++) {
        forwarder->gn_gw_id_whitelist[i] = 0;
    }
    forwarder->gn_gw_id_whitelist_len = 0;
}
int32_t MqttSnForwarderContainsGatewayNetworkWhitelistGwId(MqttSnForwarder *forwarder, uint8_t gw_id) {
    for (int32_t i = 0; i < forwarder->gn_gw_id_whitelist_len; i++) {
        if (forwarder->gn_gw_id_whitelist[i] == gw_id) {
            return 1;
        }
    }
    return 0;
}

int32_t MqttSnForwarderAddGatewayNetworkWhitelistAddr(MqttSnForwarder *forwarder, const device_address *address) {
    if (forwarder->gn_addr_whitelist_len + 1 >= MQTT_SN_FORWARDER_GATEWAY_ADDRESS_WHITELIST_LENGTH) {
        return -1;
    }
    for (int32_t i = 0; i < forwarder->gn_addr_whitelist_len; i++) {
        if (memcmp(&forwarder->gn_addr_whitelist[i], address, sizeof(device_address)) == 0) {
            return 0;
        }
    }
    forwarder->gn_addr_whitelist[forwarder->gn_addr_whitelist_len] = (*address);
    forwarder->gn_addr_whitelist_len += 1;
    return 0;
}
void MqttSnForwarderClearGatewayNetworkWhitelistAddr(MqttSnForwarder *forwarder) {
    for (int32_t i = 0; i < forwarder->gn_addr_whitelist_len; i++) {
        memset(&forwarder->gn_addr_whitelist[i], 0, sizeof(device_address));
    }
    forwarder->gn_addr_whitelist_len = 0;
}
int32_t MqttSnForwarderContainsGatewayNetworkWhitelistAddr(MqttSnForwarder *forwarder, const device_address *address) {
    for (int32_t i = 0; i < forwarder->gn_addr_whitelist_len; i++) {
        if (memcmp(&forwarder->gn_addr_whitelist[i], address, sizeof(device_address)) == 0) {
            return 1;
        }
    }
    return 0;
}
#endif
#ifdef WITH_MQTT_SN_FORWARDER_GATEWAY_NETWORK_BLACKLIST
int32_t MqttSnForwarderAddGatewayNetworkBlacklistGwId(MqttSnForwarder *forwarder, uint8_t gw_id) {
    if (forwarder->gn_gw_id_blacklist_len + 1 >= MQTT_SN_FORWARDER_GATEWAY_NETWORK_GW_ID_BLACKLIST_LENGTH) {
        return -1;
    }
    for (int32_t i = 0; i < forwarder->gn_gw_id_blacklist_len; i++) {
        if (forwarder->gn_gw_id_blacklist[i] == gw_id) {
            return 0;
        }
    }
    forwarder->gn_gw_id_blacklist[forwarder->gn_gw_id_blacklist_len] = gw_id;
    forwarder->gn_gw_id_blacklist_len += 1;
    return 0;
}
void MqttSnForwarderClearGatewayNetworkBlacklistGwId(MqttSnForwarder *forwarder) {
    for (int32_t i = 0; i < forwarder->gn_gw_id_blacklist_len; i++) {
        forwarder->gn_gw_id_blacklist[i] = 0;
    }
    forwarder->gn_gw_id_blacklist_len = 0;
}
int32_t MqttSnForwarderContainsGatewayNetworkBlacklistGwId(MqttSnForwarder *forwarder, uint8_t gw_id) {
    for (int32_t i = 0; i < forwarder->gn_gw_id_blacklist_len; i++) {
        if (forwarder->gn_gw_id_blacklist[i] == gw_id) {
            return 0;
        }
    }
    return 0;
}

int32_t MqttSnForwarderAddGatewayNetworkBlacklistAddr(MqttSnForwarder *forwarder, const device_address *address) {
    if (forwarder->gn_addr_blacklist_len + 1 >= MQTT_SN_FORWARDER_GATEWAY_ADDRESS_BLACKLIST_LENGTH) {
        return 0;
    }
    for (int32_t i = 0; i < forwarder->gn_addr_blacklist_len; i++) {
        if (memcmp(&forwarder->gn_addr_blacklist[i], address, sizeof(device_address)) == 0) {
            return 0;
        }
    }
    forwarder->gn_addr_blacklist[forwarder->gn_addr_blacklist_len] = (*address);
    forwarder->gn_addr_blacklist_len += 1;
    return 0;
}
void MqttSnForwarderClearGatewayNetworkBlacklistAddr(MqttSnForwarder *forwarder) {
    for (int32_t i = 0; i < forwarder->gn_addr_blacklist_len; i++) {
        memset(&forwarder->gn_addr_blacklist[i], 0, sizeof(device_address));
    }
    forwarder->gn_addr_blacklist_len = 0;
}
int32_t MqttSnForwarderContainsGatewayNetworkBlacklistAddr(MqttSnForwarder *forwarder, const device_address *address) {
    for (int32_t i = 0; i < forwarder->gn_addr_blacklist_len; i++) {
        if (memcmp(&forwarder->gn_addr_blacklist[i], address, sizeof(device_address)) == 0) {
            return 1;
        }
    }
    return 0;
}
#endif
#ifdef WITH_MQTT_SN_FORWARDER_CLIENT_NETWORK_WHITELIST
int32_t MqttSnForwarderAddClientNetworkWhitelistAddr(MqttSnForwarder *forwarder, const device_address *address) {
    if (forwarder->cn_addr_whitelist_len + 1 >= MQTT_SN_FORWARDER_CLIENT_ADDRESS_WHITELIST_LENGTH) {
        return 0;
    }
    for (int32_t i = 0; i < forwarder->cn_addr_whitelist_len; i++) {
        if (memcmp(&forwarder->cn_addr_whitelist[i], address, sizeof(device_address)) == 0) {
            return 0;
        }
    }
    forwarder->cn_addr_whitelist[forwarder->cn_addr_whitelist_len] = (*address);
    forwarder->cn_addr_whitelist_len += 1;
    return 0;
}
void MqttSnForwarderClearClientNetworkWhitelistAddr(MqttSnForwarder *forwarder) {
    for (int32_t i = 0; i < forwarder->cn_addr_whitelist_len; i++) {
        memset(&forwarder->cn_addr_whitelist[i], 0, sizeof(device_address));
    }
    forwarder->cn_addr_whitelist_len = 0;
}
int32_t MqttSnForwarderContainsClientNetworkWhitelistAddr(MqttSnForwarder *forwarder, const device_address *address) {
    for (int32_t i = 0; i < forwarder->cn_addr_whitelist_len; i++) {
        if (memcmp(&forwarder->cn_addr_whitelist[i], address, sizeof(device_address)) == 0) {
            return 1;
        }
    }
    return 0;
}
#endif
#ifdef WITH_MQTT_SN_FORWARDER_CLIENT_NETWORK_BLACKLIST
int32_t MqttSnForwarderAddClientNetworkBlacklistAddr(MqttSnForwarder *forwarder, const device_address *address) {
    if (forwarder->cn_addr_blacklist_len + 1 >= MQTT_SN_FORWARDER_CLIENT_ADDRESS_BLACKLIST_LENGTH) {
        return 0;
    }
    for (int32_t i = 0; i < forwarder->cn_addr_blacklist_len; i++) {
        if (memcmp(&forwarder->cn_addr_blacklist[i], address, sizeof(device_address)) == 0) {
            return 0;
        }
    }
    forwarder->cn_addr_blacklist[forwarder->cn_addr_blacklist_len] = (*address);
    forwarder->cn_addr_blacklist_len += 1;
    return 0;
}
void MqttSnForwarderClearClientNetworkBlacklistAddr(MqttSnForwarder *forwarder) {
    for (int32_t i = 0; i < forwarder->cn_addr_blacklist_len; i++) {
        memset(&forwarder->cn_addr_blacklist[i], 0, sizeof(device_address));
    }
    forwarder->cn_addr_blacklist_len = 0;
}
int32_t MqttSnForwarderContainsClientNetworkBlacklistAddr(MqttSnForwarder *forwarder, const device_address *address) {
    for (int32_t i = 0; i < forwarder->cn_addr_blacklist_len; i++) {
        if (memcmp(&forwarder->cn_addr_blacklist[i], address, sizeof(device_address)) == 0) {
            return 1;
        }
    }
    return 0;
}
#endif
#endif
