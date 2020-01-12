//
// Created by SomeDude on 24.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNFORWARDER_MQTTSNFORWARDERTESTCONTAINERINTERFACE_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNFORWARDER_MQTTSNFORWARDERTESTCONTAINERINTERFACE_H_

#include <stdint.h>
#include "MqttSnForwarderTestContainerConfiguration.h"

class MqttSnForwarderTestContainerInterface {
 public:
  const MqttSnForwarderTestContainerConfiguration configuration;
  MqttSnForwarderTestContainerInterface(const MqttSnForwarderTestContainerConfiguration& configuration) : configuration(configuration) {}

  virtual ~MqttSnForwarderTestContainerInterface() {}
  virtual int32_t initialize() = 0;
  virtual int32_t start() = 0;
  virtual void stop() = 0;
  virtual bool isStopped() = 0;
  virtual bool isRunning() = 0;
};

#endif  // CMQTTSNFORWARDER_TEST_MQTTSNFORWARDER_MQTTSNFORWARDERTESTCONTAINERINTERFACE_H_
