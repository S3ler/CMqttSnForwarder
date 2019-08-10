//
// Created by SomeDude on 31.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNNETWORKBUILDER_MQTTSNNETWORKBUILDERMQTTSNADDRESSGENERATORFACTORY_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNNETWORKBUILDER_MQTTSNNETWORKBUILDERMQTTSNADDRESSGENERATORFACTORY_H_

#include <memory>
#include "MqttSnNetworkBuilderMqttSnAddressGeneratorInterface.h"
#include "test/MqttSnNetworkBuilder/MqttSnAddressGenerator/MqttSnAddressGeneratorType.h"
#include "MqttSnUdpAddressGenerator.h"
#include <test/MqttSnGateway/MqttSnGatewayProtocolTestType.h>
#include <test/MqttSnClient/MqttSnClientProtocolTestType.h>

// TODO make singleton
class MqttSnNetworkBuilderMqttSnAddressGeneratorFactory {
 public:
  std::shared_ptr<MqttSnUdpAddressGenerator> mqttSnUdpAddressGenerator = nullptr;
 public:
  std::shared_ptr<MqttSnNetworkBuilderMqttSnAddressGeneratorInterface> GetMqttSnAddressGenerator(MqttSnAddressGeneratorType type);
  std::shared_ptr<MqttSnNetworkBuilderMqttSnAddressGeneratorInterface> GetMqttSnAddressGenerator(MqttSnGatewayProtocolTestType type);
  std::shared_ptr<MqttSnNetworkBuilderMqttSnAddressGeneratorInterface> GetMqttSnAddressGenerator(MqttSnClientProtocolTestType type);
};

#endif //CMQTTSNFORWARDER_TEST_MQTTSNNETWORKBUILDER_MQTTSNNETWORKBUILDERMQTTSNADDRESSGENERATORFACTORY_H_
