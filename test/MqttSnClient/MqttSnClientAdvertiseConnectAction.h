//
// Created by SomeDude on 01.08.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTADVERTISECONNECTACTION_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTADVERTISECONNECTACTION_H_

#include "MqttSnClientConnectAction.h"
class MqttSnClientAdvertiseConnectAction : public MqttSnClientConnectAction {
 public:
  const int64_t advertiseWaitDuration;
  MqttSnClientAdvertiseConnectAction(int64_t advertiseWaitDuration, const std::string& client_id, const std::string& client_password, uint16_t keep_alive_interval, bool clean_session);
};

#endif //CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTADVERTISECONNECTACTION_H_
