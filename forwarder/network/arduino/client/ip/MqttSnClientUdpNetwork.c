//
// Created by SomeDude on 29.04.2019.
//

#include "MqttSnClientUdpNetwork.h"
#include <string.h>
#include <forwarder/network/arduino/shared/ip/ArduinoIpAddressHelper.h>
#include <forwarder/network/arduino/shared/ip/udp/UdpHelper.h>
#include <forwarder/network/shared/shared/IpHelper.h>


int ClientArduinoUdpInit(MqttSnClientNetworkInterface *n, void *context) {
  MqttSnClientUdpNetwork *udpContext = (MqttSnClientUdpNetwork *) context;
  memset(udpContext, 0, sizeof(MqttSnClientUdpNetwork));
  strcpy(udpContext->protocol, "udp");
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
    return -1;
  }
#ifdef WITH_LOGGING
  if (n->logger) {
    log_open_socket(n->logger,
                    udpContext->protocol,
                    n->client_network_address);
  }
#endif
  return 0;
}

void ClientArduinoUdpDisconnect(MqttSnClientNetworkInterface *n, void *context) {
  MqttSnClientUdpNetwork *udpContext = (MqttSnClientUdpNetwork *) context;
  arduino_deinit_udp(udpContext->udp);
#ifdef WITH_LOGGING
  if (n->logger) {
    log_close_socket(n->logger,
                     udpContext->protocol,
                     n->client_network_address);
  }
#endif
}

int ClientArduinoUdpReceive(MqttSnClientNetworkInterface *n,
                            MqttSnFixedSizeRingBuffer *receiveBuffer,
                            int32_t timeout_ms,
                            void *context) {
  MqttSnClientUdpNetwork *udpContext = (MqttSnClientUdpNetwork *) context;

  MqttSnMessageData toReceive = {0};

  uint8_t signal_strength = 0;
  int rc = arduino_receive_udp(udpContext->udp,
                               &toReceive.address,
                               toReceive.data,
                               &toReceive.data_length,
                               &signal_strength);
  if (rc < 0) {
    return -1;
  }
  if (rc == 0) {
    return 0;
  }

  put(receiveBuffer, &toReceive);

#ifdef WITH_DEBUG_LOGGING
  if (n->logger) {
    const MqttSnMessageData *msg = back(receiveBuffer);
    log_db_rec_client_message(n->logger,
                              &msg->address,
                              n->mqtt_sn_gateway_address,
                              msg->data,
                              msg->data_length);
  }
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
  if (n->logger) {
    if (log_db_send_client_message(n->logger,
                                   n->mqtt_sn_gateway_address,
                                   &toSend.address,
                                   toSend.data,
                                   toSend.data_length)) {
      return -1;
    }
  }
#endif

  return arduino_send_udp(udpContext->udp, &toSend.address, toSend.data, toSend.data_length, UINT8_MAX);
}
