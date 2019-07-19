//
// Created by SomeDude on 19.07.2019.
//

#include "MqttSnGatewayTestContainerFactory.h"
#include "MqttSnGatewayTestType.h"
#include "MqttSnGatewayArsmbDirectTestContainer.h"

std::shared_ptr<MqttSnGatewayTestContainerInterface> MqttSnGatewayTestContainerFactory::getMqttSnGateway(
    MqttSnGatewayTestType mqttSnGatewayTestType) {
  //if (mqttSnGatewayTestType == MqttSnGatewayTestType::ARSMB_DIRECT) {
  static int32_t i = 0;
  string gw_identifier = "MqttSnGateway" + std::to_string(i);
  string gw_cmd = "-db --log_identifier MqttSnGateway" + std::to_string(i) + " -lfp MqttSnGateway"
      + std::to_string(i) + ".log " + "-cp 1000" + std::to_string(i) + " --gateway_id " + std::to_string(i);
  return std::make_shared<MqttSnGatewayArsmbDirectTestContainer>(gw_identifier, gw_cmd);
  //}
  // return nullptr;
}
std::shared_ptr<MqttSnGatewayTestContainerInterface> MqttSnGatewayTestContainerFactory::getMqttSnGateway(
    MqttSnGatewayTestType mqttSnGatewayTestType,
    const std::string &command) {
  //if (mqttSnGatewayTestType == MqttSnGatewayTestType::ARSMB_DIRECT) {
  return std::make_shared<MqttSnGatewayArsmbDirectTestContainer>("MqttSnGateway", command);
  //}

//return nullptr;
}
