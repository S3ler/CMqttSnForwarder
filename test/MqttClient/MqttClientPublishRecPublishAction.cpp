//
// Created by SomeDude on 22.07.2019.
//

#include "MqttClientPublishRecPublishAction.h"
MqttClientPublishRecPublishAction::MqttClientPublishRecPublishAction(const MqttClientPublishAction &publish_action,
                                                                     const MqttClientReceivePublishAction &receive_publish_action)
    : MqttClientAction(MqttClientActionType::PUBLISHRECEIVEPUBLISH),
      publish_action(publish_action),
      receive_publish_action(receive_publish_action) {}
