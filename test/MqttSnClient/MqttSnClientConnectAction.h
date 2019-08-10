//
// Created by SomeDude on 24.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTCONNECTACTION_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTCONNECTACTION_H_

#include <stdint.h>
#include <string>
#include <vector>
#include "MqttSnClientAction.h"
#include "MqttSnClientProtocolTestType.h"
#include "MqttSnClientTestContainerConfiguration.h"

class MqttSnClientConnectAction : public MqttSnClientAction {
 public:
  const std::string clientId;
  const std::string clientPassword;

  const int32_t keepAliveInterval;
  const bool cleanSession;

  // TODO will
  MqttSnClientConnectAction(const MqttSnClientActionType action_type,
                            const std::string &client_id,
                            const std::string &client_password,
                            const int32_t keep_alive_interval,
                            const bool clean_session);
};

#endif //CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTCONNECTACTION_H_
