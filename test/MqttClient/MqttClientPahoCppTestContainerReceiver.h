//
// Created by SomeDude on 23.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTPAHOCPPTESTCONTAINERRECEIVER_H_
#define CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTPAHOCPPTESTCONTAINERRECEIVER_H_

#include "MqttClientTestContainerReceiverInterface.h"
#include "MqttClientPublishReceivePublish.h"
#include "MqttClientPahoCppClientTestContainer.h"

class MqttClientPahoCppClientTestContainer;

class MqttClientPahoCppTestContainerReceiver : public MqttClientTestContainerReceiverInterface {
 private:
  MqttClientPahoCppClientTestContainer *callee;
 public:
  const MqttClientPublishReceivePublish publish_receive_publish;
  MqttClientPahoCppTestContainerReceiver(const MqttClientPublishReceivePublish &publish_receive_publish,
                                         MqttClientPahoCppClientTestContainer *callee) : publish_receive_publish(
      publish_receive_publish), callee(callee) {}
  bool receive(MqttClientTestContainerPublish mqtt_publish) override {
    timespec end_ts;
    clock_gettime(CLOCK_REALTIME, &end_ts);
    if (mqttPublishEquals(publish_receive_publish, mqtt_publish)) {
      callee->on_receive_publish_action_finished(MqttClientActionResultType::SUCCESS, end_ts);
    }
    return true;
  }
  bool mqttPublishEquals(const MqttClientPublishReceivePublish &exp, const MqttClientTestContainerPublish &rec) const {
    if (exp.receivePublishQos != rec.qos) {
      return false;
    }
    if (exp.publish_action.topic_name != rec.topic) {
      return false;
    }
    if (exp.publish_action.payload != rec.data) {
      return false;
    }
    /* // TODO retain
    if (exp.publish_action.retain != rec.retain) {
           return false;
    }
    */
    return true;
  }
};
#endif //CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTPAHOCPPTESTCONTAINERRECEIVER_H_
