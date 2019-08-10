//
// Created by SomeDude on 24.07.2019.
//

#include "MqttSnNetworkBuilder.h"
#include <test/MqttSnGateway/MqttSnGatewayTestContainerFactory.h>
#include <test/MqttSnClient/MqttSnClientTestContainerFactory.h>

MqttSnNetworkBuilder::MqttSnNetworkBuilder(const MqttNetworkBuilder *mqtt_network_builder) : mqttNetworkBuilder(
    mqtt_network_builder), configuration(MqttSnNetworkBuilderConfiguration::DefaultConfiguration()) {}
std::shared_ptr<MqttSnGatewayTestContainerInterface> MqttSnNetworkBuilder::getMqttSnGateway() {

  return MqttSnGatewayTestContainerFactory::getMqttSnGateway(configuration.mqtt_sn_gateway_config.type,
                                                             getMqttSnGatewayConfiguration());
}
MqttSnGatewayTestContainerConfiguration MqttSnNetworkBuilder::getMqttSnGatewayConfiguration() {
  MqttBrokerTestContainerConfiguration broker_cfg = mqttNetworkBuilder->GetMqttBrokerConfigurations().back();
  std::string clientPassword;
  if (configuration.mqtt_sn_gateway_config.baseClientPassword.length() > 0) {
    clientPassword = configuration.mqtt_sn_gateway_config.baseClientPassword
        + std::to_string(mqtt_sn_gateway_configurations.size());
  } else {
    clientPassword = std::string("");
  }

  MqttClientTestContainerConfiguration mqtt_client_config(broker_cfg,
                                                          configuration.mqtt_sn_gateway_config.baseClientId
                                                              + std::to_string(mqtt_sn_gateway_configurations.size()),
                                                          clientPassword,
                                                          configuration.mqtt_sn_gateway_config.keepAliveInterval,
                                                          configuration.mqtt_sn_gateway_config.cleanSession);

  auto addressGenerator = address_generator_factory
      .GetMqttSnAddressGenerator(configuration.mqtt_sn_gateway_config.protocol_type);

  auto result = MqttSnGatewayTestContainerConfiguration(mqtt_client_config,
                                                        configuration.mqtt_sn_gateway_config.protocol_type,
                                                        addressGenerator->GetMqttSnInterfaceAddress(),
                                                        addressGenerator->GetMqttSnBroadcastAddress(),
                                                        configuration.mqtt_sn_gateway_config.minGatewayId
                                                            + mqtt_sn_gateway_configurations.size());
  mqtt_sn_gateway_configurations.push_back(result);
  return result;
}
std::shared_ptr<MqttSnClientTestContainerInterface> MqttSnNetworkBuilder::getMqttSnClient() {
  return MqttSnClientTestContainerFactory::getMqttSnClient(configuration.mqtt_sn_client_config.type,
                                                           getMqttSnClientConfiguration());
}
MqttSnClientTestContainerConfiguration MqttSnNetworkBuilder::getMqttSnClientConfiguration() {
  if (mqtt_sn_gateway_configurations.empty()) {
    throw std::exception();
  }
  MqttSnGatewayTestContainerConfiguration gateway_cfg = mqtt_sn_gateway_configurations.back();
  std::string clientPassword;
  if (configuration.mqtt_sn_client_config.baseClientPassword.length() > 0) {
    clientPassword = configuration.mqtt_sn_client_config.baseClientPassword
        + std::to_string(mqtt_sn_client_configurations.size());
  } else {
    clientPassword = std::string("");
  }

  auto addressGenerator = address_generator_factory
      .GetMqttSnAddressGenerator(configuration.mqtt_sn_gateway_config.protocol_type);

  if (MqttSnClientFindGatewayType::SEARCH == configuration.mqtt_sn_client_config.find_gateway_type) {
    auto result =
        MqttSnClientTestContainerConfiguration(
            MqttSnClientTestContainerConfiguration::GetProtocolFromGatewayProtocol(gateway_cfg.protocol),
            addressGenerator->GetMqttSnInterfaceAddress(),
            gateway_cfg.interfaceBroadcastAddress,
            configuration.mqtt_sn_client_config.search_gateway_wait_timeout,
            configuration.mqtt_sn_client_config.search_gateway_radius,
            configuration.mqtt_sn_client_config.baseClientId + std::to_string(mqtt_sn_client_configurations.size()),
            clientPassword,
            configuration.mqtt_sn_client_config.keepAliveInterval,
            configuration.mqtt_sn_client_config.cleanSession);
    mqtt_sn_client_configurations.push_back(result);
    return result;
  }
  if (MqttSnClientFindGatewayType::ADVERTISE == configuration.mqtt_sn_client_config.find_gateway_type) {
    auto result =
        MqttSnClientTestContainerConfiguration(
            MqttSnClientTestContainerConfiguration::GetProtocolFromGatewayProtocol(gateway_cfg.protocol),
            addressGenerator->GetMqttSnInterfaceAddress(),
            gateway_cfg.interfaceBroadcastAddress,
            configuration.mqtt_sn_client_config.advertise_wait_timeout,
            configuration.mqtt_sn_client_config.baseClientId + std::to_string(mqtt_sn_client_configurations.size()),
            clientPassword,
            configuration.mqtt_sn_client_config.keepAliveInterval,
            configuration.mqtt_sn_client_config.cleanSession);
    mqtt_sn_client_configurations.push_back(result);
    return result;
  }
  if (MqttSnClientFindGatewayType::CONNECT == configuration.mqtt_sn_client_config.find_gateway_type) {
    auto result =
        MqttSnClientTestContainerConfiguration(
            MqttSnClientTestContainerConfiguration::GetProtocolFromGatewayProtocol(gateway_cfg.protocol),
            addressGenerator->GetMqttSnInterfaceAddress(),
            gateway_cfg.interfaceBroadcastAddress,
            gateway_cfg.interfaceAddress,
            configuration.mqtt_sn_client_config.baseClientId + std::to_string(mqtt_sn_client_configurations.size()),
            clientPassword,
            configuration.mqtt_sn_client_config.keepAliveInterval,
            configuration.mqtt_sn_client_config.cleanSession);
    mqtt_sn_client_configurations.push_back(result);
    return result;
  }

  throw std::exception();
}
