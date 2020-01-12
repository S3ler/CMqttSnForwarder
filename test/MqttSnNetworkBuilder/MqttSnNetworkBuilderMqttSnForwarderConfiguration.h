//
// Created by SomeDude on 17.09.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNNETWORKBUILDERMQTTSNFORWARDERCONFIGURATION_H
#define CMQTTSNFORWARDER_MQTTSNNETWORKBUILDERMQTTSNFORWARDERCONFIGURATION_H

#include <stdint.h>
#include <string>
#include <test/MqttSnForwarder/MqttSnForwarderTestType.h>
#include <test/MqttSnGateway/MqttSnGatewayProtocolTestType.h>

class MqttSnNetworkBuilderMqttSnForwarderConfiguration {
 public:
  const MqttSnForwarderTestType type;
  const int32_t maxForwarderCount;

  const std::string baseForwarderId;
  const std::string baseForwarderPassword;

  const int32_t keepAliveInterval;
  const bool cleanSession;


  const MqttSnGatewayProtocolTestType protocol;

  MqttSnNetworkBuilderMqttSnForwarderConfiguration(const MqttSnForwarderTestType type, const int32_t maxForwarderCount, const std::string& baseForwarderId, const std::string& baseForwarderPassword,
                                                   const int32_t keepAliveInterval, const bool cleanSession, const MqttSnGatewayProtocolTestType protocol);
  static const MqttSnNetworkBuilderMqttSnForwarderConfiguration GetDefaultConfiguration();
};

#endif  // CMQTTSNFORWARDER_MQTTSNNETWORKBUILDERMQTTSNFORWARDERCONFIGURATION_H
