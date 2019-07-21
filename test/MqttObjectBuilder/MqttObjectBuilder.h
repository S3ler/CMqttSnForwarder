//
// Created by SomeDude on 21.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTOBJECTBUILDER_MQTTOBJECTBUILDER_H_
#define CMQTTSNFORWARDER_TEST_MQTTOBJECTBUILDER_MQTTOBJECTBUILDER_H_

#include "MqttObjectBuilderConfiguration.h"
class MqttObjectBuilder {
 private:
  const MqttObjectBuilderConfiguration configuration;
 public:
  MqttObjectBuilder(const MqttObjectBuilderConfiguration &configuration);

};

#endif //CMQTTSNFORWARDER_TEST_MQTTOBJECTBUILDER_MQTTOBJECTBUILDER_H_
