
//
// Created by SomeDude on 31.07.2019.
//
#include "MqttSnClientTestContainerFactory.h"
#include "MqttSnClientArsmbCTestContainer.h"
std::shared_ptr<MqttSnClientTestContainerInterface> MqttSnClientTestContainerFactory::getMqttSnClient() { return getMqttSnClient(MqttSnClientTestType::ARSMB_C); }
std::shared_ptr<MqttSnClientTestContainerInterface> MqttSnClientTestContainerFactory::getMqttSnClient(MqttSnClientTestType type) {
  return getMqttSnClient(type, MqttSnClientTestContainerConfiguration::GetDefaultConfiguration());
}
std::shared_ptr<MqttSnClientTestContainerInterface> MqttSnClientTestContainerFactory::getMqttSnClient(MqttSnClientTestType type, const MqttSnClientTestContainerConfiguration configuration) {
  if (MqttSnClientTestType::ARSMB_C == type) {
    std::shared_ptr<MqttSnClientArsmbCTestContainer> r = std::make_shared<MqttSnClientArsmbCTestContainer>(configuration);
    return r;
  }
  throw std::exception();
}
