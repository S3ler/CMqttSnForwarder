//
// Created by SomeDude on 23.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTPAHOCPPTESTCONTAINERRECEIVER_H_
#define CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTPAHOCPPTESTCONTAINERRECEIVER_H_

#include "MqttClientPahoCppClientTestContainer.h"
#include "MqttClientPublishReceivePublishAction.h"
#include "MqttClientTestContainerReceiverInterface.h"

class MqttClientPahoCppClientTestContainer;

class MqttClientPahoCppTestContainerReceiver : public MqttClientTestContainerReceiverInterface {


 public:
  const MqttClientPublishReceivePublishAction publish_receive_publish;
  MqttClientPahoCppTestContainerReceiver(const MqttClientPublishReceivePublishAction &publish_receive_publish, MqttClientPahoCppClientTestContainer *callee)
      : publish_receive_publish(publish_receive_publish), callee(callee) {}
  bool receive(MqttClientTestContainerPublish mqtt_publish) override {
    timespec end_ts;
    clock_gettime(CLOCK_REALTIME, &end_ts);
    if (mqttPublishEquals(publish_receive_publish, mqtt_publish)) {
      callee->on_receive_publish_action_finished(MqttClientActionResultType::SUCCESS, end_ts);
    }
    return true;
  }
  bool mqttPublishEquals(const MqttClientPublishReceivePublishAction &exp, const MqttClientTestContainerPublish &rec) const {
    if (exp.topicName != rec.topic) {
      return false;
    }
    if (exp.retain != rec.retain) {
      return false;
    }
    return !(exp.payload != rec.data);
  }
 private:
  MqttClientPahoCppClientTestContainer *callee;
};
#endif  // CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTPAHOCPPTESTCONTAINERRECEIVER_H_
