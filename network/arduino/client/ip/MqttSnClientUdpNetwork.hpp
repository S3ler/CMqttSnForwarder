//
// Created by SomeDude on 29.04.2019.
//

#ifndef CMQTTSNFORWARDER_FORWARDER_NETWORK_ARDUINO_CLIENT_IP_MQTTSNCLIENTUDPNETWORK_H_
#define CMQTTSNFORWARDER_FORWARDER_NETWORK_ARDUINO_CLIENT_IP_MQTTSNCLIENTUDPNETWORK_H_

#include <network/MqttSnClientNetworkInterface.h>
#if defined(WIFIUDP_H) || defined(WITH_UDP_BROADCAST_CLIENT)
#include <WiFiUdp.h>
extern WiFiUDP *clientUdp;
extern WiFiUDP *clientBCUdp;
#elif defined(WITH_UDP_BROADCAST_CLIENT)
#error "Cannot use WITH_UDP_BROADCAST_CLIENT without WIFIUDP_H"
#else
#include <Udp.h>
extern UDP *clientUdp;
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define CMQTTSNFORWARDER_MQTTSNCLIENTARDUINOUDPNETWORKPROTOCOL "udp"

typedef struct MqttSnClientUdpNetwork_ {
  uint16_t port;
#if defined(WIFIUDP_H) || defined(WITH_UDP_BROADCAST_CLIENT)
  WiFiUDP *unicast_socket;
  WiFiUDP *multicast_socket;
#elif defined(WITH_UDP_BROADCAST_CLIENT)
#error "Cannot use WITH_UDP_BROADCAST_CLIENT without WIFIUDP_H"
#else
  UDP *unicast_socket;
#endif
  char protocol[sizeof("CMQTTSNFORWARDER_MQTTSNCLIENTARDUINOUDPNETWORKPROTOCOL")];
} MqttSnClientUdpNetwork;

int32_t ClientArduinoUdpInitialize(MqttSnClientNetworkInterface *n, void *context);
int32_t ClientArduinoUdpDeinitialize(MqttSnClientNetworkInterface *n, void *context);

int32_t ClientArduinoUdpConnect(MqttSnClientNetworkInterface *n, void *context);
int32_t ClientArduinoUdpDisconnect(MqttSnClientNetworkInterface *n, void *context);

int32_t ClientArduinoUdpSend(MqttSnClientNetworkInterface *n,
                             const device_address *from,
                             const device_address *to,
                             const uint8_t *data,
                             uint16_t data_length,
                             uint8_t signal_strength,
                             int32_t timeout_ms,
                             void *context);
int32_t ClientArduinoUdpReceive(MqttSnClientNetworkInterface *n,
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

#endif //CMQTTSNFORWARDER_FORWARDER_NETWORK_ARDUINO_CLIENT_IP_MQTTSNCLIENTUDPNETWORK_H_
