//
// Created by SomeDude on 29.04.2019.
//

#ifndef CMQTTSNFORWARDER_FORWARDER_NETWORK_ARDUINO_GATEWAY_IP_MQTTSNGATEWAYUDPNETWORK_H_
#define CMQTTSNFORWARDER_FORWARDER_NETWORK_ARDUINO_GATEWAY_IP_MQTTSNGATEWAYUDPNETWORK_H_

#include <network/MqttSnGatewayNetworkInterface.h>
#if defined(WIFIUDP_H) || defined(WITH_UDP_BROADCAST_GATEWAY) || defined(_WIFIUDP_H_)
#include <WiFiUdp.h>
extern WiFiUDP *gatewayUdp;
extern WiFiUDP *gatewayBCUdp;
#elif defined(WITH_UDP_BROADCAST_GATEWAY)
#error "Cannot use WITH_UDP_BROADCAST_GATEWAY without WIFIUDP_H"
#else
#include <Udp.h>
extern UDP *gatewayUdp;
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define CMQTTSNFORWARDER_MQTTSNGATEWAYARDUINOUDPNETWORKPROTOCOL "udp"

typedef struct MqttSnGatewayUdpNetwork_ {
  uint16_t port;
#if defined(WIFIUDP_H) || defined(WITH_UDP_BROADCAST_GATEWAY) || defined(_WIFIUDP_H_)
  WiFiUDP *unicast_socket;
  WiFiUDP *multicast_socket;
#elif defined(WITH_UDP_BROADCAST_GATEWAY)
#error "Cannot use WITH_UDP_BROADCAST_GATEWAY without WIFIUDP_H"
#else
  UDP *unicast_socket;
#endif
  uint64_t received_messages;
  char protocol[sizeof(CMQTTSNFORWARDER_MQTTSNGATEWAYARDUINOUDPNETWORKPROTOCOL)];
} MqttSnGatewayUdpNetwork;

int32_t GatewayArduinoUdpInitialize(MqttSnGatewayNetworkInterface *n, void *context);
int32_t GatewayArduinoUdpDeinitialize(MqttSnGatewayNetworkInterface *n, void *context);

int32_t GatewayArduinoUdpConnect(MqttSnGatewayNetworkInterface *n, void *context);
int32_t GatewayArduinoUdpDisconnect(MqttSnGatewayNetworkInterface *n, void *context);

int32_t GatewayArduinoUdpSend(MqttSnGatewayNetworkInterface *n,
                              const device_address *from,
                              const device_address *to,
                              const uint8_t *data,
                              uint16_t data_length,
                              uint8_t signal_strength,
                              int32_t timeout_ms,
                              void *context);
int32_t GatewayArduinoUdpReceive(MqttSnGatewayNetworkInterface *n,
                                 device_address *from,
                                 device_address *to,
                                 uint8_t *data,
                                 uint16_t max_data_length,
                                 uint8_t *signal_strength,
                                 int32_t timeout_ms,
                                 void *context);

#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_FORWARDER_NETWORK_ARDUINO_GATEWAY_IP_MQTTSNGATEWAYUDPNETWORK_H_
