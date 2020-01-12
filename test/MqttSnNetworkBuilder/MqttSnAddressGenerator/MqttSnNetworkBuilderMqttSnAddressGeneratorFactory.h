//
// Created by SomeDude on 31.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNNETWORKBUILDER_MQTTSNNETWORKBUILDERMQTTSNADDRESSGENERATORFACTORY_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNNETWORKBUILDER_MQTTSNNETWORKBUILDERMQTTSNADDRESSGENERATORFACTORY_H_

#include <test/MqttSnClient/MqttSnClientProtocolTestType.h>
#include <test/MqttSnGateway/MqttSnGatewayProtocolTestType.h>
#include <memory>
#include "MqttSnNetworkBuilderMqttSnAddressGeneratorInterface.h"
#include "MqttSnTcpAddressGenerator.h"
#include "MqttSnUdpAddressGenerator.h"
#include "test/MqttSnNetworkBuilder/MqttSnAddressGenerator/MqttSnAddressGeneratorType.h"

class MqttSnNetworkBuilderMqttSnAddressGeneratorFactory {
 public:
  std::shared_ptr<MqttSnUdpAddressGenerator> mqttSnUdpAddressGenerator = nullptr;
  std::shared_ptr<MqttSnTcpAddressGenerator> mqttSnTcpAddressGenerator = nullptr;
 public:
  std::shared_ptr<MqttSnNetworkBuilderMqttSnAddressGeneratorInterface> GetMqttSnAddressGenerator(MqttSnAddressGeneratorType type);
  std::shared_ptr<MqttSnNetworkBuilderMqttSnAddressGeneratorInterface> GetMqttSnAddressGenerator(MqttSnGatewayProtocolTestType type);
  std::shared_ptr<MqttSnNetworkBuilderMqttSnAddressGeneratorInterface> GetMqttSnAddressGenerator(MqttSnClientProtocolTestType type);
};

#endif //CMQTTSNFORWARDER_TEST_MQTTSNNETWORKBUILDER_MQTTSNNETWORKBUILDERMQTTSNADDRESSGENERATORFACTORY_H_
