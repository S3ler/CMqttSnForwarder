//
// Created by SomeDude on 22.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTOBJECTBUILDER_MQTTCLIENTTESTCONTAINERCONNECTOPTIONS_H_
#define CMQTTSNFORWARDER_TEST_MQTTOBJECTBUILDER_MQTTCLIENTTESTCONTAINERCONNECTOPTIONS_H_

#include <stdint.h>
#include <string>

class MqttClientTestContainerConnectOptions final {
 public:
  //const std::string brokerURI; // example tcp://localhost:1883

  const std::string protocol; // example tcp
  const std::string brokerAddress; // example localhost
  const std::string brokerPort; // example 1883

  const std::string clientId; // not optional
  const std::string clientPassword; // optional

  const int32_t keepAliveIntervall;
  const bool cleanSession;

  // TODO will
  //const std::string willTopic;
  //const std::string willMessage;
  //const int32_t willQoS;
  MqttClientTestContainerConnectOptions(const std::string &broker_uri,
                                        const std::string &client_id,
                                        const std::string &client_password,
                                        const int32_t &keep_alive_interval,
                                        const bool &clean_session,
                                        const std::string &will_topic,
                                        const std::string &will_message,
                                        const int32_t &will_qos);
};

#endif //CMQTTSNFORWARDER_TEST_MQTTOBJECTBUILDER_MQTTCLIENTTESTCONTAINERCONNECTOPTIONS_H_
