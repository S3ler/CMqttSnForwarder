//
// Created by SomeDude on 09.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTTESTCONTAINER_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTTESTCONTAINER_H_

#include <string>
#include <thread>
#include <atomic>
#include <logging/MqttSnLoggingInterface.h>
#include <forwarder/config/forwarder_config.h>
#include <client/MqttSnClient.h>
#include <client/pub/config/publish_client_config.h>
#include <network/linux/gateway/ip/tcp/MqttSnGatewayTcpNetwork.h>
#include <network/linux/gateway/plugin/MqttSnGatewayPluginNetwork.h>
#include <logging/linux/file/FileLogging.h>
#include <logging/linux/filestdout/FileStdoutLogging.h>

using std::thread;
using std::atomic_bool;
using std::vector;
using std::string;
using std::shared_ptr;

class MqttSnClientTestContainer {
 private:
  string identifier;
  string cmd;

  vector<char> line_str;
  uint16_t argc = 0;
  const static uint16_t argv_max_len = 1024;
  char *argv[argv_max_len];
  FileLoggingContext file_logging_context_= {0};
  FileStdoutLoggingContext file_stdout_logging_context_ = {0};
  int32_t start_logger(const mqtt_sn_logger_config *cfg, MqttSnLogger *logger);

  MqttSnLogger client_logger;
  publish_client_config client_cfg;
  MqttSnClient client;
  void *gatewayNetworkContext = NULL;

  thread runner;
  atomic_bool running{false};
  atomic_bool stopped{false};



 public:
  MqttSnClientTestContainer(const string &identifier, const string &cmd);
  int32_t initialize();
  int32_t start();
  void stop();
  bool isStopped();
  bool isRunning();
  void loop();

 private:
  device_address mqttSnGatewayNetworkAddress = {0};
  device_address forwarderGatewayNetworkAddress = {0};
  device_address forwarderGatewayNetworkBroadcastAddress = {0};

#ifdef WITH_LINUX_PLUGIN_NETWORK
  shared_ptr<MqttSnGatewayPluginContext> gatewayPluginContext = nullptr;
#endif
#ifdef WITH_LINUX_TCP_GATEWAY_NETWORK
  shared_ptr<MqttSnGatewayTcpNetwork> tcpGatewayNetworkContext = nullptr;
#endif
#ifdef WITH_LINUX_UDP_GATEWAY_NETWORK
  shared_ptr<MqttSnGatewayUdpNetwork> udpGatewayNetworkContext = nullptr;
#endif

  int32_t start_publish_client_plugin(const publish_client_config *cfg,
                                      MqttSnClient *publish_client,
                                      const MqttSnLogger *logger);
  int32_t start_publish_client_udp(const publish_client_config *cfg,
                                   MqttSnClient *publish_client,
                                   const MqttSnLogger *logger);
  int32_t start_publish_client_tcp(const publish_client_config *cfg,
                                   MqttSnClient *publish_client,
                                   const MqttSnLogger *logger);
};

#endif //CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTTESTCONTAINER_H_
