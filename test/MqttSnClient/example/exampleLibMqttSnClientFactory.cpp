//
// Created by SomeDude on 27.09.2019.
//

#include <test/MqttBroker/MqttBrokerTestContainerFactory.h>
#include <test/MqttSnClient/MqttSnClientTestContainerFactory.h>
#include <test/MqttSnGateway/MqttSnGatewayTestContainerFactory.h>
int main(){
  auto mqttBroker = MqttBrokerTestContainerFactory::getMqttBroker();
  auto mqttSnGateway = MqttSnGatewayTestContainerFactory::getMqttSnGateway();
  auto mqttSnClient = MqttSnClientTestContainerFactory::getMqttSnClient();

  mqttBroker->start_broker();
  mqttSnGateway->start();
  mqttSnClient->StartBackgroundHandler();

  auto connectAction = mqttSnClient->getConnectAction();
  auto delayAction = mqttSnClient->getDelayAction(5);
  auto disconnectAction = mqttSnClient->getDisconnectAction();

  mqttSnClient->addAction(connectAction);
  mqttSnClient->addAction(delayAction);
  mqttSnClient->addAction(disconnectAction);
  mqttSnClient->generateActionSequence(1);

  mqttSnClient->execActionSequence();

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

  return 0;
}