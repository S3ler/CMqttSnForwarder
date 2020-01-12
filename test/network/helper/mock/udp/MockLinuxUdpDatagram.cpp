//
// Created by SomeDude on 06.04.2019.
//

#include "MockLinuxUdpDatagram.h"

MockLinuxUdpDatagram::MockLinuxUdpDatagram(const device_address &address, const uint8_t *data, ssize_t packetLength)
    : address(address), data(data, data + packetLength), packetLength(packetLength) {}
