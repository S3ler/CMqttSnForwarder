//
// Created by SomeDude on 21.07.2019.
//

#include "MqttClientPahoCppClientTestContainer.h"
#include "MqttClientActionResult.h"

void paho_global_message_delivered(void *context, MQTTClient_deliveryToken dt) {
  MqttClientPahoCppClientTestContainer *client = static_cast<MqttClientPahoCppClientTestContainer *>(context);
  client->message_delivered(dt);
}

int paho_global_publish_arrived(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
  MqttClientPahoCppClientTestContainer *client = static_cast<MqttClientPahoCppClientTestContainer *>(context);
  client->receive_publish(topicName, topicLen, message);
  MQTTClient_freeMessage(&message);
  MQTTClient_free(topicName);
  return 1;
}

void paho_global_connection_lost(void *context, char *cause) {
  MqttClientPahoCppClientTestContainer *client = static_cast<MqttClientPahoCppClientTestContainer *>(context);
  client->connection_lost(cause);
}

MqttClientPahoCppClientTestContainer::MqttClientPahoCppClientTestContainer(const MqttClientConnectAction &initial_connect_action)
    : MqttClientTestContainerInterface(initial_connect_action) {
  brokerURI = initial_connect_action.protocol + "://" + initial_connect_action.brokerAddress + ":"
      + std::to_string(initial_connect_action.brokerPort);
  MQTTClient_create(&client,
                    brokerURI.c_str(),
                    initial_connect_action.clientId.c_str(),
                    MQTTCLIENT_PERSISTENCE_NONE,
                    NULL);
}
MqttClientPahoCppClientTestContainer::~MqttClientPahoCppClientTestContainer() {
  MQTTClient_destroy(&client);
}
MqttClientActionResult MqttClientPahoCppClientTestContainer::connect() {

  conn_opts = MQTTClient_connectOptions_initializer;
  conn_opts.keepAliveInterval = initial_connect_action.keepAliveInterval;
  conn_opts.cleansession = initial_connect_action.cleanSession;
  // TODO will

  timespec start_ts;
  timespec end_ts;
  MqttClientActionResultType action_result;

  clock_gettime(CLOCK_REALTIME, &start_ts);
  if (MQTTClient_connect(client, &conn_opts) != MQTTCLIENT_SUCCESS) {
    action_result = MqttClientActionResultType::ERROR;
    clock_gettime(CLOCK_REALTIME, &end_ts);
  } else {
    action_result = MqttClientActionResultType::SUCCESS;
    clock_gettime(CLOCK_REALTIME, &end_ts);
  }
  return MqttClientActionResult(initial_connect_action.clientId,
                                initial_connect_action.action_type,
                                MqttClientTestContainerInterface::action_number++,
                                action_result,
                                start_ts,
                                end_ts);
}

uint64_t MqttClientPahoCppClientTestContainer::connect(MqttClientConnectAction &connect_action) {
  return 0;
}
MqttClientActionResult MqttClientPahoCppClientTestContainer::disconnect() {
  timespec start_ts;
  timespec end_ts;
  MqttClientActionResultType action_result;

  clock_gettime(CLOCK_REALTIME, &start_ts);
  if (MQTTClient_disconnect(client, 10000) != MQTTCLIENT_SUCCESS) {
    action_result = MqttClientActionResultType::ERROR;
    clock_gettime(CLOCK_REALTIME, &end_ts);
  } else {
    action_result = MqttClientActionResultType::SUCCESS;
    clock_gettime(CLOCK_REALTIME, &end_ts);
  }
  return MqttClientActionResult(initial_connect_action.clientId,
                                MqttClientActionType::DISCONNECT,
                                MqttClientTestContainerInterface::action_number++,
                                action_result,
                                start_ts,
                                end_ts);
}
uint64_t MqttClientPahoCppClientTestContainer::publish(MqttClientPublishAction &publish_action) {
  return 0;
}
MqttClientActionResult MqttClientPahoCppClientTestContainer::subscribe(MqttClientSubscribeAction &subscribe_action) {
  timespec start_ts;
  timespec end_ts;
  MqttClientActionResultType action_result;

  clock_gettime(CLOCK_REALTIME, &start_ts);
  if (MQTTClient_subscribe(client, subscribe_action.topic_name.c_str(), subscribe_action.qos) != MQTTCLIENT_SUCCESS) {
    action_result = MqttClientActionResultType::ERROR;
    clock_gettime(CLOCK_REALTIME, &end_ts);
  } else {
    action_result = MqttClientActionResultType::SUCCESS;
    clock_gettime(CLOCK_REALTIME, &end_ts);
  }

  return MqttClientActionResult(initial_connect_action.clientId,
                                MqttClientActionType::DISCONNECT,
                                MqttClientTestContainerInterface::action_number++,
                                action_result,
                                start_ts,
                                end_ts);
}

bool MqttClientPahoCppClientTestContainer::StartBackgroundHandler() {
  if (running) {
    return false;
  }
  running = true;
  stopped = false;
  tk = 0;

  return MQTTClient_setCallbacks(client,
                                 this,
                                 paho_global_connection_lost,
                                 paho_global_publish_arrived,
                                 paho_global_message_delivered) == MQTTCLIENT_SUCCESS;
}
/*
void MqttClientPahoCppClientTestContainer::loop() {
  running = true;
  stopped = false;

  running = false;
}
 */
void MqttClientPahoCppClientTestContainer::StopBackgroundHandler() {
  stopped = true;
  running = false;
}
bool MqttClientPahoCppClientTestContainer::IsBackgroundHandlerRunning() {
  return running;
}
void MqttClientPahoCppClientTestContainer::message_delivered(MQTTClient_deliveryToken token) {
  // TODO locking?
}
void MqttClientPahoCppClientTestContainer::receive_publish(char *topicName, int topicLen, MQTTClient_message *message) {
  if (receiver_interface) {
    uint8_t *payload = static_cast<uint8_t *>(message->payload);
    int payloadLen = message->payloadlen;
    std::vector<uint8_t> data(payload, payload + payloadLen);
    int qos = message->qos;
    MqttClientTestContainerPublish received_publish(data, std::string(topicName), qos);
    receiver_interface->receive(received_publish);
  }
}
void MqttClientPahoCppClientTestContainer::connection_lost(char *cause) {
  // TODO locking?

}
