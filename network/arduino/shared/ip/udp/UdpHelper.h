//
// Created by SomeDude on 29.04.2019.
//

#ifndef CMQTTSNFORWARDER_FORWARDER_NETWORK_ARDUINO_SHARED_IP_UDP_UDPHELPER_H_
#define CMQTTSNFORWARDER_FORWARDER_NETWORK_ARDUINO_SHARED_IP_UDP_UDPHELPER_H_

#ifdef WITH_UDP_BROADCAST
#include <WiFiUDP.h>
#else
#include <Udp.h>
#endif

#include <stdint.h>
#include <forwarder/global_defines.h>

int arduino_init_udp(UDP *wiFiUdp, uint16_t port);

void arduino_deinit_udp(UDP *wiFiUdp);

int arduino_send_udp(UDP *wiFiUdp, device_address *destination, uint8_t *bytes, uint16_t bytes_len);

int arduino_receive_udp(UDP *wiFiUdp, device_address *from, uint8_t *bytes, uint16_t *bytes_len);

#ifdef WITH_UDP_BROADCAST
int arduino_join_multicast_udp(WiFiUDP *wiFiUdp, IPAddress bc_ip, uint16_t bc_port);

int arduino_send_multicast_udp(WiFiUDP *wiFiUdp, device_address *destination, uint8_t *bytes, uint16_t bytes_len);
#endif
#endif //CMQTTSNFORWARDER_FORWARDER_NETWORK_ARDUINO_SHARED_IP_UDP_UDPHELPER_H_
