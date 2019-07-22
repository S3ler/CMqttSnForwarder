//
// Created by SomeDude on 22.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTACTIONRESULTTYPE_H_
#define CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTACTIONRESULTTYPE_H_

#include <iostream>

enum class MqttClientActionResultType {
  SUCCESS = 0, ERROR //, TIMEOUT
};
std::ostream &operator<<(std::ostream &os, const MqttClientActionResultType &obj);


#endif //CMQTTSNFORWARDER_TEST_MQTTCLIENT_MQTTCLIENTACTIONRESULTTYPE_H_
