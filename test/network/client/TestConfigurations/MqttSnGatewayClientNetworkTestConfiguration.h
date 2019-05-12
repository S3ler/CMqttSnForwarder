//
// Created by bele on 24.02.19.
//

#ifndef CMQTTSNFORWARDER_MQTTSNCLIENTNETWORKTESTFIXTURE_H
#define CMQTTSNFORWARDER_MQTTSNCLIENTNETWORKTESTFIXTURE_H

#include <ostream>
#include "platform/device_address.h"
#include "network/MqttSnClientNetworkInterface.h"

class MqttSnGatewayClientNetworkTestConfiguration {
 public:
  bool useIdentifier = false;
  device_address forwarderAddress = {0};
  void *clientNetworkContext = nullptr;
  int (*clientNetworkInit)(struct MqttSnClientNetworkInterface *, void *context) = nullptr;
  device_address (*getDeviceAddressFromMqttSnClientTcpNetworkContext)(uint16_t identifier, void *context) = nullptr;

  MqttSnGatewayClientNetworkTestConfiguration(
      device_address &forwarderAddress,
      void *clientNetworkContext,
      int (*clientNetworkInit)(MqttSnClientNetworkInterface *, void *),
      device_address (*getDeviceAddressFromMqttSnClientTcpNetworkContext)(uint16_t identifier, void *context),
      bool useIdentifier = false) :
      useIdentifier(useIdentifier),
      forwarderAddress(forwarderAddress),
      clientNetworkContext(clientNetworkContext),
      clientNetworkInit(clientNetworkInit),
      getDeviceAddressFromMqttSnClientTcpNetworkContext(getDeviceAddressFromMqttSnClientTcpNetworkContext) {}

  friend std::ostream &operator<<(std::ostream &os, const MqttSnGatewayClientNetworkTestConfiguration &fixture) {
    /*
      os << "forwarderAddress: " << fixture.forwarderAddress << " clientNetworkContext: " << fixture.clientNetworkContext
      << " clientNetworkInit: " << fixture.clientNetworkInit << " mockClientNetworkAddress: "
      << fixture.mockClientNetworkAddress << " mockClientNetworkInterface: " << fixture.mockClientNetworkInterface;
    */
    return os;
  }
};

#endif //CMQTTSNFORWARDER_MQTTSNCLIENTNETWORKTESTFIXTURE_H
