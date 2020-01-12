//
// Created by SomeDude on 16.09.2019.
//

#ifndef CMQTTSNFORWARDER_MQTT_SN_CLIENT_INTERACTIVE_TCP_BUFFER_H
#define CMQTTSNFORWARDER_MQTT_SN_CLIENT_INTERACTIVE_TCP_BUFFER_H

#include <stdint.h>
#include <platform/device_address.h>
#include <network/linux/gateway/ip/tcp/MqttSnGatewayTcpNetwork.h>
#include <config/client/mqtt_sn_gateway_config.h>
#include <config/network/gateway/gateway_network_config.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct interactive_client_tcp_buffer_ {
    device_address          mqttSnGatewayNetworkAddress;
    device_address          forwarderGatewayNetworkAddress;
    device_address          forwarderGatewayNetworkBroadcastAddress;
    MqttSnGatewayTcpNetwork tcpGatewayNetworkContext;
} interactive_client_tcp_buffer;

void    interactive_client_tcp_buffer_init(interactive_client_tcp_buffer *cfg);
int32_t interactive_client_tcp_buffer_start(const MqttSnLogger *logger, const mqtt_sn_gateway_config *msngcfg,
                                            const gateway_network_config *gncfg, MqttSnGatewayNetworkInterface *gatewayNetwork,
                                            interactive_client_tcp_buffer *gatewayNetworkContext, device_address *mqtt_sn_gateway_address);

#ifdef __cplusplus
}
#endif
#endif  //CMQTTSNFORWARDER_MQTT_SN_CLIENT_INTERACTIVE_TCP_BUFFER_H
