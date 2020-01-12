//
// Created by SomeDude on 20.06.2019.
//

#ifndef CMQTTSNFORWARDER_GATEWAY_MQTTCLIENT_H_
#define CMQTTSNFORWARDER_GATEWAY_MQTTCLIENT_H_

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct MqttClient_ {
  int status;
} MqttClient;
int32_t MqttClientInitialize(MqttClient *mqtt_client);
int32_t MqttClientDeinitialize(MqttClient *mqtt_client);
int32_t MqttClientNetworkConnect(MqttClient *mqtt_client);
int32_t MqttClientNetworkDisconnect(MqttClient *mqtt_client);
#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_GATEWAY_MQTTCLIENT_H_
