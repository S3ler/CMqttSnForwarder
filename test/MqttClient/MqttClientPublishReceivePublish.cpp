//
// Created by SomeDude on 23.07.2019.
//

#include "MqttClientPublishReceivePublish.h"
MqttClientPublishReceivePublish::MqttClientPublishReceivePublish(const MqttClientPublishAction &publish_action,
                                                                 const int32_t receive_publish_qos)
    : MqttClientAction(MqttClientActionType::PUBLISHRECEIVEPUBLISH),
      publish_action(publish_action),
      receivePublishQos(receive_publish_qos) {}
MqttClientPublishReceivePublish::MqttClientPublishReceivePublish(const MqttClientPublishAction &publish_action)
    : MqttClientAction(MqttClientActionType::PUBLISHRECEIVEPUBLISH),
      publish_action(publish_action),
      receivePublishQos(publish_action.qos) {}
