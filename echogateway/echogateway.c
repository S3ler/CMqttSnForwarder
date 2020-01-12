//
// Created by SomeDude on 30.05.2019.
//

#include <platform/platform_compatibility.h>
#include <network/MqttSnClientNetworkInterface.h>
#include <client/MqttSnClientAsync.h>
#include <parser/MqttSnSearchGwMessage.h>
#include <parser/MqttSnGwInfoMessage.h>
#include <parser/MqttSnAdvertiseMessage.h>
#include "echogateway.h"

int32_t EchoGatewayInit(EchoGateway *egw,
                        log_level_t log_level,
                        void *clientNetworkContext,
                        const gateway_advertise_config *advertise_config) {
#ifdef WITH_LOGGING
#if defined(Arduino_h) || defined(WITH_PLATFORMIO)
  if (MqttSnLoggerInit(&mqttSnForwarder->logger, log_level, arduino_serial_log_init) != 0) {
#else
  if (MqttSnLoggerInit(&egw->logger, log_level, stdout_log_init) != 0) {
#endif
    MqttSnLoggerDeinit(&egw->logger);
    return -1;
  }
  egw->clientNetwork.logger = &egw->logger;
#endif

  egw->clientNetworkContext = clientNetworkContext;
  MqttSnFixedSizeRingBufferInit(&egw->clientNetworkReceiveBuffer);
  MqttSnFixedSizeRingBufferInit(&egw->clientNetworkSendBuffer);

#ifdef WITH_LOGGING
  if (log_status(&egw->logger)) {
    EchoGatewayDeinit(egw);
    return -1;
  }
#endif

  if (PlatformCompatibilityGetTimestampMs(&egw->last_advertisement_send) < 0) {
    EchoGatewayDeinit(egw);
    MqttSnLoggerDeinit(&egw->logger);
    return -1;
  }
  egw->advertisement_config = (*advertise_config);

  return 0;
}
int32_t EchoGatewayConnect(EchoGateway *egw) {
  if (ClientNetworkConnect(&egw->clientNetwork, egw->clientNetworkContext) != 0) {
    EchoGatewayDeinit(egw);
    return -1;
  }
  return 0;
}
int32_t EchoGatewayDisconnect(EchoGateway *egw) {
  ClientNetworkDisconnect(&egw->clientNetwork, egw->clientNetworkContext);
  return 0;
}
int32_t EchoGatewayDeinit(EchoGateway *egw) {
  ClientNetworkDeinitialize(&egw->clientNetwork, egw->clientNetworkContext);
#ifdef WITH_LOGGING
  egw->logger.log_deinit(&egw->logger);
#endif
  return 0;
}
int32_t EchoGatewayLoop(EchoGateway *egw) {
  if (ClientNetworkReceive(&egw->clientNetwork,
                           &egw->clientNetworkReceiveBuffer,
                           egw->clientNetworkReceiveTimeout,
                           egw->clientNetworkContext) < 0) {
    ClientNetworkDisconnect(&egw->clientNetwork, egw->clientNetworkContext);
  }
#ifdef Arduino_h
  yield();
#endif
  if (!isEmpty(&egw->clientNetworkReceiveBuffer) && !isFull(&egw->clientNetworkReceiveBuffer)) {
    MqttSnMessageData msg = {0};
    if (pop(&egw->clientNetworkReceiveBuffer, &msg) < 0) {
      // should never happen
      return -1;
    }

    if (EchoGatewayHandleSearchGwMessage(egw, &msg) < 0) {
      return -1;
    }

    if (!memcmp(msg.to.bytes, egw->clientNetwork.client_network_address->bytes, sizeof(device_address))) {
      device_address tmp_from = msg.from;
      msg.from = msg.to;
      msg.to = tmp_from;

      if (put(&egw->clientNetworkSendBuffer, &msg) < 0) {
        // should never happen
        return -1;
      }
    }

  }
#ifdef Arduino_h
  yield();
#endif
  if (EchoGatewayHandleAdvertise(egw) < 0) {
    return -1;
  }
#ifdef Arduino_h
  yield();
#endif
  if (ClientNetworkSend(&egw->clientNetwork,
                        &egw->clientNetworkSendBuffer,
                        egw->clientNetworkSendTimeout,
                        egw->clientNetworkContext) < 0) {
    ClientNetworkDisconnect(&egw->clientNetwork, egw->clientNetworkContext);
  }
  return 0;
}
int32_t EchoGatewayHandleSearchGwMessage(EchoGateway *egw, MqttSnMessageData *msg) {
  uint8_t radius = 0;
  if (parse_searchgw_message_byte(&radius, msg->data, msg->data_length) < 0) {
    return 0;
  }

  memset(msg->data, 0, sizeof(msg->data));
  msg->data_length = 0;
  int32_t gen_rc = generate_gwinfo_message(msg->data, sizeof(msg->data), egw->advertisement_config.gateway_id, NULL, 0);
  if (gen_rc < 0) {
    return -1;
  }
  msg->data_length = gen_rc;
  msg->from = (*egw->clientNetwork.client_network_address);
  msg->to = (*egw->clientNetwork.client_network_broadcast_address);

  put(&egw->clientNetworkSendBuffer, msg);
  return 0;
}
int32_t EchoGatewayHandleAdvertise(EchoGateway *egw) {
  uint64_t current_timestamp = 0;
  if (PlatformCompatibilityGetTimestampMs(&current_timestamp) < 0) {
    return -1;
  }
  if (current_timestamp - egw->last_advertisement_send < (uint64_t) egw->advertisement_config.advertisement_duration) {
    return 0;
  }
  if (PlatformCompatibilityGetTimestampMs(&egw->last_advertisement_send) < 0) {
    return -1;
  }
  MqttSnMessageData msg = {0};
  int32_t gen_rc = generate_advertise_message(msg.data,
                                              sizeof(msg.data),
                                              egw->advertisement_config.gateway_id,
                                              egw->advertisement_config.advertisement_duration);
  if (gen_rc < 0) {
    return -1;
  }
  msg.data_length = gen_rc;
  msg.from = (*egw->clientNetwork.client_network_address);
  msg.to = (*egw->clientNetwork.client_network_broadcast_address);
  msg.signal_strength = egw->advertisement_config.advertisement_radius;

  put(&egw->clientNetworkSendBuffer, &msg);
  return 0;
}
