//
// Created by SomeDude on 30.07.2019.
//

#include "MqttSnNetworkBuilderConfiguration.h"
const MqttSnNetworkBuilderConfiguration MqttSnNetworkBuilderConfiguration::DefaultConfiguration() {
  return MqttSnNetworkBuilderConfiguration::DefaultConfiguration(MqttNetworkBuilderConfiguration::DefaultConfiguration());
}
const MqttSnNetworkBuilderConfiguration MqttSnNetworkBuilderConfiguration::DefaultConfiguration(const MqttNetworkBuilderConfiguration &mqttNetworkBuilderConfiguration) {
  return MqttSnNetworkBuilderConfiguration(mqttNetworkBuilderConfiguration,
                                           MqttSnNetworkBuilderMqttSnGatewayConfiguration::GetDefaultConfiguration(
                                               mqttNetworkBuilderConfiguration),
                                           MqttSnNetworkBuilderMqttSnClientConfiguration::GetDefaultConfiguration());
}
MqttSnNetworkBuilderConfiguration::MqttSnNetworkBuilderConfiguration(const MqttNetworkBuilderConfiguration &mqtt_network_builder_configuration,
                                                                     const MqttSnNetworkBuilderMqttSnGatewayConfiguration &mqtt_sn_gateway_config,
                                                                     const MqttSnNetworkBuilderMqttSnClientConfiguration &mqtt_sn_client_config)
    : mqtt_network_builder_configuration(mqtt_network_builder_configuration),
      mqtt_sn_gateway_config(mqtt_sn_gateway_config),
      mqtt_sn_client_config(mqtt_sn_client_config) {}
