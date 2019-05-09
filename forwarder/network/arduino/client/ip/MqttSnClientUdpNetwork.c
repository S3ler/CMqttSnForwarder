//
// Created by SomeDude on 29.04.2019.
//

#include "MqttSnClientUdpNetwork.h"
#include <forwarder/network/arduino/shared/ip/ArduinoIpAddressHelper.h>
#include <forwarder/network/arduino/shared/ip/udp/UdpHelper.h>
#include <forwarder/network/shared/ip/IpHelper.h>
#include <forwarder/MqttSnMessageParser.h>
#include <string.h>

int ClientArduinoUdpInit(MqttSnClientNetworkInterface *n, void *context) {
  MqttSnClientUdpNetwork *udpContext = (MqttSnClientUdpNetwork *) context;
  memset(udpContext, 0, sizeof(MqttSnClientUdpNetwork));
  strcpy(udpContext->protocol, CMQTTSNFORWARDER_MQTTSNCLIENTARDUINOUDPNETWORKPROTOCOL);
  udpContext->udp = clientUdp;
  n->client_network_init = ClientArduinoUdpInit;
  n->client_network_receive = ClientArduinoUdpReceive;
  n->client_network_send = ClientArduinoUdpSend;
  n->client_network_connect = ClientArduinoUdpConnect;
  n->client_network_disconnect = ClientArduinoUdpDisconnect;
  return 0;
}

int ClientArduinoUdpConnect(MqttSnClientNetworkInterface *n, void *context) {
  MqttSnClientUdpNetwork *udpContext = (MqttSnClientUdpNetwork *) context;
  uint16_t gateway_network_port = get_port_from_device_address(n->client_network_address);
  if (arduino_init_udp(udpContext->udp, gateway_network_port) != 0) {
#ifdef WITH_LOGGING
    log_unicast_socket_failed(n->logger, udpContext->protocol, n->client_network_address);
#endif
    return -1;
  }
#ifdef WITH_LOGGING
  log_opening_unicast_socket(n->logger, udpContext->protocol, n->client_network_address);
#endif

#ifdef WITH_UDP_BROADCAST
  if (n->client_network_broadcast_address) {
    IPAddress broadcast_IPAddress;
    uint16_t broadcast_port;
    arduino_device_address_to_IPAddress_and_port(n->client_network_broadcast_address,
                                                 &broadcast_IPAddress,
                                                 &broadcast_port);
    if (arduino_join_multicast_udp(udpContext->udp, broadcast_IPAddress, broadcast_port) != 0) {
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
  arduino_deinit_udp(udpContext->udp);
#ifdef WITH_LOGGING
  log_close_unicast_socket(n->logger, udpContext->protocol, n->client_network_address);
#endif
#ifdef WITH_UDP_BROADCAST
  if (n->client_network_broadcast_address) {
    log_close_multicast_socket(n->logger, udpContext->protocol, n->client_network_broadcast_address);
  }
#endif
}

int ClientArduinoUdpReceive(MqttSnClientNetworkInterface *n,
                            MqttSnFixedSizeRingBuffer *receiveBuffer,
                            int32_t timeout_ms,
                            void *context) {
  MqttSnClientUdpNetwork *udpContext = (MqttSnClientUdpNetwork *) context;

  MqttSnMessageData toReceive = {0};

  int rc = arduino_receive_udp(udpContext->udp,
                               &toReceive.address,
                               toReceive.data,
                               &toReceive.data_length);
  if (rc < 0) {
    return -1;
  }
  if (rc == 0) {
    return 0;
  }

#ifdef WITH_UDP_BROADCAST
  if (n->client_network_broadcast_address) {
    if (memcmp(&toReceive.address, n->client_network_broadcast_address, sizeof(device_address)) == 0) {
      toReceive.broadcast_radius = MQTT_SN_ENCAPSULATED_MESSAGE_CRTL_BROADCAST_RADIUS;
    }
  }
#endif

  put(receiveBuffer, &toReceive);

#ifdef WITH_DEBUG_LOGGING
  const MqttSnMessageData *msg = back(receiveBuffer);
  log_db_rec_client_message(n->logger,
                            &msg->address,
                            n->mqtt_sn_gateway_address,
                            msg->data,
                            msg->data_length);
#endif
  return 0;
}

int ClientArduinoUdpSend(MqttSnClientNetworkInterface *n,
                         MqttSnFixedSizeRingBuffer *sendBuffer,
                         int32_t timeout_ms,
                         void *context) {
  MqttSnClientUdpNetwork *udpContext = (MqttSnClientUdpNetwork *) context;

  MqttSnMessageData toSend = {0};
  if (pop(sendBuffer, &toSend)) {
    return 0;
  }

#ifdef WITH_DEBUG_LOGGING
  if (log_db_send_client_message(n->logger,
                                 n->mqtt_sn_gateway_address,
                                 &toSend.address,
                                 toSend.data,
                                 toSend.data_length)) {
    return -1;
  }
#endif

#ifdef WITH_UDP_BROADCAST
  if (n->client_network_broadcast_address) {
    if (toSend.broadcast_radius &&
        memcmp(&toSend.address, n->client_network_broadcast_address, sizeof(device_address)) == 0) {
      return arduino_send_multicast_udp(udpContext->udp, &toSend.address, toSend.data, toSend.data_length);
    }
  }
#endif
  return arduino_send_udp(udpContext->udp, &toSend.address, toSend.data, toSend.data_length);
}
