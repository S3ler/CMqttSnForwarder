//
// Created by SomeDude on 25.03.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNGATEWAYNETWORKTCPNETWORKDEFRAGMENTATIONTESTPARAMETER_H
#define CMQTTSNFORWARDER_MQTTSNGATEWAYNETWORKTCPNETWORKDEFRAGMENTATIONTESTPARAMETER_H

#include <TestConfigurations/MqttSnGatewayNetworkValueParameter.h>
#include <ostream>
class MqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter : public MqttSnGatewayNetworkValueParameter {
 public:
  uint16_t packetSize;
  MqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter
      (std::vector<MockGatewayConfiguration> &mockGatewayConfigurations,
       uint16_t messageCount,
       uint16_t messageLength,
       uint16_t packetSize,
       MqttSnForwarderGatewayNetworkTestConfiguration &mqttSnForwarderGatewayNetworkTestConfiguration,
       device_address mqttSnGatewayNetworkAddress = {0},
       uint8_t *(*msgDataGenerator)(uint8_t *, uint16_t) = defaultMessageDataGenerator)
      : MqttSnGatewayNetworkValueParameter(
      mockGatewayConfigurations,
      messageCount,
      messageLength,
      mqttSnForwarderGatewayNetworkTestConfiguration,
      mqttSnGatewayNetworkAddress,
      msgDataGenerator),
        packetSize(packetSize) {}

  friend std::ostream &operator<<(std::ostream &os,
                                  const MqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter &parameter) {
    os << static_cast<const MqttSnGatewayNetworkValueParameter &>(parameter)
       << "packetSize" << parameter.packetSize;
    return os;
  }
};

#endif //CMQTTSNFORWARDER_MQTTSNGATEWAYNETWORKTCPNETWORKDEFRAGMENTATIONTESTPARAMETER_H
