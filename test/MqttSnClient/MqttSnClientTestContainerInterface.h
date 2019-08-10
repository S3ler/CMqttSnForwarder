//
// Created by SomeDude on 24.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTTESTCONTAINERINTERFACE_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTTESTCONTAINERINTERFACE_H_

#include <stdint.h>
#include "MqttSnClientTestContainerConfiguration.h"
#include <atomic>
#include <thread>
#include "MqttSnClientActionSequence.h"
#include "MqttSnClientActionResult.h"
#include "MqttSnClientDirectConnectAction.h"
#include "MqttSnClientAdvertiseConnectAction.h"
#include "MqttSnClientSearchGwConnectAction.h"
#include "MqttSnClientTestContainerPublish.h"
#include "MqttSnClientTestContainerReceiverInterface.h"

class MqttSnClientTestContainerInterface {
 private:
  MqttSnClientTestContainerReceiverInterface *receiver_interface = nullptr;
 protected:
  uint64_t action_number = 0;
 public:
  const MqttSnClientTestContainerConfiguration configuration;
  MqttSnClientTestContainerInterface(MqttSnClientTestContainerConfiguration configuration);
  void SetReceiverInterface(MqttSnClientTestContainerReceiverInterface *receiver_interface);
  void ReceivePublish(MqttSnClientTestContainerPublish &mqtt_sn_publish);

  virtual bool StartBackgroundHandler() = 0;
  virtual void StopBackgroundHandler() = 0;
  virtual bool IsBackgroundHandlerRunning() = 0;

  virtual MqttSnClientActionResult connect(MqttSnClientDirectConnectAction &connect_action) = 0;
  virtual MqttSnClientActionResult connect(MqttSnClientAdvertiseConnectAction &connect_action) = 0;
  virtual MqttSnClientActionResult connect(MqttSnClientSearchGwConnectAction &connect_action) = 0;
  virtual MqttSnClientActionResult disconnect() = 0;

  void addAction(std::unique_ptr<MqttSnClientAction> &action);
  bool execActionSequence();
  bool execActionSequence_async();
  const bool IsActionSequenceExecuted() const;
  const std::vector<MqttSnClientActionResult> &GetResult() const;
  std::shared_ptr<MqttSnClientActionSequence> generateActionSequence(int32_t exec_count);

 private:
  std::shared_ptr<std::vector<std::unique_ptr<MqttSnClientAction>>> sequence;
  std::vector<std::shared_ptr<MqttSnClientActionSequence>> actionSequences;
  std::vector<MqttSnClientActionResult> result;
  bool executeSequence(std::shared_ptr<std::vector<std::unique_ptr<MqttSnClientAction>>> &sequence);
  std::atomic_bool running_action_sequence{false};
  std::thread thread;
};

#endif //CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTTESTCONTAINERINTERFACE_H_
