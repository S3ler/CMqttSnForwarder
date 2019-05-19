//
// Created by bele on 26.01.19.
//

#ifndef CMQTTSNFORWARDER_MQTTSNGATEWAYTCPNETWORK_H
#define CMQTTSNFORWARDER_MQTTSNGATEWAYTCPNETWORK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <network/MqttSnGatewayNetworkInterface.h>
#include <network/linux/gateway/ip/udp/MqttSnGatewayUdpNetwork.h>

#define CMQTTSNFORWARDER_MQTTSNGATEWAYLINUXTCPNETWORKPROTOCOL "tcp"
#define CMQTTSNFORWARDER_MQTTSNGATEWAYTCPNETWORK_MAX_DATA_LENGTH 1024

typedef struct MqttSnGatewayTcpNetwork_ {
  int mqtt_sg_gateway_fd;
  uint8_t gateway_buffer[CMQTTSNFORWARDER_MQTTSNGATEWAYTCPNETWORK_MAX_DATA_LENGTH];
  uint16_t gateway_buffer_bytes;
  uint32_t to_drop_bytes;
  char protocol[sizeof(CMQTTSNFORWARDER_MQTTSNGATEWAYLINUXTCPNETWORKPROTOCOL)];
  uint64_t received_messages;
#ifdef WITH_LINUX_TCP_GATEWAY_NETWORK_BROADCAST
  MqttSnGatewayUdpNetwork udp_multicast_network;
#endif
} MqttSnGatewayTcpNetwork;

int32_t GatewayLinuxTcpInitialize(MqttSnGatewayNetworkInterface *n, void *context);
int32_t GatewayLinuxTcpDeinitialize(MqttSnGatewayNetworkInterface *n, void *context);

int32_t GatewayLinuxTcpConnect(MqttSnGatewayNetworkInterface *n, void *context);
int32_t GatewayLinuxTcpDisconnect(MqttSnGatewayNetworkInterface *n, void *context);

int32_t GatewayLinuxTcpSend(MqttSnGatewayNetworkInterface *n,
                            const device_address *from,
                            const device_address *to,
                            const uint8_t *data,
                            uint16_t data_length,
                            uint8_t signal_strength,
                            int32_t timeout_ms,
                            void *context);
int32_t GatewayLinuxTcpReceive(MqttSnGatewayNetworkInterface *n,
                               device_address *from,
                               device_address *to,
                               uint8_t *data,
                               uint16_t max_data_length,
                               uint8_t *signal_strength,
                               int32_t timeout_ms,
                               void *context);

int32_t GatewayLinuxTcpReceiveUnicast(MqttSnGatewayNetworkInterface *n,
                                      device_address *from,
                                      device_address *to,
                                      uint8_t *data,
                                      uint16_t max_data_length,
                                      MqttSnGatewayTcpNetwork *tcp_network);

#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_MQTTSNGATEWAYTCPNETWORK_H
