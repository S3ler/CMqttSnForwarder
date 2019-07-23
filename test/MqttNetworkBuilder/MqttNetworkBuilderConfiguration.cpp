//
// Created by SomeDude on 21.07.2019.
//

#include "MqttNetworkBuilderConfiguration.h"

const MqttNetworkBuilderConfiguration MqttNetworkBuilderConfiguration::DefaultConfiguration() {
  return MqttNetworkBuilderConfiguration(MqttNetworkMqttBrokerBuilderConfiguration::GetDefaultConfiguration(),
                                         MqttNetworkMqttClientBuilderConfiguration::GetDefaultConfiguration());
}
MqttNetworkBuilderConfiguration::MqttNetworkBuilderConfiguration(const MqttNetworkMqttBrokerBuilderConfiguration &mqtt_broker_config,
                                                                 const MqttNetworkMqttClientBuilderConfiguration &mqtt_client_config)
    : mqtt_broker_config(mqtt_broker_config), mqtt_client_config(mqtt_client_config) {}
