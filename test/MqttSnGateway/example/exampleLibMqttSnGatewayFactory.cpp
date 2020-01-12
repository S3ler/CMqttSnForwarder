//
// Created by SomeDude on 27.09.2019.
//

#include <test/MqttBroker/MqttBrokerTestContainerFactory.h>
#include <test/MqttSnGateway/MqttSnGatewayTestContainerFactory.h>
int main(){
  auto mqttBroker = MqttBrokerTestContainerFactory::getMqttBroker();
  auto mqttSnGateway = MqttSnGatewayTestContainerFactory::getMqttSnGateway();

  mqttBroker->start_broker();
  mqttSnGateway->start();

  std::this_thread::sleep_for (std::chrono::seconds(10));

  mqttSnGateway->stop();
  mqttBroker->stop_broker();
  return 0;
}