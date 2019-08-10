//
// Created by SomeDude on 01.08.2019.
//

#include "MqttSnClientAdvertiseConnectAction.h"
MqttSnClientAdvertiseConnectAction::MqttSnClientAdvertiseConnectAction(const MqttSnClientProtocolTestType a_protocol,
                                                                       const int32_t advertise_wait_duration,
                                                                       const std::string &client_id,
                                                                       const std::string &client_password,
                                                                       const int32_t keep_alive_interval,
                                                                       const bool clean_session)
    : MqttSnClientConnectAction(MqttSnClientActionType::ADVERTISECONNECT,
                                client_id,
                                client_password,
                                keep_alive_interval,
                                clean_session),
      advertise_wait_timeout(advertise_wait_duration) {}
