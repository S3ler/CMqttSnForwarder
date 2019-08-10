//
// Created by SomeDude on 19.07.2019.
//

#include "MqttSnGatewayTestContainerFactory.h"
#include "MqttSnGatewayTestType.h"
#include "MqttSnGatewayArsmbTestContainer.h"
#include "MqttSnGatewayPahoTestContainer.h"
#include "MqttSnGatewayTestContainerConfiguration.h"

std::shared_ptr<MqttSnGatewayTestContainerInterface> MqttSnGatewayTestContainerFactory::getMqttSnGateway(
    MqttSnGatewayTestType mqttSnGatewayTestType) {
  static int32_t i = 0;
  if (mqttSnGatewayTestType == MqttSnGatewayTestType::ARSMB) {
    /*
    string gw_identifier = "ArmsbMqttSnGateway" + std::to_string(i);
    string gw_cmd = "-db --log_identifier MqttSnGateway" + std::to_string(i) + " -lfp MqttSnGateway"
        + std::to_string(i) + ".log " + "-cp 1000" + std::to_string(i) + " --gateway_id " + std::to_string(i);
        */
    return std::make_shared<MqttSnGatewayArsmbTestContainer>(MqttSnGatewayTestContainerConfiguration::GetDefaultTestContainerConfiguration());
  }
  if (mqttSnGatewayTestType == MqttSnGatewayTestType::PAHO) {
    /*
    string gw_identifier = "PahoMqttSnGateway" + std::to_string(i);
    string gw_cmd = "";
     */
    return std::make_shared<MqttSnGatewayPahoTestContainer>(MqttSnGatewayTestContainerConfiguration::GetDefaultTestContainerConfiguration());
  }
  return nullptr;
}
std::shared_ptr<MqttSnGatewayTestContainerInterface> MqttSnGatewayTestContainerFactory::getMqttSnGateway(
    MqttSnGatewayTestType mqttSnGatewayTestType,
    const MqttSnGatewayTestContainerConfiguration &configuration) {
  if (mqttSnGatewayTestType == MqttSnGatewayTestType::ARSMB) {
    return std::make_shared<MqttSnGatewayArsmbTestContainer>(configuration);
  }
  if (mqttSnGatewayTestType == MqttSnGatewayTestType::PAHO) {
    return std::make_shared<MqttSnGatewayPahoTestContainer>(configuration);
  }

  return shared_ptr<MqttSnGatewayTestContainerInterface>();
}
/*
std::shared_ptr<MqttSnGatewayTestContainerInterface> MqttSnGatewayTestContainerFactory::getMqttSnGateway(
    MqttSnGatewayTestType mqttSnGatewayTestType, const std::string &command) {
  if (mqttSnGatewayTestType == MqttSnGatewayTestType::ARSMB) {
    return std::make_shared<MqttSnGatewayArsmbTestContainer>("ArmsbMqttSnGateway", command);
  }
  if (mqttSnGatewayTestType == MqttSnGatewayTestType::PAHO) {
    return std::make_shared<MqttSnGatewayPahoTestContainer>("PahoMqttSnGateway", command);
  }
  return nullptr;
}
*/
