//
// Created by SomeDude on 30.07.2019.
//

#include "MqttSnNetworkBuilderMqttSnClientConfiguration.h"
const MqttSnNetworkBuilderMqttSnClientConfiguration MqttSnNetworkBuilderMqttSnClientConfiguration::GetDefaultConfiguration() {
  return MqttSnNetworkBuilderMqttSnClientConfiguration::GetDefaultConfiguration(MqttSnClientFindGatewayType::CONNECT);
}
const MqttSnNetworkBuilderMqttSnClientConfiguration MqttSnNetworkBuilderMqttSnClientConfiguration::GetDefaultConfiguration(
    const MqttSnClientFindGatewayType &find_gateway_type) {
  return MqttSnNetworkBuilderMqttSnClientConfiguration(MqttSnClientTestType::ARSMB_C,
                                                       10000,
                                                       find_gateway_type,
                                                       -1,
                                                       60,
                                                       1,
                                                       "MqttSnClient",
                                                       "",
                                                       60,
                                                       true);
}
MqttSnNetworkBuilderMqttSnClientConfiguration::MqttSnNetworkBuilderMqttSnClientConfiguration(const MqttSnClientTestType type,
                                                                                             const int32_t max_client_count,
                                                                                             const MqttSnClientFindGatewayType find_gateway_type,
                                                                                             const int32_t advertise_wait_timeout,
                                                                                             const int32_t search_gateway_wait_timeout,
                                                                                             const uint8_t search_gateway_radius,
                                                                                             const std::string &base_client_id,
                                                                                             const std::string &base_client_password,
                                                                                             const int32_t keep_alive_interval,
                                                                                             const bool clean_session)
    : type(type),
      maxClientCount(max_client_count),
      find_gateway_type(find_gateway_type),
      advertise_wait_timeout(advertise_wait_timeout),
      search_gateway_wait_timeout(search_gateway_wait_timeout),
      search_gateway_radius(search_gateway_radius),
      baseClientId(base_client_id),
      baseClientPassword(base_client_password),
      keepAliveInterval(keep_alive_interval),
      cleanSession(clean_session) {}
