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
  if (mqtt_broker_configurations.size() >= configuration.mqtt_broker_config.maxBrokerCount) {
    return nullptr;
  }
  return MqttBrokerTestContainerFactory::getMqttBroker(configuration.mqtt_broker_config.type,
                                                       getMqttBrokerConfiguration());
}
MqttBrokerTestContainerConfiguration MqttNetworkBuilder::getMqttBrokerConfiguration() {
  MqttBrokerTestContainerConfiguration broker_cfg(configuration.mqtt_broker_config.protocol_type,
                                                  configuration.mqtt_broker_config.brokerAddress,
                                                  configuration.mqtt_broker_config.minBrokerPort
                                                      + mqtt_broker_configurations.size());
  mqtt_broker_configurations.push_back(broker_cfg);
  return broker_cfg;
}
std::shared_ptr<MqttClientTestContainerInterface> MqttNetworkBuilder::getMqttClient() {
  if (mqtt_broker_configurations.empty()) {
    return nullptr;
  }
  if (mqtt_client_configurations.size() >= configuration.mqtt_client_config.maxClientCount) {
    return nullptr;
  }
  return MqttClientTestContainerFactory::getMqttClient(configuration.mqtt_client_config.type,
                                                       getMqttClientConfiguration());
}
MqttClientTestContainerConfiguration MqttNetworkBuilder::getMqttClientConfiguration() {
  if (mqtt_broker_configurations.empty()) {
    throw std::exception();
  }
  MqttBrokerTestContainerConfiguration broker_cfg = mqtt_broker_configurations.back();
  std::string clientPassword;
  if (configuration.mqtt_client_config.baseClientPassword.length() > 0) {
    clientPassword = configuration.mqtt_client_config.baseClientPassword
        + std::to_string(mqtt_client_configurations.size());
  } else {
    clientPassword = std::string("");
  }

  MqttClientTestContainerConfiguration mqtt_config(broker_cfg,
                                                   configuration.mqtt_client_config.baseClientId
                                                       + std::to_string(mqtt_client_configurations.size()),
                                                   clientPassword,
                                                   configuration.mqtt_client_config.keepAliveInterval,
                                                   configuration.mqtt_client_config.cleanSession);
  mqtt_client_configurations.push_back(mqtt_config);
  return mqtt_config;
}
std::shared_ptr<MqttSnNetworkBuilder> MqttNetworkBuilder::getMqttSnNetworkBuilder() {
  return std::make_shared<MqttSnNetworkBuilder>(this);
}
const std::vector<MqttBrokerTestContainerConfiguration> &MqttNetworkBuilder::GetMqttBrokerConfigurations() const {
  return mqtt_broker_configurations;
}
const std::vector<MqttClientTestContainerConfiguration> &MqttNetworkBuilder::GetMqttClientConfigurations() const {
  return mqtt_client_configurations;
}
