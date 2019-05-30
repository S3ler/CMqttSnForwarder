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

  if (ClientNetworkConnect(&egw->clientNetwork, egw->clientNetworkContext) != 0) {
    EchoGatewayDeinit(egw);
    return -1;
  }

#ifdef WITH_LOGGING
  if (log_status(&egw->logger)) {
    EchoGatewayDeinit(egw);
    return -1;
  }
#endif

  return 0;
}
int32_t EchoGatewayDeinit(EchoGateway *egw) {
  ClientNetworkDisconnect(&egw->clientNetwork, egw->clientNetworkContext);
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

int32_t log_echogateway_started(const MqttSnLogger *logger, const mqtt_sn_version_config *msvcfg) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return log_status(logger);
  }

  log_msg_start(logger);
  log_str(logger, PSTR("echogateway version "));
  log_str(logger, msvcfg->version);
  log_str(logger, PSTR(" is "));
  log_uint32(logger, msvcfg->major);
  log_str(logger, PSTR("."));
  log_uint32(logger, msvcfg->minor);
  log_str(logger, PSTR("."));
  log_uintmax(logger, msvcfg->tweak);
  log_str(logger, PSTR(" (build date "));
  log_str(logger, msvcfg->build_date);
  log_str(logger, PSTR(") started."));
  log_flush(logger);
  return log_status(logger);
}
int32_t log_echogateway_terminated(const MqttSnLogger *logger, const mqtt_sn_version_config *msvcfg) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_DEFAULT)) {
    return log_status(logger);
  }

  log_msg_start(logger);
  log_str(logger, PSTR("echogateway version "));
  log_str(logger, msvcfg->version);
  log_str(logger, PSTR(" is "));
  log_uint32(logger, msvcfg->major);
  log_str(logger, PSTR("."));
  log_uint32(logger, msvcfg->minor);
  log_str(logger, PSTR("."));
  log_uintmax(logger, msvcfg->tweak);
  log_str(logger, PSTR(" terminated."));
  log_flush(logger);
  return log_status(logger);
}


