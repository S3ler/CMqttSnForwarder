//
// Created by SomeDude on 29.04.2019.
//

#include "UdpHelper.hpp"
#include <stdint.h>
#include <platform/device_address.h>
#include <network/arduino/shared/ip/ArduinoIpAddressHelper.hpp>
#include <string.h>

int32_t arduino_init_udp(WiFiUDP *wiFiUdp, uint16_t port) {
  if (wiFiUdp->begin(port) == 1) {
    return 0;
  }
  return -1;
}

void arduino_deinit_udp(WiFiUDP *wiFiUdp) {
  wiFiUdp->stop();
}

int32_t arduino_send_udp(WiFiUDP *wiFiUdp,
                         const device_address *destination,
                         const uint8_t *bytes,
                         uint16_t bytes_len) {
  IPAddress destination_IPAddress;
  uint16_t destination_port = 0;

  arduino_device_address_to_IPAddress_and_port(destination, &destination_IPAddress, &destination_port);

  if (wiFiUdp->beginPacket(destination_IPAddress, destination_port) == 1) {
    uint16_t write_rc = wiFiUdp->write(bytes, bytes_len);
    if (wiFiUdp->endPacket()) {
      return write_rc;
    }
  }
  return -1;
}

int32_t arduino_receive_udp(WiFiUDP *wiFiUdp,
                            device_address *from,
                            device_address *to,
                            uint8_t *bytes,
                            uint16_t max_bytes_len) {

  if (wiFiUdp->parsePacket() > 0) {
    memset(from, 0x0, sizeof(device_address));
    memset(to, 0x0, sizeof(device_address));

    int available = wiFiUdp->available();
    if (available > max_bytes_len) {
      // too much data => ignored
      wiFiUdp->flush();
      return 0;

    }
    if (wiFiUdp->read(bytes, max_bytes_len) == available) {
      IPAddress remoteIP = wiFiUdp->remoteIP();
      uint16_t remotePort = wiFiUdp->remotePort();
      arduino_ipv4_and_port_to_device_address(&remoteIP, remotePort, from);
      // ESP32 only:
      IPAddress destinationIP = wiFiUdp->remoteIP();
      uint16_t destinationPort = wiFiUdp->remotePort();
      // ESP8266 only:
      /*
      IPAddress destinationIP = wiFiUdp->destinationIP();
      uint16_t destinationPort = wiFiUdp->localPort();
      */

      arduino_ipv4_and_port_to_device_address(&destinationIP, destinationPort, to);
      return available;
    }
  }
  return 0;
}

#ifdef WITH_UDP_BROADCAST
int arduino_join_multicast_udp(WiFiUDP *wiFiUdp, IPAddress bc_ip, uint16_t bc_port) {
  // ESP32
  if (wiFiUdp->beginMulticast(bc_ip, bc_port) == 1) {
    return 0;
  }
  // ESP8266
  /*
  IPAddress localIP = WiFi.localIP();
  if (wiFiUdp->beginMulticast(localIP, bc_ip, bc_port) == 1) {
    return 0;
  }
  */
  return -1;
}

int32_t arduino_send_multicast_udp(WiFiUDP *wiFiUdp, const device_address *destination, const uint8_t *bytes, uint16_t bytes_len) {
  IPAddress destination_IPAddress;
  uint16_t destination_port = 0;

  arduino_device_address_to_IPAddress_and_port(destination, &destination_IPAddress, &destination_port);

  // ESP32
  if (wiFiUdp->beginMulticastPacket() == 1) {
    uint16_t write_rc = wiFiUdp->write(bytes, bytes_len);
    if (wiFiUdp->endPacket()) {
      return write_rc;
    }
  }
  // ESP8266
  /*
  if (wiFiUdp->beginPacketMulticast(destination_IPAddress, destination_port, WiFi.localIP()) == 1) {
    uint16_t write_rc = wiFiUdp->write(bytes, bytes_len);
    if (wiFiUdp->endPacket()) {
        return write_rc;
    }
  }
  */
  return -1;
}
#endif
