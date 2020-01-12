//
// Created by SomeDude on 01.08.2019.
//

#include "MqttSnClientAdvertiseConnectAction.h"

MqttSnClientAdvertiseConnectAction::MqttSnClientAdvertiseConnectAction(int64_t advertiseWaitDuration, const std::string& client_id, const std::string& client_password, uint16_t keep_alive_interval,
                                                           bool clean_session)
    : MqttSnClientConnectAction(MqttSnClientActionType::ADVERTISE_CONNECT, client_id, client_password, keep_alive_interval, clean_session), advertiseWaitDuration(advertiseWaitDuration) {}