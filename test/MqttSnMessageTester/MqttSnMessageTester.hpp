//
// Created by SomeDude on 17.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNMESSAGETESTER_MQTTSNMESSAGETESTER_HPP_
#define CMQTTSNFORWARDER_TEST_MQTTSNMESSAGETESTER_MQTTSNMESSAGETESTER_HPP_

#include <stdint.h>
#include <forwarder/config/forwarder_config.h>
#include <logging/linux/file/FileLogging.h>
#include <logging/linux/filestdout/FileStdoutLogging.h>
#include <network/MqttSnGatewayNetworkInterface.h>
#include <network/MqttSnClientNetworkInterface.h>
#include <ringbuffer/MqttSnFixedSizeRingBuffer.h>
#include "MqttSnTestMessageReceiverInterface.hpp"
#include "MqttSnTestMessageReceiverMock.hpp"
#include "MqttSnTestMessageSender.hpp"
#include "NetworkContextTestContainer.h"
#include <atomic>
#include <memory>
#include <string>
#include <vector>

class MqttSnTestMessageSender;
class MqttSnTestMessageReceiverInterface;

using std::shared_ptr;
using std::string;
using std::vector;

class MqttSnMessageTester {
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

 private:
  MqttSnTestMessageReceiverInterface *receiver = nullptr;

  void *clientNetworkContext = nullptr;
  void *gatewayNetworkContext = nullptr;

  MqttSnFixedSizeRingBuffer gatewayNetworkReceiveBuffer;
  MqttSnFixedSizeRingBuffer gatewayNetworkSendBuffer;
  MqttSnGatewayNetworkInterface gatewayNetwork;

  MqttSnFixedSizeRingBuffer clientNetworkReceiveBuffer;
  MqttSnFixedSizeRingBuffer clientNetworkSendBuffer;
  MqttSnClientNetworkInterface clientNetwork;

  int32_t clientNetworkSendTimeout = 10;
  int32_t clientNetworkReceiveTimeout = 10;
  int32_t gatewayNetworkSendTimeout = 10;
  int32_t gatewayNetworkReceiveTimeout = 10;

  shared_ptr<NetworkContextTestContainer> clientNetworkContextTestContainer;
  shared_ptr<NetworkContextTestContainer> gatewayNetworkContextTestContainer;

  std::atomic_bool stopped{false};
  std::atomic_bool running{false};
  std::thread thread;

 public:
  MqttSnMessageTester();
  bool send(const uint8_t *data, uint16_t data_length);
  bool isRunning();
  bool start();
  void stop();

  void loop();
  void receive(MqttSnMessageData *message);

  void SetReceiver(MqttSnTestMessageReceiverInterface *receiver);
  bool SetConfiguration(const std::string &identifier, const std::string &command);

  bool InitializeClientNetwork();
  bool InitializeGatewayNetwork();
};

#endif //CMQTTSNFORWARDER_TEST_MQTTSNMESSAGETESTER_MQTTSNMESSAGETESTER_HPP_
