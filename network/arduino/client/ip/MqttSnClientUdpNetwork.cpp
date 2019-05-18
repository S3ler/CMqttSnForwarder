//
// Created by SomeDude on 29.04.2019.
//

#include "MqttSnClientUdpNetwork.hpp"
#include <network/arduino/shared/ip/ArduinoIpAddressHelper.h>
#include <network/arduino/shared/ip/udp/UdpHelper.h>
#include <network/shared/ip/IpHelper.h>
#include <parser/MqttSnMessageParser.h>
#include <string.h>

int32_t ClientArduinoUdpInitialize(MqttSnClientNetworkInterface *n, void *context) {
  MqttSnClientUdpNetwork *udpContext = (MqttSnClientUdpNetwork *) context;
  memset(udpContext, 0, sizeof(MqttSnClientUdpNetwork));
  strcpy(udpContext->protocol, CMQTTSNFORWARDER_MQTTSNCLIENTARDUINOUDPNETWORKPROTOCOL);
  udpContext->unicast_socket = clientUdp;
#ifdef WITH_UDP_BROADCAST_CLIENT
  udpContext->multicast_socket = clientBCUdp;
#endif
  n->initialize = ClientArduinoUdpInitialize;
  n->deinitialize = ClientArduinoUdpDeinitialize;
  n->connect = ClientArduinoUdpConnect;
  n->disconnect = ClientArduinoUdpDisconnect;
  n->send = ClientArduinoUdpSend;
  n->receive = ClientArduinoUdpReceive;
  return 0;
}

int32_t ClientArduinoUdpDeinitialize(MqttSnClientNetworkInterface *n, void *context) {
  // MqttSnClientUdpNetwork *udpContext = (MqttSnClientUdpNetwork *) context;
  // TODO
  return 0;
}

int32_t ClientArduinoUdpConnect(MqttSnClientNetworkInterface *n, void *context) {
  MqttSnClientUdpNetwork *udpContext = (MqttSnClientUdpNetwork *) context;
  uint16_t gateway_network_port = get_port_from_device_address(n->client_network_address);
  if (arduino_init_udp(udpContext->unicast_socket, gateway_network_port) != 0) {
#ifdef WITH_LOGGING
    log_unicast_socket_failed(n->logger, udpContext->protocol, n->client_network_address);
#endif
    return -1;
  }
#ifdef WITH_LOGGING
  log_opening_unicast_socket(n->logger, udpContext->protocol, n->client_network_address);
#endif

#ifdef WITH_UDP_BROADCAST_CLIENT
  if (n->client_network_broadcast_address) {
    IPAddress broadcast_IPAddress;
    uint16_t broadcast_port;
    arduino_device_address_to_IPAddress_and_port(n->client_network_broadcast_address,
                                                 &broadcast_IPAddress,
                                                 &broadcast_port);
    if (arduino_join_multicast_udp(udpContext->multicast_socket, broadcast_IPAddress, broadcast_port) != 0) {
#ifdef WITH_LOGGING
      log_failed_opening_multicast_socket(n->logger, udpContext->protocol, n->client_network_broadcast_address);
#endif
      return -1;
    }
#ifdef WITH_LOGGING
    log_opening_multicast_socket(n->logger, udpContext->protocol, n->client_network_broadcast_address);
#endif
  }
#endif
  return 0;
}

void ClientArduinoUdpDisconnect(MqttSnClientNetworkInterface *n, void *context) {
  MqttSnClientUdpNetwork *udpContext = (MqttSnClientUdpNetwork *) context;
  if ((*udpContext->unicast_socket)) {
    arduino_deinit_udp(udpContext->unicast_socket);
#ifdef WITH_LOGGING
    log_close_unicast_socket(n->logger, udpContext->protocol, n->client_network_address);
#endif
  }
#ifdef WITH_UDP_BROADCAST_CLIENT
  if (n->client_network_broadcast_address) {
    if (*udpContext->multicast_socket) {
      log_close_multicast_socket(n->logger, udpContext->protocol, n->client_network_broadcast_address);
    }
  }
#endif
}

int32_t ClientArduinoUdpSend(MqttSnClientNetworkInterface *n,
                             const device_address *from,
                             const device_address *to,
                             const uint8_t *data,
                             uint16_t data_length,
                             uint8_t signal_strength,
                             int32_t timeout_ms,
                             void *context) {
  MqttSnClientUdpNetwork *udpContext = (MqttSnClientUdpNetwork *) context;

  MqttSnMessageData toSend = {0};
  if (pop(sendBuffer, &toSend)) {
    return 0;
  }

#ifdef WITH_DEBUG_LOGGING
  log_db_send_client_message(n->logger, n->mqtt_sn_gateway_address, &toSend.address, toSend.data, toSend.data_length);
#endif

#ifdef WITH_UDP_BROADCAST_CLIENT
  if (n->client_network_broadcast_address) {
    if (toSend.broadcast_radius &&
        memcmp(&toSend.address, n->client_network_broadcast_address, sizeof(device_address)) == 0) {
      return arduino_send_multicast_udp(udpContext->unicast_socket, &toSend.address, toSend.data, toSend.data_length);
    }
  }
#endif
  return arduino_send_udp(udpContext->unicast_socket, &toSend.address, toSend.data, toSend.data_length);
}

int32_t ClientArduinoUdpReceive(MqttSnClientNetworkInterface *n,
                                device_address *from,
                                device_address *to,
                                uint8_t *data,
                                uint16_t max_data_length,
                                uint8_t *signal_strength,
                                int32_t timeout_ms,
                                void *context) {
  MqttSnClientUdpNetwork *udpContext = (MqttSnClientUdpNetwork *) context;

  MqttSnMessageData toReceive = {0};

  int unicast_rc = arduino_receive_udp(udpContext->unicast_socket,
                                       &toReceive.address,
                                       toReceive.data,
                                       &toReceive.data_length);
  if (unicast_rc < 0) {
    return -1;
  }
  if (unicast_rc > 0) {
    put(receiveBuffer, &toReceive);
#ifdef WITH_DEBUG_LOGGING
    const MqttSnMessageData *msg = back(receiveBuffer);
    log_db_rec_client_message(n->logger, &msg->address, n->mqtt_sn_gateway_address, msg->data, msg->data_length);
#endif
  }

#ifdef WITH_UDP_BROADCAST_CLIENT
  if (n->client_network_broadcast_address) {
    memset(&toReceive, 0, sizeof(MqttSnMessageData));
    int multicast_rc = arduino_receive_udp(udpContext->multicast_socket,
                                           &toReceive.address,
                                           toReceive.data,
                                           &toReceive.data_length);
    if (multicast_rc < 0) {
      return -1;
    }
    if (multicast_rc > 0) {
      toReceive.broadcast_radius = MQTT_SN_ENCAPSULATED_MESSAGE_CRTL_BROADCAST_RADIUS;
      put(receiveBuffer, &toReceive);
#ifdef WITH_DEBUG_LOGGING
      const MqttSnMessageData *msg = back(receiveBuffer);
      log_db_rec_client_message(n->logger,
                                &msg->address,
                                n->client_network_broadcast_address,
                                msg->data,
                                msg->data_length);
#endif
    }
  }
#endif

  return 0;
}


