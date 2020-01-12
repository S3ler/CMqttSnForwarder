//
// Created by SomeDude on 09.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNFORWARDER_MQTTSNFORWARDERTESTCONTAINER_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNFORWARDER_MQTTSNFORWARDERTESTCONTAINER_H_

#include <forwarder/MqttSnForwarder.h>
#include <forwarder/config/forwarder_config.h>
#include <thread>
#include <atomic>
#include <vector>
#include <string>
#include <network/linux/gateway/ip/udp/MqttSnGatewayUdpNetwork.h>
#include <network/linux/client/ip/udp/MqttSnClientUdpNetwork.h>
#include <network/linux/gateway/ip/tcp/MqttSnGatewayTcpNetwork.h>
#include <network/linux/client/ip/tcp/MqttSnClientTcpNetwork.h>
#include <network/linux/gateway/plugin/MqttSnGatewayPluginNetwork.h>
#include <network/linux/client/plugin/MqttSnClientPluginNetwork.h>
#include <logging/linux/file/FileLogging.h>
#include <logging/linux/filestdout/FileStdoutLogging.h>

using std::thread;
using std::atomic_bool;
using std::string;
using std::vector;
using std::shared_ptr;

class MqttSnForwarderTestContainer {
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

 private:
  device_address mqttSnGatewayNetworkAddress = {0};
  device_address forwarderClientNetworkAddress = {0};
  device_address forwarderClientNetworkBroadcastAddress = {0};

  device_address forwarderGatewayNetworkAddress = {0};
  device_address forwarderGatewayNetworkBroadcastAddress = {0};
#ifdef WITH_LINUX_PLUGIN_NETWORK
  shared_ptr<gateway_plugin_device_address> pluginMqttSnGatewayNetworkAddress = nullptr;
  shared_ptr<gateway_plugin_device_address> pluginForwarderGatewayNetworkAddress = nullptr;
  shared_ptr<gateway_plugin_device_address> pluginForwarderGatewayNetworkBroadcastAddress = nullptr;
  shared_ptr<gateway_plugin_config> plugin_cfg = nullptr;
  shared_ptr<MqttSnGatewayPluginContext> gatewayPluginContext = nullptr;
  shared_ptr<MqttSnClientPluginContext> clientPluginContext = nullptr;
#endif
#ifdef WITH_LINUX_TCP_CLIENT_NETWORK
  shared_ptr<MqttSnGatewayTcpNetwork> tcpGatewayNetworkContext = nullptr;
  shared_ptr<MqttSnClientTcpNetwork> tcpClientNetworkContext = nullptr;
#endif
#ifdef WITH_LINUX_UDP_CLIENT_NETWORK
  shared_ptr<MqttSnGatewayUdpNetwork> udpGatewayNetworkContext = nullptr;
  shared_ptr<MqttSnClientUdpNetwork> udpClientNetworkContext = nullptr;
#endif

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
