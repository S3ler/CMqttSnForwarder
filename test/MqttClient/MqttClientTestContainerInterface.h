//
// Created by SomeDude on 21.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTTESTCONTAINERINTERFACE_H_
#define CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTTESTCONTAINERINTERFACE_H_

#include <string>
#include <atomic>
#include <thread>
#include <memory>
#include "MqttClientConnectAction.h"
#include "MqttClientPublishAction.h"
#include "MqttClientSubscribeAction.h"
#include "MqttClientDisconnectAction.h"
#include "MqttClientActionResult.h"
#include "MqttClientTestContainerReceiverInterface.h"
#include "MqttClientUnsubscribeAction.h"
#include "MqttClientPublishReceivePublish.h"
#include "MqttClientActionSequence.h"

class MqttClientTestContainerInterface {
 protected:
  uint64_t action_number = 0;
  MqttClientTestContainerReceiverInterface *receiver_interface = nullptr;
 public:
  MqttClientConnectAction initial_connect_action;
  MqttClientTestContainerInterface(const MqttClientConnectAction &initial_connect_action);
  void SetReceiverInterface(MqttClientTestContainerReceiverInterface *receiver_interface);
  void ReceivePublish(MqttClientTestContainerPublish &mqtt_publish);

  virtual bool StartBackgroundHandler() = 0;
  virtual void StopBackgroundHandler() = 0;
  virtual bool IsBackgroundHandlerRunning() = 0;

  virtual MqttClientActionResult connect() = 0;
  //virtual MqttClientActionResult connect(MqttClientConnectAction &connect_action) = 0;
  virtual MqttClientActionResult disconnect() = 0;
  virtual MqttClientActionResult subscribe(MqttClientSubscribeAction &subscribe_action) = 0;
  virtual MqttClientActionResult unsubscribe(MqttClientUnsubscribeAction &unsubscribe_action) = 0;
  virtual MqttClientActionResult publish(MqttClientPublishAction &publish_action) = 0;
  virtual MqttClientActionResult publishReceivePublish(MqttClientPublishReceivePublish &publish_receive_publish) = 0;
  // adds a action to the Sequence
  void addAction(std::unique_ptr<MqttClientAction> &action);
  bool execActionSequence();
  bool execActionSequence_async();
  const bool IsActionSequenceExecuted() const;
  const std::vector<MqttClientActionResult> &GetResult() const;
  std::shared_ptr<MqttClientActionSequence> generateActionSequence(int32_t exec_count);
  // TODO ich will die sequence wissen ob sie läuft
  // TODO ich will sie abbrechen können
  /**
   *
   * @param times 0
   * @return
   */
  // TODO bool repeatSeries(int32_t times);

  // TODO bool addPublishAction(MqttClientPublishAction publish_action);
  // TODO bool addSubscribeAction(MqttClientSubscribeAction subscribe_action);

  // saves the current saved sequence as series
  // empties the sequence
  // return the series number;
  // TODO int32_t saveSequenceAsSeries();
  // TODO bool executeSeries(int32_t series_number);
  // TODO bool repeatSeries(int32_t series_number, int32_t times);
  // TODO series timeout
  // TODO sequence timeout
  // TODO action timeout

  // TODO get results
 private:
  std::shared_ptr<std::vector<std::unique_ptr<MqttClientAction>>> sequence;
  std::vector<std::shared_ptr<MqttClientActionSequence>> actionSequences;
  std::vector<MqttClientActionResult> result;
  bool executeSequence(std::shared_ptr<std::vector<std::unique_ptr<MqttClientAction>>> &sequence);
  std::atomic_bool running_action_sequence{false};
  std::thread thread;
};

#endif //CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTTESTCONTAINERINTERFACE_H_
