//
// Created by SomeDude on 17.09.2019.
//

#include "MqttSnForwarderArsmbTestContainer.h"
#include <config/common/confg_command_buffer.h>
#include <forwarder/MqttSnForwarder.h>
#include <forwarder/starter/linux/forwarder_starter.h>
#include <logging/linux/file/FileLogging.h>
#include <logging/linux/stdout/StdoutLogging.h>
#include <platform/platform_compatibility.h>
#include <test/shared/MqttSnURLParser/MqttSnURLParser.h>

MqttSnForwarderArsmbTestContainer::MqttSnForwarderArsmbTestContainer(const MqttSnForwarderTestContainerConfiguration &configuration) : MqttSnForwarderTestContainerInterface(configuration) {}

int32_t MqttSnForwarderArsmbTestContainer::initialize() {
  if (running) {
    throw std::exception();
  }

  if (MqttSnLoggerInit(&cfg_logger, LOG_LEVEL_VERBOSE, stdout_log_init)) {
    throw std::exception();
  }

  if (forwarder_config_init(&fcfg)) {
    throw std::exception();
  }

  mqtt_sn_config_command_buffer_init(&cmdBuffer);

  std::string command = getMqttSnArsmbCommandArgumentString(MqttSnForwarderTestContainerConfiguration(configuration));
  command += " -q"; // for debugging:   command += " -db";
  if (command.length() + 1 > sizeof(cmdBuffer.buffer)) {
    throw std::exception();
  }
  memcpy(cmdBuffer.buffer, command.c_str(), command.length() + 1);

  if (mqtt_sn_config_command_buffer_parse_string(&cmdBuffer) < 0) {
    throw std::exception();
  }
  if (cmdBuffer.argc == 1) {
    throw std::exception();
  }

  int rc = forwarder_config_process_line(&fcfg, &cfg_logger, cmdBuffer.argc, cmdBuffer.argv);
  if (rc != 0) {
    throw std::exception();
  }

  return 0;
}
int32_t MqttSnForwarderArsmbTestContainer::start() {
  if (running) {
    return 0;
  }
  if (initialize() != 0) {
    return -1;
  }
  stopped = false;
  running = false;
  runner = std::thread(&MqttSnForwarderArsmbTestContainer::loop, this);
  runner.detach();

  std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  return 1;
}
void MqttSnForwarderArsmbTestContainer::stop() { stopped = true; }
bool MqttSnForwarderArsmbTestContainer::isStopped() { return stopped; }
bool MqttSnForwarderArsmbTestContainer::isRunning() { return running; }

void MqttSnForwarderArsmbTestContainer::loop() {
  MqttSnLogger *mqtt_sn_logger = NULL;
#ifdef WITH_LOGGING
  MqttSnLogger logger = {0};
  if (mqtt_sn_client_interactive_start_logger(&fcfg.mslcfg, &logger) < 0) {
    log_str(&cfg_logger, PSTR("Could not initialize mqtt sn logger\n"));
    throw std::exception();
  }
  mqtt_sn_logger = &logger;
#endif
  MqttSnForwarder mqttSnForwarder;

  running = true;
  start_forwarder(&fcfg, mqtt_sn_logger, &mqttSnForwarder);
  forwarder_config_cleanup(&fcfg);

  if (mqtt_sn_logger) {
    MqttSnLoggerDeinit(mqtt_sn_logger);
  }

  running = false;
}
int32_t MqttSnForwarderArsmbTestContainer::mqtt_sn_client_interactive_start_logger(const mqtt_sn_logger_config *cfg, MqttSnLogger *logger) {
  if (!strcmp(cfg->log_target, "console")) {
    if (cfg->log_filepath != NULL) {
      throw std::exception();
      /*
       * //TODO
      if (MqttSnLoggerInitFile(logger, cfg->log_lvl, cfg->log_file_path, file_stdout_log_init, NULL) < 0) {
        return -1;
      }
      */
    } else {
      if (MqttSnLoggerInit(logger, cfg->log_lvl, stdout_log_init) < 0) {
        return -1;
      }
    }
  } else if (!strcmp(cfg->log_target, "file")) {
    if (MqttSnLoggerInitFile(logger, cfg->log_lvl, cfg->log_filepath, file_log_init, NULL) < 0) {
      return -1;
    }
  }
  if (cfg->log_identifier) {
    logger->log_identifier = cfg->log_identifier;
  }
  return 0;
}
std::string MqttSnForwarderArsmbTestContainer::getMqttSnArsmbCommandArgumentString(const MqttSnForwarderTestContainerConfiguration config) {
  std::string command = getMqttSnArsmbGatewayNetworkCommandString(config.mqttSnClientConfiguration, config);
  command += " " + getMqttSnArsmbClientNetworkCommandString(config);
  return command;
}
std::string MqttSnForwarderArsmbTestContainer::getMqttSnArsmbGatewayNetworkCommandString(const MqttSnClientTestContainerConfiguration configuration,
                                                                                         const MqttSnForwarderTestContainerConfiguration fw_cfg) {
  std::string command = "";

  command += " ";
  command += "-li " + configuration.clientId + " ";

  command += "-L mqttsn://"+ MqttSnURLParser::toString(configuration.brokerAddress, 4) + ":" + std::to_string(MqttSnURLParser::getPort(configuration.brokerAddress));

  command += " ";
  command += "-gL ";
  if (configuration.protocol_type == MqttSnClientProtocolTestType::UDP) {
    command += "udp";
  } else if (configuration.protocol_type == MqttSnClientProtocolTestType::TCP) {
    command += "tcp";
  } else {
    throw std::exception();
  }
  command += "://" + MqttSnURLParser::toString(configuration.interfaceAddress, 4) + ":" + std::to_string(MqttSnURLParser::getPort(configuration.interfaceAddress));

  command += " ";
  command += "-gbL ";
  if (configuration.protocol_type == MqttSnClientProtocolTestType::UDP) {
    command += "udp";
  } else if (configuration.protocol_type == MqttSnClientProtocolTestType::TCP) {
    command += "udp";
  } else {
    throw std::exception();
  }
  command += "://" + MqttSnURLParser::toString(configuration.interfaceBroadcastAddress, 4) + ":" + std::to_string(MqttSnURLParser::getPort(configuration.interfaceBroadcastAddress));

  command += " ";
  command += "-fis connected ";
  command += "-fpc true ";

  return command;
}

std::string MqttSnForwarderArsmbTestContainer::getMqttSnArsmbClientNetworkCommandString(const MqttSnForwarderTestContainerConfiguration configuration) {
  std::string command = "";

  command = "-cL ";
  if (configuration.protocol == MqttSnGatewayProtocolTestType::UDP) {
    command += "udp";
  } else if (configuration.protocol == MqttSnGatewayProtocolTestType::TCP) {
    command += "tcp";
  } else {
    throw std::exception();
  }
  command += "://" + MqttSnURLParser::toString(configuration.interfaceAddress, 4) + ":" + std::to_string(MqttSnURLParser::getPort(configuration.interfaceAddress));

  command += " ";
  command += "-cbL ";
  if (configuration.protocol == MqttSnGatewayProtocolTestType::UDP) {
    command += "udp";
  } else if (configuration.protocol == MqttSnGatewayProtocolTestType::TCP) {
    command += "udp";
  } else {
    throw std::exception();
  }
  command += "://" + MqttSnURLParser::toString(configuration.interfaceBroadcastAddress, 4) + ":" + std::to_string(MqttSnURLParser::getPort(configuration.interfaceBroadcastAddress));

  return command;
}
