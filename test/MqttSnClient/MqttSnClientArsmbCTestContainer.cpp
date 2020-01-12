//
// Created by SomeDude on 31.07.2019.
//

#include "MqttSnClientArsmbCTestContainer.h"
#include <client/MqttSnClientLogger.h>
#include <client/MqttSnClientMethod.h>
#include <gateway_old/paho/pahomqttembeddedc/MQTTClient/src/MQTTLogging.h>
#include <platform/platform_compatibility.h>
#include <test/shared/MqttSnURLParser/MqttSnURLParser.h>
#include <iomanip>
#include <sstream>
#include "MqttSnClientTestContainerConfiguration.h"
#include "MqttSnClientTestContainerInterface.h"

MqttSnClientArsmbCTestContainer::MqttSnClientArsmbCTestContainer(const MqttSnClientTestContainerConfiguration &configuration) : MqttSnClientTestContainerInterface(configuration) {
  if (MqttSnLoggerInit(&cfg_logger, LOG_LEVEL_VERBOSE, stdout_log_init)) {
    throw std::exception();
  }

  if (interactive_client_config_init(&cfg)) {
    throw std::exception();
  }

  mqtt_sn_interactive_client_init(&interactive_client);
  mqtt_sn_config_command_buffer_init(&cmd_buffer);

  std::string command = getMqttSnClientTestContainerInterfaceToCmdString(configuration);
  command += " -q";  // For debugging: command += " -db";
  parseCommandIntoConfiguration(command);
}
MqttSnClientActionResult MqttSnClientArsmbCTestContainer::connect(MqttSnClientConnectAction &connect_action) {
  std::string command = getMqttSnClientConnectActionToCmdString(connect_action);
  parseCommandIntoConfiguration(command);
  mqtt_sn_client_interactive_cmd_result rc;
  {
    std::lock_guard<std::mutex> lg(runnerMutex);
    rc = mqtt_sn_cmd_buffer_execute_command(&interactive_client, MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_AUTO_CONNECT_GATEWAY, &cfg, logger);
  }
  return getMqttSnClientInteractiveCmdResultToMqttSnClientActionResult(rc);
}
MqttSnClientActionResult MqttSnClientArsmbCTestContainer::connect(MqttSnClientDirectConnectAction &connect_action) {
  std::string command = getMqttSnClientDirectConnectActionToCmdString(connect_action);
  parseCommandIntoConfiguration(command);
  mqtt_sn_client_interactive_cmd_result rc;
  {
    std::lock_guard<std::mutex> lg(runnerMutex);
    rc = mqtt_sn_cmd_buffer_execute_command(&interactive_client, MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_DIRECT_CONNECT_GATEWAY, &cfg, logger);
  }
  return getMqttSnClientInteractiveCmdResultToMqttSnClientActionResult(rc);
}
MqttSnClientActionResult MqttSnClientArsmbCTestContainer::connect(MqttSnClientAdvertiseConnectAction &connect_action) {
  std::string cmd_string = MqttSnClientAdvertiseConnectActionToCmdString(connect_action);
  mqtt_sn_client_interactive_cmd_result rc;
  {
    std::lock_guard<std::mutex> lg(runnerMutex);
    rc = mqtt_sn_cmd_buffer_execute_command(&interactive_client, MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_ADVERTISE_AND_CONNECT_GATEWAY, &cfg, logger);
  }
  return getMqttSnClientInteractiveCmdResultToMqttSnClientActionResult(rc);
}
MqttSnClientActionResult MqttSnClientArsmbCTestContainer::connect(MqttSnClientSearchGatewayConnectAction &connect_action) {
  std::string cmd_string = MqttSnClientSearchGwConnectActionToCmdString(connect_action);
  mqtt_sn_client_interactive_cmd_result rc;
  {
    std::lock_guard<std::mutex> lg(runnerMutex);
    rc = mqtt_sn_cmd_buffer_execute_command(&interactive_client, MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_SEARCHGW_AND_CONNECT_GATEWAY, &cfg, logger);
  }
  return getMqttSnClientInteractiveCmdResultToMqttSnClientActionResult(rc);
}
MqttSnClientActionResult MqttSnClientArsmbCTestContainer::disconnect() {
  std::lock_guard<std::mutex> lg(runnerMutex);
  mqtt_sn_client_interactive_cmd_result rc = mqtt_sn_cmd_buffer_execute_command(&interactive_client, MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_DISCONNECT_GATEWAY, &cfg, logger);
  return getMqttSnClientInteractiveCmdResultToMqttSnClientActionResult(rc);
}
MqttSnClientActionResult MqttSnClientArsmbCTestContainer::publish(MqttSnClientPublishAction &publishAction) {
  clearCmdPublishList();
  std::string command = getMqttSnClientPublishActionToCmdString(publishAction);
  parseCommandIntoConfiguration(command);
  mqtt_sn_client_interactive_cmd_result rc;
  {
    std::lock_guard<std::mutex> lg(runnerMutex);
    rc = mqtt_sn_cmd_buffer_execute_command(&interactive_client, MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_GATEWAY, &cfg, logger);
  }
  return getMqttSnClientInteractiveCmdResultToMqttSnClientActionResult(rc);
}
MqttSnClientActionResult MqttSnClientArsmbCTestContainer::reconnect(MqttSnClientReconnectAction &reconnectAction) {
  mqtt_sn_client_interactive_cmd_result rc;
  {
    std::lock_guard<std::mutex> lg(runnerMutex);
    rc = mqtt_sn_cmd_buffer_execute_command(&interactive_client, MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_RECONNECT_GATEWAY, &cfg, logger);
  }
  return getMqttSnClientInteractiveCmdResultToMqttSnClientActionResult(rc);
}
MqttSnClientActionResult MqttSnClientArsmbCTestContainer::registration(MqttSnClientRegistrationAction &registrationAction) {
  clearCmdRegisterList();
  std::string command = getMqttSnClientRegisterActionToCmdString(registrationAction);
  parseCommandIntoConfiguration(command);
  mqtt_sn_client_interactive_cmd_result rc;
  {
    std::lock_guard<std::mutex> lg(runnerMutex);
    rc = mqtt_sn_cmd_buffer_execute_command(&interactive_client, MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_REGISTER_GATEWAY, &cfg, logger);
  }
  return getMqttSnClientInteractiveCmdResultToMqttSnClientActionResult(rc);
}
MqttSnClientActionResult MqttSnClientArsmbCTestContainer::subscribe(MqttSnClientSubscribeAction &subscribeAction) {
  clearCmdSubscribeList();
  std::string command = getMqttSnClientSubscribeActionToCmdString(subscribeAction);
  parseCommandIntoConfiguration(command);
  mqtt_sn_client_interactive_cmd_result rc;
  {
    std::lock_guard<std::mutex> lg(runnerMutex);
    rc = mqtt_sn_cmd_buffer_execute_command(&interactive_client, MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_SUBSCRIBE_GATEWAY, &cfg, logger);
  }
  return getMqttSnClientInteractiveCmdResultToMqttSnClientActionResult(rc);
}
MqttSnClientActionResult MqttSnClientArsmbCTestContainer::unsubscribe(MqttSnClientUnsubscribeAction &unsubscribeAction) {
  clearCmdUnsubscribeList();
  std::string command = getMqttSnClientUnsubscribeActionToCmdString(unsubscribeAction);
  parseCommandIntoConfiguration(command);
  mqtt_sn_client_interactive_cmd_result rc;
  {
    std::lock_guard<std::mutex> lg(runnerMutex);
    rc = mqtt_sn_cmd_buffer_execute_command(&interactive_client, MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_UNSUBSCRIBE_GATEWAY, &cfg, logger);
  }
  return getMqttSnClientInteractiveCmdResultToMqttSnClientActionResult(rc);
}
MqttSnClientActionResult MqttSnClientArsmbCTestContainer::publishReceivePublish(MqttSnClientPublishReceivePublishAction &publishReceivePublishAction) {
  clearCmdRegisterList();
  clearCmdPublishList();
  clearCmdSubscribeList();
  clearCmdUnsubscribeList();

  std::string command = getMqttSnClientReceivePublishActionToCmdString(publishReceivePublishAction);
  parseCommandIntoConfiguration(command);
  mqtt_sn_client_interactive_cmd_result rc;
  {
    std::lock_guard<std::mutex> lg(runnerMutex);
    rc = mqtt_sn_cmd_buffer_execute_command(&interactive_client, MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_RECEIVE_GATEWAY, &cfg, logger);
  }
  return getMqttSnClientInteractiveCmdResultToMqttSnClientActionResult(rc);
}
MqttSnClientActionResult MqttSnClientArsmbCTestContainer::sleep(MqttSnClientSleepAction &sleepAction) {
  std::string command = getMqttSnClientSleepActionToCmdString(sleepAction);
  parseCommandIntoConfiguration(command);
  mqtt_sn_client_interactive_cmd_result rc;
  {
    std::lock_guard<std::mutex> lg(runnerMutex);
    rc = mqtt_sn_cmd_buffer_execute_command(&interactive_client, MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_SLEEP_GATEWAY, &cfg, logger);
  }
  return getMqttSnClientInteractiveCmdResultToMqttSnClientActionResult(rc);
}
MqttSnClientActionResult MqttSnClientArsmbCTestContainer::wakeup(MqttSnClientWakeupAction &wakeupAction) {
  mqtt_sn_client_interactive_cmd_result rc;
  {
    std::lock_guard<std::mutex> lg(runnerMutex);
    rc = mqtt_sn_cmd_buffer_execute_command(&interactive_client, MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_WAKEUP_GATEWAY, &cfg, logger);
  }
  return getMqttSnClientInteractiveCmdResultToMqttSnClientActionResult(rc);
}
MqttSnClientActionResult MqttSnClientArsmbCTestContainer::ping(MqttSnClientPingAction &pingAction) {
  mqtt_sn_client_interactive_cmd_result rc;
  {
    std::lock_guard<std::mutex> lg(runnerMutex);
    rc = mqtt_sn_cmd_buffer_execute_command(&interactive_client, MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PING_GATEWAY, &cfg, logger);
  }
  return getMqttSnClientInteractiveCmdResultToMqttSnClientActionResult(rc);
}
MqttSnClientActionResult MqttSnClientArsmbCTestContainer::delay(MqttSnClientDelayAction &delayAction) {
  timespec start_ts;
  timespec end_ts;
  clock_gettime(CLOCK_REALTIME, &start_ts);
  std::this_thread::sleep_for(std::chrono::seconds(delayAction.delayDuration));
  clock_gettime(CLOCK_REALTIME, &end_ts);

  std::string identifier;
  if (cfg_logger.log_identifier) {
    std::string i(cfg_logger.log_identifier);
    identifier = i;
  } else {
    std::string i(cfg.cccfg.client_id);
    identifier = i;
  }
  return MqttSnClientActionResult(identifier, interactive_client.action_number++, MqttSnClientActionType::DELAY, MqttSnClientActionResultType ::SUCCESS, start_ts, end_ts);
}
std::string MqttSnClientArsmbCTestContainer::getMqttSnClientRegisterActionToCmdString(MqttSnClientRegistrationAction &registrationAction) {
  std::string command = "-rt " + registrationAction.topicName;
  return command;
}
bool MqttSnClientArsmbCTestContainer::StartBackgroundHandler() {
  if (running) {
    return false;
  }

  std::lock_guard<std::mutex> lg(runnerMutex);
  mqtt_sn_client_interactive_cmd_result reinit_logger_rc = mqtt_sn_cmd_buffer_execute_command(&interactive_client, MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_REINIT_LOGGER, &cfg, logger);
  if (reinit_logger_rc.action_result != MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS) {
    return false;
  }

  mqtt_sn_client_interactive_cmd_result init_client_rc = mqtt_sn_cmd_buffer_execute_command(&interactive_client, MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_INIT_CLIENT, &cfg, logger);
  if (init_client_rc.action_result != MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS) {
    return false;
  }

  runner = std::thread(&MqttSnClientArsmbCTestContainer::loop, this);
  runner.detach();

  return true;
}
void MqttSnClientArsmbCTestContainer::StopBackgroundHandler() {
  stopped = true;
  while (IsBackgroundHandlerRunning()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  running = false;
}
bool MqttSnClientArsmbCTestContainer::IsBackgroundHandlerRunning() { return running; }

void MqttSnClientArsmbCTestContainer::loop() {
  stopped = false;
  running = true;
  while (!stopped) {
    MQTT_SN_CLIENT_LOOP_RETURN_CODE loop_rc;
    {
      std::lock_guard<std::mutex> lg(runnerMutex);
      loop_rc = MqttSnClientLoop(&interactive_client.client);
    }
    if (loop_rc == MQTT_SN_CLIENT_LOOP_RETURN_ERROR) {
      std::lock_guard<std::mutex> lg(runnerMutex);
      mqtt_sn_client_interactive_cmd_result exec_rc = mqtt_sn_cmd_buffer_execute_command(&interactive_client, MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_DEINIT_CLIENT, &cfg, logger);
      if (exec_rc.action_result != MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS) {
        throw std::exception();
      }
      break;
    }
  }

  running = false;
}
MqttSnClientActionResult MqttSnClientArsmbCTestContainer::getMqttSnClientInteractiveCmdResultToMqttSnClientActionResult(mqtt_sn_client_interactive_cmd_result rc) {
  MqttSnClientActionType actionType = getMqttSnClientInteractiveCmdActionTypeToMqttSnClientActionType(rc.action_type);
  MqttSnClientActionResultType actionResult = getMqttSnClientInteractiveCmdActionResultTypeToMqttSnClientActionResultType(rc.action_result);
  auto result = MqttSnClientActionResult(rc.identifier, rc.action_number, actionType, actionResult, rc.start_ms, rc.end_ms, rc.duration_ns);
  return result;
}
MqttSnClientActionType MqttSnClientArsmbCTestContainer::getMqttSnClientInteractiveCmdActionTypeToMqttSnClientActionType(MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE action_type) {
  switch (action_type) {
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_CONNECT_GATEWAY:
      return MqttSnClientActionType::CONNECT;
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_AUTO_CONNECT_GATEWAY:
      return MqttSnClientActionType::AUTO_CONNECT;
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_ADVERTISE_AND_CONNECT_GATEWAY:
      return MqttSnClientActionType::ADVERTISE_CONNECT;
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_SEARCHGW_AND_CONNECT_GATEWAY:
      return MqttSnClientActionType::SEARCHGW_CONNECT;
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_DIRECT_CONNECT_GATEWAY:
      return MqttSnClientActionType::DIRECT_CONNECT;
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_DISCONNECT_GATEWAY:
      return MqttSnClientActionType::DISCONNECT;
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_REGISTER_GATEWAY:
      return MqttSnClientActionType::REGISTER;
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_SUBSCRIBE_GATEWAY:
      return MqttSnClientActionType::SUBSCRIBE;
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PREDEFINED_SUBSCRIBE_GATEWAY:
      return MqttSnClientActionType::PREDEFINED_SUBSCRIBE;
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_TOPICID_SUBSCRIBE_GATEWAY:
      return MqttSnClientActionType::TOPICID_SUBSCRIBE;
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_TOPICNAME_SUBSCRIBE_GATEWAY:
      return MqttSnClientActionType::TOPICNAME_SUBSCRIBE;
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_UNSUBSCRIBE_GATEWAY:
      return MqttSnClientActionType::UNSUBSCRIBE;
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_GATEWAY:
      return MqttSnClientActionType::PUBLISH;
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_UNCONNECTED_PREDEFINED_QOSM1_GATEWAY:
      return MqttSnClientActionType::UNCONNECTED_PREDEFINED_PUBLISH_QOSM1;
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_PREDEFINED_QOSM1_GATEWAY:
      return MqttSnClientActionType::PREDEFINED_PUBLISH_QOSM1;
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_PREDEFINED_QOS0_GATEWAY:
      return MqttSnClientActionType::PREDEFINED_PUBLISH_QOS0;
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_PREDEFINED_QOS1_GATEWAY:
      return MqttSnClientActionType::PREDEFINED_PUBLISH_QOS1;
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_PREDEFINED_QOS2_GATEWAY:
      return MqttSnClientActionType::PREDEFINED_PUBLISH_QOS2;
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_TOPICNAME_QOSM1_GATEWAY:
      return MqttSnClientActionType::TOPICNAME_PUBLISH_QOSM1;
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_TOPICNAME_QOS0_GATEWAY:
      return MqttSnClientActionType::TOPICNAME_PUBLISH_QOS0;
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_TOPICNAME_QOS1_GATEWAY:
      return MqttSnClientActionType::TOPICNAME_PUBLISH_QOS1;
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_TOPICNAME_QOS2_GATEWAY:
      return MqttSnClientActionType::TOPICNAME_PUBLISH_QOS2;
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_TOPICID_QOSM1_GATEWAY:
      return MqttSnClientActionType::TOPICID_PUBLISH_QOSM1;
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_TOPICID_QOS0_GATEWAY:
      return MqttSnClientActionType::TOPICID_PUBLISH_QOS0;
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_TOPICID_QOS1_GATEWAY:
      return MqttSnClientActionType::TOPICID_PUBLISH_QOS1;
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_TOPICID_QOS2_GATEWAY:
      return MqttSnClientActionType::TOPICID_PUBLISH_QOS2;

    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_RECEIVE_GATEWAY:
      return MqttSnClientActionType::PUBLISH_RECEIVE_PUBLISH;

    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_RECEIVE_PUBLISH_PREDEFINED_QOSM1_GATEWAY:
      return MqttSnClientActionType::PUBLISH_RECEIVE_PUBLISH_PREDEFINED_PUBLISH_QOSM1;
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_RECEIVE_PUBLISH_PREDEFINED_QOS0_GATEWAY:
      return MqttSnClientActionType::PUBLISH_RECEIVE_PUBLISH_PREDEFINED_PUBLISH_QOS0;
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_RECEIVE_PUBLISH_PREDEFINED_QOS1_GATEWAY:
      return MqttSnClientActionType::PUBLISH_RECEIVE_PUBLISH_PREDEFINED_PUBLISH_QOS1;
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_RECEIVE_PUBLISH_PREDEFINED_QOS2_GATEWAY:
      return MqttSnClientActionType::PUBLISH_RECEIVE_PUBLISH_PREDEFINED_PUBLISH_QOS2;

    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_RECEIVE_PUBLISH_TOPICNAME_QOSM1_GATEWAY:
      return MqttSnClientActionType::PUBLISH_RECEIVE_PUBLISH_TOPICNAME_PUBLISH_QOSM1;
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_RECEIVE_PUBLISH_TOPICNAME_QOS0_GATEWAY:
      return MqttSnClientActionType::PUBLISH_RECEIVE_PUBLISH_TOPICNAME_PUBLISH_QOS0;
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_RECEIVE_PUBLISH_TOPICNAME_QOS1_GATEWAY:
      return MqttSnClientActionType::PUBLISH_RECEIVE_PUBLISH_TOPICNAME_PUBLISH_QOS1;
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_RECEIVE_PUBLISH_TOPICNAME_QOS2_GATEWAY:
      return MqttSnClientActionType::PUBLISH_RECEIVE_PUBLISH_TOPICNAME_PUBLISH_QOS2;

    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_RECEIVE_PUBLISH_TOPICID_QOSM1_GATEWAY:
      return MqttSnClientActionType::PUBLISH_RECEIVE_PUBLISH_TOPICID_PUBLISH_QOSM1;
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_RECEIVE_PUBLISH_TOPICID_QOS0_GATEWAY:
      return MqttSnClientActionType::PUBLISH_RECEIVE_PUBLISH_TOPICID_PUBLISH_QOS0;
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_RECEIVE_PUBLISH_TOPICID_QOS1_GATEWAY:
      return MqttSnClientActionType::PUBLISH_RECEIVE_PUBLISH_TOPICID_PUBLISH_QOS1;
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_RECEIVE_PUBLISH_TOPICID_QOS2_GATEWAY:
      return MqttSnClientActionType::PUBLISH_RECEIVE_PUBLISH_TOPICID_PUBLISH_QOS2;

    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_SLEEP_GATEWAY:
      return MqttSnClientActionType::SLEEP;
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_WAKEUP_GATEWAY:
      return MqttSnClientActionType::WAKEUP;
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_RECONNECT_GATEWAY:
      return MqttSnClientActionType::RECONNECT;
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PING_GATEWAY:
      return MqttSnClientActionType::PING;
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_NO_COMMAND:
      return MqttSnClientActionType::UNKNOWNACTION;
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_REINIT_LOGGER:
      return MqttSnClientActionType::UNKNOWNACTION;
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_INIT_CLIENT:
      return MqttSnClientActionType::UNKNOWNACTION;
    case MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_DEINIT_CLIENT:
      return MqttSnClientActionType::UNKNOWNACTION;
  }
  return MqttSnClientActionType::UNKNOWNACTION;
}
MqttSnClientActionResultType MqttSnClientArsmbCTestContainer::getMqttSnClientInteractiveCmdActionResultTypeToMqttSnClientActionResultType(MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE action_result) {
  switch (action_result) {
    case MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS:
      return MqttSnClientActionResultType::SUCCESS;
    case MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_ERROR:
      return MqttSnClientActionResultType::ERROR;
    case MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_TIMEOUT:
      return MqttSnClientActionResultType::TIMEOUT;
    case MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_UNKNOWNACTION:
      return MqttSnClientActionResultType::UNKNOWNACTION;
  }
  return MqttSnClientActionResultType::UNKNOWNACTION;
}
std::string MqttSnClientArsmbCTestContainer::getMqttSnClientConnectActionToCmdString(MqttSnClientConnectAction &connect_action) {
  std::string client_connect_options = "-ci " + connect_action.clientId + /*" -cp " + connect_action.clientPassword +*/ " -cd " + std::to_string(connect_action.keepAliveInterval) + " -cs " +
                                       getTrueOrFalse(connect_action.cleanSession);
  return client_connect_options;
}
std::string MqttSnClientArsmbCTestContainer::getMqttSnClientDirectConnectActionToCmdString(MqttSnClientDirectConnectAction &connect_action) {
  std::string client_connect_options = getMqttSnClientConnectActionToCmdString(connect_action);
  std::string direct_connect_command = "-gfp connect -L mqttsn://" + MqttSnURLParser::toString(connect_action.brokerAddress, 4) + ":" + std::to_string(MqttSnURLParser::getPort(connect_action.brokerAddress));
  // std::string direct_connect_command = "-L mqttsn://" + MqttSnURLParser::toString(connect_action.brokerAddress, connect_action.brokerAddress.size()) + ":-1 " + "-gfp connect";
  std::string command = client_connect_options + " " + direct_connect_command;
  return command;
}
bool MqttSnClientArsmbCTestContainer::parseCommandIntoConfiguration(std::string command) {
  if (command.length() + 1 > sizeof(cmd_buffer.buffer)) {
    throw std::exception();
  }
  memcpy(cmd_buffer.buffer, command.c_str(), command.length() + 1);

  // update configuration

  // create a copy for simulating
  mqtt_sn_config_command_buffer cmd_buffer_cp = mqtt_sn_config_command_buffer_cp(&cmd_buffer);
  if (mqtt_sn_config_command_buffer_parse_string(&cmd_buffer_cp) < 0) {
    log_str(&cfg_logger, PSTR("Error tokenizing.\n"));
    throw std::exception();
  }
  if (cmd_buffer_cp.argc == 1) {
    throw std::exception();
  }
  int rc;
  if ((rc = mqtt_sn_cmd_buffer_interactive_client_simulate(&cfg, &cfg_logger, cmd_buffer_cp.argc, cmd_buffer_cp.argv)) < 0) {
    // no valid configuration - no update
    throw std::exception();
  }
  if (rc) {
    throw std::exception();
  }

  // simulation successful now write real values into configuration
  if (mqtt_sn_config_command_buffer_parse_string(&cmd_buffer) < 0) {
    throw std::exception();
  }
  if (cmd_buffer.argc == 1) {
    throw std::exception();
  }
  interactive_client_config_process_line(&cfg, &cfg_logger, cmd_buffer.argc, cmd_buffer.argv);
  interactive_client_config_copy_to_buffer(&cfg);

  mqtt_sn_config_command_buffer_reset(&cmd_buffer);
  return true;
}
std::string MqttSnClientArsmbCTestContainer::MqttSnClientAdvertiseConnectActionToCmdString(MqttSnClientAdvertiseConnectAction &action) {
  std::string client_connect_options = getMqttSnClientConnectActionToCmdString(action);
  std::string direct_connect_command = "-gfp advertise";
  std::string command = client_connect_options + " " + direct_connect_command;
  return command;
}
std::string MqttSnClientArsmbCTestContainer::MqttSnClientSearchGwConnectActionToCmdString(MqttSnClientSearchGatewayConnectAction &action) {
  std::string client_connect_options = getMqttSnClientConnectActionToCmdString(action);
  std::string direct_connect_command = "-gfp search";
  std::string command = client_connect_options + " " + direct_connect_command;
  return command;
}
std::string MqttSnClientArsmbCTestContainer::getMqttSnClientTestContainerInterfaceToCmdString(const MqttSnClientTestContainerConfiguration &configuration) {
  std::string command = "";
  command += "-li "+ configuration.clientId + " ";

  command += "-L mqttsn://" +  MqttSnURLParser::toString(configuration.brokerAddress, 4) + ":" + std::to_string(MqttSnURLParser::getPort(configuration.brokerAddress));
      //MqttSnURLParser::toString(configuration.brokerAddress, configuration.brokerAddress.size()) + ":-1";

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
  command += "-gfp ";
  if (configuration.find_gateway_type == MqttSnClientFindGatewayType::ANY) {
    command += "any";
  } else if (configuration.find_gateway_type == MqttSnClientFindGatewayType::CONNECT) {
    command += "connect";
  } else if (configuration.find_gateway_type == MqttSnClientFindGatewayType::ADVERTISE) {
    command += "advertise";
  } else if (configuration.find_gateway_type == MqttSnClientFindGatewayType::SEARCH) {
    command += "search";
  } else {
    throw std::exception();
  }

  command += " ";
  command += "-awt " + std::to_string(configuration.advertise_wait_timeout);

  command += " ";
  command += "-sgt " + std::to_string(configuration.search_gateway_wait_timeout);

  command += " ";
  command += "-sgr " + std::to_string(configuration.search_gateway_radius);

  auto connectAction = MqttSnClientConnectAction(configuration.clientId, configuration.clientPassword, configuration.keepAliveInterval, configuration.cleanSession);
  command += " " + getMqttSnClientConnectActionToCmdString(connectAction);
  return command;
}

void MqttSnClientArsmbCTestContainer::clearCmdPublishList() { parseCommandIntoConfiguration("-pc true"); }
void MqttSnClientArsmbCTestContainer::clearCmdRegisterList() { parseCommandIntoConfiguration("-rc true"); }
void MqttSnClientArsmbCTestContainer::clearCmdSubscribeList() { parseCommandIntoConfiguration("-sc true"); }
void MqttSnClientArsmbCTestContainer::clearCmdUnsubscribeList() { parseCommandIntoConfiguration("-uc true"); }

std::string MqttSnClientArsmbCTestContainer::getMqttSnClientPublishActionToCmdString(MqttSnClientPublishAction &publish_action) {
  if (publish_action.topicIdType == MqttSnClientPublishTopicIdType::PREDEFINED) {
    std::string result = "-pmhbL predefined://" + std::to_string(publish_action.qos) + ":" + getTrueOrFalse(publish_action.retain) + ":" + std::to_string(publish_action.topicId) + ":" +
                         getHexString(publish_action.payload, ":");
    return result;
  } else if (publish_action.topicIdType == MqttSnClientPublishTopicIdType::SHORT_NAME) {
    std::string result =
        "-pmhbL topicname://" + std::to_string(publish_action.qos) + ":" + getTrueOrFalse(publish_action.retain) + ":" + publish_action.topicName + ":" + getHexString(publish_action.payload, ":");
    return result;
  } else if (publish_action.topicIdType == MqttSnClientPublishTopicIdType::TOPIC_ID) {
    std::string result = "-pmhbL topicid://" + std::to_string(publish_action.qos) + ":" + getTrueOrFalse(publish_action.retain) + ":" + std::to_string(publish_action.topicId) + ":" +
                         getHexString(publish_action.payload, ":");
    return result;
  }
  throw std::exception();
}
std::string MqttSnClientArsmbCTestContainer::getTrueOrFalse(bool b) { return b ? "true" : "false"; }
std::string MqttSnClientArsmbCTestContainer::getHexString(const std::vector<uint8_t> &v, std::string d) {
  std::stringstream ss;
  ss << std::hex << std::setfill('0');
  std::vector<uint8_t>::const_iterator it;

  for (it = v.begin(); it != v.end(); it++) {
    //    ss << "\\x" << std::setw(2) << static_cast<unsigned>(*it) << d;
    ss << std::setw(2) << static_cast<unsigned>(*it) << d;
  }
  return ss.str();
}

std::string MqttSnClientArsmbCTestContainer::getMqttSnClientSubscribeActionToCmdString(MqttSnClientSubscribeAction &subscribeAction) {
  std::string command = "-stL ";
  if (subscribeAction.topicIdType == MqttSnClientPublishTopicIdType::SHORT_NAME) {
    command += "topicname";
  } else if (subscribeAction.topicIdType == MqttSnClientPublishTopicIdType::TOPIC_ID) {
    command += "topicid";
  } else if (subscribeAction.topicIdType == MqttSnClientPublishTopicIdType::PREDEFINED) {
    command += "predefined";
  } else {
    throw std::exception();
  }
  command += "://";
  command += std::to_string(subscribeAction.qos) + ":";
  if (subscribeAction.topicIdType == MqttSnClientPublishTopicIdType::SHORT_NAME) {
    command += subscribeAction.topicName;
  } else if (subscribeAction.topicIdType == MqttSnClientPublishTopicIdType::TOPIC_ID) {
    command += std::to_string(subscribeAction.topicId);
  } else if (subscribeAction.topicIdType == MqttSnClientPublishTopicIdType::PREDEFINED) {
    command += std::to_string(subscribeAction.topicId);
  } else {
    throw std::exception();
  }
  return command;
}
std::string MqttSnClientArsmbCTestContainer::getMqttSnClientUnsubscribeActionToCmdString(MqttSnClientUnsubscribeAction &unsubscribeAction) {
  std::string command = "-uL ";
  if (unsubscribeAction.topicIdType == MqttSnClientPublishTopicIdType::SHORT_NAME) {
    command += "topicname";
  } else if (unsubscribeAction.topicIdType == MqttSnClientPublishTopicIdType::TOPIC_ID) {
    command += "topicid";
  } else if (unsubscribeAction.topicIdType == MqttSnClientPublishTopicIdType::PREDEFINED) {
    command += "predefined";
  } else {
    throw std::exception();
  }
  command += "://";
  if (unsubscribeAction.topicIdType == MqttSnClientPublishTopicIdType::SHORT_NAME) {
    command += unsubscribeAction.topicName;
  } else if (unsubscribeAction.topicIdType == MqttSnClientPublishTopicIdType::TOPIC_ID) {
    command += std::to_string(unsubscribeAction.topicId);
  } else if (unsubscribeAction.topicIdType == MqttSnClientPublishTopicIdType::PREDEFINED) {
    command += std::to_string(unsubscribeAction.topicId);
  }
  return command;
}
std::string MqttSnClientArsmbCTestContainer::getMqttSnClientSleepActionToCmdString(MqttSnClientSleepAction &sleepAction) {
  std::string command = "-sd " + std::to_string(sleepAction.sleepDuration);
  return command;
}
std::string MqttSnClientArsmbCTestContainer::getMqttSnClientReceivePublishActionToCmdString(MqttSnClientPublishReceivePublishAction &action) {
  std::string command = "-pmhbL ";
  if (action.topicIdType == MqttSnClientPublishTopicIdType::PREDEFINED) {
    command += "predefined";
  } else if (action.topicIdType == MqttSnClientPublishTopicIdType::TOPIC_ID) {
    command += "topicid";
  } else if (action.topicIdType == MqttSnClientPublishTopicIdType::SHORT_NAME) {
    command += "topicname";
  }
  command += "://" + std::to_string(action.qos) + ":" + getTrueOrFalse(action.retain) + ":";
  if (action.topicIdType == MqttSnClientPublishTopicIdType::PREDEFINED) {
    command += std::to_string(action.topicId);
  } else if (action.topicIdType == MqttSnClientPublishTopicIdType::TOPIC_ID) {
    command += std::to_string(action.topicId);
  } else if (action.topicIdType == MqttSnClientPublishTopicIdType::SHORT_NAME) {
    command += action.topicName;
  }
  command += ":";
  command += getHexString(action.payload, ":");
  return command;
}
