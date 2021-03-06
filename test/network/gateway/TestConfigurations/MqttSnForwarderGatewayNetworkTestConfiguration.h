//
// Created by bele on 26.02.19.
//

#ifndef CMQTTSNFORWARDER_MQTTSNFORWARDERGATEWAYNETWORKTESTCONFIGURATION_H
#define CMQTTSNFORWARDER_MQTTSNFORWARDERGATEWAYNETWORKTESTCONFIGURATION_H

#include <ostream>
#include "platform/device_address.h"
#include "network/MqttSnGatewayNetworkInterface.h"

class MqttSnForwarderGatewayNetworkTestConfiguration {
 public:
  bool useIdentifier = false;
  device_address forwarderAddress;
  void *gatewayNetworkContext = nullptr;
  int (*gateway_network_init)(struct MqttSnGatewayNetworkInterface *, void *context) = nullptr;
  device_address (*getDeviceAddressFromMqttSnGatewayNetworkContext)(uint16_t identifier, void *context) = nullptr;

  MqttSnForwarderGatewayNetworkTestConfiguration(
      device_address &forwarderGatewayNetworkAddress,
      void *gatewayNetworkContext,
      int (*gateway_network_init)(struct MqttSnGatewayNetworkInterface *, void *),
      device_address (*getDeviceAddressFromMqttSnGatewayNetworkContext)(uint16_t identifier, void *context),
      bool useIdentifier = false) :
      useIdentifier(useIdentifier),
      forwarderAddress(forwarderGatewayNetworkAddress),
      gatewayNetworkContext(gatewayNetworkContext),
      gateway_network_init(gateway_network_init),
      getDeviceAddressFromMqttSnGatewayNetworkContext(getDeviceAddressFromMqttSnGatewayNetworkContext) {}

  friend std::ostream &operator<<(std::ostream &os,
                                  const MqttSnForwarderGatewayNetworkTestConfiguration &configuration) {
    /*
      os << "useIdentifier: " << configuration.useIdentifier << " forwarderAddress: "
      << configuration.forwarderAddress << " gatewayNetworkContext: "
      << configuration.gatewayNetworkContext << " gateway_network_init: " << configuration.gateway_network_init;
    */
    return os;
  }
};

#endif //CMQTTSNFORWARDER_MQTTSNFORWARDERGATEWAYNETWORKTESTCONFIGURATION_H
