//
// Created by SomeDude on 09.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNGATEWAY_MQTTSNGATEWAYTESTCONTAINER_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNGATEWAY_MQTTSNGATEWAYTESTCONTAINER_H_

#include <string>
#include <thread>
#include <atomic>
#include <logging/MqttSnLoggingInterface.h>
#include <gateway/MqttSnGateway.h>
#include <gateway/config/gateway_config.h>

using std::thread;
using std::atomic_bool;
using std::string;

class MqttSnGatewayTestContainer {
 private:
  string identifier;
  string cmd;

  MqttSnLogger gw_logger;
  mqtt_sn_gateway__config gateway_config;
  MqttSnGateway gateway;
  void *clientNetworkContext = NULL;

  thread runner;
  atomic_bool running{false};
  atomic_bool stopped{false};
 public:
  MqttSnGatewayTestContainer(const string &identifier, const string &cmd);
  int32_t initialize();
  int32_t start();
  void stop();
  bool isStopped();
  bool isRunning();

  void loop();

 private:
  int start_mqtt_sn_gateway_plugin(const mqtt_sn_gateway__config *fcfg,
                                   const MqttSnLogger *logger,
                                   MqttSnGateway *mqtt_sn_gateway);
  int start_mqtt_sn_gateway_tcp(const mqtt_sn_gateway__config *fcfg,
                                const MqttSnLogger *logger,
                                MqttSnGateway *mqtt_sn_gateway);
  int start_mqtt_sn_gateway_client_udp(const mqtt_sn_gateway__config *fcfg,
                                       const MqttSnLogger *logger,
                                       MqttSnGateway *mqtt_sn_gateway);
};

#endif //CMQTTSNFORWARDER_TEST_MQTTSNGATEWAY_MQTTSNGATEWAYTESTCONTAINER_H_
