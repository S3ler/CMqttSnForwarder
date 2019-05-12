//
// Created by SomeDude on 12.05.2019.
//

#ifndef CMQTTSNFORWARDER_UTILITY_LINUX_IPADDRESS_FAKE_IPADDRESS_H_
#define CMQTTSNFORWARDER_UTILITY_LINUX_IPADDRESS_FAKE_IPADDRESS_H_

#include <platform/device_address.h>
#include <assert.h>
#include <string.h>

class IPAddress {
 private:
  device_address _ip;
  bool is_v4;
  uint8_t *raw_address() {
    return reinterpret_cast<uint8_t * >(&_ip.bytes);
  }
  const uint8_t *raw_address() const {
    return reinterpret_cast<const uint8_t *>(&_ip.bytes);
  }
 public:
  // Constructors
  IPAddress();

  bool isV4() const;
  void setV4();

  // Overloaded index operator to allow getting and setting individual octets of the address
  uint8_t operator[](int index) const {
    return isV4() ? *(raw_address() + index) : 0;
  }
  uint8_t &operator[](int index) {
    setV4();
    return *(raw_address() + index);
  }
  IPAddress &operator=(const uint8_t *address) {
    assert(sizeof(device_address) >= 4);
    memcpy(&_ip, address, 4);
    return *this;
  }
};

#endif //CMQTTSNFORWARDER_UTILITY_LINUX_IPADDRESS_FAKE_IPADDRESS_H_
