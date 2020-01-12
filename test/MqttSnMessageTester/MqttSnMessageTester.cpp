//
// Created by SomeDude on 17.07.2019.
//

#include <logging/linux/stderr/StderrLogging.h>
#include <logging/linux/stdout/StdoutLogging.h>
#include <forwarder/config/forwarder_config.h>
#include <config/common/config_command_helper.h>
#include "MqttSnMessageTester.hpp"
#include <platform/MqttSnMessageData.h>

MqttSnMessageTester::MqttSnMessageTester() { forwarder_config_init(&fw_cfg); }

void MqttSnMessageTester::receive(MqttSnMessageData *message) {
  uint8_t *data = message->data;
  test_header *header = (test_header *) data;
  // TODO three byte header
  switch (header->type) {
    case TEST_MQTTSN_ADVERTISE:this->receiver->receive_advertise((test_advertise *) data);
      break;
    case TEST_MQTTSN_SEARCHGW:this->receiver->receive_searchgw((test_searchgw *) data);
      break;
    case TEST_MQTTSN_GWINFO:this->receiver->receive_gwinfo((test_gwinfo *) data);
      break;
    case TEST_MQTTSN_CONNECT:this->receiver->receive_connect((test_connect *) data);
      break;
    case TEST_MQTTSN_DISCONNECT:this->receiver->receive_disconnect((test_disconnect *) data);
      break;
    case TEST_MQTTSN_CONNACK:this->receiver->receive_connack((test_connack *) data);
      break;
    case TEST_MQTTSN_WILLTOPICREQ:this->receiver->receive_willtopicreq((test_willtopicreq *) data);
      break;
    case TEST_MQTTSN_WILLTOPIC:this->receiver->receive_willtopic((test_willtopic *) data);
      break;
    case TEST_MQTTSN_WILLMSGREQ:this->receiver->receive_willmsgreq((test_willmsgreq *) data);
      break;
    case TEST_MQTTSN_WILLMSG:this->receiver->receiver_willmsg((test_willmsg *) data);
      break;
    case TEST_MQTTSN_PUBLISH:this->receiver->receive_publish((test_publish *) data);
      break;
    case TEST_MQTTSN_PUBACK:this->receiver->receive_puback((test_puback *) data);
      break;
    case TEST_MQTTSN_PUBREC:this->receiver->receive_pubrec((test_pubrec *) data);
      break;
    case TEST_MQTTSN_PUBREL:this->receiver->receive_pubrel((test_pubrel *) data);
      break;
    case TEST_MQTTSN_PUBCOMP:this->receiver->receive_pubcomp((test_pubcomp *) data);
      break;
    case TEST_MQTTSN_REGISTER:this->receiver->receiver_register((test_register *) data);
      break;
    case TEST_MQTTSN_REGACK:this->receiver->receive_regack((test_regack *) data);
      break;
    case TEST_MQTTSN_SUBSCRIBE:this->receiver->receive_subscribe((test_subscribe *) data);
      break;
    case TEST_MQTTSN_SUBACK:this->receiver->receive_suback((test_suback *) data);
      break;
    case TEST_MQTTSN_UNSUBSCRIBE:this->receiver->receive_unsubscribe((test_unsubscribe *) data);
      break;
    case TEST_MQTTSN_UNSUBACK:this->receiver->receive_unsuback((test_unsuback *) data);
      break;
    case TEST_MQTTSN_PINGREQ:this->receiver->receive_pingreq((test_pingreq *) data);
      break;
    case TEST_MQTTSN_PINGRESP:this->receiver->receive_pingresp((test_pingresp *) data);
      break;
      /*
     case TEST_MQTTSN_WILLTOPICUPD:
         //this->receiver->receive_willtopicudp((msg_willtopicudp *) data);
         // TODO
         throw new std::invalid_argument("not implemented yet");
         break;
     case TEST_MQTTSN_WILLTOPICRESP:
         this->receiver->receive_willtopicresp((msg_willtopicresp *) data);
         break;
     case TEST_MQTTSN_WILLMSGUPD:
         //this->receiver->receive_willmsgudp((msg_willmsgupd*) data);
         // TODO
         throw new std::invalid_argument("not implemented yet");
         break;
     case TEST_MQTTSN_WILLMSGRESP:
         this->receiver->receive_willmsgresp((msg_willmsgresp *) data);
         break;
     case TEST_MQTTSN_ENCAPSULATION_HEADER:
         this->receiver->receive_encapsulationmessage((msg_encapsulationmessage *) data);
         break;
          */
    default:this->receiver->receive_any_message((uint16_t) header->length, header->type, data);
      break;
  }
}
void MqttSnMessageTester::loop() {
  running = true;

  bool network_error = false;
  while (!stopped && !network_error) {
    if (clientNetworkContext) {
      if (ClientNetworkReceive(&clientNetwork,
                               &clientNetworkReceiveBuffer,
                               clientNetworkReceiveTimeout,
                               clientNetworkContext) < 0) {
        network_error = true;
      }
      MqttSnMessageData msg = {0};
      if (pop(&clientNetworkReceiveBuffer, &msg) == 0) {
        receive(&msg);
      }
      if (ClientNetworkSend(&clientNetwork,
                            &clientNetworkSendBuffer,
                            clientNetworkSendTimeout,
                            clientNetworkContext) < 0) {
        network_error = true;
      }
    }
    if (gatewayNetworkContext) {
      if (GatewayNetworkReceive(&gatewayNetwork,
                                &gatewayNetworkReceiveBuffer,
                                gatewayNetworkReceiveTimeout,
                                gatewayNetworkContext) < 0) {
        network_error = true;
      }
      MqttSnMessageData msg = {0};
      if (pop(&gatewayNetworkReceiveBuffer, &msg) == 0) {
        receive(&msg);
      }
      if (GatewayNetworkSend(&gatewayNetwork,
                             &gatewayNetworkSendBuffer,
                             gatewayNetworkSendTimeout,
                             gatewayNetworkContext) < 0) {
        network_error = true;
      }
    }

  }

  if (network_error) {
    // TODO log
  }
  if (gatewayNetworkContext) {
    GatewayNetworkDisconnect(&gatewayNetwork, gatewayNetworkContext);
    GatewayNetworkDeinitialize(&gatewayNetwork, gatewayNetworkContext);
  }
  if (clientNetworkContext) {
    ClientNetworkDisconnect(&clientNetwork, clientNetworkContext);
    ClientNetworkDeinitialize(&clientNetwork, clientNetworkContext);
  }
  forwarder_config_cleanup(&fw_cfg);
  running = false;
}
bool MqttSnMessageTester::isRunning() {
  return running;
}
bool MqttSnMessageTester::start() {
  if (receiver == nullptr) {
    return false;
  }
  if (clientNetworkContext == nullptr && gatewayNetworkContext == nullptr) {
    return false;
  }
  if (clientNetworkSendTimeout < -1) {
    return false;
  }
  if (clientNetworkReceiveTimeout < -1) {
    return false;
  }
  if (gatewayNetworkSendTimeout < -1) {
    return false;
  }
  if (gatewayNetworkReceiveTimeout < -1) {
    return false;
  }

  bool start_result = true;
  if (clientNetworkContext) {
    clientNetwork.logger = &fw_logger;
    if (ClientNetworkConnect(&clientNetwork, clientNetworkContext) < 0) {
      start_result = false;
    }
  }
  if (gatewayNetworkContext) {
    gatewayNetwork.logger = &fw_logger;
    if (GatewayNetworkConnect(&gatewayNetwork, gatewayNetworkContext) < 0) {
      start_result = false;
    }
  }

  if (!start_result) {
    if (clientNetworkContext) {
      ClientNetworkDeinitialize(&clientNetwork, clientNetworkContext);
    }
    if (gatewayNetworkContext) {
      GatewayNetworkDeinitialize(&gatewayNetwork, gatewayNetworkContext);
    }
  }

  if (start_result) {
    this->thread = std::thread(&MqttSnMessageTester::loop, this);
    this->thread.detach();
  }

  return start_result;
}

