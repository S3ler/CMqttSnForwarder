//
// Created by SomeDude on 31.07.2019.
//

#include "MqttSnNetworkBuilderMqttSnAddressGeneratorFactory.h"
std::shared_ptr<MqttSnNetworkBuilderMqttSnAddressGeneratorInterface> MqttSnNetworkBuilderMqttSnAddressGeneratorFactory::GetMqttSnAddressGenerator(
    MqttSnAddressGeneratorType type) {
  if (type == MqttSnAddressGeneratorType::UDP) {
    if (!mqttSnUdpAddressGenerator) {
      mqttSnUdpAddressGenerator = std::make_shared<MqttSnUdpAddressGenerator>();
    }
    return mqttSnUdpAddressGenerator;
  }
  if (type == MqttSnAddressGeneratorType::TCP) {
    // TODO return always localhost
    throw std::exception();
  }
  if (type == MqttSnAddressGeneratorType::BLE) {
    // TODO limited count of addresses
    throw std::exception();
  }
  throw std::exception();
}
std::shared_ptr<MqttSnNetworkBuilderMqttSnAddressGeneratorInterface> MqttSnNetworkBuilderMqttSnAddressGeneratorFactory::GetMqttSnAddressGenerator(
    MqttSnGatewayProtocolTestType type) {
  if (type == MqttSnGatewayProtocolTestType::UDP) {
    return GetMqttSnAddressGenerator(MqttSnAddressGeneratorType::UDP);
  }
  throw std::exception();
}
std::shared_ptr<MqttSnNetworkBuilderMqttSnAddressGeneratorInterface> MqttSnNetworkBuilderMqttSnAddressGeneratorFactory::GetMqttSnAddressGenerator(
    MqttSnClientProtocolTestType type) {
  if (type == MqttSnClientProtocolTestType::UDP) {
    return GetMqttSnAddressGenerator(MqttSnAddressGeneratorType::UDP);
  }
  throw std::exception();
}
