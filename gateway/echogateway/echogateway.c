//
// Created by SomeDude on 30.05.2019.
//

#include <platform/platform_compatibility.h>
#include <network/MqttSnClientNetworkInterface.h>
#include "echogateway.h"

int32_t EchoGatewayInit(EchoGateway *egw, log_level_t log_level, void *clientNetworkContext) {
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
                           egw->clientNetworkContext)) {
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

    device_address tmp_from = msg.from;
    msg.from = msg.to;
    msg.to = tmp_from;

    if (put(&egw->clientNetworkSendBuffer, &msg) < 0) {
      // should never happen
      return -1;
    }
  }
#ifdef Arduino_h
  yield();
#endif
  if (ClientNetworkSend(&egw->clientNetwork,
                        &egw->clientNetworkSendBuffer,
                        egw->clientNetworkSendTimeout,
                        egw->clientNetworkContext)) {
    ClientNetworkDisconnect(&egw->clientNetwork, egw->clientNetworkContext);
  }
  return 0;
}


