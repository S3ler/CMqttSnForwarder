//
// Created by SomeDude on 19.07.2019.
//

#include "MqttSnGatewayTestType.h"

const char *MqttSnGatewayTestTypeEnumString[] = {"ARSMB", "PAHO"};

const char *getTextFromEnum(MqttSnGatewayTestType enumVal) {
  if (enumVal == MqttSnGatewayTestType::ARSMB) {
    return "ARSMB";
  }
  if (enumVal == MqttSnGatewayTestType::PAHO) {
    return "PAHO";
  }
  return "UNKNOWN";
}
