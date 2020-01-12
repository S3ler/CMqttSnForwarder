//
// Created by SomeDude on 01.08.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTSEARCHGWCONNECTACTION_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTSEARCHGWCONNECTACTION_H_

#include "MqttSnClientConnectAction.h"

class MqttSnClientSearchGatewayConnectAction : public MqttSnClientConnectAction {
 public:
  const int32_t search_gateway_wait_timeout;
  const uint8_t search_gateway_radius;
  MqttSnClientSearchGatewayConnectAction(const int32_t search_gateway_wait_timeout, const uint8_t search_gateway_radius, const std::string& client_id, const std::string& client_password,
                                         const int32_t keep_alive_interval, const bool clean_session);
};

#endif //CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTSEARCHGWCONNECTACTION_H_
