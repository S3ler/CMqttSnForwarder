#ifndef CMQTTSNFORWARDER_MQTTSNCLIENTUDPNETWORK_H
#define CMQTTSNFORWARDER_MQTTSNCLIENTUDPNETWORK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <network/MqttSnClientNetworkInterface.h>

#define CMQTTSNFORWARDER_MQTTSNCLIENTLINUXUDPNETWORKPROTOCOL "udp"

typedef struct MqttSnClientUdpNetwork_ {
  int unicast_socket;
  char protocol[sizeof(CMQTTSNFORWARDER_MQTTSNCLIENTLINUXUDPNETWORKPROTOCOL)];
  uint64_t received_messages;
#ifdef WITH_UDP_BROADCAST
  int multicast_socket;
#endif
} MqttSnClientUdpNetwork;

int32_t ClientLinuxUdpInitialize(MqttSnClientNetworkInterface *n, void *context);
int32_t ClientLinuxUdpDeinitialize(MqttSnClientNetworkInterface *n, void *context);

int32_t ClientLinuxUdpConnect(MqttSnClientNetworkInterface *n, void *context);
int32_t ClientLinuxUdpDisconnect(MqttSnClientNetworkInterface *n, void *context);

int32_t ClientLinuxUdpSend(MqttSnClientNetworkInterface *n,
                           const device_address *from,
                           const device_address *to,
                           const uint8_t *data,
                           uint16_t data_length,
                           uint8_t signal_strength,
                           int32_t timeout_ms,
                           void *context);
int32_t ClientLinuxUdpReceive(MqttSnClientNetworkInterface *n,
                              device_address *from,
                              device_address *to,
                              uint8_t *data,
                              uint16_t max_data_length,
                              uint8_t *signal_strength,
                              int32_t timeout_ms,
                              void *context);

int32_t ClientLinuxUdpReceiveUnicast(MqttSnClientNetworkInterface *n,
                                     device_address *from,
                                     device_address *to,
                                     uint8_t *data,
                                     uint16_t max_data_length,
                                     MqttSnClientUdpNetwork *udpNetwork);
int32_t ClientLinuxUdpReceiveMulticast(MqttSnClientNetworkInterface *n,
                                       device_address *from,
                                       device_address *to,
                                       uint8_t *data,
                                       uint16_t max_data_length,
                                       uint8_t *signal_strength,
                                       MqttSnClientUdpNetwork *udpNetwork);
#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_MQTTSNCLIENTUDPNETWORK_H
