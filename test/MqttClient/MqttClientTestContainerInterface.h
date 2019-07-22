//
// Created by SomeDude on 21.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTTESTCONTAINERINTERFACE_H_
#define CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTTESTCONTAINERINTERFACE_H_

#include <string>
#include <memory>
#include "../MqttNetworkObjectBuilder/MqttClientTestContainerConnectOptions.h"
#include "MqttClientConnectAction.h"
#include "MqttClientPublishAction.h"
#include "MqttClientSubscribeAction.h"
#include "MqttClientDisconnectAction.h"
#include "MqttClientActionResult.h"
#include "MqttClientTestContainerReceiverInterface.h"

class MqttClientTestContainerInterface {
 protected:
  uint64_t action_number = 0;
  MqttClientTestContainerReceiverInterface* receiver_interface = nullptr;
 public:
  MqttClientConnectAction initial_connect_action;
  MqttClientTestContainerInterface(const MqttClientConnectAction &initial_connect_action);
  void SetReceiverInterface(MqttClientTestContainerReceiverInterface *receiver_interface);
  // synchronous methods - return time in ns?
  virtual MqttClientActionResult connect() = 0;

  virtual bool StartBackgroundHandler() = 0;
  virtual void StopBackgroundHandler() = 0;
  virtual bool IsBackgroundHandlerRunning() = 0;

  virtual uint64_t connect(MqttClientConnectAction &connect_action) = 0;
  virtual MqttClientActionResult disconnect() = 0;
  virtual uint64_t publish(MqttClientPublishAction &publish_action) = 0;
  virtual MqttClientActionResult subscribe(MqttClientSubscribeAction &subscribe_action) = 0;

  // adds a action to the Sequence
  void addAction(MqttClientAction action);
  bool executeSequence();
  /**
   *
   * @param times 0
   * @return
   */
  // TODO bool repeatSeries(int32_t times);

  void addConnectAction(MqttClientConnectAction connect_action);
  void addDisconnectAction(MqttClientDisconnectAction disconnect_action);
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
  std::vector<MqttClientAction> sequence;
  std::vector<std::vector<MqttClientAction>> actionSeries;
};

#endif //CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTTESTCONTAINERINTERFACE_H_
