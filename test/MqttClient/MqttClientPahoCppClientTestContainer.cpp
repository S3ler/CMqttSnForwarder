//
// Created by SomeDude on 21.07.2019.
//

#include "MqttClientPahoCppClientTestContainer.h"
#include <external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/MQTTAsync.h>
#include <gateway_old/paho/pahomqttembeddedc/MQTTClient/src/MQTTLogging.h>
#include <mutex>
#include "MqttClientActionResult.h"
#include "MqttClientPahoCppTestContainerReceiver.h"

int paho_global_publish_arrived(void *context, char *topicName, int topicLen, MQTTAsync_message *message) {
  MqttClientPahoCppClientTestContainer *client = static_cast<MqttClientPahoCppClientTestContainer *>(context);
  client->receive_publish(topicName, topicLen, message);
  MQTTAsync_freeMessage(&message);
  MQTTAsync_free(topicName);
  return 1;
}
void paho_global_connection_lost(void *context, char *cause) { static_cast<MqttClientPahoCppClientTestContainer *>(context)->connection_lost(cause); }
void paho_global_on_connect_failure(void *context, MQTTAsync_failureData *response) {
  static_cast<MqttClientPahoCppClientTestContainer *>(context)->on_action_finished(MqttClientActionResultType::ERROR);
}
void paho_global_on_connect_success(void *context, MQTTAsync_successData *response) {
  static_cast<MqttClientPahoCppClientTestContainer *>(context)->on_action_finished(MqttClientActionResultType::SUCCESS);
}
void paho_global_on_disconnect(void *context, MQTTAsync_successData *response) {
  static_cast<MqttClientPahoCppClientTestContainer *>(context)->on_action_finished(MqttClientActionResultType::SUCCESS);
}
void paho_global_on_subscribe(void *context, MQTTAsync_successData *response) { static_cast<MqttClientPahoCppClientTestContainer *>(context)->on_action_finished(MqttClientActionResultType::SUCCESS); }
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
  static_cast<MqttClientPahoCppClientTestContainer *>(context)->on_publish_action_finished(MqttClientActionResultType::SUCCESS, response->token);
}
void paho_global_on_publish_failure(void *context, MQTTAsync_failureData *response) {
  static_cast<MqttClientPahoCppClientTestContainer *>(context)->on_publish_action_finished(MqttClientActionResultType::ERROR, response->token);
}
void paho_global_on_publish_receive_publish(void *context, MQTTAsync_successData *response) {
  // nothing to do here
  /*
  static_cast<MqttClientPahoCppClientTestContainer *>(context)->on_publish_action_finished(MqttClientActionResultType::SUCCESS,
                                                                                           response->token);
  */
}
void paho_global_on_publish_receive_publish_failure(void *context, MQTTAsync_failureData *response) {
  static_cast<MqttClientPahoCppClientTestContainer *>(context)->on_publish_action_finished(MqttClientActionResultType::ERROR, response->token);
}
MqttClientPahoCppClientTestContainer::MqttClientPahoCppClientTestContainer(const MqttClientTestContainerConfiguration &configuration) : MqttClientTestContainerInterface(configuration) {}
bool MqttClientPahoCppClientTestContainer::StartBackgroundHandler() {
  if (running) {
    return false;
  }
  running = true;
  stopped = false;
  return running;
}
void MqttClientPahoCppClientTestContainer::StopBackgroundHandler() {
  stopped = true;
  running = false;
}
bool MqttClientPahoCppClientTestContainer::IsBackgroundHandlerRunning() { return running; }

