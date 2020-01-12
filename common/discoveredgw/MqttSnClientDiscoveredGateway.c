//
// Created by SomeDude on 03.09.2019.
//

#include "MqttSnClientDiscoveredGateway.h"
#include "common/timeout/MqttSnClientTimeout.h"
#include <string.h>

int32_t MqttSnDiscoveredGatewayListUpdate(MqttSnDiscoveredGatewayList *list, const device_address *from, uint8_t signal_strength,
                                          const MqttSnAdvertise *advertise, const MqttSnGwInfo *gw_info, uint64_t current_time_s) {
    MqttSnDiscoveredGatewayListUpdateStates(list, current_time_s);
    MqttSnDiscoveredGatewayListRemoveInactive(list);
    // try to update
    for (uint8_t i = 0; i < list->current_len; i++) {
        if (advertise) {
            if (list->gateways[i].gw_id == advertise->gwId) {
                list->gateways[i].network_address = (*from);
                list->gateways[i].duration        = advertise->duration;
                list->gateways[i].signal_strength = signal_strength;
                list->gateways[i].state           = MqttSnDiscoveredGatewayStateActive;
                list->gateways[i].last_message_s  = current_time_s;
                return 0;
            }
        }
        if (gw_info) {
            if (list->gateways[i].gw_id == gw_info->gwId) {
                list->gateways[i].network_address = (*from);
                list->gateways[i].signal_strength = signal_strength;
                list->gateways[i].state           = MqttSnDiscoveredGatewayStateActive;
                list->gateways[i].last_message_s  = current_time_s;
                return 0;
            }
        }
    }

    if (list->current_len == list->max_len) {
        // no update and no slot free - ignored
        return 0;
    }
    // not saved yet
    for (uint8_t i = 0; i < list->max_len; i++) {
        if (list->gateways[i].state == MqttSnDiscoveredGatewayStateInactive) {
            // empty slot found
            if (advertise) {
                list->gateways[i].gw_id           = advertise->gwId;
                list->gateways[i].network_address = (*from);
                list->gateways[i].duration        = advertise->duration;
                list->gateways[i].signal_strength = signal_strength;
                list->gateways[i].state           = MqttSnDiscoveredGatewayStateActive;
                list->gateways[i].last_message_s  = current_time_s;
                if ((i + 1) >= list->current_len) {
                    list->current_len += 1;
                    return 0;
                }
            }
            if (gw_info) {
                list->gateways[i].gw_id           = gw_info->gwId;
                list->gateways[i].network_address = (*from);
                list->gateways[i].duration        = 0;
                list->gateways[i].signal_strength = signal_strength;
                list->gateways[i].state           = MqttSnDiscoveredGatewayStateActive;
                list->gateways[i].last_message_s  = current_time_s;
                if ((i + 1) >= list->current_len) {
                    list->current_len += 1;
                    return 0;
                }
            }
        }
    }
    return 0;
}
int32_t MqttSnDiscoveredGatewayListInit(MqttSnDiscoveredGatewayList *list) {
    memset(list, 0, sizeof(*list));
    list->max_len     = MQTT_SN_FIND_GATEWAY_CLIENT_DISCOVERED_GATEWAY_LIST_LENGTH;
    list->current_len = 0;
    for (uint8_t i = 0; i < list->max_len; i++) {
        list->gateways[i].state = MqttSnDiscoveredGatewayStateInactive;
    }
    return 0;
}
void MqttSnDiscoveredGatewayListUpdateStates(MqttSnDiscoveredGatewayList *list, uint64_t current_time_s) {
    for (uint8_t i = 0; i < list->current_len; i++) {
        if ((current_time_s - list->gateways[i].last_message_s) > get_tolerance_timeout(list->gateways[i].duration, 60, 50, 10)) {
            list->gateways[i].state = MqttSnDiscoveredGatewayStateInactive;
        }
    }
}
int32_t MqttSnDiscoveredGatewayListRemoveInactive(MqttSnDiscoveredGatewayList *list) {
    for (int32_t i = 0; i < list->current_len; i++) {
        if (list->gateways[i].state == MqttSnDiscoveredGatewayStateInactive && list->gateways[i].last_message_s > 0) {
            memset(&list->gateways[i], 0, sizeof(MqttSnDiscoveredGateway));
            for (uint8_t j = i; (j + 1) < list->current_len; j++) {
                list->gateways[j] = list->gateways[j + 1];
            }
            list->current_len -= 1;
            i -= 1;
            if (list->max_len > 0) {
                for (int32_t j = (list->max_len - 1); j >= list->current_len; j--) {
                    memset(&list->gateways[j], 0, sizeof(MqttSnDiscoveredGateway));
                }
            }
        }
    }
    return 0;
}
uint8_t MqttSnDiscoveredGatewayListExists(MqttSnDiscoveredGatewayList *list, const device_address *address, int32_t gw_id) {
    if (!address && gw_id < 0) {
        return 0;
    }
    for (int32_t i = 0; i < list->current_len; i++) {
        if (address && gw_id > 0) {
            if ((memcmp(&list->gateways[i].network_address, address, sizeof(device_address)) == 0) &&
                (list->gateways[i].gw_id == (uint8_t)gw_id)) {
                return 1;
            }
        } else if (address && gw_id < 0) {
            if (memcmp(&list->gateways[i].network_address, address, sizeof(device_address)) == 0) {
                return 1;
            }
        } else if (gw_id > 0) {
            if (list->gateways[i].gw_id == (uint8_t)gw_id) {
                return 1;
            }
        }
    }
    return 0;
}
