//
// Created by SomeDude on 01.08.2019.
//

#include "MqttSnClientSearchGatewayConnectAction.h"

MqttSnClientSearchGatewayConnectAction::MqttSnClientSearchGatewayConnectAction(const int32_t search_gateway_wait_timeout, const uint8_t search_gateway_radius, const std::string& client_id,
                                                                               const std::string& client_password, const int32_t keep_alive_interval, const bool clean_session)
    : MqttSnClientConnectAction(MqttSnClientActionType ::SEARCHGW_CONNECT, client_id, client_password, keep_alive_interval, clean_session),
      search_gateway_wait_timeout(search_gateway_wait_timeout), search_gateway_radius(search_gateway_radius) {}
