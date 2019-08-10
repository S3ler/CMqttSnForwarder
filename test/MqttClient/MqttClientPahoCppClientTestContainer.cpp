//
// Created by SomeDude on 21.07.2019.
//

#include "MqttClientPahoCppClientTestContainer.h"
#include "MqttClientActionResult.h"
#include "MqttClientPahoCppTestContainerReceiver.h"

int paho_global_publish_arrived(void *context, char *topicName, int topicLen, MQTTAsync_message *message) {
  MqttClientPahoCppClientTestContainer *client = static_cast<MqttClientPahoCppClientTestContainer *>(context);
  client->receive_publish(topicName, topicLen, message);
  MQTTAsync_freeMessage(&message);
  MQTTAsync_free(topicName);
  return 1;
}
void paho_global_connection_lost(void *context, char *cause) {
  static_cast<MqttClientPahoCppClientTestContainer *>(context)->connection_lost(cause);
}
void paho_global_on_connect_failure(void *context, MQTTAsync_failureData *response) {
  static_cast<MqttClientPahoCppClientTestContainer *>(context)->on_action_finished(MqttClientActionResultType::ERROR);
}
void paho_global_on_connect_success(void *context, MQTTAsync_successData *response) {
  static_cast<MqttClientPahoCppClientTestContainer *>(context)->on_action_finished(MqttClientActionResultType::SUCCESS);
}
void paho_global_on_disconnect(void *context, MQTTAsync_successData *response) {
  static_cast<MqttClientPahoCppClientTestContainer *>(context)->on_action_finished(MqttClientActionResultType::SUCCESS);
}
void paho_global_on_subscribe(void *context, MQTTAsync_successData *response) {
  static_cast<MqttClientPahoCppClientTestContainer *>(context)->on_action_finished(MqttClientActionResultType::SUCCESS);
}
void paho_global_on_subscribe_failure(void *context, MQTTAsync_failureData *response) {
  static_cast<MqttClientPahoCppClientTestContainer *>(context)->on_action_finished(MqttClientActionResultType::ERROR);
}
void paho_global_on_unsubscribe(void *context, MQTTAsync_successData *response) {
  static_cast<MqttClientPahoCppClientTestContainer *>(context)->on_action_finished(MqttClientActionResultType::SUCCESS);
}
void paho_global_on_unsubscribe_failure(void *context, MQTTAsync_failureData *response) {
  static_cast<MqttClientPahoCppClientTestContainer *>(context)->on_action_finished(MqttClientActionResultType::ERROR);
}
void paho_global_on_publish(void *context, MQTTAsync_successData *response) {
  static_cast<MqttClientPahoCppClientTestContainer *>(context)->on_publish_action_finished(MqttClientActionResultType::SUCCESS,
                                                                                           response->token);
}
void paho_global_on_publish_failure(void *context, MQTTAsync_failureData *response) {
  static_cast<MqttClientPahoCppClientTestContainer *>(context)->on_publish_action_finished(MqttClientActionResultType::ERROR,
                                                                                           response->token);
}
void paho_global_on_publish_receive_publish(void *context, MQTTAsync_successData *response) {
  // nothing to do here
  /*
  static_cast<MqttClientPahoCppClientTestContainer *>(context)->on_publish_action_finished(MqttClientActionResultType::SUCCESS,
                                                                                           response->token);
  */
}
void paho_global_on_publish_receive_publish_failure(void *context, MQTTAsync_failureData *response) {
  static_cast<MqttClientPahoCppClientTestContainer *>(context)->on_publish_action_finished(MqttClientActionResultType::ERROR,
                                                                                           response->token);
}
MqttClientPahoCppClientTestContainer::MqttClientPahoCppClientTestContainer(const MqttClientTestContainerConfiguration &configuration)
    : MqttClientTestContainerInterface(configuration) {
  clientId = initial_connect_action.clientId;
  brokerURI = initial_connect_action.protocol + "://" + initial_connect_action.brokerAddress + ":"
      + std::to_string(initial_connect_action.brokerPort);
  // MQTTAsync_create(&client, brokerURI.c_str(), clientId.c_str(), MQTTCLIENT_PERSISTENCE_NONE, NULL);
}
MqttClientPahoCppClientTestContainer::~MqttClientPahoCppClientTestContainer() {
  // MQTTAsync_destroy(&client);
}
bool MqttClientPahoCppClientTestContainer::StartBackgroundHandler() {
  if (running) {
    return false;
  }
  running = true;
  stopped = false;

  MQTTAsync_create(&client, brokerURI.c_str(), clientId.c_str(), MQTTCLIENT_PERSISTENCE_NONE, NULL);
  return MQTTAsync_setCallbacks(client,
                                this,
                                paho_global_connection_lost,
                                paho_global_publish_arrived,
                                NULL) == MQTTASYNC_SUCCESS;
}
void MqttClientPahoCppClientTestContainer::StopBackgroundHandler() {
  stopped = true;
  running = false;
  MQTTAsync_destroy(&client);
}
bool MqttClientPahoCppClientTestContainer::IsBackgroundHandlerRunning() {
  return running;
}

