//
// Created by SomeDude on 22.07.2019.
//

#include "MqttClientReceivePublishAction.h"
MqttClientReceivePublishAction::MqttClientReceivePublishAction(const std::string &subscription_topic_name,
                                                               const std::vector<uint8_t> &payload,
                                                               const int32_t subscription_qos)
    : subscription_topic_name(subscription_topic_name), payload(payload), subscription_qos(subscription_qos) {}
