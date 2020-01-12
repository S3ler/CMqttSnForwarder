//
// Created by SomeDude on 18.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNGATEWAY_MQTTSNGATEWAYTESTCONTAINER_CPP_MQTTSNGATEWAYTESTCONTAINERINTERFACE_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNGATEWAY_MQTTSNGATEWAYTESTCONTAINER_CPP_MQTTSNGATEWAYTESTCONTAINERINTERFACE_H_

#include <string.h>
#include <atomic>
#include <string>
#include <thread>
#include <vector>
#include "MqttSnGatewayTestContainerConfiguration.h"

using std::vector;

class MqttSnGatewayTestContainerInterface {
 public:
  const MqttSnGatewayTestContainerConfiguration configuration;
  MqttSnGatewayTestContainerInterface(const MqttSnGatewayTestContainerConfiguration &configuration) : configuration(configuration) {}

  virtual ~MqttSnGatewayTestContainerInterface() {}
  virtual int32_t initialize() = 0;
  virtual int32_t start() = 0;
  virtual void stop() = 0;
  virtual bool isStopped() = 0;
  virtual bool isRunning() = 0;
};

#endif  // CMQTTSNFORWARDER_TEST_MQTTSNGATEWAY_MQTTSNGATEWAYTESTCONTAINER_CPP_MQTTSNGATEWAYTESTCONTAINERINTERFACE_H_
