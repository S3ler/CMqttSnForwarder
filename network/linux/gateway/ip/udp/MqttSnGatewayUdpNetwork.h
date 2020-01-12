//
// Created by bele on 26.01.19.
//

#ifndef CMQTTSNFORWARDER_MQTTSNGATEWAYUDPNETWORK_H
#define CMQTTSNFORWARDER_MQTTSNGATEWAYUDPNETWORK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <network/MqttSnGatewayNetworkInterface.h>

#define CMQTTSNFORWARDER_MQTTSNGATEWAYLINUXUDPNETWORKPROTOCOL "udp"

typedef struct MqttSnGatewayUdpNetwork_ {
  int unicast_socket;
  char protocol[sizeof(CMQTTSNFORWARDER_MQTTSNGATEWAYLINUXUDPNETWORKPROTOCOL)];
  uint64_t received_messages;
#ifdef WITH_LINUX_UDP_NETWORK_BROADCAST
  int multicast_socket;
#endif
} MqttSnGatewayUdpNetwork;

int32_t GatewayLinuxUdpInitialize(MqttSnGatewayNetworkInterface *n, void *context);
int32_t GatewayLinuxUdpDeinitialize(MqttSnGatewayNetworkInterface *n, void *context);

int32_t GatewayLinuxUdpConnect(MqttSnGatewayNetworkInterface *n, void *context);
int32_t GatewayLinuxUdpDisconnect(MqttSnGatewayNetworkInterface *n, void *context);

int32_t GatewayLinuxUdpSend(MqttSnGatewayNetworkInterface *n,
                            const device_address *from,
                            const device_address *to,
                            const uint8_t *data,
                            uint16_t data_length,
                            uint8_t signal_strength,
                            int32_t timeout_ms,
                            void *context);
int32_t GatewayLinuxUdpReceive(MqttSnGatewayNetworkInterface *n,
                               device_address *from,
                               device_address *to,
                               uint8_t *data,
                               uint16_t max_data_length,
                               uint8_t *signal_strength,
                               int32_t timeout_ms,
                               void *context);

int32_t GatewayLinuxUdpReceiveUnicast(MqttSnGatewayNetworkInterface *n,
                                      device_address *from,
                                      device_address *to,
                                      uint8_t *data,
                                      uint16_t max_data_length,
                                      MqttSnGatewayUdpNetwork *udpNetwork);
#ifdef WITH_LINUX_UDP_NETWORK_BROADCAST
int32_t GatewayLinuxUdpReceiveMulticast(MqttSnGatewayNetworkInterface *n,
                                        device_address *from,
                                        device_address *to,
                                        uint8_t *data,
                                        uint16_t max_data_length,
                                        uint8_t *signal_strength,
                                        MqttSnGatewayUdpNetwork *udpNetwork);
#endif

#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_MQTTSNGATEWAYUDPNETWORK_H
