//
// Created by SomeDude on 20.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNGATEWAY_MQTTSNGATEWAYPAHOTESTCONTAINER_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNGATEWAY_MQTTSNGATEWAYPAHOTESTCONTAINER_H_

#include "MqttSnGatewayTestContainerInterface.h"
#include <MQTTSNGateway.h>
#include <MQTTSNGWBrokerRecvTask.h>
#include <MQTTSNGWBrokerSendTask.h>
#include <MQTTSNGWClientRecvTask.h>
#include <MQTTSNGWClientSendTask.h>
#include <MQTTSNGWPacketHandleTask.h>

using namespace MQTTSNGW;

class MqttSnGatewayPahoTestContainer : public MqttSnGatewayTestContainerInterface {
 private:
  std::string gatewayIdentifierString;
  Gateway gateway;
  PacketHandleTask  task1;
  ClientRecvTask    task2;
  ClientSendTask    task3;
  BrokerRecvTask    task4;
  BrokerSendTask    task5;

  thread runner;
  atomic_bool running{false};
  atomic_bool stopped{false};
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
    memset(argv, 0, argv_max_len);
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
  MqttSnGatewayPahoTestContainer(const MqttSnGatewayTestContainerConfiguration &configuration);
  int32_t initialize() override;
  int32_t start() override;
  void stop() override;
  bool isStopped() override;
  bool isRunning() override;
  void loop();
  bool createGatewayConfFile(const MqttSnGatewayTestContainerConfiguration &cfg);
  bool removeGatewayConfFile();
  bool createPredefinedTopicConfFile();
  bool removePredefinedTopicConfFile();

};

#endif //CMQTTSNFORWARDER_TEST_MQTTSNGATEWAY_MQTTSNGATEWAYPAHOTESTCONTAINER_H_
