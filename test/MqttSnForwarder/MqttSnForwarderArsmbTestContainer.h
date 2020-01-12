//
// Created by SomeDude on 17.09.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNFORWARDERARSMBTESTCONTAINER_H
#define CMQTTSNFORWARDER_MQTTSNFORWARDERARSMBTESTCONTAINER_H

#include <config/common/confg_command_buffer.h>
#include <forwarder/config/forwarder_config.h>
#include <logging/MqttSnLogging.h>
#include <atomic>
#include <thread>
#include "MqttSnForwarderTestContainerInterface.h"
class MqttSnForwarderArsmbTestContainer : public MqttSnForwarderTestContainerInterface {
 private:
  std::thread runner;
  std::atomic_bool running{false};
  std::atomic_bool stopped{false};

  MqttSnLogger cfg_logger;
  forwarder_config fcfg;
  mqtt_sn_config_command_buffer cmdBuffer;

 public:
  MqttSnForwarderArsmbTestContainer(const MqttSnForwarderTestContainerConfiguration& configuration);
  int32_t initialize() override;
  int32_t start() override;
  void stop() override;
  bool isStopped() override;
  bool isRunning() override;
  void loop();
  int32_t mqtt_sn_client_interactive_start_logger(const mqtt_sn_logger_config* cfg, MqttSnLogger* logger);
  std::string getMqttSnArsmbCommandArgumentString(const MqttSnForwarderTestContainerConfiguration config);
  std::string getMqttSnArsmbGatewayNetworkCommandString(const MqttSnClientTestContainerConfiguration configuration, const MqttSnForwarderTestContainerConfiguration fw_cfg);
  std::string getMqttSnArsmbClientNetworkCommandString(const MqttSnForwarderTestContainerConfiguration configuration);
};

#endif  // CMQTTSNFORWARDER_MQTTSNFORWARDERARSMBTESTCONTAINER_H
