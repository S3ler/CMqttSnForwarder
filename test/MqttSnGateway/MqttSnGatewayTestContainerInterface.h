//
// Created by SomeDude on 18.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNGATEWAY_MQTTSNGATEWAYTESTCONTAINER_CPP_MQTTSNGATEWAYTESTCONTAINERINTERFACE_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNGATEWAY_MQTTSNGATEWAYTESTCONTAINER_CPP_MQTTSNGATEWAYTESTCONTAINERINTERFACE_H_

#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <logging/MqttSnLoggingInterface.h>
#include <gateway/MqttSnGateway.h>
#include <gateway/config/gateway_config.h>
#include <network/linux/client/ip/udp/MqttSnClientUdpNetwork.h>
class MqttSnGatewayTestContainerInterface {
 private:
  MqttSnGatewayTestContainerInterface();
 protected:
  const std::string identifier;
  const std::string cmd;
 public:
  MqttSnGatewayTestContainerInterface(const std::string &identifier, const std::string &cmd)
      : identifier(identifier), cmd(cmd) {}
  virtual ~MqttSnGatewayTestContainerInterface() {}
  virtual int32_t initialize() = 0;
  virtual int32_t start() = 0;
  virtual void stop() = 0;
  virtual bool isStopped() = 0;
  virtual bool isRunning() = 0;
};

#endif //CMQTTSNFORWARDER_TEST_MQTTSNGATEWAY_MQTTSNGATEWAYTESTCONTAINER_CPP_MQTTSNGATEWAYTESTCONTAINERINTERFACE_H_
