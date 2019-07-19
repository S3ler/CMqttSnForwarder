//
// Created by SomeDude on 09.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNGATEWAY_MQTTSNGATEWAYARSMBDIRECTTESTCONTAINER_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNGATEWAY_MQTTSNGATEWAYARSMBDIRECTTESTCONTAINER_H_

#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <logging/MqttSnLoggingInterface.h>
#include <gateway/MqttSnGateway.h>
#include <gateway/config/gateway_config.h>
#include <network/linux/client/ip/udp/MqttSnClientUdpNetwork.h>
#include <logging/linux/file/FileLogging.h>
#include <logging/linux/filestdout/FileStdoutLogging.h>
#include "MqttSnGatewayTestContainerInterface.h"

using std::thread;
using std::atomic_bool;
using std::string;
using std::vector;
using std::shared_ptr;

class MqttSnGatewayArsmbDirectTestContainer : public MqttSnGatewayTestContainerInterface {
 private:
  vector<char> line_str;
  uint16_t argc = 0;
  const static uint16_t argv_max_len = 1024;
  char *argv[argv_max_len];
  FileLoggingContext file_logging_context_= {0};
  FileStdoutLoggingContext file_stdout_logging_context_ = {0};
  int32_t start_logger(const mqtt_sn_logger_config *cfg, MqttSnLogger *logger);

  MqttSnLogger gw_logger;
  mqtt_sn_gateway__config gateway_config;
  MqttSnGateway gateway;
  void *clientNetworkContext = NULL;

  thread runner;
  atomic_bool running{false};
  atomic_bool stopped{false};

  shared_ptr<MqttSnClientUdpNetwork> udpClientNetworkContext = nullptr;
 public:
  MqttSnGatewayArsmbDirectTestContainer(const string &identifier, const string &cmd);
  int32_t initialize() override;
  int32_t start() override;
  void stop() override;
  bool isStopped() override;
  bool isRunning() override;

  void loop();

 private:
  device_address forwarderClientNetworkAddress = {0};
  device_address mqttSnGatewayNetworkAddress = {0};
  device_address forwarderClientNetworkBroadcastAddress = {0};
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

#endif //CMQTTSNFORWARDER_TEST_MQTTSNGATEWAY_MQTTSNGATEWAYARSMBDIRECTTESTCONTAINER_H_
