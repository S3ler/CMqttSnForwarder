//
// Created by SomeDude on 23.07.2019.
//

#include "MqttNetworkBuilderMqttClientBuilderConfiguration.h"
MqttNetworkBuilderMqttClientBuilderConfiguration::MqttNetworkBuilderMqttClientBuilderConfiguration(const MqttClientTestType& type, const uint16_t max_client_count, const std::string& base_client_id,
                                                                                                   const std::string& base_client_password, const int32_t keep_alive_interval, const bool clean_session,
                                                                                                   const std::string& baseWillTopic, const std::vector<uint8_t>& baseWillMessage, const int32_t willQoS,
                                                                                                   const bool willRetain, const int64_t publishReceivePublishTimeout)
    : type(type),
      maxClientCount(max_client_count),
      baseClientId(base_client_id),
      baseClientPassword(base_client_password),
      keepAliveInterval(keep_alive_interval),
      cleanSession(clean_session),
      baseWillTopic(baseWillTopic),
      baseWillMessage(baseWillMessage),
      willQoS(willQoS),
      willRetain(willRetain),
      publishReceivePublishTimeout(publishReceivePublishTimeout) {}
MqttNetworkBuilderMqttClientBuilderConfiguration::MqttNetworkBuilderMqttClientBuilderConfiguration(const MqttClientTestType& type, const uint16_t max_client_count, const std::string& base_client_id,
                                                                                                   const std::string& base_client_password, const int32_t keep_alive_interval, const bool clean_session,
                                                                                                   const int64_t publishReceivePublishTimeout)
    : type(type),
      maxClientCount(max_client_count),
      baseClientId(base_client_id),
      baseClientPassword(base_client_password),
      keepAliveInterval(keep_alive_interval),
      cleanSession(clean_session),
      willQoS(0),
      willRetain(false),
      publishReceivePublishTimeout(publishReceivePublishTimeout) {}
const MqttNetworkBuilderMqttClientBuilderConfiguration MqttNetworkBuilderMqttClientBuilderConfiguration::GetDefaultConfiguration() {
  return MqttNetworkBuilderMqttClientBuilderConfiguration(MqttClientTestType::PAHO_CPP, UINT16_MAX / 2 - 1024, std::string("MqttTestClient"), std::string(""), 60, true, 10);
}
