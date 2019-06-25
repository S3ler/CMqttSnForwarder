//
// Created by SomeDude on 31.05.2019.
//

#include <logging/MqttSnLoggingInterface.h>
#include <logging/linux/stdout/StdoutLogging.h>
#include <parser/MqttSnPublishMessage.h>
#include "MqttSnClient.h"
#include <string.h>

int32_t MqttSnClientInit(MqttSnClient *client,
                         log_level_t log_level,
                         int32_t (*publish_cb)(MqttSnClient, const char *, uint16_t, uint8_t *, uint16_t),
                         void *gatewayNetworkContext) {
  //memset(client, 0, sizeof(*client));

  client->default_timeout = MQTT_SN_CLIENT_DEFAULT_TIMEOUT_MS;
  client->default_signal_strength = MQTT_SN_CLIENT_DEFAULT_SIGNAL_STRENGTH;
  client->status = 0;
  client->msg_counter = 7;
  client->connect_duration = MQTT_SN_CLIENT_DEFAULT_CONNECT_DURATION;

#ifdef WITH_LOGGING
#if defined(Arduino_h) || defined(WITH_PLATFORMIO)
  if (MqttSnLoggerInit(&mqttSnForwarder->logger, log_level, arduino_serial_log_init) != 0) {
#else
  if (MqttSnLoggerInit(&client->logger, log_level, stdout_log_init) != 0) {
#endif
    MqttSnLoggerDeinit(&client->logger);
    return -1;
  }
  client->gatewayNetwork.logger = &client->logger;
#endif

  client->gatewayNetworkContext = gatewayNetworkContext;

  if (GatewayNetworkConnect(&client->gatewayNetwork, client->gatewayNetworkContext) != 0) {
    return -1;
  }
  client->mqtt_sn_gateway_address = client->gatewayNetwork.mqtt_sn_gateway_address;

  return 0;
}
int32_t MqttSnClientDeinit(MqttSnClient *client) {
  GatewayNetworkDisconnect(&client->gatewayNetwork, client->gatewayNetworkContext);
  GatewayNetworkDeinitialize(&client->gatewayNetwork, client->gatewayNetworkContext);
  return 0;
}
int32_t MqttSnClientLoop(MqttSnClient *client) {
  // TODO do stuff
  return 0;
}
int32_t MqttSnClientPublishPredefinedM1(MqttSnClient *client,
                                        uint16_t predefined_topic_id,
                                        uint8_t retain,
                                        uint8_t *data,
                                        uint16_t data_len) {
  uint8_t msg_data[MQTT_SN_MAXIMUM_MESSAGE_DATA_LENGTH];
  memset(&msg_data, 0, sizeof(msg_data));

  int32_t gen_rc = generate_publish(msg_data,
                                sizeof(msg_data),
                                0,
                                -1,
                                retain,
                                MQTT_SN_FLAG_TOPIC_ID_TYPE_PREDEFINED_TOPIC_ID,
                                predefined_topic_id,
                                client->msg_counter++,
                                data,
                                data_len);
  if (gen_rc < 0) {
    return -1;
  }
  int32_t send_rc = GatewayNetworkSendTo(&client->gatewayNetwork,
                                         client->mqtt_sn_gateway_address,
                                         msg_data,
                                         gen_rc,
                                         client->default_signal_strength,
                                         client->gatewayNetworkSendTimeout,
                                         client->gatewayNetworkContext);

  if (send_rc < 0) {
    return -1;
  }
  return send_rc;
}
MQTT_SN_RETURN_CODE MqttSnClientConnect(MqttSnClient *client) {
    // gen
    // send
   // message inflight - await
   // await
  // TODO implement me
  return RETURN_CODE_RESERVED_INVALID;
}
