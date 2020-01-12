//
// Created by SomeDude on 03.06.2019.
//

#include <assert.h>
#include "MqttSnFindGatewayMonitor.h"
#include <parser/MqttSnSearchGwMessage.h>
#include <platform/platform_compatibility.h>

#include <parser/logging/MqttSnForwarderLoggingMessages.h>
#include <parser/logging/MqttSnMessageParserLogging.h>

MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE MqttSnFindGatewayClientInit(MqttSnFindGatewayMonitor *client, MqttSnLogger *logger,
                                                                     MqttSnGatewayNetworkInterface *gatewayNetwork,
                                                                     void *gatewayNetworkContext, int32_t gatewayNetworkSendTimeout,
                                                                     int32_t gatewayNetworkReceiveTimeout) {
    memset(client, 0, sizeof(*client));

    client->logger                       = logger;
    client->gatewayNetwork               = gatewayNetwork;
    client->gatewayNetworkContext        = gatewayNetworkContext;
    client->gatewayNetworkSendTimeout    = gatewayNetworkSendTimeout;
    client->gatewayNetworkReceiveTimeout = gatewayNetworkReceiveTimeout;
    if (MqttSnDiscoveredGatewayListInit(&client->gatewayList) < 0) {
        return MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE_ERROR;
    }
    return MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE_SUCCESS;
}
void MqttSnFindGatewayClientDeinit(MqttSnFindGatewayMonitor *client) {
    client->logger                = NULL;
    client->gatewayNetwork        = NULL;
    client->gatewayNetworkContext = NULL;
}

MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE MqttSnFindGatewayClientParseAndCGwInfoCb(
  MqttSnFindGatewayMonitor *client, MqttSnMessageData *msg, uint64_t start_timestamp, uint64_t current_timestamp, int32_t timeout,
  void *context, int32_t (*gwinfo_cb)(const MqttSnFindGatewayMonitor *, const int32_t, const MqttSnReceivedGwInfo *, void *)) {
    if (gwinfo_cb == NULL) {
        return MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE_SUCCESS;
    }
    MqttSnReceivedGwInfo rec_gwinfo = { 0 };
    if (parse_gwinfo_message_byte(&rec_gwinfo.gw_info.gwId, &rec_gwinfo.gw_info.gwAdd, &rec_gwinfo.gw_info.gwAddLen, msg->data,
                                  msg->data_length) < 0) {
        return MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE_SUCCESS;
    }
    if (rec_gwinfo.gw_info.gwAddLen > sizeof(rec_gwinfo.gw_info.gwAdd)) {
        // incompatible gateway device address - too long
        return MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE_SUCCESS;
    }
    rec_gwinfo.from      = msg->from;
    rec_gwinfo.timeout   = current_timestamp - start_timestamp;
    rec_gwinfo.radius    = msg->signal_strength;
    int32_t timeout_left = timeout - rec_gwinfo.timeout;
    int32_t gwinfo_cb_rc = gwinfo_cb(client, timeout_left, &rec_gwinfo, context);
    if (gwinfo_cb_rc < 0) {
        return MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE_ERROR;
    }
    if (gwinfo_cb_rc > 0) {
        return 1;
    }
    return MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE_SUCCESS;
}
MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE MqttSnFindGatewayClientParseAndCAdvCb(
  MqttSnFindGatewayMonitor *client, MqttSnMessageData *msg, uint64_t start_timestamp, uint64_t current_timestamp, int32_t timeout,
  void *context, int32_t (*adv_cb)(const MqttSnFindGatewayMonitor *, const int32_t, const MqttSnReceivedAdvertise *, void *)) {
    if (adv_cb == NULL) {
        return MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE_SUCCESS;
    }
    MqttSnReceivedAdvertise rec_adv = { 0 };
    int32_t parse_adv_rc = parse_advertise_byte(&rec_adv.advertise.gwId, &rec_adv.advertise.duration, msg->data, msg->data_length);
    if (parse_adv_rc < 0) {
        return MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE_SUCCESS;
    }
    rec_adv.from         = msg->from;
    rec_adv.timeout      = current_timestamp - start_timestamp;
    int32_t timeout_left = timeout - rec_adv.timeout;
    if (adv_cb(client, timeout_left, &rec_adv, context)) {
        return MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE_ERROR;
    }
    return MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE_SUCCESS;
}
MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE MqttSnFindGatewayClientReceiveAndParse(
  MqttSnFindGatewayMonitor *client, uint64_t start_timestamp, int32_t timeout, void *context,
  int32_t (*gwinfo_cb)(const MqttSnFindGatewayMonitor *, const int32_t, const MqttSnReceivedGwInfo *, void *),
  int32_t (*adv_cb)(const MqttSnFindGatewayMonitor *, const int32_t, const MqttSnReceivedAdvertise *, void *)) {
    uint64_t          current_timestamp = 0;
    MqttSnMessageData msg               = { 0 };
    int32_t           received_bytes =
      GatewayNetworkReceiveFom(client->gatewayNetwork, &msg.from, &msg.to, msg.data, sizeof(msg.data), &msg.signal_strength,
                               client->gatewayNetworkReceiveTimeout, client->gatewayNetworkContext);
    if (PlatformCompatibilityGetTimestampMs(&current_timestamp) < 0) {
        return MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE_ERROR;
    }
    if (received_bytes < 0) {
        return MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE_ERROR;
    }
    msg.data_length = received_bytes;

    if (received_bytes > 0) {
        ParsedMqttSnHeader h            = { 0 };
        int32_t            parsed_bytes = 0;
        if (parse_header(&h, ANY_MESSAGE_TYPE, msg.data, msg.data_length, &parsed_bytes) < 0) {
#ifdef WITH_DEBUG_LOGGING
            log_malformed_message(client->logger, &msg);
#endif
            return MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE_SUCCESS;
        }
#ifdef WITH_LOGGING
        log_any_message(client->logger, &msg);
#endif
        if (h.msg_type == GWINFO) {
            return MqttSnFindGatewayClientParseAndCGwInfoCb(client, &msg, start_timestamp, current_timestamp, timeout, context, gwinfo_cb);
        } else if (h.msg_type == ADVERTISE) {
            return MqttSnFindGatewayClientParseAndCAdvCb(client, &msg, start_timestamp, current_timestamp, timeout, context, adv_cb);
        }
    }
    return MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE_SUCCESS;
}

MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE MqttSnFindGatewayAwaitAdvertise(MqttSnFindGatewayMonitor *client, int32_t timeout, void *context,
                                                                         int32_t (*adv_cb)(const MqttSnFindGatewayMonitor *, const int32_t,
                                                                                           const MqttSnReceivedAdvertise *, void *)) {
    assert(client != NULL);
    assert(timeout >= -1);

    uint64_t start_timestamp   = 0;
    uint64_t current_timestamp = 0;
    if (PlatformCompatibilityGetTimestampMs(&start_timestamp) < 0) {
        return MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE_ERROR;
    }

    if (timeout == 0) {
        return MqttSnFindGatewayClientReceiveAndParse(client, start_timestamp, timeout, context, NULL, adv_cb);
        // once and done
    }

    // with timeout == -1
    if (timeout == -1) {
        if (PlatformCompatibilityGetTimestampMs(&current_timestamp) < 0) {
            return MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE_ERROR;
        }
        while (1) {
            MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE rec_rc =
              MqttSnFindGatewayClientReceiveAndParse(client, start_timestamp, timeout, context, NULL, adv_cb);
            if (rec_rc != MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE_SUCCESS) {
                return rec_rc;
            }
        }
    }

    // with timeout > 0
    if (PlatformCompatibilityGetTimestampMs(&current_timestamp) < 0) {
        return MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE_ERROR;
    }
    while (((current_timestamp - start_timestamp) < (uint64_t)timeout)) {
        MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE rec_rc =
          MqttSnFindGatewayClientReceiveAndParse(client, start_timestamp, timeout, context, NULL, adv_cb);
        if (rec_rc != MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE_SUCCESS) {
            return rec_rc;
        }
        if (PlatformCompatibilityGetTimestampMs(&current_timestamp) < 0) {
            return MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE_ERROR;
        }
    }

    return MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE_SUCCESS;
}
MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE
MqttSnFindGatewaySearchGw(MqttSnFindGatewayMonitor *client, int32_t timeout, uint8_t radius, void *context,
                          int32_t (*gwinfo_cb)(const MqttSnFindGatewayMonitor *, const int32_t, const MqttSnReceivedGwInfo *, void *),
                          int32_t (*adv_cb)(const MqttSnFindGatewayMonitor *, const int32_t, const MqttSnReceivedAdvertise *, void *)) {
    assert(client != NULL);
    assert(gwinfo_cb != NULL);
    assert(timeout >= -1);

    MqttSnMessageData toSend = { 0 };
    int32_t           gen_rc = generate_searchgw_message(toSend.data, sizeof(toSend.data), radius);
    if (gen_rc < 0) {
        return MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE_ERROR;
    }
    toSend.data_length = gen_rc;
    int32_t send_rc =
      GatewayNetworkSendTo(client->gatewayNetwork, NULL, client->gatewayNetwork->gateway_network_broadcast_address, toSend.data,
                           toSend.data_length, radius, client->gatewayNetworkSendTimeout, client->gatewayNetworkContext);
    if (send_rc < 0) {
        return MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE_ERROR;
    }
    if (send_rc != toSend.data_length) {
        return MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE_ERROR;
    }

    uint64_t start_timestamp   = 0;
    uint64_t current_timestamp = 0;

    if (get_timestamp_s(&start_timestamp) < 0) {
        return MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE_ERROR;
    }

    if (timeout == 0) {
        return MqttSnFindGatewayClientReceiveAndParse(client, start_timestamp, timeout, context, gwinfo_cb, adv_cb);
        // once and done
    }

    // with timeout == -1 means without any timeout
    if (timeout == -1) {
        if (get_timestamp_s(&current_timestamp) < 0) {
            return MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE_ERROR;
        }
        while (1) {
            MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE rec_rc =
              MqttSnFindGatewayClientReceiveAndParse(client, start_timestamp, timeout, context, gwinfo_cb, adv_cb);
            if (rec_rc != MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE_SUCCESS) {
                return rec_rc;
            }
        }
    }

    // with timeout > 0
    if (PlatformCompatibilityGetTimestampMs(&current_timestamp) < 0) {
        return MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE_ERROR;
    }
    while (((current_timestamp - start_timestamp) < ((uint64_t)timeout / (uint64_t)1000))) {
        MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE rec_rc =
          MqttSnFindGatewayClientReceiveAndParse(client, start_timestamp, timeout, NULL, gwinfo_cb, adv_cb);
        if (rec_rc != MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE_SUCCESS) {
            return rec_rc;
        }
        if (PlatformCompatibilityGetTimestampMs(&current_timestamp) < 0) {
            return MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE_ERROR;
        }
        MqttSnDiscoveredGatewayListUpdateStates(&client->gatewayList, current_timestamp);
    }
    MqttSnDiscoveredGatewayListUpdateStates(&client->gatewayList, current_timestamp);
    return MQTT_SN_FIND_GATEWAY_MONITOR_RETURN_CODE_SUCCESS;
}
