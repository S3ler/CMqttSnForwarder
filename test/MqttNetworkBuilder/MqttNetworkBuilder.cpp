//
// Created by SomeDude on 21.07.2019.
//

#include "MqttNetworkBuilder.h"
#include "../MqttBroker/MqttBrokerTestContainerFactory.h"
#include "../MqttClient/MqttClientTestContainerFactory.h"

MqttNetworkBuilder::MqttNetworkBuilder() : configuration(MqttNetworkBuilderConfiguration::DefaultConfiguration()) {}
MqttNetworkBuilder::MqttNetworkBuilder(const MqttNetworkBuilderConfiguration &configuration) : configuration(configuration) {}
std::shared_ptr<MqttBrokerTestContainerInterface> MqttNetworkBuilder::getMqttBroker() {
  if (mqtt_broker_configurations.size() >= configuration.mqtt_broker_config.maxBrokerCount) {
    return nullptr;
  }
  auto broker = MqttBrokerTestContainerFactory::getMqttBroker(configuration.mqtt_broker_config.type, getMqttBrokerConfiguration());
  mqttBrokers.push_back(broker);
  return broker;
}
MqttBrokerTestContainerConfiguration MqttNetworkBuilder::getMqttBrokerConfiguration() {
  MqttBrokerTestContainerConfiguration broker_cfg(configuration.mqtt_broker_config.protocol_type, configuration.mqtt_broker_config.brokerAddress,
                                                  configuration.mqtt_broker_config.minBrokerPort + mqtt_broker_configurations.size());
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
  auto client = MqttClientTestContainerFactory::getMqttClient(configuration.mqtt_client_config.type, getMqttClientConfiguration());
  mqttClients.push_back(client);
  return client;
}
MqttClientTestContainerConfiguration MqttNetworkBuilder::getMqttClientConfiguration() {
  if (mqtt_broker_configurations.empty()) {
    throw std::exception();
  }
  MqttBrokerTestContainerConfiguration broker_cfg = mqtt_broker_configurations.back();
  std::string clientPassword;
  if (configuration.mqtt_client_config.baseClientPassword.length() > 0) {
    clientPassword = configuration.mqtt_client_config.baseClientPassword + std::to_string(mqtt_client_configurations.size());
  } else {
    clientPassword = std::string("");
  }

  std::string clientId = configuration.mqtt_client_config.baseClientId + std::to_string(mqtt_client_configurations.size());
  if (configuration.mqtt_client_config.baseWillTopic.length() > 0) {
    std::vector<uint8_t> willMessage;
    willMessage.insert(willMessage.end(), configuration.mqtt_client_config.baseWillMessage.begin(), configuration.mqtt_client_config.baseWillMessage.end());
    willMessage.insert(willMessage.end(), clientId.begin(), clientId.end());

    MqttClientTestContainerConfiguration mqtt_config(broker_cfg, clientId, clientPassword, configuration.mqtt_client_config.keepAliveInterval, configuration.mqtt_client_config.cleanSession,
                                                     configuration.mqtt_client_config.baseWillTopic + clientId, willMessage, configuration.mqtt_client_config.willQoS,
                                                     configuration.mqtt_client_config.willRetain, configuration.mqtt_client_config.publishReceivePublishTimeout);
    mqtt_client_configurations.push_back(mqtt_config);
    return mqtt_config;
  }
  MqttClientTestContainerConfiguration mqtt_config(broker_cfg, clientId, clientPassword, configuration.mqtt_client_config.keepAliveInterval, configuration.mqtt_client_config.cleanSession,
                                                   configuration.mqtt_client_config.publishReceivePublishTimeout);
  mqtt_client_configurations.push_back(mqtt_config);
  return mqtt_config;
}
/*
std::shared_ptr<MqttSnNetworkBuilder> MqttNetworkBuilder::getMqttSnNetworkBuilder() {
  auto mqttSnNetworkBuilder = std::make_shared<MqttSnNetworkBuilder>(this);
  mqttSnNetworkBuilders.push_back(mqttSnNetworkBuilder);
  return mqttSnNetworkBuilder;
}
*/
const std::vector<MqttBrokerTestContainerConfiguration> &MqttNetworkBuilder::GetMqttBrokerConfigurations() const { return mqtt_broker_configurations; }
const std::vector<MqttClientTestContainerConfiguration> &MqttNetworkBuilder::GetMqttClientConfigurations() const { return mqtt_client_configurations; }
MqttNetworkBuilder::~MqttNetworkBuilder() {
  mqtt_broker_configurations.clear();
  mqtt_client_configurations.clear();
  mqttBrokers.clear();
  mqttClients.clear();
  //mqttSnNetworkBuilders.clear();
}
const std::vector<std::shared_ptr<MqttBrokerTestContainerInterface>> &MqttNetworkBuilder::getMqttBrokers() const { return mqttBrokers; }
const std::vector<std::shared_ptr<MqttClientTestContainerInterface>> &MqttNetworkBuilder::getMqttClients() const { return mqttClients; }

bool MqttNetworkBuilder::startMqttBrokers() {
  for (auto &mqttBroker : mqttBrokers) {
    if (!mqttBroker->start_broker()) {
      return false;
    }
  }
  for (auto &mqttBroker : mqttBrokers) {
    while (!mqttBroker->isRunning()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  }
  return true;
}
bool MqttNetworkBuilder::startMqttClients() {
  for (auto &mqttClient : mqttClients) {
    if (!mqttClient->StartBackgroundHandler()) {
      return false;
    }
  }
  for (auto &mqttClient : mqttClients) {
    if (!mqttClient->IsBackgroundHandlerRunning()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  }
  return true;
}
void MqttNetworkBuilder::stopMqttBrokers() {
  for (auto &mqttBroker : mqttBrokers) {
    mqttBroker->stop_broker();
  }
  for (auto &mqttBroker : mqttBrokers) {
    while (mqttBroker->isRunning()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  }
}
void MqttNetworkBuilder::stopMqttClients() {
  for (auto &mqttClient : mqttClients) {
    mqttClient->StopBackgroundHandler();
  }
  for (auto &mqttClient : mqttClients) {
    if (!mqttClient->IsBackgroundHandlerRunning()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  }
}
void MqttNetworkBuilder::execMqttClientsActionSequenceAsync() {
  for (auto &client : mqttClients) {
    client->execActionSequence_async();
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(5000));
}
std::tuple<int, int> MqttNetworkBuilder::getMqttClientsActionsSequenceExecutingFinished() {
  uint32_t running_clients = 0, finished_clients = 0;
  for (auto &client : mqttClients) {
    if (client->IsActionSequenceExecuted()) {
      running_clients += 1;
    } else {
      finished_clients += 1;
    }
  }
  return std::make_tuple<int, int>(running_clients, finished_clients);
}
