//
// Created by SomeDude on 09.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNFORWARDER_MQTTSNFORWARDERTESTCONTAINER_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNFORWARDER_MQTTSNFORWARDERTESTCONTAINER_H_

#include <forwarder/MqttSnForwarder.h>
#include <forwarder/config/forwarder_config.h>
#include <thread>
#include <atomic>

using std::thread;
using std::atomic_bool;
using std::string;

class MqttSnForwarderTestContainer {
 private:
  string identifier;
  string cmd;

  MqttSnLogger fw_logger;
  forwarder_config fw_cfg;
  MqttSnForwarder forwarder;
  void *gatewayNetworkContext = NULL;
  void *clientNetworkContext = NULL;

  thread runner;
  atomic_bool running{false};
  atomic_bool stopped{false};
 private:
  int32_t start_client_plugin(const forwarder_config *fcfg,
                          const MqttSnLogger *logger,
                          MqttSnForwarder *mqttSnForwarder);
  int32_t start_client_tcp(const forwarder_config *fcfg,
                       const MqttSnLogger *logger,
                       MqttSnForwarder *mqttSnForwarder);
  int32_t start_client_udp(const forwarder_config *fcfg,
                       const MqttSnLogger *logger,
                       MqttSnForwarder *mqttSnForwarder);
 public:
  MqttSnForwarderTestContainer(const string &identifier, const string &cmd);
  int32_t initialize();
  int32_t start();
  void stop();
  bool isStopped();
  bool isRunning();

  void loop();

  int32_t start_gateway_plugin(const forwarder_config *cfg,
                               const MqttSnLogger *logger,
                               MqttSnForwarder *mqttSnForwarder);
  int32_t start_gateway_tcp(const forwarder_config *cfg,
                            const MqttSnLogger *logger,
                            MqttSnForwarder *mqttSnForwarder);
  int32_t start_gateway_udp(const forwarder_config *cfg,
                            const MqttSnLogger *logger,
                            MqttSnForwarder *mqttSnForwarder);
};
#endif //CMQTTSNFORWARDER_TEST_MQTTSNFORWARDER_MQTTSNFORWARDERTESTCONTAINER_H_
