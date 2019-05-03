//
// Created by SomeDude on 29.04.2019.
//

#ifndef CMQTTSNFORWARDER_FORWARDER_NETWORK_ARDUINO_SHARED_IP_UDP_UDPHELPER_H_
#define CMQTTSNFORWARDER_FORWARDER_NETWORK_ARDUINO_SHARED_IP_UDP_UDPHELPER_H_

#include <Udp.h>
#include <stdint.h>
#include <forwarder/global_defines.h>

int arduino_init_udp(UDP *wiFiUdp, uint16_t port);

void arduino_deinit_udp(UDP *wiFiUdp);

int arduino_send_udp(UDP *wiFiUdp,
                     device_address *destination,
                     uint8_t *bytes,
                     uint16_t bytes_len,
                     uint8_t signal_strength);

int arduino_receive_udp(UDP *wiFiUdp,
                        device_address *from,
                        uint8_t *bytes,
                        uint16_t *bytes_len,
                        uint8_t *signal_strength);

#endif //CMQTTSNFORWARDER_FORWARDER_NETWORK_ARDUINO_SHARED_IP_UDP_UDPHELPER_H_
