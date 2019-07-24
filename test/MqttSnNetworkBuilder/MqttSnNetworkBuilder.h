//
// Created by SomeDude on 24.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNNETWORKBUILDER_MQTTSNNETWORKBUILDER_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNNETWORKBUILDER_MQTTSNNETWORKBUILDER_H_

#include <memory>
#include <test/MqttNetworkBuilder/MqttNetworkBuilder.h>
#include <test/MqttSnGateway/MqttSnGatewayTestContainerInterface.h>
#include <test/MqttSnClient/MqttSnClientTestContainerInterface.h>
#include <test/MqttSnForwarder/MqttSnForwarderTestContainerInterface.h>

class MqttNetworkBuilder;

class MqttSnNetworkBuilder {
 private:
  const MqttNetworkBuilder *mqttNetworkBuilder;
 public:
  MqttSnNetworkBuilder(const MqttNetworkBuilder *mqtt_network_builder);

  static std::shared_ptr<MqttSnGatewayTestContainerInterface> getMqttSnGateway();
  static std::shared_ptr<MqttSnForwarderTestContainerInterface> getMqttSnForwarder();
  static std::shared_ptr<MqttSnClientTestContainerInterface> getMqttSnClient();
};

#endif //CMQTTSNFORWARDER_TEST_MQTTSNNETWORKBUILDER_MQTTSNNETWORKBUILDER_H_
