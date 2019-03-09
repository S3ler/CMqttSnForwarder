//
// Created by bele on 26.02.19.
//

#ifndef CMQTTSNFORWARDER_MQTTSNFORWARDERGATEWAYNETWORKTESTCONFIGURATION_H
#define CMQTTSNFORWARDER_MQTTSNFORWARDERGATEWAYNETWORKTESTCONFIGURATION_H

#include <ostream>
#include "../../../../forwarder/global_defines.h"
#include "../../../../forwarder/MqttSnGatewayNetworkInterface.h"

class MqttSnForwarderGatewayNetworkTestConfiguration {
 public:

  MqttSnForwarderGatewayNetworkTestConfiguration(
      device_address &forwarderGatewayNetworkAddress,
      void *gatewayNetworkContext,
      int (*gateway_network_init)(struct MqttSnGatewayNetworkInterface *, void *),
      bool useIdentifier = false) :
      forwarderGatewayNetworkAddress(forwarderGatewayNetworkAddress),
      gatewayNetworkContext(gatewayNetworkContext),
      gateway_network_init(gateway_network_init),
      useIdentifier(useIdentifier) {}

  bool useIdentifier = false;
  device_address forwarderGatewayNetworkAddress;
  void *gatewayNetworkContext = nullptr;
  int (*gateway_network_init)(struct MqttSnGatewayNetworkInterface *, void *context) = nullptr;

  friend std::ostream &operator<<(std::ostream &os,
                                  const MqttSnForwarderGatewayNetworkTestConfiguration &configuration) {
    /*
      os << "useIdentifier: " << configuration.useIdentifier << " forwarderGatewayNetworkAddress: "
      << configuration.forwarderGatewayNetworkAddress << " gatewayNetworkContext: "
      << configuration.gatewayNetworkContext << " gateway_network_init: " << configuration.gateway_network_init;
    */
    return os;
  }
};

#endif //CMQTTSNFORWARDER_MQTTSNFORWARDERGATEWAYNETWORKTESTCONFIGURATION_H