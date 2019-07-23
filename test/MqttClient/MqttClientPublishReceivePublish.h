//
// Created by SomeDude on 23.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTPUBLISHRECEIVEPUBLISH_H_
#define CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTPUBLISHRECEIVEPUBLISH_H_

#include "MqttClientAction.h"
#include "MqttClientPublishAction.h"

class MqttClientPublishReceivePublish  : public MqttClientAction {
 public:
  const MqttClientPublishAction publish_action;
  const int32_t receivePublishQos;
  MqttClientPublishReceivePublish(const MqttClientPublishAction &publish_action);
  MqttClientPublishReceivePublish(const MqttClientPublishAction &publish_action,
                                  const int32_t receive_publish_qos);
};

#endif //CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTPUBLISHRECEIVEPUBLISH_H_
