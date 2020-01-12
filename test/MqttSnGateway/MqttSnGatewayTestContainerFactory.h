//
// Created by SomeDude on 19.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNGATEWAY_MQTTSNGATEWAYTESTCONTAINERFACTORY_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNGATEWAY_MQTTSNGATEWAYTESTCONTAINERFACTORY_H_

#include <memory>
#include <string>
#include "MqttSnGatewayTestContainerInterface.h"
#include "MqttSnGatewayTestType.h"

class MqttSnGatewayTestContainerFactory {
 public:
  static std::shared_ptr<MqttSnGatewayTestContainerInterface> getMqttSnGateway();
  static std::shared_ptr<MqttSnGatewayTestContainerInterface> getMqttSnGateway(MqttSnGatewayTestType mqttSnGatewayTestType);
  static std::shared_ptr<MqttSnGatewayTestContainerInterface> getMqttSnGateway(MqttSnGatewayTestType mqttSnGatewayTestType,
                                                                               const MqttSnGatewayTestContainerConfiguration &configuration);
};

#endif //CMQTTSNFORWARDER_TEST_MQTTSNGATEWAY_MQTTSNGATEWAYTESTCONTAINERFACTORY_H_
