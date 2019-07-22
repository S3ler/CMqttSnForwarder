//
// Created by SomeDude on 22.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTPUBLISHRECPUBLISHACTION_H_
#define CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTPUBLISHRECPUBLISHACTION_H_

#include "MqttClientPublishAction.h"
#include "MqttClientReceivePublishAction.h"

class MqttClientPublishRecPublishAction : public MqttClientAction{
  const MqttClientPublishAction publish_action;
  const MqttClientReceivePublishAction receive_publish_action;

 public:
  MqttClientPublishRecPublishAction(const MqttClientPublishAction &publish_action,
                                    const MqttClientReceivePublishAction &receive_publish_action);
};

#endif //CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTPUBLISHRECPUBLISHACTION_H_
