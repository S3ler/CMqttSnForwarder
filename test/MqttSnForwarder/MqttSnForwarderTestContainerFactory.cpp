//
// Created by SomeDude on 17.09.2019.
//

#include "MqttSnForwarderTestContainerFactory.h"
#include "MqttSnForwarderArsmbTestContainer.h"
std::shared_ptr<MqttSnForwarderTestContainerInterface> MqttSnForwarderTestContainerFactory::getMqttSnForwarder() { return getMqttSnForwarder(MqttSnForwarderTestType::ARSMB); }
std::shared_ptr<MqttSnForwarderTestContainerInterface> MqttSnForwarderTestContainerFactory::getMqttSnForwarder(MqttSnForwarderTestType mqttSnForwarderTestType) {
  return getMqttSnForwarder(mqttSnForwarderTestType, MqttSnForwarderTestContainerConfiguration::GetDefaultTestContainerConfiguration());
}
std::shared_ptr<MqttSnForwarderTestContainerInterface> MqttSnForwarderTestContainerFactory::getMqttSnForwarder(MqttSnForwarderTestType mqttSnForwarderTestType,
                                                                                                               const MqttSnForwarderTestContainerConfiguration& configuration) {
  if (mqttSnForwarderTestType == MqttSnForwarderTestType::ARSMB) {
    return std::make_shared<MqttSnForwarderArsmbTestContainer>(configuration);
  }
  return std::shared_ptr<MqttSnForwarderTestContainerInterface>();
}
