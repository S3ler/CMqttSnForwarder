//
// Created by SomeDude on 17.09.2019.
//

#include "MqttSnNetworkBuilderMqttSnForwarderConfiguration.h"
const MqttSnNetworkBuilderMqttSnForwarderConfiguration MqttSnNetworkBuilderMqttSnForwarderConfiguration::GetDefaultConfiguration() {
  return MqttSnNetworkBuilderMqttSnForwarderConfiguration(MqttSnForwarderTestType::ARSMB, 1000, "ArsmbMqttSnForwarder", "", 60, true, MqttSnGatewayProtocolTestType::UDP);
}
MqttSnNetworkBuilderMqttSnForwarderConfiguration::MqttSnNetworkBuilderMqttSnForwarderConfiguration(const MqttSnForwarderTestType type, const int32_t maxForwarderCount,
                                                                                                   const std::string& baseForwarderId, const std::string& baseForwarderPassword,
                                                                                                   const int32_t keepAliveInterval, const bool cleanSession,
                                                                                                   const MqttSnGatewayProtocolTestType protocol)
    : type(type),
      maxForwarderCount(maxForwarderCount),
      baseForwarderId(baseForwarderId),
      baseForwarderPassword(baseForwarderPassword),
      keepAliveInterval(keepAliveInterval),
      cleanSession(cleanSession),
      protocol(protocol) {}
