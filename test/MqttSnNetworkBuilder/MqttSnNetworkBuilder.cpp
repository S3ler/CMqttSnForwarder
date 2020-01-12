//
// Created by SomeDude on 24.07.2019.
//

#include "MqttSnNetworkBuilder.h"
#include <test/MqttSnClient/MqttSnClientTestContainerConfiguration.h>
#include <test/MqttSnClient/MqttSnClientTestContainerFactory.h>
#include <test/MqttSnForwarder/MqttSnForwarderTestContainerConfiguration.h>
#include <test/MqttSnForwarder/MqttSnForwarderTestContainerFactory.h>
#include <test/MqttSnGateway/MqttSnGatewayTestContainerFactory.h>
#include <test/shared/MqttSnURLParser/MqttSnURLParser.h>
#include <fstream>

MqttSnNetworkBuilder::MqttSnNetworkBuilder(const MqttNetworkBuilder* mqtt_network_builder)
    : mqttNetworkBuilder(mqtt_network_builder), configuration(MqttSnNetworkBuilderConfiguration::DefaultConfiguration()) {}
std::shared_ptr<MqttSnGatewayTestContainerInterface> MqttSnNetworkBuilder::getMqttSnGateway() {
  auto mqttSnGateway = MqttSnGatewayTestContainerFactory::getMqttSnGateway(configuration.mqtt_sn_gateway_config.type, getMqttSnGatewayConfiguration());
  mqttSnGateways.push_back(mqttSnGateway);
  return mqttSnGateway;
}
std::shared_ptr<MqttSnGatewayTestContainerInterface> MqttSnNetworkBuilder::getMqttSnGateway(const MqttSnGatewayTestContainerConfiguration& gatewayConfiguration) {
  return MqttSnGatewayTestContainerFactory::getMqttSnGateway(configuration.mqtt_sn_gateway_config.type, gatewayConfiguration);
}
MqttSnGatewayTestContainerConfiguration MqttSnNetworkBuilder::getMqttSnGatewayConfiguration() {
  auto addressGenerator = address_generator_factory.GetMqttSnAddressGenerator(configuration.mqtt_sn_gateway_config.protocol_type);
  return getMqttSnGatewayConfiguration(addressGenerator->GetMqttSnBroadcastAddress());
}
MqttSnGatewayTestContainerConfiguration MqttSnNetworkBuilder::getMqttSnGatewayConfiguration(const std::vector<uint8_t>& interface_broadcast_address) {
  MqttBrokerTestContainerConfiguration broker_cfg = mqttNetworkBuilder->GetMqttBrokerConfigurations().back();
  std::string clientPassword;
  if (configuration.mqtt_sn_gateway_config.baseClientPassword.length() > 0) {
    clientPassword = configuration.mqtt_sn_gateway_config.baseClientPassword + std::to_string(mqtt_sn_gateway_configurations.size());
  } else {
    clientPassword = std::string("");
  }

  MqttClientTestContainerConfiguration mqtt_client_config(broker_cfg, configuration.mqtt_sn_gateway_config.baseClientId + std::to_string(mqtt_sn_gateway_configurations.size()), clientPassword,
                                                          configuration.mqtt_sn_gateway_config.keepAliveInterval, configuration.mqtt_sn_gateway_config.cleanSession, -1);

  auto addressGenerator = address_generator_factory.GetMqttSnAddressGenerator(configuration.mqtt_sn_gateway_config.protocol_type);

  auto result = MqttSnGatewayTestContainerConfiguration(mqtt_client_config, configuration.mqtt_sn_gateway_config.protocol_type, addressGenerator->GetMqttSnInterfaceAddress(),
                                                        interface_broadcast_address, configuration.mqtt_sn_gateway_config.minGatewayId + mqtt_sn_gateway_configurations.size(),
                                                        configuration.mqtt_sn_gateway_config.keepAliveInterval, configuration.mqtt_sn_gateway_config.predefined_topics);
  mqtt_sn_gateway_configurations.push_back(result);
  return result;
}
std::shared_ptr<MqttSnForwarderTestContainerInterface> MqttSnNetworkBuilder::getMqttSnForwarder() {
  auto mqttSnForwarder = MqttSnForwarderTestContainerFactory::getMqttSnForwarder(configuration.mqtt_sn_forwarder_config.type, getMqttSnForwarderTestContainerConfiguration());
  mqttSnForwarders.push_back(mqttSnForwarder);
  return mqttSnForwarder;
}

