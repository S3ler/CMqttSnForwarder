//
// Created by SomeDude on 12.05.2019.
//

#include "IPAddress.h"

IPAddress::IPAddress() {
  assert(sizeof(device_address) >= 4);
  memset(&_ip, 0, sizeof(device_address));
  is_v4 = true;
}

bool IPAddress::isV4() const { return is_v4; }
void IPAddress::setV4() { is_v4 = true; }