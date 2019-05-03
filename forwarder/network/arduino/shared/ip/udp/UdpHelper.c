//
// Created by SomeDude on 29.04.2019.
//

#include "UdpHelper.h"
#include <Udp.h>
#include <stdint.h>
#include <forwarder/global_defines.h>
#include <forwarder/network/arduino/shared/ip/ArduinoIpAddressHelper.h>
#include <string.h>
#include <CMqttSnForwarderArduino.h>

int arduino_init_udp(UDP *wiFiUdp, uint16_t port) {
  // TODO multicast socket receive of advertisements
  //  uint8_t beginMulticast(IPAddress interfaceAddr, IPAddress multicast, uint16_t port);
  if (wiFiUdp->begin(port) == 1) {
    return 0;
  }
  return -1;
}

void arduino_deinit_udp(UDP *wiFiUdp) {
  // TODO multicast socket receive of advertisements
  wiFiUdp->stop();
}

int arduino_send_udp(UDP *wiFiUdp,
                     device_address *destination,
                     uint8_t *bytes,
                     uint16_t bytes_len,
                     uint8_t signal_strength) {
  IPAddress destination_IPAddress;
  uint16_t destination_port = 0;

  arduino_device_address_to_ipv4_and_port(destination, &destination_IPAddress, &destination_port);

  if (wiFiUdp->beginPacket(destination_IPAddress, destination_port) == 1) {
    if (wiFiUdp->write(bytes, bytes_len) == bytes_len) {
      if (wiFiUdp->endPacket() == 1) {
        return 0;
      }
    }
  }
  return -1;
}

int arduino_receive_udp(UDP *wiFiUdp,
                        device_address *from,
                        uint8_t *bytes,
                        uint16_t *bytes_len,
                        uint8_t* signal_strength) {
  IPAddress destination_IPAddress;
  uint16_t destination_port = 0;

  if (wiFiUdp->parsePacket() > 0) {
    memset(from, 0x0, sizeof(device_address));
    memset(bytes, 0x0, CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH);

    int available = wiFiUdp->available();
    if (available <= CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH) {
      *bytes_len = available;
      if (wiFiUdp->read(bytes, CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH) == available) {
        IPAddress remoteIP = wiFiUdp->remoteIP();
        uint16_t remotePort = wiFiUdp->remotePort();
        arduino_ipv4_and_port_to_device_address(&remoteIP, remotePort, from);

        *signal_strength = UINT8_MAX;
        return 1;
      }
    }
    // too much data => ignored
    wiFiUdp->flush();
  }
  return 0;
}