void MqttSnMessageTester::stop() {
  this->stopped = true;
}
bool MqttSnMessageTester::send(const uint8_t *data, uint16_t data_length) {
  MqttSnMessageData msg_data = {0};
  if (data_length > sizeof(msg_data.data)) {
    return false;
  }
  bool put_rc = true;

  if (clientNetworkContext) {
    memcpy(msg_data.data, data, data_length);
    msg_data.data_length = data_length;
    msg_data.to = (*clientNetwork.mqtt_sn_gateway_address);
    msg_data.from = (*clientNetwork.client_network_address);
    // TODO create time
    // TODO signal strength
    if (clientNetworkContext) {
      if (put(&clientNetworkSendBuffer, &msg_data) < 0) {
        put_rc = false;
      }
    }
  }
  if (gatewayNetworkContext) {
    memcpy(msg_data.data, data, data_length);
    msg_data.data_length = data_length;
    msg_data.to = (*gatewayNetwork.mqtt_sn_gateway_address);
    msg_data.from = (*gatewayNetwork.gateway_network_address);
    // TODO create time
    // TODO signal strength
    if (gatewayNetworkContext) {
      if (put(&gatewayNetworkSendBuffer, &msg_data) < 0) {
        put_rc = false;
      }
    }
  }

  return put_rc;
}
bool MqttSnMessageTester::SetConfiguration(const std::string &identifier, const std::string &command) {
  if (isRunning()) {
    return false;
  }

  MqttSnLogger cfg_logger = {0};
  if (MqttSnLoggerInit(&cfg_logger, LOG_LEVEL_VERBOSE, stderr_log_init)) {
    return false;
  }

  if (forwarder_config_init(&fw_cfg) < 0) {
    return false;
  }

  this->identifier = identifier;
  this->cmd = command;

  std::copy(command.begin(), command.end(), std::back_inserter(line_str));
  line_str.push_back('\0');
  int32_t rc = process_config_cmd_str(&cfg_logger,
                                      line_str.data(),
                                      line_str.size(),
                                      identifier.data(),
                                      &fw_cfg,
                                      forwarder_config_file_process_command_callback,
                                      argv,
                                      &argc,
                                      argv_max_len);

  if (rc) {
    forwarder_config_cleanup(&fw_cfg);
    MqttSnLoggerDeinit(&cfg_logger);
    return false;
  }

  clientNetworkSendTimeout = fw_cfg.cncfg.client_network_send_timeout;
  clientNetworkReceiveTimeout = fw_cfg.cncfg.client_network_receive_timeout;
  gatewayNetworkSendTimeout = fw_cfg.gncfg.gateway_network_send_timeout;
  gatewayNetworkReceiveTimeout = fw_cfg.gncfg.gateway_network_receive_timeout;

  if (start_logger(&fw_cfg.mslcfg, &fw_logger) < 0) {
    log_str(&cfg_logger, "Could not initialize mqtt sn logger\n");
    MqttSnLoggerDeinit(&cfg_logger);
    return false;
  }

  MqttSnLoggerDeinit(&cfg_logger);
  return true;
}
void MqttSnMessageTester::SetReceiver(MqttSnTestMessageReceiverInterface *mqttSnTestMessageReceiverInterface) {
  MqttSnMessageTester::receiver = mqttSnTestMessageReceiverInterface;
}
int32_t MqttSnMessageTester::start_logger(const mqtt_sn_logger_config *cfg, MqttSnLogger *logger) {
  if (!strcmp(cfg->log_target, "console")) {
    if (cfg->log_filepath != NULL) {
      if (MqttSnLoggerInitFile(logger,
                               cfg->log_lvl,
                               cfg->log_filepath,
                               file_stdout_log_init,
                               &file_stdout_logging_context_) < 0) {
        return -1;
      }
    } else {
      if (MqttSnLoggerInit(logger, cfg->log_lvl, stdout_log_init) < 0) {
        return -1;
      }
    }
  } else if (!strcmp(cfg->log_target, "file")) {
    if (MqttSnLoggerInitFile(logger, cfg->log_lvl, cfg->log_filepath, file_log_init, &file_logging_context_) < 0) {
      return -1;
    }
  }
  if (cfg->log_identifier) {
    logger->log_identifier = cfg->log_identifier;
  }
  return 0;
}
bool MqttSnMessageTester::InitializeClientNetwork() {
  MqttSnFixedSizeRingBufferInit(&clientNetworkSendBuffer);
  MqttSnFixedSizeRingBufferInit(&clientNetworkReceiveBuffer);
  clientNetworkContext = clientNetworkContextTestContainer->InitializeClientNetwork(&clientNetwork,
                                                                                    &fw_cfg.cncfg,
                                                                                    &fw_cfg.msgcfg);
  return clientNetworkContext != nullptr;
}
bool MqttSnMessageTester::InitializeGatewayNetwork() {
  MqttSnFixedSizeRingBufferInit(&gatewayNetworkSendBuffer);
  MqttSnFixedSizeRingBufferInit(&gatewayNetworkReceiveBuffer);
  gatewayNetworkContextTestContainer = std::make_shared<NetworkContextTestContainer>();
  gatewayNetworkContext = gatewayNetworkContextTestContainer->InitializeGatewayNetwork(&gatewayNetwork,
                                                                                       &fw_cfg.gncfg,
                                                                                       &fw_cfg.msgcfg,
                                                                                       &fw_cfg.cfmsngcfg);
  return gatewayNetworkContext != nullptr;
}
