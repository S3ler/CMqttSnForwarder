//
// Created by bele on 24.02.19.
//

#ifndef CMQTTSNFORWARDER_MQTTSNCLIENTNETWORKTESTFIXTURE_H
#define CMQTTSNFORWARDER_MQTTSNCLIENTNETWORKTESTFIXTURE_H

#include <ostream>
#include "../../../../forwarder/global_defines.h"
#include "../../../../forwarder/MqttSnClientNetworkInterface.h"

class MqttSnGatewayClientNetworkTestConfiguration {
 public:

  MqttSnGatewayClientNetworkTestConfiguration(
      device_address &forwarderAddress,
      void *clientNetworkContext,
      int (*clientNetworkInit)(MqttSnClientNetworkInterface *, void *),
      bool useIdentifier = false) :
      forwarderAddress(forwarderAddress),
      clientNetworkContext(clientNetworkContext),
      clientNetworkInit(clientNetworkInit),
      useIdentifier(useIdentifier) {}

  bool useIdentifier = false;
  device_address forwarderAddress = {0};
  void *clientNetworkContext = nullptr;
  int (*clientNetworkInit)(struct MqttSnClientNetworkInterface *, void *context) = nullptr;

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
