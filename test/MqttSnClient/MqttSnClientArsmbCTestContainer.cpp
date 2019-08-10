//
// Created by SomeDude on 31.07.2019.
//

#include <test/shared/MqttSnURLParser/MqttSnURLParser.h>
#include "MqttSnClientArsmbCTestContainer.h"
#include "MqttSnClientTestContainerInterface.h"
#include "MqttSnClientTestContainerConfiguration.h"
#include <client/MqttSnClient.h>
#include <platform/platform_compatibility.h>
#include <gateway_old/paho/pahomqttembeddedc/MQTTClient/src/MQTTLogging.h>

MqttSnClientArsmbCTestContainer::MqttSnClientArsmbCTestContainer(const MqttSnClientTestContainerConfiguration &configuration)
    : MqttSnClientTestContainerInterface(configuration) {
  interactive_client_config_init(&cfg);
  mqtt_sn_interactive_client_init(&interactive_client);
  mqtt_sn_cmd_buffer_init(&cmd_buffer);
}
MqttSnClientActionResult MqttSnClientArsmbCTestContainer::connect(MqttSnClientDirectConnectAction &connect_action) {
  {
    std::string client_connect_options = "-ci " + connect_action.clientId
        + " -cp " + connect_action.clientPassword
        + " -cd " + std::to_string(connect_action.keepAliveInterval)
        + " -cs " + (connect_action.cleanSession ? "true" : "false");

    std::string direct_connect_command = "-L mqttsn://"
        + MqttSnURLParser::toString(connect_action.brokerAddress, connect_action.brokerAddress.size()) + ":-1 "
        + "-gfp connect";

    std::string command = client_connect_options + " " + direct_connect_command;

    mqtt_sn_cmd_buffer_reset(&cmd_buffer);
    if (command.length() + 1 > sizeof(cmd_buffer.buffer)) {
      throw std::exception();
    }

    memcpy(cmd_buffer.buffer, command.c_str(), command.length() + 1);
    if (mqtt_sn_cmd_buffer_parse_string(&cmd_buffer) < 0) {
      throw std::exception();
    }

    if (cmd_buffer.argc == 1) {
      throw std::exception();
    }

    // update configuration
    if (cmd_buffer.argc > 1) {
      int rc;
      if ((rc = mqtt_sn_cmd_buffer_simulate(&cfg, &cfg_logger, cmd_buffer.argc, cmd_buffer.argv)) < 0) {
        // no valid configuration - no update
        throw std::exception();
      }
      if (rc) {
        throw std::exception();
      }
      interactive_client_config_process_line(&cfg, &cfg_logger, cmd_buffer.argc, cmd_buffer.argv);
      interactive_client_config_copy_to_buffer(&cfg);
    }
  }
  std::string interactive_command = "connect";
  mqtt_sn_cmd_buffer_reset(&cmd_buffer);
  if (interactive_command.length() + 1 > sizeof(cmd_buffer.buffer)) {
    throw std::exception();
  }
  memcpy(cmd_buffer.buffer, interactive_command.c_str(), interactive_command.length() + 1);
  if (mqtt_sn_cmd_buffer_parse_string(&cmd_buffer) < 0) {
    throw std::exception();
  }
  if (cmd_buffer.argc == 1) {
    throw std::exception();
  }

  MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE command_token;
  command_token = mqtt_sn_cmd_buffer_parse_command(&cmd_buffer);
  if (command_token == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_NO_COMMAND
      || command_token == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_INIT_CLIENT
      || command_token == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_DEINIT_CLIENT
      || command_token == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_REINIT_LOGGER) {
    throw std::exception();
  }
  mqtt_sn_client_interactive_cmd_result exec_rc;
  // TODO lock guard pthread_mutex_lock(&thread_mqtt_sn_client_interactive_lock);
  exec_rc = mqtt_sn_cmd_buffer_execute_command(&interactive_client, command_token, &cfg, logger);
  // TODO unlock guard pthread_mutex_unlock(&thread_mqtt_sn_client_interactive_lock);
  //print_mqtt_sn_client_interactive_cmd_result(logger, &exec_rc);


  MqttSnClientActionResultType action_result_type = GetActionResultType(exec_rc.action_result);

  return MqttSnClientActionResult(exec_rc.identifier,
                                  exec_rc.action_number,
                                  connect_action.action_type,
                                  action_result_type,
                                  exec_rc.start_ms,
                                  exec_rc.end_ms);
}
MqttSnClientActionResult MqttSnClientArsmbCTestContainer::connect(MqttSnClientAdvertiseConnectAction &connect_action) {
  // TODO implement me
  timespec start_ts;
  timespec end_ts;
  clock_gettime(CLOCK_REALTIME, &start_ts);
  clock_gettime(CLOCK_REALTIME, &end_ts);
  return MqttSnClientActionResult(configuration.clientId,
                                  action_number++,
                                  connect_action.action_type,
                                  MqttSnClientActionResultType::ERROR,
                                  start_ts,
                                  end_ts);
}
MqttSnClientActionResult MqttSnClientArsmbCTestContainer::connect(MqttSnClientSearchGwConnectAction &connect_action) {
  // TODO implement me
  timespec start_ts;
  timespec end_ts;
  clock_gettime(CLOCK_REALTIME, &start_ts);
  clock_gettime(CLOCK_REALTIME, &end_ts);
  return MqttSnClientActionResult(configuration.clientId,
                                  action_number++,
                                  connect_action.action_type,
                                  MqttSnClientActionResultType::ERROR,
                                  start_ts,
                                  end_ts);
}
MqttSnClientActionResult MqttSnClientArsmbCTestContainer::disconnect() {
  // TODO implement me
  timespec start_ts;
  timespec end_ts;
  clock_gettime(CLOCK_REALTIME, &start_ts);
  clock_gettime(CLOCK_REALTIME, &end_ts);
  return MqttSnClientActionResult(configuration.clientId,
                                  action_number++,
                                  MqttSnClientActionType::DISCONNECT,
                                  MqttSnClientActionResultType::ERROR,
                                  start_ts,
                                  end_ts);
}
bool MqttSnClientArsmbCTestContainer::StartBackgroundHandler() {
  stopped = false;
  // TODO parse gateway network config
  // TODO init gateway network config
  // first initialize logger
  {
    std::string interactive_command_logger = "logger";
    mqtt_sn_cmd_buffer_reset(&cmd_buffer);
    if (interactive_command_logger.length() + 1 > sizeof(cmd_buffer.buffer)) {
      throw std::exception();
    }
    memcpy(cmd_buffer.buffer, interactive_command_logger.c_str(), interactive_command_logger.length() + 1);
    if (mqtt_sn_cmd_buffer_parse_string(&cmd_buffer) < 0) {
      throw std::exception();
    }
    if (cmd_buffer.argc == 1) {
      throw std::exception();
    }
  }
  {
    std::string interactive_command_init = "init";
    mqtt_sn_cmd_buffer_reset(&cmd_buffer);
    if (interactive_command_init.length() + 1 > sizeof(cmd_buffer.buffer)) {
      throw std::exception();
    }
    memcpy(cmd_buffer.buffer, interactive_command_init.c_str(), interactive_command_init.length() + 1);
    if (mqtt_sn_cmd_buffer_parse_string(&cmd_buffer) < 0) {
      throw std::exception();
    }
    if (cmd_buffer.argc == 1) {
      throw std::exception();
    }
  }

  running = true;
  return running;
}
void MqttSnClientArsmbCTestContainer::StopBackgroundHandler() {
  stopped = true;
  running = false;
}
bool MqttSnClientArsmbCTestContainer::IsBackgroundHandlerRunning() {
  return running;
}
MqttSnClientActionResultType MqttSnClientArsmbCTestContainer::GetActionResultType(
    MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE type) {
  switch (type) {
    case MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS:return MqttSnClientActionResultType::SUCCESS;
    case MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR:return MqttSnClientActionResultType::ERROR;
    case MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_TIMEOUT:return MqttSnClientActionResultType::TIMEOUT;
    case MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_UNKNOWNACTION:return MqttSnClientActionResultType::UNKNOWNACTION;
    default:
      throw std::exception();
  }
}
void MqttSnClientArsmbCTestContainer::loop(){
  // FROM here to thread
  if (client != nullptr) {
    throw std::exception();
  }
  while (!stopped) {
    // TODO lock guard pthread_mutex_lock(&thread_mqtt_sn_client_interactive_lock);
    int32_t loop_rc = MqttSnClientLoop(client);
    if (loop_rc < 0) {
      break;
    }
    // TODO ulock guardpthread_mutex_unlock(&thread_mqtt_sn_client_interactive_lock);
  }
  {
    std::string interactive_command_deinit = "deinit";
    mqtt_sn_cmd_buffer_reset(&cmd_buffer);
    if (interactive_command_deinit.length() + 1 > sizeof(cmd_buffer.buffer)) {
      throw std::exception();
    }
    memcpy(cmd_buffer.buffer, interactive_command_deinit.c_str(), interactive_command_deinit.length() + 1);
    if (mqtt_sn_cmd_buffer_parse_string(&cmd_buffer) < 0) {
      throw std::exception();
    }
    if (cmd_buffer.argc == 1) {
      throw std::exception();
    }
  }
  running = false;
  // TO HERE to thread
}