MqttClientActionResult MqttClientPahoCppClientTestContainer::connect(MqttClientConnectAction &connectAction) {
  if (!action_completed) {
    throw std::exception();
  }

  MqttClientActionType actionType = connectAction.action_type;
  MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
  MQTTAsync_willOptions willOptions = MQTTAsync_willOptions_initializer;

  brokerURI = connectAction.protocol + "://" + connectAction.brokerAddress + ":" + std::to_string(connectAction.brokerPort);
  clientId = connectAction.clientId;

  conn_opts = MQTTAsync_connectOptions_initializer;
  conn_opts.keepAliveInterval = connectAction.keepAliveInterval;
  conn_opts.cleansession = connectAction.cleanSession;

  if (connectAction.willTopic.length() > 0) {
    willOptions.topicName = connectAction.willTopic.c_str();
    willOptions.payload.data = connectAction.willMessage.data();
    willOptions.payload.len = connectAction.willMessage.size();
    willOptions.retained = connectAction.willRetain;
    willOptions.qos = connectAction.willQoS;
    conn_opts.will = &willOptions;
    actionType = MqttClientActionType::CONNECT_WITH_WILL;
  } else {
    conn_opts.will = NULL;
  }

  conn_opts.onSuccess = paho_global_on_connect_success;
  conn_opts.onFailure = paho_global_on_connect_failure;
  conn_opts.context = this;

  action_completed = false;

  clock_gettime(CLOCK_REALTIME, &start_ts);
  MQTTAsync_create(&client, brokerURI.c_str(), clientId.c_str(), MQTTCLIENT_PERSISTENCE_NONE, NULL);
  if (MQTTAsync_setCallbacks(client, this, paho_global_connection_lost, paho_global_publish_arrived, NULL) == MQTTASYNC_SUCCESS) {
    if (MQTTAsync_connect(client, &conn_opts) != MQTTASYNC_SUCCESS) {
      on_action_finished(MqttClientActionResultType::ERROR);
    }
    std::unique_lock<std::mutex> lk(m);
    cv.wait(lk);
    //while (!action_completed) {  }
  }

  return MqttClientActionResult(clientId, actionType, MqttClientTestContainerInterface::action_number++, action_result, start_ts, end_ts);
}
MqttClientActionResult MqttClientPahoCppClientTestContainer::disconnect(MqttClientDisconnectAction &disconnectAction) {
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

  std::unique_lock<std::mutex> lk(m);
  cv.wait(lk);
  //while (!action_completed) {   }

  if (connected) {
    connected = false;
    MQTTAsync_destroy(&client);
  }
  return MqttClientActionResult(clientId, MqttClientActionType::DISCONNECT, MqttClientTestContainerInterface::action_number++, action_result, start_ts, end_ts);
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
  if (MQTTAsync_subscribe(client, subscribe_action.topic_name.c_str(), subscribe_action.qos, &opts) != MQTTASYNC_SUCCESS) {
    on_action_finished(MqttClientActionResultType::ERROR);
  }

  std::unique_lock<std::mutex> lk(m);
  cv.wait(lk);
  // while (!action_completed) { }

  return MqttClientActionResult(clientId, subscribe_action.action_type, MqttClientTestContainerInterface::action_number++, action_result, start_ts, end_ts);
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
  std::unique_lock<std::mutex> lk(m);
  cv.wait(lk);
  //while (!action_completed) { }

  return MqttClientActionResult(clientId, unsubscribe_action.action_type, MqttClientTestContainerInterface::action_number++, action_result, start_ts, end_ts);
}
MqttClientActionResult MqttClientPahoCppClientTestContainer::publish(MqttClientPublishAction &publish_action) {
  if (!action_completed) {
    throw std::exception();
  }
  MqttClientActionType actionType = publish_action.action_type;
  if (publish_action.qos == 0) {
    actionType = MqttClientActionType::PUBLISH_QOS0;
  } else if (publish_action.qos == 1) {
    actionType = MqttClientActionType::PUBLISH_QOS1;
  } else if (publish_action.qos == 2) {
    actionType = MqttClientActionType::PUBLISH_QOS2;
  }

  MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
  opts.onSuccess = paho_global_on_publish;
  opts.onFailure = paho_global_on_publish_failure;
  opts.context = this;

  MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
  pubmsg.payload = (void *)publish_action.payload.data();
  pubmsg.payloadlen = publish_action.payload.size();
  pubmsg.qos = publish_action.qos;
  pubmsg.retained = publish_action.retain;

  action_completed = false;
  // publish_token = 0;

  clock_gettime(CLOCK_REALTIME, &start_ts);
  if (MQTTAsync_sendMessage(client, publish_action.topicName.c_str(), &pubmsg, &opts) != MQTTASYNC_SUCCESS) {
    on_action_finished(MqttClientActionResultType::ERROR);
  }
  std::unique_lock<std::mutex> lk(m);
  cv.wait(lk);
  //while (!action_completed) { }

  return MqttClientActionResult(clientId, actionType, MqttClientTestContainerInterface::action_number++, action_result, start_ts, end_ts);
}
MqttClientActionResult MqttClientPahoCppClientTestContainer::publishReceivePublish(MqttClientPublishReceivePublishAction &publish_receive_publish) {
  if (!action_completed) {
    throw std::exception();
  }
  auto publish_action = publish_receive_publish;
  MqttClientActionType actionType = publish_action.action_type;
  if (publish_action.qos == 0) {
    actionType = MqttClientActionType::PUBLISH_RECEIVE_PUBLISH_QOS0;
  } else if (publish_action.qos == 1) {
    actionType = MqttClientActionType::PUBLISH_RECEIVE_PUBLISH_QOS1;
  } else if (publish_action.qos == 2) {
    actionType = MqttClientActionType::PUBLISH_RECEIVE_PUBLISH_QOS2;
  }

  MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
  opts.onSuccess = paho_global_on_publish_receive_publish;
  opts.onFailure = paho_global_on_publish_receive_publish_failure;
  opts.context = this;

  MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
  pubmsg.payload = (void *)publish_action.payload.data();
  pubmsg.payloadlen = publish_action.payload.size();
  pubmsg.qos = publish_action.qos;
  pubmsg.retained = publish_action.retain;

  MqttClientTestContainerReceiverInterface *old_interface = MqttClientTestContainerInterface::receiver_interface;
  MqttClientPahoCppTestContainerReceiver receiver_interface(publish_receive_publish, this);
  SetReceiverInterface(&receiver_interface);

  action_completed = false;
  // publish_token = 0;

  clock_gettime(CLOCK_REALTIME, &start_ts);
  if (MQTTAsync_sendMessage(client, publish_action.topicName.c_str(), &pubmsg, &opts) != MQTTASYNC_SUCCESS) {
    on_action_finished(MqttClientActionResultType::ERROR);
    SetReceiverInterface(old_interface);
    return MqttClientActionResult(clientId, actionType, MqttClientTestContainerInterface::action_number++, action_result, start_ts, end_ts);
  }

  std::unique_lock<std::mutex> lk(m);
  cv.wait(lk);
  // while (!action_completed) {  }

  return MqttClientActionResult(clientId, actionType, MqttClientTestContainerInterface::action_number++, action_result, start_ts, end_ts);
}
void MqttClientPahoCppClientTestContainer::receive_publish(char *topicName, int topicLen, MQTTAsync_message *message) {
  uint8_t *payload = static_cast<uint8_t *>(message->payload);
  int payloadLen = message->payloadlen;
  std::vector<uint8_t> data(payload, payload + payloadLen);
  int qos = message->qos;
  MqttClientTestContainerPublish received_publish(data, std::string(topicName), qos, message->retained);
  ReceivePublish(received_publish);
}
void MqttClientPahoCppClientTestContainer::connection_lost(char *cause) { on_action_finished(MqttClientActionResultType::CONNECTIONLOST); }
void MqttClientPahoCppClientTestContainer::on_action_finished(MqttClientActionResultType action_result) {
  clock_gettime(CLOCK_REALTIME, &end_ts);
  this->action_result = action_result;
  action_completed = true;
  std::unique_lock<std::mutex> lk(m);
  lk.unlock();
  cv.notify_one();
}
void MqttClientPahoCppClientTestContainer::on_publish_action_finished(MqttClientActionResultType action_result, MQTTAsync_token publish_token) {
  // if (this->publish_token == publish_token) {
  on_action_finished(action_result);
  //}
}
void MqttClientPahoCppClientTestContainer::on_receive_publish_action_finished(MqttClientActionResultType action_result, timespec end_ts) {
  this->end_ts = end_ts;
  this->action_result = action_result;
  action_completed = true;
  std::unique_lock<std::mutex> lk(m);
  lk.unlock();
  cv.notify_one();
}
MqttClientActionResult MqttClientPahoCppClientTestContainer::delay(MqttClientDelayAction &delayAction) {
  clock_gettime(CLOCK_REALTIME, &start_ts);
  std::this_thread::sleep_for(std::chrono::seconds(delayAction.duration));
  clock_gettime(CLOCK_REALTIME, &end_ts);
  return MqttClientActionResult(clientId, delayAction.action_type, MqttClientTestContainerInterface::action_number++, MqttClientActionResultType::SUCCESS, start_ts, end_ts);
}
