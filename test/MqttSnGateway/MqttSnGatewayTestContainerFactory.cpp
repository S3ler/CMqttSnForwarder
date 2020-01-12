//
// Created by SomeDude on 19.07.2019.
//

#include "MqttSnGatewayTestContainerFactory.h"
#include "MqttSnGatewayArsmbTestContainer.h"
#include "MqttSnGatewayPahoTestContainer.h"
#include "MqttSnGatewayTestContainerConfiguration.h"
#include "MqttSnGatewayTestType.h"

std::shared_ptr<MqttSnGatewayTestContainerInterface> MqttSnGatewayTestContainerFactory::getMqttSnGateway() { return getMqttSnGateway(MqttSnGatewayTestType::PAHO); }
std::shared_ptr<MqttSnGatewayTestContainerInterface> MqttSnGatewayTestContainerFactory::getMqttSnGateway(MqttSnGatewayTestType mqttSnGatewayTestType) {
  return getMqttSnGateway(mqttSnGatewayTestType, MqttSnGatewayTestContainerConfiguration::GetDefaultTestContainerConfiguration());
}
std::shared_ptr<MqttSnGatewayTestContainerInterface> MqttSnGatewayTestContainerFactory::getMqttSnGateway(MqttSnGatewayTestType mqttSnGatewayTestType,
                                                                                                         const MqttSnGatewayTestContainerConfiguration &configuration) {
  if (mqttSnGatewayTestType == MqttSnGatewayTestType::ARSMB) {
    return std::make_shared<MqttSnGatewayArsmbTestContainer>(configuration);
  }
  if (mqttSnGatewayTestType == MqttSnGatewayTestType::PAHO) {
    return std::make_shared<MqttSnGatewayPahoTestContainer>(configuration);
  }
  return shared_ptr<MqttSnGatewayTestContainerInterface>();
}
