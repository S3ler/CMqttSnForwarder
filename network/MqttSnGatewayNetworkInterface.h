//
// Created by bele on 25.01.19.
//

#ifndef CMQTTSNFORWARDER_MQTTSNNETWORKINTERFACE_H
#define CMQTTSNFORWARDER_MQTTSNNETWORKINTERFACE_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <ringbuffer/MqttSnFixedSizeRingBuffer.h>
#ifdef WITH_LOGGING
#include <logging/MqttSnLoggingInterface.h>
#endif

typedef enum MQTT_SN_GATEWAY_NETWORK_INTERFACE_STATUS_ {
  MQTT_SN_GATEWAY_NETWORK_INTERFACE_STATUS_DEINITIALIZED = 0,
  MQTT_SN_GATEWAY_NETWORK_INTERFACE_STATUS_INITIALIZED = 1,
  MQTT_SN_GATEWAY_NETWORK_INTERFACE_STATUS_DISCONNECTED = 2,
  MQTT_SN_GATEWAY_NETWORK_INTERFACE_STATUS_CONNECTED = 3
} MQTT_SN_GATEWAY_NETWORK_INTERFACE_STATUS;

typedef struct MqttSnGatewayNetworkInterface_ {
  MQTT_SN_GATEWAY_NETWORK_INTERFACE_STATUS status;
  uint16_t max_data_length;

  device_address *mqtt_sn_gateway_address;
  device_address *gateway_network_address;
  device_address *gateway_network_broadcast_address;

  int32_t (*initialize)(struct MqttSnGatewayNetworkInterface_ *n, void *context);
  int32_t (*deinitialize)(struct MqttSnGatewayNetworkInterface_ *n, void *context);

  int32_t (*connect)(struct MqttSnGatewayNetworkInterface_ *n, void *context);
  int32_t (*disconnect)(struct MqttSnGatewayNetworkInterface_ *n, void *context);

  int32_t (*send)(struct MqttSnGatewayNetworkInterface_ *n,
                  const device_address *from,
                  const device_address *to,
                  const uint8_t *data,
                  uint16_t data_length,
                  uint8_t signal_strength,
                  int32_t timeout_ms,
                  void *context);
  int32_t (*receive)(struct MqttSnGatewayNetworkInterface_ *n,
                     device_address *from,
                     device_address *to,
                     uint8_t *data,
                     uint16_t data_length,
                     uint8_t *signal_strength,
                     int32_t timeout_ms,
                     void *context);

#ifdef WITH_LOGGING
  MqttSnLogger *logger;
#endif

} MqttSnGatewayNetworkInterface;

int32_t GatewayNetworkInitialize(MqttSnGatewayNetworkInterface *n,
                                 uint16_t max_data_length,
                                 device_address *mqtt_sn_gateway_address,
                                 device_address *gateway_network_address,
                                 device_address *gateway_network_broadcast_address,
                                 void *context,
                                 int (*gateway_network_initialize)(MqttSnGatewayNetworkInterface *, void *));

int32_t GatewayNetworkDeinitialize(MqttSnGatewayNetworkInterface *n, void *context);

int32_t GatewayNetworkConnect(MqttSnGatewayNetworkInterface *n, void *context);

int32_t GatewayNetworkDisconnect(MqttSnGatewayNetworkInterface *n, void *context);

int32_t GatewayNetworkSend(MqttSnGatewayNetworkInterface *n,
                           MqttSnFixedSizeRingBuffer *sendBuffer,
                           int timeout_ms,
                           void *context);

int32_t GatewayNetworkSendTo(MqttSnGatewayNetworkInterface *n,
                             device_address *to,
                             uint8_t *data,
                             uint16_t data_length,
                             uint8_t signal_strength,
                             int32_t timeout_ms,
                             void *context);

int32_t GatewayNetworkReceive(MqttSnGatewayNetworkInterface *n,
                              MqttSnFixedSizeRingBuffer *receiveBuffer,
                              int timeout_ms,
                              void *context);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_MQTTSNNETWORKINTERFACE_H
