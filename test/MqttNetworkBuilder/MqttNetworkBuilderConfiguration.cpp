//
// Created by SomeDude on 21.07.2019.
//

#include "MqttNetworkBuilderConfiguration.h"

const MqttNetworkBuilderConfiguration MqttNetworkBuilderConfiguration::DefaultConfiguration() {
  return MqttNetworkBuilderConfiguration(MqttNetworkBuilderMqttBrokerBuilderConfiguration::GetDefaultConfiguration(),
                                         MqttNetworkBuilderMqttClientBuilderConfiguration::GetDefaultConfiguration());
}
MqttNetworkBuilderConfiguration::MqttNetworkBuilderConfiguration(const MqttNetworkBuilderMqttBrokerBuilderConfiguration &mqtt_broker_config,
                                                                 const MqttNetworkBuilderMqttClientBuilderConfiguration &mqtt_client_config)
    : mqtt_broker_config(mqtt_broker_config), mqtt_client_config(mqtt_client_config) {}
