//
// Created by SomeDude on 03.06.2019.
//

#include <assert.h>
#include "MqttSnFindGatewayClient.h"
#include <parser/logging/MqttSnForwarderLoggingMessages.h>
#include <parser/MqttSnSearchGwMessage.h>
#include <platform/platform_compatibility.h>

int32_t MqttSnFindGatewayClientInit(MqttSnFindGatewayClient *client,
                                    MqttSnLogger *logger,
                                    MqttSnGatewayNetworkInterface *gatewayNetwork,
                                    void *gatewayNetworkContext,
                                    int32_t gatewayNetworkSendTimeout,
                                    int32_t gatewayNetworkReceiveTimeout) {
  client->logger = logger;
  client->gatewayNetwork = gatewayNetwork;
  client->gatewayNetworkContext = gatewayNetworkContext;
  client->gatewayNetworkSendTimeout = gatewayNetworkSendTimeout;
  client->gatewayNetworkReceiveTimeout = gatewayNetworkReceiveTimeout;
  return 0;
}
int32_t MqttSnFindGatewayClientDeinit(MqttSnFindGatewayClient *client) {
  return 0;
}

int32_t MqttSnFindGatewayClientParseAndCGwInfoCb(MqttSnFindGatewayClient *client,
                                                 MqttSnMessageData *msg,
                                                 uint64_t start_timestamp,
                                                 uint64_t current_timestamp,
                                                 int32_t timeout,
                                                 int32_t (*gwinfo_cb)(const MqttSnFindGatewayClient *,
                                                                      const int32_t,
                                                                      const MqttSnReceivedGwInfo *)) {
  if (gwinfo_cb == NULL) {
    return 0;
  }
  MqttSnReceivedGwInfo rec_gwinfo = {0};
  if (parse_gwinfo_message_byte(&rec_gwinfo.gw_info.gwId,
                                &rec_gwinfo.gw_info.gwAdd,
                                &rec_gwinfo.gw_info.gwAddLen,
                                msg->data,
                                msg->data_length) < 0) {
    return 0;
  }
  if (rec_gwinfo.gw_info.gwAddLen > sizeof(rec_gwinfo.gw_info.gwAdd)) {
    // incompatible gateway device address as it is too long
    return 0;
  }
  rec_gwinfo.from = msg->from;
  rec_gwinfo.timeout = current_timestamp - start_timestamp;
  rec_gwinfo.radius = msg->signal_strength;
  int32_t timeout_left = timeout - rec_gwinfo.timeout;
  int32_t gwinfo_cb_rc = gwinfo_cb(client, timeout_left, &rec_gwinfo);
  if (gwinfo_cb_rc < 0) {
    return -1;
  }
  if (gwinfo_cb_rc > 0) {
    return 1;
  }
  return 0;
}
int32_t MqttSnFindGatewayClientParseAndCAdvCb(MqttSnFindGatewayClient *client,
                                              MqttSnMessageData *msg,
                                              uint64_t start_timestamp,
                                              uint64_t current_timestamp,
                                              int32_t timeout,
                                              int32_t (*adv_cb)(const MqttSnFindGatewayClient *,
                                                                const int32_t,
                                                                const MqttSnReceivedAdvertise *)) {
  if (adv_cb == NULL) {
    return 0;
  }
  MqttSnReceivedAdvertise rec_adv = {0};
  int32_t parse_adv_rc = parse_advertise_byte(&rec_adv.advertise.gwId,
                                              &rec_adv.advertise.duration,
                                              msg->data,
                                              msg->data_length);
  if (parse_adv_rc < 0) {
    return 0;
  }
  rec_adv.from = msg->from;
  rec_adv.timeout = current_timestamp - start_timestamp;
  int32_t timeout_left = timeout - rec_adv.timeout;
  if (adv_cb(client, timeout_left, &rec_adv)) {
    return -1;
  }
  return 0;
}
int32_t MqttSnFindGatewayClientReceiveAndParse(MqttSnFindGatewayClient *client,
                                               uint64_t start_timestamp,
                                               int32_t timeout,
                                               int32_t (*gwinfo_cb)(const MqttSnFindGatewayClient *,
                                                                    const int32_t,
                                                                    const MqttSnReceivedGwInfo *),
                                               int32_t (*adv_cb)(const MqttSnFindGatewayClient *,
                                                                 const int32_t,
                                                                 const MqttSnReceivedAdvertise *)) {

  uint64_t current_timestamp = 0;
  MqttSnMessageData msg = {0};
  int32_t received_bytes = GatewayNetworkReceiveFom(client->gatewayNetwork,
                                                    &msg.from,
                                                    &msg.to,
                                                    msg.data,
                                                    sizeof(msg.data),
                                                    &msg.signal_strength,
                                                    client->gatewayNetworkReceiveTimeout,
                                                    client->gatewayNetworkContext);
  if (PlatformCompatibilityGetTimestamp(&current_timestamp) < 0) {
    return -1;
  }
  if (received_bytes < 0) {
    return -1;
  }
  msg.data_length = received_bytes;

  if (received_bytes > 0) {
    ParsedMqttSnHeader h = {0};
    int32_t parsed_bytes = 0;
    if (parse_header(&h, ANY_MESSAGE_TYPE, msg.data, msg.data_length, &parsed_bytes) < 0) {
#ifdef WITH_DEBUG_LOGGING
      log_client_mqtt_sn_message_malformed(client->logger,
                                           &msg.from,
                                           msg.data,
                                           msg.data_length,
                                           msg.signal_strength);
#endif
      return 0;
    }
#ifdef WITH_LOGGING
    log_gateway_mqtt_sn_message(client->logger, &msg.from, msg.data, msg.data_length, NULL);
#endif
    if (h.msg_type == GWINFO) {
      return MqttSnFindGatewayClientParseAndCGwInfoCb(client,
                                                      &msg,
                                                      start_timestamp,
                                                      current_timestamp,
                                                      timeout,
                                                      gwinfo_cb);
    } else if (h.msg_type == ADVERTISE) {
      return MqttSnFindGatewayClientParseAndCAdvCb(client, &msg, start_timestamp, current_timestamp, timeout, adv_cb);
    }
  }
  return 0;
}

