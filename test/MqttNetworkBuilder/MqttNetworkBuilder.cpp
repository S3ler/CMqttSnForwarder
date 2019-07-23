//
// Created by SomeDude on 21.07.2019.
//

#include "MqttNetworkBuilder.h"
#include "../MqttBroker/MqttBrokerTestContainerFactory.h"
#include "../MqttClient/MqttClientTestContainerFactory.h"
MqttNetworkBuilder::MqttNetworkBuilder() : configuration(MqttNetworkBuilderConfiguration::DefaultConfiguration()) {}
MqttNetworkBuilder::MqttNetworkBuilder(const MqttNetworkBuilderConfiguration &configuration)
    : configuration(configuration) {}
std::shared_ptr<MqttBrokerTestContainerInterface> MqttNetworkBuilder::getMqttBroker() {
  if (createdMqttBrokers >= configuration.mqtt_broker_config.maxBrokerCount) {
    return nullptr;
  }
  auto result = MqttBrokerTestContainerFactory::getMqttBroker(configuration.mqtt_broker_config.type,
                                                              getMqttBrokerConfiguration());
  createdMqttBrokers += 1;
  return result;
}
MqttBrokerTestContainerConfiguration MqttNetworkBuilder::getMqttBrokerConfiguration() const {
  MqttBrokerTestContainerConfiguration broker_cfg(configuration.mqtt_broker_config.protocol,
                                                  configuration.mqtt_broker_config.brokerAddress,
                                                  configuration.mqtt_broker_config.minBrokerPort + createdMqttBrokers);
  return broker_cfg;
}
std::shared_ptr<MqttClientTestContainerInterface> MqttNetworkBuilder::getMqttClient() {
  if (createdMqttBrokers == 0) {
    return nullptr;
  }
  if (createdMqttClients >= configuration.mqtt_client_config.maxClientCount) {
    return nullptr;
  }
  auto result = MqttClientTestContainerFactory::getMqttClient(configuration.mqtt_client_config.type,
                                                              getMqttClientConfiguration());
  createdMqttClients += 1;
  return result;
}
MqttClientConnectAction MqttNetworkBuilder::getMqttClientConfiguration() const {
  auto broker_cfg = getMqttBrokerConfiguration();
  std::string clientPassword;
  if (configuration.mqtt_client_config.baseClientPassword.length() > 0) {
    clientPassword = configuration.mqtt_client_config.baseClientPassword + std::to_string(createdMqttClients);
  } else {
    clientPassword = std::string("");
  }
  MqttClientConnectAction connect_action(broker_cfg.protocol,
                                         broker_cfg.brokerAddress,
                                         broker_cfg.brokerPort - 1,
                                         configuration.mqtt_client_config.baseClientId
                                             + std::to_string(createdMqttClients),
                                         clientPassword,
                                         configuration.mqtt_client_config.keepAliveInterval,
                                         configuration.mqtt_client_config.cleanSession);
  return connect_action;
}
