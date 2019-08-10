//
// Created by SomeDude on 27.06.2019.
//

#include <platform/platform_compatibility.h>
#include "MqttSnClientLogger.h"
int32_t log_mqtt_sn_client(const MqttSnLogger *logger, MQTT_SN_CLIENT_RETURN_CODE mqtt_sn_client_return_code) {
  // TODO implement with generation
  switch (mqtt_sn_client_return_code) {
    case MQTT_SN_CLIENT_RETURN_SUCCESS:
      log_str(logger, "MQTT_SN_CLIENT_RETURN_SUCCESS");
      break;
    case MQTT_SN_CLIENT_RETURN_CONJESTION:
      log_str(logger, "MQTT_SN_CLIENT_RETURN_CONJESTION");
      break;
    case MQTT_SN_CLIENT_RETURN_NOT_SUPPORTED:
      log_str(logger, "MQTT_SN_CLIENT_RETURN_NOT_SUPPORTED");
      break;
    case MQTT_SN_CLIENT_RETURN_TIMEOUT:
      log_str(logger, "MQTT_SN_CLIENT_RETURN_TIMEOUT");
      break;
    case MQTT_SN_CLIENT_RETURN_ERROR:
      log_str(logger, "MQTT_SN_CLIENT_RETURN_ERROR");
      break;
    default:break;
  }
  log_flush(logger);

  return log_status(logger);
}
int32_t log_mqtt_sn_client_connection_status(const MqttSnLogger *logger,
                                             uint8_t connection_status,
                                             const device_address *gateway_address) {
  log_msg_start(logger);
  if (connection_status) {
    log_str(logger, PSTR("connected to gateway "));
  }else{
    log_str(logger, PSTR("disconnected from gateway "));
  }
  log_device_address(logger, gateway_address);
  log_str(logger, PSTR("."));
  log_flush(logger);
  return log_status(logger);
}