int32_t MqttSnClientAwaitAdvertise(MqttSnFindGatewayClient *client,
                                   int32_t timeout,
                                   int32_t (*adv_cb)(const MqttSnFindGatewayClient *,
                                                     const int32_t,
                                                     const MqttSnReceivedAdvertise *)) {
  assert(client != NULL);
  assert(timeout >= -1);

  uint64_t start_timestamp = 0;
  uint64_t current_timestamp = 0;
  if (PlatformCompatibilityGetTimestamp(&start_timestamp) < 0) {
    return -1;
  }

  if (timeout == 0) {
    return MqttSnFindGatewayClientReceiveAndParse(client, start_timestamp, timeout, NULL, adv_cb);
    // once and done
  }

  // with timeout == -1
  if (timeout == -1) {
    if (PlatformCompatibilityGetTimestamp(&current_timestamp) < 0) {
      return -1;
    }
    while (1) {
      int32_t rec_rc = MqttSnFindGatewayClientReceiveAndParse(client, start_timestamp, timeout, NULL, adv_cb);
      if (rec_rc < 0) {
        return -1;
      }
      if (rec_rc > 0) {
        return 0;
      }
    }
  }

  // with timeout > 0
  if (PlatformCompatibilityGetTimestamp(&current_timestamp) < 0) {
    return -1;
  }
  while (((current_timestamp - start_timestamp) < (uint64_t) timeout)) {
    int32_t rec_rc = MqttSnFindGatewayClientReceiveAndParse(client, start_timestamp, timeout, NULL, adv_cb);
    if (rec_rc < 0) {
      return -1;
    }
    if (rec_rc > 0) {
      return 0;
    }
    if (PlatformCompatibilityGetTimestamp(&current_timestamp) < 0) {
      return -1;
    }
  }

  return 0;
}
int32_t MqttSnClientSearchGw(MqttSnFindGatewayClient *client,
                             int32_t timeout,
                             uint8_t radius,
                             int32_t (*gwinfo_cb)(const MqttSnFindGatewayClient *,
                                                  const int32_t,
                                                  const MqttSnReceivedGwInfo *),
                             int32_t (*adv_cb)(const MqttSnFindGatewayClient *,
                                               const int32_t,
                                               const MqttSnReceivedAdvertise *)) {
  assert(client != NULL);
  assert(gwinfo_cb != NULL);
  assert(timeout >= -1);

  MqttSnMessageData toSend = {0};
  int32_t gen_rc = generate_searchgw_message(toSend.data, sizeof(toSend.data), radius);
  if (gen_rc < 0) {
    return -1;
  }
  toSend.data_length = gen_rc;
  int32_t send_rc = GatewayNetworkSendTo(client->gatewayNetwork,
                                         client->gatewayNetwork->gateway_network_broadcast_address,
                                         toSend.data,
                                         toSend.data_length,
                                         radius,
                                         client->gatewayNetworkSendTimeout,
                                         client->gatewayNetworkContext);
  if (send_rc < 0) {
    return -1;
  }
  if (send_rc != toSend.data_length) {
    return -1;
  }

  uint64_t start_timestamp = 0;
  uint64_t current_timestamp = 0;
  int32_t get_timestamp_rc = PlatformCompatibilityGetTimestamp(&start_timestamp);
  if (get_timestamp_rc < 0) {
    return -1;
  }

  if (timeout == 0) {
    return MqttSnFindGatewayClientReceiveAndParse(client, start_timestamp, timeout, gwinfo_cb, adv_cb);
    // once and done
  }

  // with timeout == -1
  if (timeout == -1) {
    if (PlatformCompatibilityGetTimestamp(&current_timestamp) < 0) {
      return -1;
    }
    while (1) {
      int32_t rec_rc = MqttSnFindGatewayClientReceiveAndParse(client, start_timestamp, timeout, gwinfo_cb, adv_cb);
      if (rec_rc < 0) {
        return -1;
      }
      if (rec_rc > 0) {
        return 0;
      }
    }
  }

  // with timeout > 0
  if (PlatformCompatibilityGetTimestamp(&current_timestamp) < 0) {
    return -1;
  }
  while (((current_timestamp - start_timestamp) < (uint64_t) timeout)) {
    int32_t rec_rc = MqttSnFindGatewayClientReceiveAndParse(client, start_timestamp, timeout, gwinfo_cb, adv_cb);
    if (rec_rc < 0) {
      return -1;
    }
    if (rec_rc > 0) {
      return 0;
    }
    if (PlatformCompatibilityGetTimestamp(&current_timestamp) < 0) {
      return -1;
    }
  }

  return 0;
}
int32_t MqttSnFindGatewayClientLoop(MqttSnFindGatewayClient *client) {
  return 0;
}

