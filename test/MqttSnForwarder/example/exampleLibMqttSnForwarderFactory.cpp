//
// Created by SomeDude on 26.09.2019.
//

#include <test/MqttBroker/MqttBrokerTestContainerFactory.h>
#include <test/MqttSnClient/MqttSnClientTestContainerFactory.h>
#include <test/MqttSnForwarder/MqttSnForwarderTestContainerFactory.h>
#include <test/MqttSnGateway/MqttSnGatewayTestContainerFactory.h>
#include <chrono>
#include <thread>
int main() {

  auto mqttBroker = MqttBrokerTestContainerFactory::getMqttBroker();
  auto mqttSnGateway = MqttSnGatewayTestContainerFactory::getMqttSnGateway();
  auto mqttSnForwarder = MqttSnForwarderTestContainerFactory::getMqttSnForwarder();

  mqttBroker->start_broker();
  mqttSnGateway->start();
  mqttSnForwarder->start();

  // TODO ocnfig from Forwarder for Client
  // TODO getClientDefaultConfig + getForwarderDefaultConfig
  auto mqttSnClient = MqttSnClientTestContainerFactory::getMqttSnClient();
  mqttSnClient->StartBackgroundHandler();

  auto connectAction = mqttSnClient->getConnectAction();
  auto delayAction = mqttSnClient->getDelayAction(5);
  auto disconnectAction = mqttSnClient->getDisconnectAction();

  mqttSnClient->addAction(connectAction);
  mqttSnClient->addAction(delayAction);
  mqttSnClient->addAction(disconnectAction);
  mqttSnClient->generateActionSequence(1);

  mqttSnClient->execActionSequence();

  mqttSnForwarder->stop();
  mqttSnGateway->stop();
  mqttBroker->stop_broker();

  auto sequenceResult = mqttSnClient->GetResult();
  std::cout <<  std::endl;
  std::cout <<  std::endl;
  std::cout << MqttSnClientActionResult::GetCsvColumns() << std::endl;
  for (auto& result : sequenceResult) {
    std::cout << result << std::endl;
  }
  std::cout <<  std::endl;
  std::cout <<  std::endl;

  return 0;
}