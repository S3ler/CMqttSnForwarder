//
// Created by SomeDude on 31.07.2019.
//

#include "MqttSnNetworkBuilderMqttSnAddressGeneratorFactory.h"
#include "MqttSnTcpAddressGenerator.h"
std::shared_ptr<MqttSnNetworkBuilderMqttSnAddressGeneratorInterface> MqttSnNetworkBuilderMqttSnAddressGeneratorFactory::GetMqttSnAddressGenerator(MqttSnAddressGeneratorType type) {
  if (type == MqttSnAddressGeneratorType::UDP) {
    if (!mqttSnUdpAddressGenerator) {
      mqttSnUdpAddressGenerator = std::make_shared<MqttSnUdpAddressGenerator>();
    }
    return mqttSnUdpAddressGenerator;
  }
  if (type == MqttSnAddressGeneratorType::TCP) {
    if (!mqttSnTcpAddressGenerator) {
      mqttSnTcpAddressGenerator = std::make_shared<MqttSnTcpAddressGenerator>();
    }
    return mqttSnTcpAddressGenerator;
  }
  // FEAUTRE BLE
  /*
  if (type == MqttSnAddressGeneratorType::BLE) {
    // FEAUTRE BLE limited count of addresses from a List which are the network adapter
    throw std::exception();
  }
   */
  throw std::exception();
}
std::shared_ptr<MqttSnNetworkBuilderMqttSnAddressGeneratorInterface> MqttSnNetworkBuilderMqttSnAddressGeneratorFactory::GetMqttSnAddressGenerator(MqttSnGatewayProtocolTestType type) {
  if (type == MqttSnGatewayProtocolTestType::UDP) {
    return GetMqttSnAddressGenerator(MqttSnAddressGeneratorType::UDP);
  }
  throw std::exception();
}
std::shared_ptr<MqttSnNetworkBuilderMqttSnAddressGeneratorInterface> MqttSnNetworkBuilderMqttSnAddressGeneratorFactory::GetMqttSnAddressGenerator(MqttSnClientProtocolTestType type) {
  if (type == MqttSnClientProtocolTestType::UDP) {
    return GetMqttSnAddressGenerator(MqttSnAddressGeneratorType::UDP);
  }
  throw std::exception();
}
