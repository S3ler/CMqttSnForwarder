//
// Created by SomeDude on 01.08.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTADVERTISECONNECTACTION_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTADVERTISECONNECTACTION_H_

#include "MqttSnClientConnectAction.h"
class MqttSnClientAdvertiseConnectAction : public MqttSnClientConnectAction {
 public:
  const int32_t advertise_wait_timeout;
  MqttSnClientAdvertiseConnectAction(const MqttSnClientProtocolTestType a_protocol,
                                     const int32_t advertise_wait_duration,
                                     const std::string &client_id,
                                     const std::string &client_password,
                                     const int32_t keep_alive_interval,
                                     const bool clean_session);
};

#endif //CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTADVERTISECONNECTACTION_H_
