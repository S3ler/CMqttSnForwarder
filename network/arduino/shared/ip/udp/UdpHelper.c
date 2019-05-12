//
// Created by SomeDude on 29.04.2019.
//

#include "UdpHelper.h"
#include <stdint.h>
#include <forwarder/global_defines.h>
#include <network/arduino/shared/ip/ArduinoIpAddressHelper.h>
#include <string.h>
#include <CMqttSnForwarderArduino.h>
"utility/arduino/system/system.c"
"utility/arduino/system/system.h"
"utility/arduino/eeprom/eeprom_config.c"
"utility/arduino/eeprom/eeprom_config.h"
"forwarder/MqttSnForwarderLogging.h"
"forwarder/MqttSnClientNetworkInterface.c"
"forwarder/MqttSnClientNetworkInterface.h"
"forwarder/MqttSnFixedSizeRingBuffer.c"
"forwarder/MqttSnFixedSizeRingBuffer.h"
"forwarder/MqttSnForwarder.c"
"forwarder/MqttSnForwarder.h"
"forwarder/MqttSnForwarderLogging.c"
"forwarder/MqttSnForwarderLogging.h"
"forwarder/MqttSnGatewayNetworkInterface.c"
"forwarder/MqttSnGatewayNetworkInterface.h"
"network/shared/ip/IpHelper.c"
"network/shared/ip/IpHelper.h"
"main/forwarder_config.c"
"main/forwarder_config.h"

int arduino_init_udp(UDP *wiFiUdp, uint16_t port) {
  if (wiFiUdp->begin(port) == 1) {
    return 0;
  }
  return -1;
}

void arduino_deinit_udp(UDP *wiFiUdp) {
  wiFiUdp->stop();
}

int arduino_send_udp(UDP *wiFiUdp, device_address *destination, uint8_t *bytes, uint16_t bytes_len) {
  IPAddress destination_IPAddress;
  uint16_t destination_port = 0;

  arduino_device_address_to_IPAddress_and_port(destination, &destination_IPAddress, &destination_port);

  if (wiFiUdp->beginPacket(destination_IPAddress, destination_port) == 1) {
    if (wiFiUdp->write(bytes, bytes_len) == bytes_len) {
      if (wiFiUdp->endPacket() == 1) {
        return 0;
      }
    }
  }
  return -1;
}

int arduino_receive_udp(UDP *wiFiUdp, device_address *from, uint8_t *bytes, uint16_t *bytes_len) {

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
        return 1;
      }
    }
    // too much data => ignored
    wiFiUdp->flush();
  }
  return 0;
}

#ifdef WITH_UDP_BROADCAST
int arduino_join_multicast_udp(WiFiUDP *wiFiUdp, IPAddress bc_ip, uint16_t bc_port) {
  IPAddress localIP = WiFi.localIP();
  if (wiFiUdp->beginMulticast(localIP, bc_ip, bc_port) == 1) {
    return 0;
  }
  return -1;
}

int arduino_send_multicast_udp(WiFiUDP *wiFiUdp, device_address *destination, uint8_t *bytes, uint16_t bytes_len) {
  IPAddress destination_IPAddress;
  uint16_t destination_port = 0;

  arduino_device_address_to_IPAddress_and_port(destination, &destination_IPAddress, &destination_port);

  if (wiFiUdp->beginPacketMulticast(destination_IPAddress, destination_port, WiFi.localIP()) == 1) {
    if (wiFiUdp->write(bytes, bytes_len) == bytes_len) {
      if (wiFiUdp->endPacket() == 1) {
        return 0;
      }
    }
  }
  return -1;
}
#endif
