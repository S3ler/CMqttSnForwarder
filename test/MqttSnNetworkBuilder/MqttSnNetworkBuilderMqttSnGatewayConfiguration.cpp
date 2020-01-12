//
// Created by SomeDude on 30.07.2019.
//

#include "MqttSnNetworkBuilderMqttSnGatewayConfiguration.h"
#include <config/gateway/gateway_advertisment_config.h>
#include <config/network/client/client_network_config.h>
#include <test/MqttNetworkBuilder/MqttNetworkBuilderConfiguration.h>
const MqttSnNetworkBuilderMqttSnGatewayConfiguration MqttSnNetworkBuilderMqttSnGatewayConfiguration::GetDefaultConfiguration() {
  return MqttSnNetworkBuilderMqttSnGatewayConfiguration::GetDefaultConfiguration(MqttNetworkBuilderConfiguration::DefaultConfiguration());
}
const MqttSnNetworkBuilderMqttSnGatewayConfiguration MqttSnNetworkBuilderMqttSnGatewayConfiguration::GetDefaultConfiguration(
    const MqttNetworkBuilderConfiguration& mqtt_network_builder_configuration) {
  std::vector<std::tuple<uint16_t, std::string>> predefinedTopics;
  predefinedTopics.emplace_back(50, "/unsubscribed/client/topic/name");
  predefinedTopics.emplace_back(20, "/another/predefined/topic");
  return MqttSnNetworkBuilderMqttSnGatewayConfiguration(mqtt_network_builder_configuration, "MqttSnGateway", "", MQTT_SN_GATEWAY_ADVERTISEMENT_DEFAULT_DURATION, true, MqttSnGatewayTestType::PAHO,
                                                        UINT8_MAX, MqttSnGatewayProtocolTestType::UDP, std::string("localhost"), 10000, DEFAULT_MQTT_SN_CLIENT_BROADCAST_ADDRESS,
                                                        DEFAULT_MQTT_SN_CLIENT_BROADCAST_BIND_PORT, MQTT_SN_GATEWAY_ADVERTISEMENT_DEFAULT_GW_ID, predefinedTopics);
}

MqttSnNetworkBuilderMqttSnGatewayConfiguration::MqttSnNetworkBuilderMqttSnGatewayConfiguration(const MqttNetworkBuilderConfiguration& mqtt_network_builder_configuration,
                                                                                               const std::string& base_client_id, const std::string& base_client_password,
                                                                                               const int32_t keep_alive_interval, const bool clean_session, const MqttSnGatewayTestType type,
                                                                                               const int32_t max_gateway_count, const MqttSnGatewayProtocolTestType protocol_type,
                                                                                               const std::string& gateway_address_url, const uint16_t min_gateway_port,
                                                                                               const std::string& gatewayBroadcastAddressURL, const uint16_t gatewayBroadcastPort,
                                                                                               const uint8_t mingGatewayId, const std::vector<std::tuple<uint16_t, std::string>>& predefined_topics)
    : mqtt_network_builder_configuration(mqtt_network_builder_configuration),
      baseClientId(base_client_id),
      baseClientPassword(base_client_password),
      keepAliveInterval(keep_alive_interval),
      cleanSession(clean_session),
      type(type),
      maxGatewayCount(max_gateway_count),
      protocol_type(protocol_type),
      gatewayAddressURL(gateway_address_url),
      minGatewayPort(min_gateway_port),
      gatewayBroadcastAddressURL(gatewayBroadcastAddressURL),
      gatewayBroadcastPort(gatewayBroadcastPort),
      minGatewayId(mingGatewayId),
      predefined_topics(predefined_topics) {}