MqttSnForwarderTestContainerConfiguration MqttSnNetworkBuilder::getMqttSnForwarderTestContainerConfiguration() {
  auto addressGenerator = address_generator_factory.GetMqttSnAddressGenerator(configuration.mqtt_sn_forwarder_config.protocol);
  return getMqttSnForwarderTestContainerConfiguration(addressGenerator->GetMqttSnBroadcastAddress());
}

MqttSnForwarderTestContainerConfiguration MqttSnNetworkBuilder::getMqttSnForwarderTestContainerConfiguration(const std::vector<uint8_t>& interface_broadcast_address) {
  if (mqtt_sn_gateway_configurations.empty()) {
    throw std::exception();
  }
  MqttSnGatewayTestContainerConfiguration gateway_cfg = mqtt_sn_gateway_configurations.back();
  std::string forwarderPassword;
  if (configuration.mqtt_sn_forwarder_config.baseForwarderPassword.length() > 0) {
    forwarderPassword = configuration.mqtt_sn_forwarder_config.baseForwarderPassword + std::to_string(mqtt_sn_forwarder_configurations.size());
  } else {
    forwarderPassword = std::string("");
  }
  auto gatewayNetworkAddressGenerator = address_generator_factory.GetMqttSnAddressGenerator(gateway_cfg.protocol);
  auto mqttSnClientConfiguration = MqttSnClientTestContainerConfiguration(
      MqttSnClientTestContainerConfiguration::GetProtocolFromGatewayProtocol(gateway_cfg.protocol), gatewayNetworkAddressGenerator->GetMqttSnInterfaceAddress(), gateway_cfg.interfaceBroadcastAddress,
      gateway_cfg.interfaceAddress, configuration.mqtt_sn_forwarder_config.baseForwarderId + std::to_string(mqtt_sn_forwarder_configurations.size()), forwarderPassword,
      configuration.mqtt_sn_forwarder_config.keepAliveInterval, configuration.mqtt_sn_forwarder_config.cleanSession);

  auto clientNetworkAddressGenerator = address_generator_factory.GetMqttSnAddressGenerator(configuration.mqtt_sn_forwarder_config.protocol);

  auto result = MqttSnForwarderTestContainerConfiguration(mqttSnClientConfiguration, configuration.mqtt_sn_forwarder_config.protocol, clientNetworkAddressGenerator->GetMqttSnInterfaceAddress(),
                                                          interface_broadcast_address);

  mqtt_sn_forwarder_configurations.push_back(result);
  return result;
}
std::shared_ptr<MqttSnClientTestContainerInterface> MqttSnNetworkBuilder::getMqttSnClient() {
  auto mqttSnClient = MqttSnClientTestContainerFactory::getMqttSnClient(configuration.mqtt_sn_client_config.type, getMqttSnClientConfiguration());
  mqttSnClients.push_back(mqttSnClient);
  return mqttSnClient;
}
std::shared_ptr<MqttSnClientTestContainerInterface> MqttSnNetworkBuilder::getForwardedMqttSnClient() {
  auto mqttSnClient = MqttSnClientTestContainerFactory::getMqttSnClient(configuration.mqtt_sn_client_config.type, getForwardedMqttSnClientConfiguration());
  mqttSnClients.push_back(mqttSnClient);
  return mqttSnClient;
}
MqttSnClientTestContainerConfiguration MqttSnNetworkBuilder::getForwardedMqttSnClientConfiguration() {
  if (mqtt_sn_forwarder_configurations.empty()) {
    throw std::exception();
  }
  MqttSnForwarderTestContainerConfiguration forwarder_cfg = mqtt_sn_forwarder_configurations.back();
  std::string clientPassword;
  if (configuration.mqtt_sn_client_config.baseClientPassword.length() > 0) {
    clientPassword = configuration.mqtt_sn_client_config.baseClientPassword + std::to_string(mqtt_sn_client_configurations.size());
  } else {
    clientPassword = std::string("");
  }

  auto addressGenerator = address_generator_factory.GetMqttSnAddressGenerator(forwarder_cfg.protocol);
  auto result = MqttSnClientTestContainerConfiguration(MqttSnClientTestContainerConfiguration::GetProtocolFromGatewayProtocol(forwarder_cfg.protocol), addressGenerator->GetMqttSnInterfaceAddress(),
                                                       forwarder_cfg.interfaceBroadcastAddress, forwarder_cfg.interfaceAddress,
                                                       configuration.mqtt_sn_client_config.baseClientId + std::to_string(mqtt_sn_client_configurations.size()), clientPassword,
                                                       configuration.mqtt_sn_client_config.keepAliveInterval, configuration.mqtt_sn_client_config.cleanSession);
  mqtt_sn_client_configurations.push_back(result);
  return result;
}
MqttSnClientTestContainerConfiguration MqttSnNetworkBuilder::getMqttSnClientConfiguration() {
  if (mqtt_sn_gateway_configurations.empty()) {
    throw std::exception();
  }
  MqttSnGatewayTestContainerConfiguration gateway_cfg = mqtt_sn_gateway_configurations.back();
  std::string clientPassword;
  if (configuration.mqtt_sn_client_config.baseClientPassword.length() > 0) {
    clientPassword = configuration.mqtt_sn_client_config.baseClientPassword + std::to_string(mqtt_sn_client_configurations.size());
  } else {
    clientPassword = std::string("");
  }

  auto addressGenerator = address_generator_factory.GetMqttSnAddressGenerator(gateway_cfg.protocol);
  auto result = MqttSnClientTestContainerConfiguration(MqttSnClientTestContainerConfiguration::GetProtocolFromGatewayProtocol(gateway_cfg.protocol), addressGenerator->GetMqttSnInterfaceAddress(),
                                                       gateway_cfg.interfaceBroadcastAddress, gateway_cfg.interfaceAddress,
                                                       configuration.mqtt_sn_client_config.baseClientId + std::to_string(mqtt_sn_client_configurations.size()), clientPassword,
                                                       configuration.mqtt_sn_client_config.keepAliveInterval, configuration.mqtt_sn_client_config.cleanSession);
  mqtt_sn_client_configurations.push_back(result);
  return result;
}
bool MqttSnNetworkBuilder::startMqttSnGateways() {
  createPahoClientsConfFile(mqtt_sn_client_configurations, mqtt_sn_forwarder_configurations);
  createPahoPredefinedTopicConfFile(mqtt_sn_gateway_configurations, mqtt_sn_client_configurations);

  for (auto& mqttSnGateway : mqttSnGateways) {
    if (!mqttSnGateway->start()) {
      return false;
    }
  }
  for (auto& mqttSnGateway : mqttSnGateways) {
    if (!mqttSnGateway->isRunning()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  }
  return true;
}
bool MqttSnNetworkBuilder::createPahoClientsConfFile(const vector<MqttSnClientTestContainerConfiguration>& mqtt_sn_client_configurations,
                                                     const vector<MqttSnForwarderTestContainerConfiguration>& mqtt_sn_forwarder_configurations) {
  bool withUnstableLine = 1;
  std::string clientsConfFileName = "./clients.conf";

  std::ofstream outfile;
  outfile.open(clientsConfFileName, std::ios::out);

  for (auto& forwarderConfiguration : mqtt_sn_forwarder_configurations) {
    MqttSnClientTestContainerConfiguration mqttSnClientConfiguration = forwarderConfiguration.mqttSnClientConfiguration;
    if (mqttSnClientConfiguration.protocol_type == MqttSnClientProtocolTestType::UDP) {
      std::vector<uint8_t> interfaceAddress;
      if (MqttSnURLParser::isIpv4Zero(mqttSnClientConfiguration.interfaceAddress)) {
        interfaceAddress.push_back(127);
        interfaceAddress.push_back(0);
        interfaceAddress.push_back(0);
        interfaceAddress.push_back(1);

        uint16_t brokerPort = MqttSnURLParser::getPort(mqttSnClientConfiguration.interfaceAddress);
        std::vector<uint8_t> brokerPortVector = MqttSnURLParser::GetPortVectorFromInt32(brokerPort);
        interfaceAddress.insert(interfaceAddress.end(), brokerPortVector.begin(), brokerPortVector.end());
      } else {
        interfaceAddress = mqttSnClientConfiguration.interfaceAddress;
      }
      if (withUnstableLine) {
        std::string forwarderConfig = mqttSnClientConfiguration.clientId + "," + MqttSnURLParser::toString(interfaceAddress, 4) + ":" + std::to_string(MqttSnURLParser::getPort(interfaceAddress)) +
                                      ",unstableLine,forwarder" + "\n";
        outfile << forwarderConfig;
      } else {
        std::string forwarderConfig =
            mqttSnClientConfiguration.clientId + "," + MqttSnURLParser::toString(interfaceAddress, 4) + ":" + std::to_string(MqttSnURLParser::getPort(interfaceAddress)) + ",forwarder" + "\n";
        outfile << forwarderConfig;
      }
    }
  }

  for (auto& mqttSnClientConfiguration : mqtt_sn_client_configurations) {
    if (mqttSnClientConfiguration.protocol_type == MqttSnClientProtocolTestType::UDP) {
      std::vector<uint8_t> interfaceAddress;
      if (MqttSnURLParser::isIpv4Zero(mqttSnClientConfiguration.interfaceAddress)) {
        interfaceAddress.push_back(127);
        interfaceAddress.push_back(0);
        interfaceAddress.push_back(0);
        interfaceAddress.push_back(1);

        uint16_t brokerPort = MqttSnURLParser::getPort(mqttSnClientConfiguration.interfaceAddress);
        std::vector<uint8_t> brokerPortVector = MqttSnURLParser::GetPortVectorFromInt32(brokerPort);
        interfaceAddress.insert(interfaceAddress.end(), brokerPortVector.begin(), brokerPortVector.end());
      } else {
        interfaceAddress = mqttSnClientConfiguration.interfaceAddress;
      }
      if (withUnstableLine) {
        std::string clientConfig = mqttSnClientConfiguration.clientId + "," + MqttSnURLParser::toString(interfaceAddress, 4) + ":" + std::to_string(MqttSnURLParser::getPort(interfaceAddress)) +
                                       ",unstableLine" + "\n ";
                                       outfile
                                   << clientConfig;
      } else {
        std::string clientConfig = mqttSnClientConfiguration.clientId + "," + MqttSnURLParser::toString(interfaceAddress, 4) + ":" + std::to_string(MqttSnURLParser::getPort(interfaceAddress)) + "\n";
        outfile << clientConfig;
      }
      // FEATURE add config option to MqttSnClientConfiguration for MQTT-SN Clients only sending unconnected QoS-1 publishes
      /* needed when the client send only QoS-1 - but there is no config option for such case
      std::string QosM1ClientConfig =
          "QoS-1_" + mqttSnClientConfiguration.clientId + "," + MqttSnURLParser::toString(interfaceAddress, 4) + ":" + std::to_string(MqttSnURLParser::getPort(interfaceAddress)) + ",QoS-1" + "\n";
      outfile << QosM1ClientConfig;
        */
    }
  }
  outfile << "\n";

  outfile << std::endl;
  outfile.close();
  return true;
}
bool MqttSnNetworkBuilder::createPahoPredefinedTopicConfFile(const std::vector<MqttSnGatewayTestContainerConfiguration> mqtt_sn_gateway_configurations,
                                                             const vector<MqttSnClientTestContainerConfiguration>& mqtt_sn_client_configurations) {
  std::string predefineTopicConfFileName = "./predefinedTopic.conf";

  std::ofstream outfile;
  outfile.open(predefineTopicConfFileName, std::ios::out);

  for (auto& mqttSnGatewayConfiguration : mqtt_sn_gateway_configurations) {
    // allow anyone to publish to all predefined topics
    for (std::tuple<uint16_t, std::string> topic : mqttSnGatewayConfiguration.predefined_topics) {
      outfile << "*," << std::get<1>(topic) << ", " << std::to_string(std::get<0>(topic)) << "\n";
    }
    outfile << "\n";
    // allow clients to publish all predefined topics of the gateeway with QoS-1
    for (std::tuple<uint16_t, std::string> topic : mqttSnGatewayConfiguration.predefined_topics) {
      for (auto& mqttSnClientConfiguration : mqtt_sn_client_configurations) {
        outfile << "QoS-1_" + mqttSnClientConfiguration.clientId + "," << std::get<1>(topic) << ", " << std::to_string(std::get<0>(topic)) << "\n";
      }
    }
    for (std::tuple<uint16_t, std::string> topic : mqttSnGatewayConfiguration.predefined_topics) {
      for (auto& mqttSnClientConfiguration : mqtt_sn_client_configurations) {
        outfile << mqttSnClientConfiguration.clientId + "," << std::get<1>(topic) << ", " << std::to_string(std::get<0>(topic)) << "\n";
      }
    }
  }
  // TODO
  /*
  for (auto& mqttSnClientConfiguration : mqtt_sn_client_configurations) {
    for (std::tuple<uint16_t, std::string> topic : mqttSnClientConfiguration.predefined_topics) {
      outfile << "QoS-1_" + mqttSnClientConfiguration.clientId + "," << std::get<1>(topic) << ", " << std::to_string(std::get<0>(topic)) << "\n";
    }
  }
  */
  outfile << std::endl;
  outfile.close();
  return true;
}
bool MqttSnNetworkBuilder::startMqttSnForwarders() {
  for (auto& mqttSnForwarder : mqttSnForwarders) {
    if (!mqttSnForwarder->start()) {
      return false;
    }
  }
  for (auto& mqttSnForwarder : mqttSnForwarders) {
    while (!mqttSnForwarder->isRunning()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  }
  return true;
}
bool MqttSnNetworkBuilder::startMqttSnClients() {
  for (auto& mqttSnClient : mqttSnClients) {
    if (!mqttSnClient->StartBackgroundHandler()) {
      return false;
    }
  }
  for (auto& mqttSnClient : mqttSnClients) {
    if (!mqttSnClient->IsBackgroundHandlerRunning()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  }
  return true;
}
void MqttSnNetworkBuilder::stopMqttSnGateways() {
  for (auto& mqttSnGateway : mqttSnGateways) {
    mqttSnGateway->stop();
  }
}
void MqttSnNetworkBuilder::stopMqttSnForwarders() {
  for (auto& mqttSnForwarder : mqttSnForwarders) {
    mqttSnForwarder->stop();
  }
  // TODO see starting reformat stuff of fw
  /*
  for (auto& mqttSnForwarder : mqttSnForwarders) {
    while (mqttSnForwarder->isRunning()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  }
  */
}
void MqttSnNetworkBuilder::stopMqttSnClients() {
  for (auto& mqttSnClient : mqttSnClients) {
    mqttSnClient->StopBackgroundHandler();
  }
  for (auto& mqttSnClient : mqttSnClients) {
    if (!mqttSnClient->IsBackgroundHandlerRunning()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  }
}

const std::vector<std::shared_ptr<MqttSnGatewayTestContainerInterface>>& MqttSnNetworkBuilder::getMqttSnGateways() const { return mqttSnGateways; }
const std::vector<std::shared_ptr<MqttSnForwarderTestContainerInterface>>& MqttSnNetworkBuilder::getMqttSnForwarders() const { return mqttSnForwarders; }
const std::vector<std::shared_ptr<MqttSnClientTestContainerInterface>>& MqttSnNetworkBuilder::getMqttSnClients() const { return mqttSnClients; }
void MqttSnNetworkBuilder::execMqttSnClientsActionSequenceAsync() {
  for (auto& mqtt_sn_client : mqttSnClients) {
    mqtt_sn_client->execActionSequence_async();
  }
}
std::tuple<int, int> MqttSnNetworkBuilder::getMqttSnClientsActionsSequenceExecutingFinished() {
  uint32_t running_clients = 0, finished_clients = 0;
  for (auto& client : mqttSnClients) {
    if (client->IsActionSequenceExecuted()) {
      running_clients += 1;
    } else {
      finished_clients += 1;
    }
  }
  return std::make_tuple<int, int>(running_clients, finished_clients);
}
