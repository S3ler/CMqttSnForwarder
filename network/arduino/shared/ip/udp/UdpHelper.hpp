//
// Created by SomeDude on 29.04.2019.
//

#ifndef CMQTTSNFORWARDER_FORWARDER_NETWORK_ARDUINO_SHARED_IP_UDP_UDPHELPER_H_
#define CMQTTSNFORWARDER_FORWARDER_NETWORK_ARDUINO_SHARED_IP_UDP_UDPHELPER_H_

#ifdef WITH_UDP_BROADCAST
#include <WiFiUdp.h>
#include <WiFi.h>
#else
#include <Udp.h>
#endif

#include <stdint.h>
#include <platform/device_address.h>

int32_t arduino_init_udp(WiFiUDP *wiFiUdp, uint16_t port);

void arduino_deinit_udp(WiFiUDP *wiFiUdp);

int32_t arduino_send_udp(WiFiUDP *wiFiUdp, const device_address *destination, const uint8_t *bytes, uint16_t bytes_len);

int32_t arduino_receive_udp(WiFiUDP *wiFiUdp,
                            device_address *from,
                            device_address *to,
                            uint8_t *bytes,
                            uint16_t max_bytes_len);

#ifdef WITH_UDP_BROADCAST
int arduino_join_multicast_udp(WiFiUDP *wiFiUdp, IPAddress bc_ip, uint16_t bc_port);

int32_t arduino_send_multicast_udp(WiFiUDP *wiFiUdp, const device_address *destination, const uint8_t *bytes, uint16_t bytes_len);
#endif
#endif //CMQTTSNFORWARDER_FORWARDER_NETWORK_ARDUINO_SHARED_IP_UDP_UDPHELPER_H_
