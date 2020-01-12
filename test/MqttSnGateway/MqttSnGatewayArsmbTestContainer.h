//
// Created by SomeDude on 09.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNGATEWAY_MQTTSNGATEWAYARSMBTESTCONTAINER_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNGATEWAY_MQTTSNGATEWAYARSMBTESTCONTAINER_H_

#include <gateway/MqttSnGateway.h>
#include <gateway/config/gateway_config.h>
#include <logging/MqttSnLogging.h>
#include <logging/linux/file/FileLogging.h>
#include <logging/linux/filestdout/FileStdoutLogging.h>
#include <network/linux/client/ip/udp/MqttSnClientUdpNetwork.h>
#include <atomic>
#include <string>
#include <thread>
#include <vector>
#include "MqttSnGatewayTestContainerInterface.h"

using std::thread;
using std::atomic_bool;
using std::string;
using std::vector;
using std::shared_ptr;

class MqttSnGatewayArsmbTestContainer : public MqttSnGatewayTestContainerInterface {
 private:
  /*
  vector<char> line_str;
  uint16_t argc = 0;
  const static uint16_t argv_max_len = 1024;
  char *argv[argv_max_len];
   */
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
 protected:
  std::string identifier;
  std::string cmd;

  vector<char> line_str;
  uint16_t argc = 0;
  const static uint16_t argv_max_len = 1024;
  char *argv[argv_max_len];
  int32_t CmdStrToArgcArgv(){
    line_str.clear();
    std::copy(cmd.begin(), cmd.end(), std::back_inserter(line_str));
    line_str.push_back('\0');

    //int32_t len = line_str.size();
    //char* line = line_str.data();
    /*
    if (len == 0) {
      assert(!(len == 0));
      return -1;
    }

    if (strlen(line) + 1 != len) {
      assert(!(strlen(line) + 1 != len));
      return -1;
    }

    if (strlen(line) == 0) {
      assert(!(strlen(line) == 0));
      return -1;
    }
    */
    int tk_count = 0;
    {
      vector<char> line_copy;
      std::copy(line_str.begin(), line_str.end(), std::back_inserter(line_copy));
      // estimates argc
      for (char *tk = strtok(line_copy.data(), " "); tk != NULL; tk = strtok(NULL, " ")) {
        tk_count++;
      }
    }
    if (tk_count > argv_max_len) {
      // TOOD log too much argc
      return -1;
    }
    //char *argv[tk_count];
    argc = 0;
    memset(argv, 0, argv_max_len*sizeof(char*));
    {
      argv[argc++] = (char *) identifier.data();
      for (char *tk = strtok(line_str.data(), " "); tk != NULL; tk = strtok(NULL, " ")) {
        argv[argc++] = tk;
      }
    }

    // remove '\n' from tokens
    for (uint16_t i = 1; i < argc; i++) {
      if (argv[i][(strlen(argv[i]) - 1)] == '\n') {
        argv[i][(strlen(argv[i]) - 1)] = '\0';
      }
    }
    return 0;
  }
 public:
  //MqttSnGatewayArsmbTestContainer(const string &identifier, const string &cmd);
  MqttSnGatewayArsmbTestContainer(const MqttSnGatewayTestContainerConfiguration &configuration);
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
  /*int start_mqtt_sn_gateway_plugin(const mqtt_sn_gateway__config *fcfg,
                                   const MqttSnLogger *logger,
                                   MqttSnGateway *mqtt_sn_gateway);*/
  int start_mqtt_sn_gateway_tcp(const mqtt_sn_gateway__config *fcfg,
                                const MqttSnLogger *logger,
                                MqttSnGateway *mqtt_sn_gateway);
  int start_mqtt_sn_gateway_client_udp(const mqtt_sn_gateway__config *fcfg,
                                       const MqttSnLogger *logger,
                                       MqttSnGateway *mqtt_sn_gateway);
};

#endif //CMQTTSNFORWARDER_TEST_MQTTSNGATEWAY_MQTTSNGATEWAYARSMBTESTCONTAINER_H_