MqttClientActionResult MqttClientPahoCppClientTestContainer::connect() {
  if (!action_completed) {
    throw std::exception();
  }

  conn_opts = MQTTAsync_connectOptions_initializer;
  conn_opts.keepAliveInterval = initial_connect_action.keepAliveInterval;
  conn_opts.cleansession = initial_connect_action.cleanSession;
  conn_opts.onSuccess = paho_global_on_connect_success;
  conn_opts.onFailure = paho_global_on_connect_failure;
  conn_opts.context = this;

  action_completed = false;

  clock_gettime(CLOCK_REALTIME, &start_ts);
  if (MQTTAsync_connect(client, &conn_opts) != MQTTASYNC_SUCCESS) {
    on_action_finished(MqttClientActionResultType::ERROR);
  }
  while (!action_completed) { /*TODO remove busy wait by better construct*/ }

  return MqttClientActionResult(clientId,
                                initial_connect_action.action_type,
                                MqttClientTestContainerInterface::action_number++,
                                action_result,
                                start_ts,
                                end_ts);
}
/*
MqttClientActionResult MqttClientPahoCppClientTestContainer::connect(MqttClientConnectAction &connect_action) {
  if (!action_completed) {
    throw std::exception();
  }
  brokerURI = connect_action.protocol + "://" + connect_action.brokerAddress + ":"
      + std::to_string(connect_action.brokerPort);
  clientId = connect_action.clientId;

  conn_opts = MQTTAsync_connectOptions_initializer;
  conn_opts.keepAliveInterval = connect_action.keepAliveInterval;
  conn_opts.cleansession = connect_action.cleanSession;
  conn_opts.onSuccess = paho_global_on_connect_success;
  conn_opts.onFailure = paho_global_on_connect_failure;
  conn_opts.context = this;

  action_completed = false;

  clock_gettime(CLOCK_REALTIME, &start_ts);
  if (MQTTAsync_connect(client, &conn_opts) != MQTTASYNC_SUCCESS) {
    on_action_finished(MqttClientActionResultType::ERROR);
  }
  while (!action_completed) {  }

  return MqttClientActionResult(clientId,
                                connect_action.action_type,
                                MqttClientTestContainerInterface::action_number++,
                                action_result,
                                start_ts,
                                end_ts);
}
*/
MqttClientActionResult MqttClientPahoCppClientTestContainer::disconnect() {
  if (!action_completed) {
    throw std::exception();
  }

  MQTTAsync_disconnectOptions disc_opts = MQTTAsync_disconnectOptions_initializer;
  disc_opts.onSuccess = paho_global_on_disconnect;
  disc_opts.context = this;

  action_completed = false;

  clock_gettime(CLOCK_REALTIME, &start_ts);
  if (MQTTAsync_disconnect(client, &disc_opts) != MQTTASYNC_SUCCESS) {
    on_action_finished(MqttClientActionResultType::ERROR);
  }
  while (!action_completed) { /*TODO remove busy wait by better construct*/ }
  return MqttClientActionResult(clientId,
                                MqttClientActionType::DISCONNECT,
                                MqttClientTestContainerInterface::action_number++,
                                action_result,
                                start_ts,
                                end_ts);
}
MqttClientActionResult MqttClientPahoCppClientTestContainer::subscribe(MqttClientSubscribeAction &subscribe_action) {
  if (!action_completed) {
    throw std::exception();
  }

  MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
  opts.onSuccess = paho_global_on_subscribe;
  opts.onFailure = paho_global_on_subscribe_failure;
  opts.context = this;

  action_completed = false;

  clock_gettime(CLOCK_REALTIME, &start_ts);
  if (MQTTAsync_subscribe(client, subscribe_action.topic_name.c_str(), subscribe_action.qos, &opts)
      != MQTTASYNC_SUCCESS) {
    on_action_finished(MqttClientActionResultType::ERROR);
  }
  while (!action_completed) { /*TODO remove busy wait by better construct*/ }

  return MqttClientActionResult(clientId,
                                subscribe_action.action_type,
                                MqttClientTestContainerInterface::action_number++,
                                action_result,
                                start_ts,
                                end_ts);
}
MqttClientActionResult MqttClientPahoCppClientTestContainer::unsubscribe(MqttClientUnsubscribeAction &unsubscribe_action) {
  if (!action_completed) {
    throw std::exception();
  }

  MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
  opts.onSuccess = paho_global_on_unsubscribe;
  opts.onFailure = paho_global_on_unsubscribe_failure;
  opts.context = this;

  action_completed = false;

  clock_gettime(CLOCK_REALTIME, &start_ts);
  if (MQTTAsync_unsubscribe(client, unsubscribe_action.topic_name.c_str(), &opts) != MQTTASYNC_SUCCESS) {
    on_action_finished(MqttClientActionResultType::ERROR);
  }
  while (!action_completed) { /*TODO remove busy wait by better construct*/ }

  return MqttClientActionResult(clientId,
                                unsubscribe_action.action_type,
                                MqttClientTestContainerInterface::action_number++,
                                action_result,
                                start_ts,
                                end_ts);
}
MqttClientActionResult MqttClientPahoCppClientTestContainer::publish(MqttClientPublishAction &publish_action) {
  if (!action_completed) {
    throw std::exception();
  }

  MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
  opts.onSuccess = paho_global_on_publish;
  opts.onFailure = paho_global_on_publish_failure;
  opts.context = this;

  MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
  pubmsg.payload = (void *) publish_action.payload.data();
  pubmsg.payloadlen = publish_action.payload.size();
  pubmsg.qos = publish_action.qos;
  pubmsg.retained = publish_action.retain;

  action_completed = false;
  publish_token = 0;

  clock_gettime(CLOCK_REALTIME, &start_ts);
  if (MQTTAsync_sendMessage(client, publish_action.topic_name.c_str(), &pubmsg, &opts) != MQTTASYNC_SUCCESS) {
    on_action_finished(MqttClientActionResultType::ERROR);
  }

  while (!action_completed) { /*TODO remove busy wait by better construct*/ }

  return MqttClientActionResult(clientId,
                                publish_action.action_type,
                                MqttClientTestContainerInterface::action_number++,
                                action_result,
                                start_ts,
                                end_ts);
}
MqttClientActionResult MqttClientPahoCppClientTestContainer::publishReceivePublish(MqttClientPublishReceivePublish &publish_receive_publish) {
  if (!action_completed) {
    throw std::exception();
  }
  auto publish_action = publish_receive_publish.publish_action;

  MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
  opts.onSuccess = paho_global_on_publish_receive_publish;
  opts.onFailure = paho_global_on_publish_receive_publish_failure;
  opts.context = this;

  MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
  pubmsg.payload = (void *) publish_action.payload.data();
  pubmsg.payloadlen = publish_action.payload.size();
  pubmsg.qos = publish_action.qos;
  pubmsg.retained = publish_action.retain;

  MqttClientTestContainerReceiverInterface *old_interface = MqttClientTestContainerInterface::receiver_interface;
  MqttClientPahoCppTestContainerReceiver receiver_interface(publish_receive_publish, this);
  SetReceiverInterface(&receiver_interface);

  action_completed = false;
  publish_token = 0;

  clock_gettime(CLOCK_REALTIME, &start_ts);
  if (MQTTAsync_sendMessage(client, publish_action.topic_name.c_str(), &pubmsg, &opts) != MQTTASYNC_SUCCESS) {
    on_action_finished(MqttClientActionResultType::ERROR);
    SetReceiverInterface(old_interface);
    return MqttClientActionResult(clientId,
                                  publish_receive_publish.action_type,
                                  MqttClientTestContainerInterface::action_number++,
                                  action_result,
                                  start_ts,
                                  end_ts);
  }

  while (!action_completed) { /*TODO remove busy wait by better construct*/ }

  return MqttClientActionResult(clientId,
                                publish_receive_publish.action_type,
                                MqttClientTestContainerInterface::action_number++,
                                action_result,
                                start_ts,
                                end_ts);
}
void MqttClientPahoCppClientTestContainer::receive_publish(char *topicName, int topicLen, MQTTAsync_message *message) {
  uint8_t *payload = static_cast<uint8_t *>(message->payload);
  int payloadLen = message->payloadlen;
  std::vector<uint8_t> data(payload, payload + payloadLen);
  int qos = message->qos;
  MqttClientTestContainerPublish received_publish(data, std::string(topicName), qos);
  ReceivePublish(received_publish);
}
void MqttClientPahoCppClientTestContainer::connection_lost(char *cause) {
  on_action_finished(MqttClientActionResultType::CONNECTIONLOST);
}
void MqttClientPahoCppClientTestContainer::on_action_finished(MqttClientActionResultType action_result) {
  clock_gettime(CLOCK_REALTIME, &end_ts);
  this->action_result = action_result;
  action_completed = true;
}
void MqttClientPahoCppClientTestContainer::on_publish_action_finished(MqttClientActionResultType action_result,
                                                                      MQTTAsync_token publish_token) {
  if (this->publish_token == publish_token) {
    on_action_finished(action_result);
  }
}
void MqttClientPahoCppClientTestContainer::on_receive_publish_action_finished(MqttClientActionResultType action_result,
                                                                              timespec end_ts) {
  this->end_ts = end_ts;
  this->action_result = action_result;
  action_completed = true;
}
