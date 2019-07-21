//
// Created by SomeDude on 21.07.2019.
//

#include "MqttBrokerTestType.h"

const char *getTextFromEnum(MqttBrokerTestType enumVal){
  if (enumVal == MqttBrokerTestType::MOSQUITTO_DOCKER) {
    return "MOSQUITTO_DOCKER";
  }
  if (enumVal == MqttBrokerTestType::EXTERNAL) {
    return "EXTERNAL";
  }
  return "UNKNOWN";
}
