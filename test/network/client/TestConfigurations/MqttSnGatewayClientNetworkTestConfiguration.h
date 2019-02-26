//
// Created by bele on 24.02.19.
//

#ifndef CMQTTSNFORWARDER_MQTTSNCLIENTNETWORKTESTFIXTURE_H
#define CMQTTSNFORWARDER_MQTTSNCLIENTNETWORKTESTFIXTURE_H

#include <ostream>
#include "../../../forwarder/global_defines.h"
#include "../../../forwarder/MqttSnClientNetworkInterface.h"
#include "MockClientNetwork/MockClient.h"
#include "MockClientNetwork/MockClientNetworkReceiver.h"
#include "MockClientNetwork/MockClientMqttSnMessageData.h"

class MqttSnGatewayClientNetworkTestConfiguration {
 public:

  MqttSnGatewayClientNetworkTestConfiguration(device_address &forwarderAddress,
                                 void *clientNetworkContext,
                                 int (*clientNetworkInit)(MqttSnClientNetworkInterface *, void *) ) :
      forwarderAddress(forwarderAddress),
      clientNetworkContext(
          clientNetworkContext),
      clientNetworkInit(
          clientNetworkInit) {}

  device_address forwarderAddress;
  void *clientNetworkContext;
  int (*clientNetworkInit)(struct MqttSnClientNetworkInterface *, void *context) = nullptr;
  //device_address (*generateMockClientNetworkAddress)(uint16_t mockClientIdentifier) = nullptr;

  friend std::ostream &operator<<(std::ostream &os, const MqttSnGatewayClientNetworkTestConfiguration &fixture) {
    return os;
    /*os << "forwarderAddress: " << fixture.forwarderAddress << " clientNetworkContext: " << fixture.clientNetworkContext
       << " clientNetworkInit: " << fixture.clientNetworkInit << " mockClientNetworkAddress: "
       << fixture.mockClientNetworkAddress << " mockClientNetworkInterface: " << fixture.mockClientNetworkInterface;
    return os;*/
  }
};

#endif //CMQTTSNFORWARDER_MQTTSNCLIENTNETWORKTESTFIXTURE_H
