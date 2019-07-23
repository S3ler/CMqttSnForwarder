//
// Created by SomeDude on 23.07.2019.
//

#include "MqttNetworkMqttClientBuilderConfiguration.h"
MqttNetworkMqttClientBuilderConfiguration::MqttNetworkMqttClientBuilderConfiguration(const MqttClientTestType &type,
                                                                                     const int32_t max_client_count,
                                                                                     const std::string &base_client_id,
                                                                                     const std::string &base_client_password,
                                                                                     const int32_t keep_alive_interval,
                                                                                     const bool clean_session)
    : type(type),
      maxClientCount(max_client_count),
      baseClientId(base_client_id),
      baseClientPassword(base_client_password),
      keepAliveInterval(keep_alive_interval),
      cleanSession(clean_session) {}
const MqttNetworkMqttClientBuilderConfiguration MqttNetworkMqttClientBuilderConfiguration::GetDefaultConfiguration() {
  return MqttNetworkMqttClientBuilderConfiguration(MqttClientTestType::PAHO_CPP,
                                                   UINT16_MAX / 2 - 1024,
                                                   std::string("MqttTestClient"),
                                                   std::string(""),
                                                   60,
                                                   true);
}
