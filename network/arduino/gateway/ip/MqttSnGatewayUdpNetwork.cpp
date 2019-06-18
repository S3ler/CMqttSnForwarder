//
// Created by SomeDude on 29.04.2019.
//

#include "MqttSnGatewayUdpNetwork.hpp"
#include <network/arduino/shared/ip/ArduinoIpAddressHelper.hpp>
#include <network/arduino/shared/ip/udp/UdpHelper.hpp>
#include <network/shared/ip/IpHelper.h>
#include <network/shared/ip/IpHelperLogging.h>
#include <network/shared/gateway/logging/MqttSnDebugMessageLogging.h>
#include <parser/MqttSnMessageParser.h>
#include <string.h>

int32_t GatewayArduinoUdpInitialize(MqttSnGatewayNetworkInterface *n, void *context) {
  MqttSnGatewayUdpNetwork *udpContext = (MqttSnGatewayUdpNetwork *) context;
  memset(udpContext, 0, sizeof(MqttSnGatewayUdpNetwork));
  strcpy(udpContext->protocol, CMQTTSNFORWARDER_MQTTSNGATEWAYARDUINOUDPNETWORKPROTOCOL);
  udpContext->unicast_socket = gatewayUdp;
#ifdef WITH_UDP_BROADCAST_GATEWAY
  udpContext->multicast_socket = gatewayBCUdp;
#endif
  n->initialize = GatewayArduinoUdpInitialize;
  n->deinitialize = GatewayArduinoUdpDeinitialize;
  n->connect = GatewayArduinoUdpConnect;
  n->disconnect = GatewayArduinoUdpDisconnect;
  n->send = GatewayArduinoUdpSend;
  n->receive = GatewayArduinoUdpReceive;

  return 0;
}

int32_t GatewayArduinoUdpDeinitialize(MqttSnGatewayNetworkInterface *n, void *context) {
  // MqttSnGatewayUdpNetwork *udpContext = (MqttSnGatewayUdpNetwork *) context;
  // TODO
  return 0;
}

int32_t GatewayArduinoUdpConnect(MqttSnGatewayNetworkInterface *n, void *context) {
  MqttSnGatewayUdpNetwork *udpContext = (MqttSnGatewayUdpNetwork *) context;
  if (n->mqtt_sn_gateway_address == NULL) {
    // FEATURE implement searching for gateway
    return -1;
  }
  uint16_t gateway_network_port = get_port_from_device_address(n->gateway_network_address);
  if (arduino_init_udp(udpContext->unicast_socket, gateway_network_port) != 0) {
#ifdef WITH_LOGGING
    log_unicast_socket_failed(n->logger, udpContext->protocol, n->gateway_network_address);
#endif
    return -1;
  }
#ifdef WITH_LOGGING
  log_opening_unicast_socket(n->logger, udpContext->protocol, n->gateway_network_address);
#endif

#ifdef WITH_UDP_BROADCAST_GATEWAY
  if (n->gateway_network_broadcast_address) {
    IPAddress broadcast_IPAddress;
    uint16_t broadcast_port;
    arduino_device_address_to_IPAddress_and_port(n->gateway_network_broadcast_address,
                                                 &broadcast_IPAddress,
                                                 &broadcast_port);
    if (arduino_join_multicast_udp(udpContext->multicast_socket, broadcast_IPAddress, broadcast_port) != 0) {
#ifdef WITH_LOGGING
      log_failed_opening_multicast_socket(n->logger, udpContext->protocol, n->gateway_network_broadcast_address);
#endif
      return -1;
    }
#ifdef WITH_LOGGING
    log_opening_multicast_socket(n->logger, udpContext->protocol, n->gateway_network_broadcast_address);
#endif
  }
#endif

  return 0;
}

int32_t GatewayArduinoUdpDisconnect(MqttSnGatewayNetworkInterface *n, void *context) {
  MqttSnGatewayUdpNetwork *udpContext = (MqttSnGatewayUdpNetwork *) context;
  if ((udpContext->unicast_socket)) {
    arduino_deinit_udp(udpContext->unicast_socket);
#ifdef WITH_LOGGING
    log_close_unicast_socket(n->logger, udpContext->protocol, n->gateway_network_address);
#endif
  }
#ifdef WITH_UDP_BROADCAST_GATEWAY
  if (n->gateway_network_broadcast_address) {
    if ((udpContext->multicast_socket)) {
      arduino_deinit_udp(udpContext->multicast_socket);
      log_close_multicast_socket(n->logger, udpContext->protocol, n->gateway_network_broadcast_address);
    }
  }
#endif
  return 0;
}

int32_t GatewayArduinoUdpSend(MqttSnGatewayNetworkInterface *n,
                              const device_address *from,
                              const device_address *to,
                              const uint8_t *data,
                              uint16_t data_length,
                              uint8_t signal_strength,
                              int32_t timeout_ms,
                              void *context) {
  MqttSnGatewayUdpNetwork *udpContext = (MqttSnGatewayUdpNetwork *) context;

#ifdef WITH_DEBUG_LOGGING
  log_db_send_gateway_message(n->logger, from, to, data, data_length);
#endif

#ifdef WITH_UDP_BROADCAST_GATEWAY
  if (n->gateway_network_broadcast_address) {
    if (memcmp(to, n->gateway_network_broadcast_address, sizeof(device_address)) == 0) {
      return arduino_send_multicast_udp(udpContext->unicast_socket, to, data, data_length);
    }
  }
#endif
  return arduino_send_udp(udpContext->unicast_socket, to, data, data_length);
}

int32_t GatewayArduinoUdpReceive(MqttSnGatewayNetworkInterface *n,
                                 device_address *from,
                                 device_address *to,
                                 uint8_t *data,
                                 uint16_t max_data_length,
                                 uint8_t *signal_strength,
                                 int32_t timeout_ms,
                                 void *context) {
  MqttSnGatewayUdpNetwork *udpContext = (MqttSnGatewayUdpNetwork *) context;

#ifdef WITH_UDP_BROADCAST_CLIENT
  if (n->gateway_network_broadcast_address)
  {
    if (udpContext->received_messages % 2)
    {
      int multicast_rc = arduino_receive_udp(udpContext->multicast_socket, from, to, data, max_data_length);
      if (multicast_rc < 0)
      {
        return multicast_rc;
      }
      if (multicast_rc > 0)
      {
#ifdef WITH_DEBUG_LOGGING
        log_db_rec_gateway_message(n->logger, from, to, data, multicast_rc);
#endif
        return multicast_rc;
      }
    }
  }
#endif

  int32_t unicast_rc = arduino_receive_udp(udpContext->unicast_socket, from, to, data, max_data_length);
#ifdef WITH_DEBUG_LOGGING
  if (unicast_rc > 0)
  {
    log_db_rec_gateway_message(n->logger, from, to, data, unicast_rc);
  }
#endif

  return unicast_rc;
}
